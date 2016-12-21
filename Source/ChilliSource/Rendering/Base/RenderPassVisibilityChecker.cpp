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
#include <ChilliSource/Rendering/Base/RenderPasses.h>
#include <ChilliSource/Rendering/Base/RenderFrame.h>
#include <ChilliSource/Rendering/Base/RenderPass.h>
#include <ChilliSource/Rendering/Base/RenderObject.h>
#include <ChilliSource/Rendering/Base/RenderPassObject.h>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_objectsPerVisibilityBatch = 50;
    }
    
    //------------------------------------------------------------------------------
    std::vector<RenderObject> RenderPassVisibilityChecker::CalculateVisibleObjects(const TaskContext& taskContext, const RenderCamera& camera, const std::vector<RenderObject>& renderObjects) noexcept
    {
        std::mutex visibleObjectsMutex;
        std::vector<RenderObject> visibleRenderObjects;
        
        u32 numTasks = 1 + (u32(renderObjects.size()) / k_objectsPerVisibilityBatch);
        std::vector<Task> tasks;
        for (u32 taskIndex = 0; taskIndex < numTasks; ++taskIndex)
        {
            tasks.push_back([=, &camera, &renderObjects, &visibleObjectsMutex, &visibleRenderObjects](const TaskContext& innerTaskContext)
            {
                std::vector<RenderObject> taskVisibleObjects;
                for(u32 objectIndex = 0; objectIndex < k_objectsPerVisibilityBatch; ++objectIndex)
                {
                    u32 index = taskIndex * k_objectsPerVisibilityBatch + objectIndex;
                    if (index >= renderObjects.size())
                    {
                        break;
                    }
                    
                    const auto& renderPassObject = renderObjects[index];
                    
                    if (camera.GetFrustrum().SphereCullTest(renderPassObject.GetBoundingSphere()))
                    {
                        taskVisibleObjects.push_back(renderPassObject);
                    }
                }
                
                std::unique_lock<std::mutex> lock(visibleObjectsMutex);
                visibleRenderObjects.insert(visibleRenderObjects.end(), taskVisibleObjects.begin(), taskVisibleObjects.end());
            });
        }
        
        taskContext.ProcessChildTasks(tasks);
        
        return visibleRenderObjects;
    }
} 
