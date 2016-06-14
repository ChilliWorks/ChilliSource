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
        : m_renderCommandProcessor(IRenderCommandProcessor::Create()), m_currentSnapshot(Colour::k_black)
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
    void Renderer::ProcessRenderSnapshot(RenderSnapshot renderSnapshot) noexcept
    {
        WaitThenStartRenderPrep();
        
        m_currentSnapshot = std::move(renderSnapshot);
        
        auto taskScheduler = Application::Get()->GetTaskScheduler();
        taskScheduler->ScheduleTask(TaskType::k_small, [=](const TaskContext& taskContext)
        {
            auto clearColour = m_currentSnapshot.GetClearColour();
            auto renderCamera = m_currentSnapshot.ClaimRenderCamera();
            auto renderAmbientLights = m_currentSnapshot.ClaimRenderAmbientLights();
            auto renderDirectionalLights = m_currentSnapshot.ClaimRenderDirectionalLights();
            auto renderPointLights = m_currentSnapshot.ClaimRenderPointLights();
            auto renderObjects = m_currentSnapshot.ClaimRenderObjects();
            auto preRenderCommandList = m_currentSnapshot.ClaimPreRenderCommandList();
            auto postRenderCommandList = m_currentSnapshot.ClaimPostRenderCommandList();
            
            auto renderFrame = RenderFrameCompiler::CompileRenderFrame(renderCamera, renderAmbientLights, renderDirectionalLights, renderPointLights, renderObjects);
            auto targetRenderPassGroups = m_renderPassCompiler->CompileTargetRenderPassGroups(taskContext, renderFrame);
            
            //TODO: Properly build render command queue.
            RenderCommandQueue renderCommandQueue;
            renderCommandQueue.push_back(std::move(preRenderCommandList));
            renderCommandQueue.push_back(std::move(postRenderCommandList));
            
            WaitThenPushCommandQueue(std::move(renderCommandQueue));
            EndRenderPrep();
        });
    }
    
    //------------------------------------------------------------------------------
    void Renderer::ProcessRenderCommandQueue() noexcept
    {
        auto renderCommandQueue = WaitThenPopCommandQueue();
        m_renderCommandProcessor->Process(std::move(renderCommandQueue));
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
    void Renderer::WaitThenPushCommandQueue(RenderCommandQueue renderCommandQueue) noexcept
    {
        std::unique_lock<std::mutex> lock(m_renderCommandQueuesMutex);
        
        while (m_renderCommandQueues.size() >= k_maxQueueSize)
        {
            m_renderCommandQueuesCondition.wait(lock);
        }
        
        m_renderCommandQueues.push_back(std::move(renderCommandQueue));
        
        m_renderCommandQueuesCondition.notify_all();
    }
    
    //------------------------------------------------------------------------------
    RenderCommandQueue Renderer::WaitThenPopCommandQueue() noexcept
    {
        std::unique_lock<std::mutex> lock(m_renderCommandQueuesMutex);
        
        while (m_renderCommandQueues.empty())
        {
            m_renderCommandQueuesCondition.wait(lock);
        }
        
        auto renderCommandQueue = std::move(m_renderCommandQueues.front());
        m_renderCommandQueues.pop_front();
        
        m_renderCommandQueuesCondition.notify_all();
        
        return renderCommandQueue;
    }
}
