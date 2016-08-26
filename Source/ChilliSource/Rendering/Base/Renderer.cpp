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
    
    namespace
    {
        /// Converts the snapshot into a render frame
        ///
        /// @param snapshot
        ///     Snapshot to convert. Some of the data will be moved
        /// @return Render frame
        ///
        RenderFrame CompileRenderFrame(RenderSnapshot& snapshot)
        {
            auto resolution = snapshot.GetResolution();
            auto clearColour = snapshot.GetClearColour();
            auto renderCamera = snapshot.GetRenderCamera();
            auto renderAmbientLights = snapshot.ClaimAmbientRenderLights();
            auto renderDirectionalLights = snapshot.ClaimDirectionalRenderLights();
            auto renderPointLights = snapshot.ClaimPointRenderLights();
            auto renderObjects = snapshot.ClaimRenderObjects();
            auto offscreenTarget = snapshot.GetOffscreenRenderTarget();
            
            return RenderFrameCompiler::CompileRenderFrame(offscreenTarget, resolution, clearColour, renderCamera, renderAmbientLights, renderDirectionalLights, renderPointLights, renderObjects);
        }
    }
    
    //------------------------------------------------------------------------------
    RendererUPtr Renderer::Create() noexcept
    {
        return RendererUPtr(new Renderer());
    }
    
    //------------------------------------------------------------------------------
    Renderer::Renderer() noexcept
        : m_currentMainSnapshot(nullptr, Integer2::k_zero, Colour::k_black, RenderCamera())

    {

    }
    
    //------------------------------------------------------------------------------
    bool Renderer::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (Renderer::InterfaceID == interfaceId);
    }
    
    //------------------------------------------------------------------------------
    RenderSnapshot Renderer::CreateRenderSnapshot(const RenderTargetGroup* renderTarget, const Integer2& resolution, const Colour& clearColour, const RenderCamera& renderCamera) noexcept
    {
        return RenderSnapshot(renderTarget, resolution, clearColour, renderCamera);
    }
    
    //------------------------------------------------------------------------------
    void Renderer::ProcessRenderSnapshots(IAllocator* frameAllocator, RenderSnapshot mainRenderSnapshot, std::vector<RenderSnapshot> offscreenRenderSnapshots) noexcept
    {
        WaitThenStartRenderPrep();
        
        m_currentMainSnapshot = std::move(mainRenderSnapshot);
        m_currentOffscreenSnapshots = std::move(offscreenRenderSnapshots);
        
        auto taskScheduler = Application::Get()->GetTaskScheduler();
        
        taskScheduler->ScheduleTask(TaskType::k_small, [=](const TaskContext& taskContext)
        {
            std::vector<RenderFrame> renderFrames(m_currentOffscreenSnapshots.size());
            std::vector<RenderFrameData> renderFramesData(m_currentOffscreenSnapshots.size());
            
            for(auto i=0; i<m_currentOffscreenSnapshots.size(); ++i)
            {
                CS_ASSERT(m_currentOffscreenSnapshots[i].GetPreRenderCommandList()->GetOrderedList().size() == 0 && m_currentOffscreenSnapshots[i].GetPostRenderCommandList()->GetOrderedList().size() == 0, "Offscreen render snapshots cannot have pre or post render commands");
                
                auto renderFrameData = m_currentOffscreenSnapshots[i].ClaimRenderFrameData();
                renderFramesData.push_back(std::move(renderFrameData));
                
                auto renderFrame = CompileRenderFrame(m_currentOffscreenSnapshots[i]);
                renderFrames.push_back(std::move(renderFrame));
            }
            
            auto preRenderCommandList = m_currentMainSnapshot.ClaimPreRenderCommandList();
            auto postRenderCommandList = m_currentMainSnapshot.ClaimPostRenderCommandList();
            
            auto renderFrameData = m_currentMainSnapshot.ClaimRenderFrameData();
            renderFramesData.push_back(std::move(renderFrameData));
            
            auto renderFrame = CompileRenderFrame(m_currentMainSnapshot);
            renderFrames.push_back(std::move(renderFrame));
            
            auto targetRenderPassGroups = m_renderPassCompiler->CompileTargetRenderPassGroups(taskContext, std::move(renderFrames));
            auto renderCommandBuffer = RenderCommandCompiler::CompileRenderCommands(taskContext, std::move(frameAllocator), targetRenderPassGroups, std::move(preRenderCommandList), std::move(postRenderCommandList), std::move(renderFramesData));
            
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
        m_currentOffscreenSnapshots.clear();
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
