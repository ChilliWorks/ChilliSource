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

#include <ChilliSource/Rendering/Base/RenderCommandCompiler.h>

#include <ChilliSource/Rendering/Base/CameraRenderPassGroup.h>
#include <ChilliSource/Rendering/Base/RenderPass.h>
#include <ChilliSource/Rendering/Base/TargetRenderPassGroup.h>

namespace ChilliSource
{
    namespace
    {
        /// Calculates whether a Camera Render Pass Group contains at least one render pass
        /// object.
        ///
        /// @param cameraRenderPassGroup
        ///     The camera render pass group.
        ///
        /// @return Whether or not there is at least one object.
        ///
        bool ContainsRenderPassObject(const CameraRenderPassGroup& cameraRenderPassGroup) noexcept
        {
            for (const auto& renderPass : cameraRenderPassGroup.GetRenderPasses())
            {
                if (renderPass.GetRenderPassObjects().size() > 0)
                {
                    return true;
                }
            }
            
            return false;
        }
        
        /// Calculates the number of render command lists required to process the given list
        /// of target render pass groups, and pre and post command lists.
        ///
        /// @param targetRenderPassGroups
        ///     The list of target render pass groups.
        /// @param preRenderCommandList
        ///     The render command list that should be inserted at the beginning of the queue if
        ///     the list contains any commands.
        /// @param postRenderCommandList
        ///     The render command list that should be inserted at the end of the queue if
        ///     the list contains any commands.
        ///
        /// @return The number of render command lists required.
        ///
        u32 CalcNumRenderCommandLists(const std::vector<TargetRenderPassGroup>& targetRenderPassGroups, const RenderCommandList* preRenderCommandList, const RenderCommandList* postRenderCommandList) noexcept
        {
            u32 count = 0;
            
            if (preRenderCommandList->GetOrderedList().size() > 0)
            {
                ++count;
            }
            
            for (const auto& targetRenderPassGroup : targetRenderPassGroups)
            {
                ++count; // target setup
                
                for (const auto& cameraRenderPassGroup : targetRenderPassGroup.GetRenderCameraGroups())
                {
                    for (const auto& renderPass : cameraRenderPassGroup.GetRenderPasses())
                    {
                        if (renderPass.GetRenderPassObjects().size() > 0)
                        {
                            ++count;
                        }
                    }
                }
                
                ++count; // target cleanup
            }
            
            if (postRenderCommandList->GetOrderedList().size() > 0)
            {
                ++count;
            }
            
            return count;
        }
        
        /// Compiles the render commands for the given render pass. The render pass must contain
        /// render pass objects otherwise this will assert.
        ///
        /// @param renderPass
        ///     The render pass.
        /// @param renderCommandList
        ///     The render command list to add the commands to.
        ///
        void CompileRenderCommandsForPass(const RenderPass& renderPass, RenderCommandList* renderCommandList) noexcept
        {
            //TODO: Handle lights
            
            const auto& renderPassObjects = renderPass.GetRenderPassObjects();
            CS_ASSERT(renderPassObjects.size() > 0, "Cannot compile a pass with no objects.");
            
            const RenderMaterial* currentMaterial = nullptr;
            const RenderMesh* currentStaticMesh = nullptr;
            const RenderDynamicMesh* currentDynamicMesh = nullptr;
            
            for (const auto& renderPassObject : renderPassObjects)
            {
                if (renderPassObject.GetRenderMaterial() != currentMaterial)
                {
                    currentMaterial = renderPassObject.GetRenderMaterial();
                    currentStaticMesh = nullptr;
                    
                    renderCommandList->AddApplyMaterialCommand(currentMaterial);
                }
                
                switch (renderPassObject.GetType())
                {
                    case RenderPassObject::Type::k_static:
                    {
                        if (renderPassObject.GetRenderMesh() != currentStaticMesh)
                        {
                            currentStaticMesh = renderPassObject.GetRenderMesh();
                            currentDynamicMesh = nullptr;
                            
                            renderCommandList->AddApplyMeshCommand(currentStaticMesh);
                        }
                        break;
                    }
                    case RenderPassObject::Type::k_dynamic:
                    {
                        if (renderPassObject.GetRenderDynamicMesh() != currentDynamicMesh)
                        {
                            currentStaticMesh = nullptr;
                            currentDynamicMesh = renderPassObject.GetRenderDynamicMesh();
                            
                            renderCommandList->AddApplyDynamicMeshCommand(currentDynamicMesh);
                        }
                        break;
                    }
                    default:
                        CS_LOG_FATAL("Invalid RenderPassObject type.");
                        break;
                }
                
                renderCommandList->AddRenderInstanceCommand(renderPassObject.GetWorldMatrix());
            }
        }
    }
    
    //------------------------------------------------------------------------------
    RenderCommandBufferCUPtr RenderCommandCompiler::CompileRenderCommands(const TaskContext& taskContext, const std::vector<TargetRenderPassGroup>& targetRenderPassGroups, const Integer2& resolution,
                                                                          const Colour& clearColour, std::vector<RenderDynamicMeshUPtr> renderDynamicMeshes, RenderCommandListUPtr preRenderCommandList,
                                                                          RenderCommandListUPtr postRenderCommandList) noexcept
    {
        u32 numLists = CalcNumRenderCommandLists(targetRenderPassGroups, preRenderCommandList.get(), postRenderCommandList.get());
        RenderCommandBufferUPtr renderCommandBuffer(new RenderCommandBuffer(numLists, std::move(renderDynamicMeshes)));
        u32 currentList = 0;
        
        if (preRenderCommandList->GetOrderedList().size() > 0)
        {
            *renderCommandBuffer->GetRenderCommandList(currentList++) = std::move(*preRenderCommandList);
        }
        
        for (const auto& targetRenderPassGroup : targetRenderPassGroups)
        {
            renderCommandBuffer->GetRenderCommandList(currentList++)->AddBeginCommand(resolution, clearColour);
                
            for (const auto& cameraRenderPassGroup : targetRenderPassGroup.GetRenderCameraGroups())
            {
                if (ContainsRenderPassObject(cameraRenderPassGroup))
                {
                    const auto& camera = cameraRenderPassGroup.GetCamera();
                    renderCommandBuffer->GetRenderCommandList(currentList)->AddApplyCameraCommand(camera.GetWorldMatrix().GetTranslation(), camera.GetViewProjectionMatrix());
                    
                    for (const auto& renderPass : cameraRenderPassGroup.GetRenderPasses())
                    {
                        if (renderPass.GetRenderPassObjects().size() > 0)
                        {
                            CompileRenderCommandsForPass(renderPass, renderCommandBuffer->GetRenderCommandList(currentList++));
                        }
                    }
                }
            }
            
            renderCommandBuffer->GetRenderCommandList(currentList++)->AddEndCommand();
        }
        
        if (postRenderCommandList->GetOrderedList().size() > 0)
        {
            *renderCommandBuffer->GetRenderCommandList(currentList++) = std::move(*postRenderCommandList);
        }
        
        return std::move(renderCommandBuffer);
    }
}
