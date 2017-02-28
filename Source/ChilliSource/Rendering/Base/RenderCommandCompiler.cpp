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

#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Base/CameraRenderPassGroup.h>
#include <ChilliSource/Rendering/Base/RenderPass.h>
#include <ChilliSource/Rendering/Base/TargetRenderPassGroup.h>
#include <ChilliSource/Rendering/Model/SmallMeshBatcher.h>
#include <ChilliSource/Rendering/Target/RenderTargetGroup.h>

namespace ChilliSource
{
    namespace
    {
        /// An container for the current cached state of a render command list.
        ///
        struct RenderCommandListStateCache final
        {
            const RenderMaterial* m_material = nullptr;
            const RenderMesh* m_mesh = nullptr;
            const RenderDynamicMesh* m_dynamicMesh = nullptr;
            const RenderSkinnedAnimation* m_skinnedAnimation = nullptr;
        };
        
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
        
        /// Adds a new begin command or begin with target group command, depending on whether or
        /// not a RenderTargetGroup exists.
        ///
        /// @param targetRenderPassGroup
        ///     The target render pass group.
        /// @param renderCommandList
        ///     The render command list to add the command to.
        ///
        void AddBeginCommand(const TargetRenderPassGroup& targetRenderPassGroup, RenderCommandList* renderCommandList) noexcept
        {
            if (targetRenderPassGroup.GetRenderTargetGroup())
            {
                renderCommandList->AddBeginWithTargetGroupCommand(targetRenderPassGroup.GetRenderTargetGroup(), targetRenderPassGroup.GetClearColour());
            }
            else
            {
                renderCommandList->AddBeginCommand(targetRenderPassGroup.GetResolution(), targetRenderPassGroup.GetClearColour());
            }
        }
        
        /// Adds a new apply light command to the list for the given render pass.
        ///
        /// @param renderPass
        ///     The render pass.
        /// @param renderCommandList
        ///     The render command list to add the command to.
        ///
        void AddApplyLightCommand(const RenderPass& renderPass, RenderCommandList* renderCommandList) noexcept
        {
            switch (renderPass.GetLightType())
            {
                case RenderPass::LightType::k_none:
                {
                    renderCommandList->AddApplyAmbientLightCommand(Colour::k_black);
                    break;
                }
                case RenderPass::LightType::k_ambient:
                {
                    const auto& ambientLight = renderPass.GetAmbientLight();
                    renderCommandList->AddApplyAmbientLightCommand(ambientLight.GetColour());
                    break;
                }
                case RenderPass::LightType::k_directional:
                {
                    const auto& directionalLight = renderPass.GetDirectionalLight();
                    auto viewProj = Matrix4::Inverse(directionalLight.GetLightWorldMatrix()) * directionalLight.GetLightProjectionMatrix();
                    
                    const auto& shadowMapTarget = directionalLight.GetShadowMapTarget();
                    const RenderTexture* shadowMapTexture = nullptr;
                    if (shadowMapTarget)
                    {
                        shadowMapTexture = shadowMapTarget->GetDepthTarget();
                        CS_ASSERT(shadowMapTexture, "Shadow map target must have depth texture.");
                    }
                    
                    renderCommandList->AddApplyDirectionalLightCommand(directionalLight.GetColour(), directionalLight.GetDirection(), viewProj, directionalLight.GetShadowTolerance(), shadowMapTexture);
                    break;
                }
                case RenderPass::LightType::k_point:
                {
                    const auto& pointLight = renderPass.GetPointLight();
                    renderCommandList->AddApplyPointLightCommand(pointLight.GetColour(), pointLight.GetPosition(), pointLight.GetAttenuation());
                    break;
                }
                default:
                {
                    CS_LOG_FATAL("Invalid light type.");
                    break;
                }
            }
        }
        
        /// Adds a new apply material command.
        ///
        /// If the cache already contains the material the no command will be generated.
        ///
        /// @param renderPassObject
        ///     The render pass object to create the command for.
        /// @param renderCommandList
        ///     The render command list to add the command to.
        /// @param cache
        ///     The current render command list state cache.
        /// @param batcher
        ///     The current small mesh batcher.
        ///
        void AddApplyMaterialCommand(const RenderPassObject& renderPassObject, RenderCommandList* renderCommandList, RenderCommandListStateCache& cache, SmallMeshBatcher& batcher) noexcept
        {
            if (renderPassObject.GetRenderMaterial() != cache.m_material)
            {
                batcher.Flush();
                
                cache.m_material = renderPassObject.GetRenderMaterial();
                cache.m_mesh = nullptr;
                cache.m_dynamicMesh = nullptr;
                cache.m_skinnedAnimation = nullptr;
                
                renderCommandList->AddApplyMaterialCommand(cache.m_material);
            }
        }
        
        /// Adds either a new apply mesh, or apply dynamic mesh command depending on the type of
        /// render pass object.
        ///
        /// The the cache already contains the mesh then no command will be generated.
        ///
        /// @param renderPassObject
        ///     The render pass object to create the command for.
        /// @param renderCommandList
        ///     The render command list to add the command to.
        /// @param cache
        ///     The current render command list state cache.
        ///
        void AddApplyMeshCommand(const RenderPassObject& renderPassObject, RenderCommandList* renderCommandList, RenderCommandListStateCache& cache) noexcept
        {
            switch (renderPassObject.GetType())
            {
                case RenderPassObject::Type::k_static:
                case RenderPassObject::Type::k_staticAnimated:
                {
                    if (renderPassObject.GetRenderMesh() != cache.m_mesh)
                    {
                        cache.m_mesh = renderPassObject.GetRenderMesh();
                        cache.m_dynamicMesh = nullptr;
                        cache.m_skinnedAnimation = nullptr;
                        
                        renderCommandList->AddApplyMeshCommand(cache.m_mesh);
                    }
                    break;
                }
                case RenderPassObject::Type::k_dynamic:
                case RenderPassObject::Type::k_dynamicAnimated:
                {
                    if (renderPassObject.GetRenderDynamicMesh() != cache.m_dynamicMesh)
                    {
                        cache.m_mesh = nullptr;
                        cache.m_dynamicMesh = renderPassObject.GetRenderDynamicMesh();
                        cache.m_skinnedAnimation = nullptr;
                        
                        renderCommandList->AddApplyDynamicMeshCommand(cache.m_dynamicMesh);
                    }
                    break;
                }
                default:
                    CS_LOG_FATAL("Invalid RenderPassObject type.");
                    break;
            }
        }
        
        /// Adds either a new apply skinned animation command if the object has animation.
        ///
        /// The the cache already contains the skinned animation, then no command will be
        /// generated.
        ///
        /// @param renderPassObject
        ///     The render pass object to create the command for.
        /// @param renderCommandList
        ///     The render command list to add the command to.
        /// @param cache
        ///     The current render command list state cache.
        ///
        void AddApplySkinnedAnimationCommand(const RenderPassObject& renderPassObject, RenderCommandList* renderCommandList, RenderCommandListStateCache& cache) noexcept
        {
            if ((renderPassObject.GetType() == RenderPassObject::Type::k_staticAnimated || renderPassObject.GetType() == RenderPassObject::Type::k_dynamicAnimated)
                && cache.m_skinnedAnimation != renderPassObject.GetRenderSkinnedAnimation())
            {
                cache.m_skinnedAnimation = renderPassObject.GetRenderSkinnedAnimation();
                
                renderCommandList->AddApplySkinnedAnimationCommand(cache.m_skinnedAnimation);
            }
        }
        
        /// Compiles the render commands for the given render pass. The render pass must contain
        /// render pass objects otherwise this will assert.
        ///
        /// @param renderPass
        ///     The render pass.
        /// @param renderCommandList
        ///     The render command list to add the commands to.
        /// @param renderFrameData
        ///     Container for all allocations which must live until the end of the frame.
        ///
        void CompileRenderCommandsForPass(const RenderPass& renderPass, RenderCommandList* renderCommandList) noexcept
        {
            AddApplyLightCommand(renderPass, renderCommandList);
            
            const auto& renderPassObjects = renderPass.GetRenderPassObjects();
            CS_ASSERT(renderPassObjects.size() > 0, "Cannot compile a pass with no objects.");
            
            RenderCommandListStateCache cache;
            SmallMeshBatcher batcher(renderCommandList);
            
            for (const auto& renderPassObject : renderPassObjects)
            {
                AddApplyMaterialCommand(renderPassObject, renderCommandList, cache, batcher);
                
                if (SmallMeshBatcher::CanBatch(renderPassObject))
                {
                    cache.m_mesh = nullptr;
                    cache.m_dynamicMesh = nullptr;
                
                    batcher.Batch(renderPassObject);
                }
                else
                {
                    batcher.Flush();
                    
                    AddApplyMeshCommand(renderPassObject, renderCommandList, cache);
                    AddApplySkinnedAnimationCommand(renderPassObject, renderCommandList, cache);
                    renderCommandList->AddRenderInstanceCommand(renderPassObject.GetWorldMatrix());
                }
                
            }
            
            batcher.Flush();
        }
    }
    
    //------------------------------------------------------------------------------
    RenderCommandBufferUPtr RenderCommandCompiler::CompileRenderCommands(const TaskContext& taskContext, IAllocator* frameAllocator, const std::vector<TargetRenderPassGroup>& targetRenderPassGroups, RenderCommandListUPtr preRenderCommandList,
                                                                         RenderCommandListUPtr postRenderCommandList, std::vector<RenderFrameData> renderFramesData) noexcept
    {
        u32 numLists = CalcNumRenderCommandLists(targetRenderPassGroups, preRenderCommandList.get(), postRenderCommandList.get());
        RenderCommandBufferUPtr renderCommandBuffer(new RenderCommandBuffer(numLists, frameAllocator, std::move(renderFramesData)));
        std::vector<Task> tasks;
        u32 currentList = 0;
        
        if (preRenderCommandList->GetOrderedList().size() > 0)
        {
            *renderCommandBuffer->GetRenderCommandList(currentList++) = std::move(*preRenderCommandList);
        }
        
        for (const auto& targetRenderPassGroup : targetRenderPassGroups)
        {
            AddBeginCommand(targetRenderPassGroup, renderCommandBuffer->GetRenderCommandList(currentList++));
                
            for (const auto& cameraRenderPassGroup : targetRenderPassGroup.GetRenderCameraGroups())
            {
                if (ContainsRenderPassObject(cameraRenderPassGroup))
                {
                    const auto& camera = cameraRenderPassGroup.GetCamera();
                    renderCommandBuffer->GetRenderCommandList(currentList)->AddApplyCameraCommand(camera.GetWorldMatrix().GetTranslation(), Matrix4::Inverse(camera.GetWorldMatrix()), camera.GetViewProjectionMatrix());
                    
                    for (const auto& renderPass : cameraRenderPassGroup.GetRenderPasses())
                    {
                        if (renderPass.GetRenderPassObjects().size() > 0)
                        {
                            auto renderCommandList = renderCommandBuffer->GetRenderCommandList(currentList++);
                            tasks.push_back([=, &renderPass, &renderCommandBuffer](const TaskContext& innerTaskContext)
                            {
                                CompileRenderCommandsForPass(renderPass, renderCommandList);
                            });
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
        
        if (tasks.size() > 0)
        {
            taskContext.ProcessChildTasks(tasks);
        }
        
        return renderCommandBuffer;
    }
}
