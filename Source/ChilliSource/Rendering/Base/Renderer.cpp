//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/Rendering/Base/Renderer.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Base/ForwardRenderPassCompiler.h>
#include <ChilliSource/Rendering/Base/RenderCommandCompiler.h>
#include <ChilliSource/Rendering/Base/RenderCommandBufferManager.h>
#include <ChilliSource/Rendering/Base/RenderFrameCompiler.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Renderer);
    
    //------------------------------------------------------------------------------
    RendererUPtr Renderer::Create() noexcept
    {
        return RendererUPtr(new Renderer());
    }
    
    //------------------------------------------------------------------------------
    Renderer::Renderer() noexcept
        : m_currentSnapshot(nullptr, Integer2::k_zero, Colour::k_black, RenderCamera())
    {
    }
    
    //------------------------------------------------------------------------------
    bool Renderer::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (Renderer::InterfaceID == interfaceId);
    }
    //------------------------------------------------------------------------------
    RenderSnapshot Renderer::CreateRenderSnapshot(const Integer2& resolution, const Colour& clearColour, const RenderCamera& renderCamera) noexcept
    {
        auto allocator = m_frameAllocatorQueue.Pop();
        
        return RenderSnapshot(allocator, resolution, clearColour, renderCamera);
    }
    
    //------------------------------------------------------------------------------
    void Renderer::ProcessRenderSnapshot(RenderSnapshot renderSnapshot) noexcept
    {
        WaitThenStartRenderPrep();
        
        m_currentSnapshot = std::move(renderSnapshot);
        
        auto taskScheduler = Application::Get()->GetTaskScheduler();
        taskScheduler->ScheduleTask(TaskType::k_small, [=](const TaskContext& taskContext)
        {
            auto resolution = m_currentSnapshot.GetResolution();
            auto clearColour = m_currentSnapshot.GetClearColour();
            auto renderCamera = m_currentSnapshot.GetRenderCamera();
            auto renderAmbientLights = m_currentSnapshot.ClaimAmbientRenderLights();
            auto renderDirectionalLights = m_currentSnapshot.ClaimDirectionalRenderLights();
            auto renderPointLights = m_currentSnapshot.ClaimPointRenderLights();
            auto renderObjects = m_currentSnapshot.ClaimRenderObjects();
            auto preRenderCommandList = m_currentSnapshot.ClaimPreRenderCommandList();
            auto postRenderCommandList = m_currentSnapshot.ClaimPostRenderCommandList();
            auto renderFrameData = m_currentSnapshot.ClaimRenderFrameData();
            
            auto renderFrame = RenderFrameCompiler::CompileRenderFrame(resolution, clearColour, renderCamera, renderAmbientLights, renderDirectionalLights, renderPointLights, renderObjects);
            auto targetRenderPassGroups = m_renderPassCompiler->CompileTargetRenderPassGroups(taskContext, renderFrame);
            auto renderCommandBuffer = RenderCommandCompiler::CompileRenderCommands(taskContext, targetRenderPassGroups, std::move(preRenderCommandList), std::move(postRenderCommandList), std::move(renderFrameData));
            
            m_commandRecycleSystem->WaitThenPushCommandBuffer(std::move(renderCommandBuffer));
            EndRenderPrep();
        });
    }
    
    //------------------------------------------------------------------------------
    void Renderer::ProcessRenderCommandBuffer() noexcept
    {
        auto renderCommandBuffer = m_commandRecycleSystem->WaitThenPopCommandBuffer();
        m_renderCommandProcessor->Process(renderCommandBuffer.get());
        
        auto allocator = renderCommandBuffer->GetFrameAllocator();
        renderCommandBuffer.reset();
        
        m_frameAllocatorQueue.Push(allocator);
    }
    
    //------------------------------------------------------------------------------
    void Renderer::WaitThenStartRenderPrep() noexcept
    {
        std::unique_lock<std::mutex> lock(m_renderPrepMutex);
        
        while (m_renderPrepActive)
        {
            m_renderPrepCondition.wait(lock);
        }
        
        m_renderPrepActive = true;
    }
    
    //------------------------------------------------------------------------------
    void Renderer::EndRenderPrep() noexcept
    {
        std::unique_lock<std::mutex> lock(m_renderPrepMutex);
        m_renderPrepActive = false;
        m_renderPrepCondition.notify_all();
    }
    
    //------------------------------------------------------------------------------
    void Renderer::OnInit() noexcept
    {
        //TODO: Handle forward vs deferred rendering
        m_renderPassCompiler = IRenderPassCompilerUPtr(new ForwardRenderPassCompiler());
        m_renderCommandProcessor = IRenderCommandProcessor::Create();

        m_commandRecycleSystem = Application::Get()->GetSystem<RenderCommandBufferManager>();
    }
    
    //------------------------------------------------------------------------------
    void Renderer::OnSystemResume() noexcept
    {
        if(m_initialised)
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            m_renderCommandProcessor->Restore();
#endif
        }
        
        m_initialised = true;
    }

    //------------------------------------------------------------------------------
    void Renderer::OnDestroy() noexcept
    {
        m_commandRecycleSystem = nullptr;
        m_renderCommandProcessor.reset();
        m_renderPassCompiler.reset();
    }
    
    //------------------------------------------------------------------------------
    void Renderer::OnSystemSuspend() noexcept
    {
#ifdef CS_TARGETPLATFORM_ANDROID
        m_renderCommandProcessor->Invalidate();
#endif
    }
}
