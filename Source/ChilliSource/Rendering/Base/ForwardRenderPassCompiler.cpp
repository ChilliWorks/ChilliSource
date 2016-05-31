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

#include <ChilliSource/Rendering/Base/ForwardRenderPassCompiler.h>

#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Base/RenderFrame.h>
#include <ChilliSource/Rendering/Base/RenderPass.h>
#include <ChilliSource/Rendering/Base/RenderObject.h>
#include <ChilliSource/Rendering/Base/RenderPassObject.h>
#include <ChilliSource/Rendering/Base/RenderPassUtils.h>
#include <ChilliSource/Rendering/Base/RenderFrame.h>
#include <ChilliSource/Rendering/Base/RenderSortUtils.h>


namespace ChilliSource
{
    namespace Rendering
    {
        namespace
        {
            enum class Passes
            {
                k_base,
                k_directional,
                k_transparent,
                k_total
            };
            
            constexpr u32 k_reservedRenderPasses = 2;//Base + Transparent
        }
        
        //------------------------------------------------------------------------------
        std::vector<TargetRenderPassGroup> ForwardRenderPassCompiler::CompileRenderPasses(const TaskContext& in_outerTaskContext, const RenderFrame& renderFrame) noexcept
        {
            return std::vector<TargetRenderPassGroup>();
        }
        
        std::vector<CameraRenderPassGroup> ForwardRenderPassCompiler::CompileLightRenderPasses(const TaskContext& outerTaskContext, const RenderFrame& renderFrame) noexcept
        {
//            renderFrame.GetRenderAmbientLight()
            const u32 numPasses = k_reservedRenderPasses + u32(renderFrame.GetRenderDirectionalLights().size()) + u32(renderFrame.GetRenderPointLights().size());
            std::vector<RenderPass> renderPasses(numPasses);
            std::vector<Task> tasks;
            u32 nextPassIndex = 0;
            
            u32 basePassIndex = nextPassIndex++;
            tasks.push_back([=, &renderPasses, &renderFrame](const TaskContext& innerTaskContext)
            {
                auto renderPassObjects = RenderPassUtils::GetBaseRenderPassObjects(renderFrame.GetRenderObjects());
//                renderPasses[basePassIndex] = CompileRenderPass(innerTaskContext, renderFrame.GetRenderCamera(), RenderLight::k_null, renderPassObjects, OpaqueSort);
            });
            
            for (const auto& directionalLight : renderFrame.GetRenderDirectionalLights())
            {
                u32 directionLightPassIndex = nextPassIndex++;
                tasks.push_back([=, &renderPasses, &renderFrame](const TaskContext& innerTaskContext)
                {
                    auto renderPassObjects = RenderPassUtils::GetDirectionalLightRenderPassObjects(renderFrame.GetRenderObjects());
//                    renderPasses[directionLightPassIndex] = CompileRenderPass(innerTaskContext, renderFrame.GetRenderCamera(), directionalLight, renderPassObjects, OpaqueSort);
                });
            }
            
            for (const auto& pointLight : renderFrame.GetRenderPointLights())
            {
                u32 directionLightPassIndex = nextPassIndex++;
                tasks.push_back([=, &renderPasses, &renderFrame](const TaskContext& innerTaskContext)
                {
                    auto renderPassObjects = RenderPassUtils::GetDirectionalLightRenderPassObjects(renderFrame.GetRenderObjects());
                    //renderPasses[directionLightPassIndex] = CompileRenderPass(innerTaskContext, renderFrame.GetRenderCamera(), directionalLight, renderPassObjects, OpaqueSort);
                });
            }
            
            u32 transparentPassIndex = nextPassIndex++;
            tasks.push_back([=, &renderPasses, &renderFrame](const TaskContext& innerTaskContext)
            {
                auto renderPassObjects = RenderPassUtils::GetTransparentRenderPassObjects(renderFrame.GetRenderObjects());
//                renderPasses[transparentPassIndex] = CompileRenderPass(innerTaskContext, renderFrame.GetRenderCamera(), RenderLight::k_null, renderPassObjects, TransparentSort);
            });
            
            outerTaskContext.ProcessChildTasks(tasks);
            
            std::vector<CameraRenderPassGroup> renderPassGroups;
            renderPassGroups.push_back(CameraRenderPassGroup(renderFrame.GetRenderCamera(), renderPasses));
            return renderPassGroups;
        }
    }
} 