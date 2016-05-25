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
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
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
    {
        //TODO: Create Render Pass Compiler & RenderCommandProcessor.
    }
    
    //------------------------------------------------------------------------------
    bool Renderer::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (Renderer::InterfaceID == interfaceId);
    }
    
    //------------------------------------------------------------------------------
    void Renderer::ProcessRenderSnapshot(RenderSnapshot& renderSnapshot) noexcept
    {
        // claim data from the render snapshot to ensure that it can't be externally modified when passed
        // to background tasks.
        auto clearColour = renderSnapshot.GetClearColour();
        auto renderCamera = renderSnapshot.ClaimRenderCamera();
        auto renderAmbientLights = renderSnapshot.ClaimRenderAmbientLights();
        auto renderDirectionalLights = renderSnapshot.ClaimRenderDirectionalLights();
        auto renderPointLights = renderSnapshot.ClaimRenderPointLights();
        auto renderObjects = renderSnapshot.ClaimRenderObjects();
        auto preRenderCommandList = renderSnapshot.ClaimPreRenderCommandList();
        auto postRenderCommandList = renderSnapshot.ClaimPostRenderCommandList();
        
        auto taskScheduler = Application::Get()->GetTaskScheduler();
        taskScheduler->ScheduleTask(TaskType::k_small, [=](const TaskContext& taskContext)
        {
            auto renderFrame = RenderFrameCompiler::CompileRenderFrame(renderCamera, renderAmbientLights, renderDirectionalLights, renderPointLights, renderObjects);
            
            //TODO: Process the rest of the render pipeline.
        });
    }
    
    //------------------------------------------------------------------------------
    void Renderer::ProcessRenderCommandQueue() noexcept
    {
        //TODO: process render command queue.
    }
}
