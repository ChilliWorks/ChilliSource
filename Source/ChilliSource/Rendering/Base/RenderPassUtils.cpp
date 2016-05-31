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

#include <ChilliSource/Rendering/Base/RenderPassUtils.h>

#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Base/RenderFrame.h>
#include <ChilliSource/Rendering/Base/RenderPass.h>
#include <ChilliSource/Rendering/Base/RenderObject.h>
#include <ChilliSource/Rendering/Base/RenderPassObject.h>
#include <ChilliSource/Rendering/Base/RenderSortUtils.h>

namespace ChilliSource
{
    namespace Rendering
    {
        namespace RenderPassUtils
        {
            //------------------------------------------------------------------------------
            std::vector<RenderPassObject> GetBaseRenderPassObjects(const std::vector<RenderObject>& in_renderObjects) noexcept
            {
                std::vector<RenderPassObject> baseRenderPassObjects;
                
                for (const auto& renderObject : in_renderObjects)
                {
                    //                    auto renderMaterial = renderObject.GetRenderMaterialGroup()->GetStaticRenderMaterial(static_cast<u32>(Passes::k_base));
                    //
                    //                    if (renderMaterial)
                    //                    {
                    //                        baseRenderPassObjects.push_back(RenderPassObject(renderMaterial, renderObject.GetRenderMesh(), renderObject.GetWorldMatrix()));
                    //                    }
                }
                
                return baseRenderPassObjects;
            }
            //------------------------------------------------------------------------------
            std::vector<RenderPassObject> GetDirectionalLightRenderPassObjects(const std::vector<RenderObject>& in_renderObjects) noexcept
            {
                std::vector<RenderPassObject> baseRenderPassObjects;
                
                for (const auto& renderObject : in_renderObjects)
                {
                    //                    auto renderMaterial = renderObject.GetRenderMaterialGroup()->GetStaticRenderMaterial(static_cast<u32>(Passes::k_directional));
                    //
                    //                    if (renderMaterial)
                    //                    {
                    //                        baseRenderPassObjects.push_back(RenderPassObject(renderMaterial, renderObject.GetRenderMesh(), renderObject.GetWorldMatrix()));
                    //                    }
                }
                
                return baseRenderPassObjects;
            }
            //------------------------------------------------------------------------------
            std::vector<RenderPassObject> GetTransparentRenderPassObjects(const std::vector<RenderObject>& in_renderObjects) noexcept
            {
                std::vector<RenderPassObject> transparentRenderPassObjects;
                
                for (const auto& renderObject : in_renderObjects)
                {
                    //                    auto renderMaterial = renderObject.GetRenderMaterialGroup()->GetStaticRenderMaterial(static_cast<u32>(Passes::k_transparent));
                    //
                    //                    if (renderMaterial)
                    //                    {
                    //                        transparentRenderPassObjects.push_back(RenderPassObject(renderMaterial, renderObject.GetRenderMesh(), renderObject.GetWorldMatrix()));
                    //                    }
                }
                
                return transparentRenderPassObjects;
            }
            //------------------------------------------------------------------------------
            bool IsRenderObjectVisible(const RenderCamera& in_camera, const RenderPassObject& in_renderPassObject) noexcept
            {
                //TODO: !? calculate visiblilty
                return true;
            }
            //------------------------------------------------------------------------------
            std::vector<RenderPassObject> CalculateVisibleObjects(const TaskContext& outerTaskContext, const RenderCamera& camera, const std::vector<RenderPassObject>& renderPassObjects) noexcept
            {
                constexpr u32 k_objectsPerBatch = 25;
                
                std::mutex visibleObjectsMutex;
                std::vector<RenderPassObject> visibleRenderPassObjects;
                
                u32 numTasks = 1 + (renderPassObjects.size() / k_objectsPerBatch);
                std::vector<Task> tasks;
                for (u32 taskIndex = 0; taskIndex < numTasks; ++taskIndex)
                {
                    tasks.push_back([=, &camera, &renderPassObjects, &visibleObjectsMutex, &visibleRenderPassObjects](const TaskContext& innerTaskContext)
                    {
                        for(u32 objectIndex = 0; objectIndex < k_objectsPerBatch; ++objectIndex)
                        {
                            u32 index = taskIndex * k_objectsPerBatch + objectIndex;
                            if (index >= renderPassObjects.size())
                            {
                                break;
                            }
                            
                            const auto& renderPassObject = renderPassObjects[index];
                            
                            if (IsRenderObjectVisible(camera, renderPassObject))
                            {
                                std::unique_lock<std::mutex> lock(visibleObjectsMutex);
                                visibleRenderPassObjects.push_back(renderPassObject);
                            }
                        }
                    });
                }
                
                outerTaskContext.ProcessChildTasks(tasks);
                
                return visibleRenderPassObjects;
            }
            //------------------------------------------------------------------------------
            RenderPass CompileRenderPass(const TaskContext& taskContext, const RenderCamera& camera, const RenderAmbientLight& light, const std::vector<RenderPassObject>& renderPassObjects,
                                         const RenderSortFunc& sort) noexcept
            {
                auto visibleRenderPassObjects = CalculateVisibleObjects(taskContext, camera, renderPassObjects);
                sort(camera, visibleRenderPassObjects);
                
                return RenderPass(light, visibleRenderPassObjects);
            }
        }
    }
} 