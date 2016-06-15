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

#include <ChilliSource/Rendering/Base/RenderPassVisibilityChecker.h>

#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Base/ForwardRenderPasses.h>
#include <ChilliSource/Rendering/Base/RenderFrame.h>
#include <ChilliSource/Rendering/Base/RenderPass.h>
#include <ChilliSource/Rendering/Base/RenderObject.h>
#include <ChilliSource/Rendering/Base/RenderPassObject.h>

namespace ChilliSource
{
    namespace RenderPassVisibilityChecker
    {
        namespace
        {
            constexpr u32 k_objectsPerVisibilityBatch = 50;
            
            //------------------------------------------------------------------------------
            bool IsRenderObjectVisible(const RenderCamera& camera, const RenderPassObject& renderPassObject) noexcept
            {
                //In the future, we could cache this worldspace sphere in the renderpass object if it turns out its needed in different steps.
                const Sphere& localSphere = renderPassObject.GetRenderMesh()->GetBoundingSphere();
                
                CS::Vector3 position;
                CS::Vector3 scale;
                CS::Quaternion orientation;
                renderPassObject.GetWorldMatrix().Decompose(position, scale, orientation);
                
                f32 radius = localSphere.fRadius * std::max(scale.x, std::max(scale.y, scale.z));
                Sphere worldSphere(position + localSphere.vOrigin, radius);
                return camera.GetFrustrum().SphereCullTest(worldSphere);
            }
        }
        
        //------------------------------------------------------------------------------
        std::vector<RenderPassObject> CalculateVisibleObjects(const TaskContext& taskContext, const RenderCamera& camera, const std::vector<RenderPassObject>& renderPassObjects) noexcept
        {
            std::mutex visibleObjectsMutex;
            std::vector<RenderPassObject> visibleRenderPassObjects;
            
            u32 numTasks = 1 + (u32(renderPassObjects.size()) / k_objectsPerVisibilityBatch);
            std::vector<Task> tasks;
            for (u32 taskIndex = 0; taskIndex < numTasks; ++taskIndex)
            {
                tasks.push_back([=, &camera, &renderPassObjects, &visibleObjectsMutex, &visibleRenderPassObjects](const TaskContext& innerTaskContext)
                {
                    std::vector<RenderPassObject> taskVisiblePassObjects;
                    for(u32 objectIndex = 0; objectIndex < k_objectsPerVisibilityBatch; ++objectIndex)
                    {
                        u32 index = taskIndex * k_objectsPerVisibilityBatch + objectIndex;
                        if (index >= renderPassObjects.size())
                        {
                            break;
                        }
                        
                        const auto& renderPassObject = renderPassObjects[index];
                        
                        if (IsRenderObjectVisible(camera, renderPassObject))
                        {
                            taskVisiblePassObjects.push_back(renderPassObject);
                        }
                    }
                    
                    std::unique_lock<std::mutex> lock(visibleObjectsMutex);
                    visibleRenderPassObjects.insert(visibleRenderPassObjects.end(), taskVisiblePassObjects.begin(), taskVisiblePassObjects.end());
                });
            }
            
            taskContext.ProcessChildTasks(tasks);
            
            return visibleRenderPassObjects;
        }
    }
} 