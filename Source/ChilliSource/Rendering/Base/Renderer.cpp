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
#include <ChilliSource/Rendering/Base/RenderFrameCompiler.h>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_maxQueueSize = 1;
    }
    
    CS_DEFINE_NAMEDTYPE(Renderer);
    
    //------------------------------------------------------------------------------
    RendererUPtr Renderer::Create() noexcept
    {
        return RendererUPtr(new Renderer());
    }
    
    //------------------------------------------------------------------------------
    Renderer::Renderer() noexcept
        : m_renderCommandProcessor(IRenderCommandProcessor::Create()), m_currentSnapshot(nullptr, Integer2::k_zero, Colour::k_black, RenderCamera())
    {
        //TODO: Handle forward vs deferred rendering
        m_renderPassCompiler = IRenderPassCompilerUPtr(new ForwardRenderPassCompiler());
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
            auto frameAllocator = m_currentSnapshot.GetFrameAllocator();
            auto resolution = m_currentSnapshot.GetResolution();
            auto clearColour = m_currentSnapshot.GetClearColour();
            auto renderCamera = m_currentSnapshot.GetRenderCamera();
            auto renderAmbientLights = m_currentSnapshot.ClaimAmbientRenderLights();
            auto renderDirectionalLights = m_currentSnapshot.ClaimDirectionalRenderLights();
            auto renderPointLights = m_currentSnapshot.ClaimPointRenderLights();
            auto renderObjects = m_currentSnapshot.ClaimRenderObjects();
            auto renderDynamicMeshes = m_currentSnapshot.ClaimRenderDynamicMeshes();
            auto preRenderCommandList = m_currentSnapshot.ClaimPreRenderCommandList();
            auto postRenderCommandList = m_currentSnapshot.ClaimPostRenderCommandList();
            
            auto renderFrame = RenderFrameCompiler::CompileRenderFrame(resolution, clearColour, renderCamera, renderAmbientLights, renderDirectionalLights, renderPointLights, renderObjects);
            auto targetRenderPassGroups = m_renderPassCompiler->CompileTargetRenderPassGroups(taskContext, renderFrame);
            auto renderCommandBuffer = RenderCommandCompiler::CompileRenderCommands(taskContext, frameAllocator, targetRenderPassGroups, std::move(renderDynamicMeshes),
                                                                                    std::move(preRenderCommandList), std::move(postRenderCommandList));
            
            WaitThenPushCommandBuffer(std::move(renderCommandBuffer));
            EndRenderPrep();
        });
    }
    
    //------------------------------------------------------------------------------
    void Renderer::ProcessRenderCommandBuffer() noexcept
    {
        auto renderCommandBuffer = WaitThenPopCommandBuffer();
        m_renderCommandProcessor->Process(renderCommandBuffer.get());
        
        m_frameAllocatorQueue.Push(renderCommandBuffer->GetFrameAllocator());
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
    void Renderer::WaitThenPushCommandBuffer(RenderCommandBufferCUPtr renderCommandBuffer) noexcept
    {
        std::unique_lock<std::mutex> lock(m_renderCommandBuffersMutex);
        
        while (m_renderCommandBuffers.size() >= k_maxQueueSize)
        {
            m_renderCommandBuffersCondition.wait(lock);
        }
        
        m_renderCommandBuffers.push_back(std::move(renderCommandBuffer));
        
        m_renderCommandBuffersCondition.notify_all();
    }
    
    //------------------------------------------------------------------------------
    RenderCommandBufferCUPtr Renderer::WaitThenPopCommandBuffer() noexcept
    {
        std::unique_lock<std::mutex> lock(m_renderCommandBuffersMutex);
        
        while (m_renderCommandBuffers.empty())
        {
            m_renderCommandBuffersCondition.wait(lock);
        }
        
        auto renderCommandBuffer = std::move(m_renderCommandBuffers.front());
        m_renderCommandBuffers.pop_front();
        
        m_renderCommandBuffersCondition.notify_all();
        
        return renderCommandBuffer;
    }
}
