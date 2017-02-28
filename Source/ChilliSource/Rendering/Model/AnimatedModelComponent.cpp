//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Rendering/Model/AnimatedModelComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/Skeleton.h>

#include <algorithm>
#include <limits>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(AnimatedModelComponent);
    
    //------------------------------------------------------------------------------
    AnimatedModelComponent::AnimatedModelComponent(const ModelCSPtr& in_model, const MaterialCSPtr& material, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept
    :  m_model(in_model)
    {
        CS_ASSERT(in_model, "Model cannot be null");
        CS_ASSERT(in_model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(material, "Material cannot be null");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        m_materials.resize(in_model->GetNumMeshes());
        for (u32 i = 0; i < u32(m_materials.size()); ++i)
        {
            m_materials[i] = material;
        }
        
        Reset();
        SetAnimation(skinnedAnimation, playbackType);
    }
    
    //------------------------------------------------------------------------------
    AnimatedModelComponent::AnimatedModelComponent(const ModelCSPtr& model, const std::vector<MaterialCSPtr>& materials, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept
        : m_model(model), m_materials(materials)
    {
#if CS_ENABLE_DEBUG
        CS_ASSERT(m_model, "Model cannot be null");
        CS_ASSERT(m_model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(m_materials.size() == m_model->GetNumMeshes(), "Model component must have the same number of materials as there are meshes in the model.");
        
        for (const auto& material : m_materials)
        {
            CS_ASSERT(material, "Material cannot be null");
            CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        }
#endif
        
        Reset();
        SetAnimation(skinnedAnimation, playbackType);
    }
    
    //------------------------------------------------------------------------------
    bool AnimatedModelComponent::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return  (interfaceId == VolumeComponent::InterfaceID || interfaceId == AnimatedModelComponent::InterfaceID);
    }
    
    //------------------------------------------------------------------------------
    const AABB& AnimatedModelComponent::GetAABB() noexcept
    {
        if(GetEntity())
        {
            //Rebuild the box
            const AABB& cAABB = m_model->GetAABB();
            const Matrix4& matWorld = GetEntity()->GetTransform().GetWorldTransform();
            Vector3 vBackBottomLeft(cAABB.BackBottomLeft() * matWorld);
            Vector3 vBackBottomRight(cAABB.BackBottomRight() * matWorld);
            Vector3 vBackTopLeft(cAABB.BackTopLeft() * matWorld);
            Vector3 vBackTopRight(cAABB.BackTopRight() * matWorld);
            Vector3 vFrontBottomLeft(cAABB.FrontBottomLeft() * matWorld);
            Vector3 vFrontBottomRight(cAABB.FrontBottomRight() * matWorld);
            Vector3 vFrontTopLeft(cAABB.FrontTopLeft() *matWorld);
            Vector3 vFrontTopRight(cAABB.FrontTopRight() * matWorld);
            
            Vector3 vMin(std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity());
            vMin.x = std::min(vMin.x, vBackBottomLeft.x);
            vMin.x = std::min(vMin.x, vBackBottomRight.x);
            vMin.x = std::min(vMin.x, vBackTopLeft.x);
            vMin.x = std::min(vMin.x, vBackTopRight.x);
            vMin.x = std::min(vMin.x, vFrontBottomLeft.x);
            vMin.x = std::min(vMin.x, vFrontBottomRight.x);
            vMin.x = std::min(vMin.x, vFrontTopLeft.x);
            vMin.x = std::min(vMin.x, vFrontTopRight.x);
            
            vMin.y = std::min(vMin.y, vBackBottomLeft.y);
            vMin.y = std::min(vMin.y, vBackBottomRight.y);
            vMin.y = std::min(vMin.y, vBackTopLeft.y);
            vMin.y = std::min(vMin.y, vBackTopRight.y);
            vMin.y = std::min(vMin.y, vFrontBottomLeft.y);
            vMin.y = std::min(vMin.y, vFrontBottomRight.y);
            vMin.y = std::min(vMin.y, vFrontTopLeft.y);
            vMin.y = std::min(vMin.y, vFrontTopRight.y);
            
            vMin.z = std::min(vMin.z, vBackBottomLeft.z);
            vMin.z = std::min(vMin.z, vBackBottomRight.z);
            vMin.z = std::min(vMin.z, vBackTopLeft.z);
            vMin.z = std::min(vMin.z, vBackTopRight.z);
            vMin.z = std::min(vMin.z, vFrontBottomLeft.z);
            vMin.z = std::min(vMin.z, vFrontBottomRight.z);
            vMin.z = std::min(vMin.z, vFrontTopLeft.z);
            vMin.z = std::min(vMin.z, vFrontTopRight.z);
            
            Vector3 vMax(-std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity());
            vMax.x = std::max(vMax.x, vBackBottomLeft.x);
            vMax.x = std::max(vMax.x, vBackBottomRight.x);
            vMax.x = std::max(vMax.x, vBackTopLeft.x);
            vMax.x = std::max(vMax.x, vBackTopRight.x);
            vMax.x = std::max(vMax.x, vFrontBottomLeft.x);
            vMax.x = std::max(vMax.x, vFrontBottomRight.x);
            vMax.x = std::max(vMax.x, vFrontTopLeft.x);
            vMax.x = std::max(vMax.x, vFrontTopRight.x);
            
            vMax.y = std::max(vMax.y, vBackBottomLeft.y);
            vMax.y = std::max(vMax.y, vBackBottomRight.y);
            vMax.y = std::max(vMax.y, vBackTopLeft.y);
            vMax.y = std::max(vMax.y, vBackTopRight.y);
            vMax.y = std::max(vMax.y, vFrontBottomLeft.y);
            vMax.y = std::max(vMax.y, vFrontBottomRight.y);
            vMax.y = std::max(vMax.y, vFrontTopLeft.y);
            vMax.y = std::max(vMax.y, vFrontTopRight.y);
            
            vMax.z = std::max(vMax.z, vBackBottomLeft.z);
            vMax.z = std::max(vMax.z, vBackBottomRight.z);
            vMax.z = std::max(vMax.z, vBackTopLeft.z);
            vMax.z = std::max(vMax.z, vBackTopRight.z);
            vMax.z = std::max(vMax.z, vFrontBottomLeft.z);
            vMax.z = std::max(vMax.z, vFrontBottomRight.z);
            vMax.z = std::max(vMax.z, vFrontTopLeft.z);
            vMax.z = std::max(vMax.z, vFrontTopRight.z);
            
            m_aabb.SetSize( vMax - vMin );
            m_aabb.SetOrigin( cAABB.Centre() * matWorld);
            
        }
        
        return m_aabb;
    }
    
    //------------------------------------------------------------------------------
    const OOBB& AnimatedModelComponent::GetOOBB() noexcept
    {
        if(GetEntity())
        {
            m_oobb.SetTransform(GetEntity()->GetTransform().GetWorldTransform());
        }
        return m_oobb;
    }
    
    //------------------------------------------------------------------------------
    const Sphere& AnimatedModelComponent::GetBoundingSphere() noexcept
    {
        if(GetEntity())
        {
            const AABB& sAABB = GetAABB();
            m_boundingSphere.vOrigin = sAABB.GetOrigin();
            m_boundingSphere.fRadius = (sAABB.BackTopRight() - sAABB.FrontBottomLeft()).Length() * 0.5f;
        }
        return m_boundingSphere;
    }
    
    //------------------------------------------------------------------------------
    const ModelCSPtr& AnimatedModelComponent::GetModel() const noexcept
    {
        return m_model;
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetModel(const ModelCSPtr& model) noexcept
    {
        CS_ASSERT(model, "Cannot set null model.");
        CS_ASSERT(model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        
        m_model = model;
        m_materials.resize(model->GetNumMeshes());
        
        m_oobb.SetSize(m_model->GetAABB().GetSize());
        m_oobb.SetOrigin(m_model->GetAABB().GetOrigin());
        
        SetMaterial(GetMaterialForMesh(0));
        
        Reset();
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetModel(const ModelCSPtr& model, const MaterialCSPtr& material) noexcept
    {
        CS_ASSERT(model, "Cannot set null model.");
        CS_ASSERT(model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(material, "Cannot set null material.");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        m_model = model;
        m_materials.resize(m_model->GetNumMeshes());
        
        m_oobb.SetSize(m_model->GetAABB().GetSize());
        m_oobb.SetOrigin(m_model->GetAABB().GetOrigin());
        
        SetMaterial(material);
        
        Reset();
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetModel(const ModelCSPtr& model, const std::vector<MaterialCSPtr>& materials) noexcept
    {
#if CS_ENABLE_DEBUG
        CS_ASSERT(model, "Cannot set null model.");
        CS_ASSERT(model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(materials.size() == model->GetNumMeshes(), "Model component must have the same number of materials as there are meshes in the model.");
        
        for (const auto& material : materials)
        {
            CS_ASSERT(material, "Material cannot be null");
            CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        }
#endif
        m_model = model;
        m_materials = materials;
        
        m_oobb.SetSize(m_model->GetAABB().GetSize());
        m_oobb.SetOrigin(m_model->GetAABB().GetOrigin());
        
        Reset();
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetModel(const ModelCSPtr& model, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept
    {
        SetModel(model);
        SetAnimation(skinnedAnimation, playbackType);
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetModel(const ModelCSPtr& model, const MaterialCSPtr& material, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept
    {
        SetModel(model, material);
        SetAnimation(skinnedAnimation, playbackType);
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetModel(const ModelCSPtr& model, const std::vector<MaterialCSPtr>& materials, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept
    {
        SetModel(model, materials);
        SetAnimation(skinnedAnimation, playbackType);
    }
    
    //------------------------------------------------------------------------------
    const MaterialCSPtr& AnimatedModelComponent::GetMaterialForMesh(u32 meshIndex) const noexcept
    {
        CS_ASSERT(meshIndex < s32(m_materials.size()), "Invalid mesh index.");
        
        return m_materials[meshIndex];
    }
    
    //------------------------------------------------------------------------------
    const MaterialCSPtr& AnimatedModelComponent::GetMaterialForMesh(const std::string& meshName) const noexcept
    {
        CS_ASSERT(m_model, "Cannot get material without a model.");
        
        auto meshIndex = m_model->GetMeshIndex(meshName);
        CS_ASSERT(meshIndex >= 0 && meshIndex < s32(m_materials.size()), "Invalid mesh index.");
        
        return m_materials[meshIndex];
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetMaterial(const MaterialCSPtr& material) noexcept
    {
        CS_ASSERT(m_model, "Cannot set material without a model.");
        CS_ASSERT(material, "Cannot set null material.");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        for (u32 i = 0; i < m_materials.size(); i++)
        {
            m_materials[i] = material;
        }
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetMaterialForMesh(const MaterialCSPtr& material, u32 meshIndex) noexcept
    {
        CS_ASSERT(m_model, "Cannot set material without a model.");
        CS_ASSERT(material, "Cannot set null material.");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        CS_ASSERT(meshIndex < s32(m_materials.size()), "Invalid mesh index.");
        
        m_materials[meshIndex] = material;
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetMaterialForMesh(const MaterialCSPtr& material, const std::string& meshName) noexcept
    {
        CS_ASSERT(m_model, "Cannot set material without a model.");
        CS_ASSERT(material, "Cannot set null material.");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        auto meshIndex = m_model->GetMeshIndex(meshName);
        CS_ASSERT(meshIndex >= 0 && meshIndex < s32(m_materials.size()), "Invalid mesh index.");
        
        m_materials[meshIndex] = material;
    }
    
    //------------------------------------------------------------------------------
    std::vector<SkinnedAnimationCSPtr> AnimatedModelComponent::GetAnimations() const noexcept
    {
        CS_ASSERT(m_activeAnimationGroup, "There must be an active animation group.");
        
        std::vector<SkinnedAnimationCSPtr> output;
        m_activeAnimationGroup->GetAnimations(output);
        
        return output;
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetAnimation(const SkinnedAnimationCSPtr& animation, PlaybackType playbackType) noexcept
    {
        ClearAnimations();
        AttachAnimation(animation, 0.0f);
        SetPlaybackType(playbackType);
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::AttachAnimation(const SkinnedAnimationCSPtr& animation, f32 blendlinePosition) noexcept
    {
        CS_ASSERT(m_activeAnimationGroup, "There must be an active animation group.");
        
        m_activeAnimationGroup->AttachAnimation(animation, blendlinePosition);
        m_animationDataDirty = true;
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::DetatchAnimation(const SkinnedAnimationCSPtr& animation) noexcept
    {
        CS_ASSERT(m_activeAnimationGroup, "There must be an active animation group.");

        m_activeAnimationGroup->DetatchAnimation(animation);
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::FadeTo(const SkinnedAnimationCSPtr& animation, PlaybackType playbackType, AnimationBlendType fadeType, f32 fadeOutTime) noexcept
    {
        FadeOut(fadeType, fadeOutTime);
        AttachAnimation(animation, 0.0f);
        SetPlaybackType(playbackType);
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::FadeOut(AnimationBlendType fadeType, f32 fadeOutTime) noexcept
    {
        CS_ASSERT(m_activeAnimationGroup, "There must be an active animation group.");
        
        if (m_activeAnimationGroup->IsPrepared())
        {
            m_fadingAnimationGroup = m_activeAnimationGroup;
            m_activeAnimationGroup = SkinnedAnimationGroupSPtr(new SkinnedAnimationGroup(m_model->GetSkeleton()));
            m_fadePlaybackPosition = m_playbackPosition;
            m_fadeBlendlinePosition = m_blendlinePosition;
            m_maxFadeTime = fadeOutTime;
            m_fadeTimer = 0.0f;
            SetPlaybackPosition(0.0f);
            m_animationChangedEvent.NotifyConnections(this);
        }
        else
        {
            m_activeAnimationGroup->ClearAnimations();
        }
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::ClearAnimations() noexcept
    {
        CS_ASSERT(m_activeAnimationGroup, "There must be an active animation group.");

        m_activeAnimationGroup->ClearAnimations();
        m_fadingAnimationGroup.reset();
        SetPlaybackPosition(0.0f);
        m_blendlinePosition = 0.0f;
        m_fadeTimer = 0.0f;
        m_animationChangedEvent.NotifyConnections(this);
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetBlendlinePosition(f32 blendlinePosition) noexcept
    {
        m_blendlinePosition = blendlinePosition;
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetPlaybackType(PlaybackType playbackType) noexcept
    {
        m_playbackType = playbackType;
        m_finished = false;
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetPlaybackPosition(f32 position) noexcept
    {
        m_playbackPosition = position;
        m_finished = false;
        m_animationDataDirty = true;
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::SetPlaybackPositionNormalised(f32 position) noexcept
    {
        m_playbackPosition = position * GetAnimationLength();
        m_finished = false;
    }
    
    //------------------------------------------------------------------------------
    f32 AnimatedModelComponent::GetAnimationLength() const noexcept
    {
        CS_ASSERT(m_activeAnimationGroup, "There must be an active animation group.");
        
        return m_activeAnimationGroup->GetAnimationLength();
    }

    //------------------------------------------------------------------------------
    void AnimatedModelComponent::AttachEntity(const EntitySPtr& entity, const std::string& nodeName) noexcept
    {
        if (nullptr == GetEntity())
        {
            CS_LOG_ERROR("Could not attach entity to animated mesh because the mesh is not yet attached to an entity.");
            return;
        }
        
        if (nullptr != entity->GetParent() || nullptr != entity->GetScene())
        {
            CS_LOG_ERROR("Could not attach entity to animated mesh because the entity already has a parent.");
            return;
        }
        
        //check that it has not already been added.
        for (auto it = m_attachedEntities.begin(); it != m_attachedEntities.end(); ++it)
        {
            if (EntitySPtr attachedEntity = it->first.lock())
            {
                CS_ASSERT(attachedEntity.get() != entity.get(), "Entity is already attached.");
            }
        }
        
        s32 dwNodeIndex = m_model->GetSkeleton().GetNodeIndexByName(nodeName);
        CS_ASSERT(dwNodeIndex != -1, "Could not attach entity to the animated mesh because the skeleton node name could not be found.");

        GetEntity()->AddEntity(entity);
        m_attachedEntities.push_back(std::pair<EntityWPtr, s32>(EntityWPtr(entity), dwNodeIndex));
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::DetatchEntity(const Entity* inpEntity) noexcept
    {
        AttachedEntityList::iterator it;
        for (it = m_attachedEntities.begin(); it != m_attachedEntities.end(); ++it)
        {
            if (EntitySPtr pEntity = it->first.lock())
            {
                if (pEntity.get() == inpEntity)
                {
                    pEntity->RemoveFromParent();
                    break;
                }
            }
        }
        
        if (it != m_attachedEntities.end())
        {
            m_attachedEntities.erase(it);
        }
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::DetatchAllEntities() noexcept
    {
        for (AttachedEntityList::const_iterator it = m_attachedEntities.begin(); it != m_attachedEntities.end(); ++it)
        {
            if (EntitySPtr pEntity = it->first.lock())
            {
                pEntity->RemoveFromParent();
            }
        }
        
        m_attachedEntities.clear();
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::UpdateAnimation(f32 deltaTime) noexcept
    {
        CS_ASSERT(GetEntity(), "Must be attached to an entity.");
        CS_ASSERT(GetEntity()->GetScene(), "Must be attached to the scene.");
        CS_ASSERT(m_activeAnimationGroup, "Must have an active animation group.");
        CS_ASSERT(m_activeAnimationGroup->GetAnimationCount() > 0, "Must have at least one attached animation.");
        
        UpdateAnimationTimer(deltaTime);
        m_activeAnimationGroup->BuildAnimationData(m_animationBlendType, m_playbackPosition, m_blendlinePosition);
        
        //if there is a group fading out, then apply this to the active data.
        if (m_fadingAnimationGroup)
        {
            if (m_maxFadeTime > 0.0f && m_fadeTimer < m_maxFadeTime)
            {
                m_fadingAnimationGroup->BuildAnimationData(m_animationBlendType, m_fadePlaybackPosition, m_fadeBlendlinePosition);
                f32 fGroupBlendFactor = 1.0f - (m_fadeTimer / m_maxFadeTime);
                m_activeAnimationGroup->BlendGroup(m_animationBlendType, m_fadingAnimationGroup, fGroupBlendFactor);
            }
            else
            {
                m_fadingAnimationGroup.reset();
            }
        }
        
        m_activeAnimationGroup->BuildMatrices();
        UpdateAttachedEntities();
        
        m_animationDataDirty = false;
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::UpdateAnimationTimer(f32 deltaTime) noexcept
    {
        CS_ASSERT(m_activeAnimationGroup, "Must have an active animation group.");
        
        //Update the playback position
        m_playbackPosition += deltaTime * m_playbackSpeedMultiplier;
        
        switch (m_playbackType)
        {
            case PlaybackType::k_once:
            {
                if (m_playbackPosition >= m_activeAnimationGroup->GetAnimationLength())
                {
                    m_playbackPosition = m_activeAnimationGroup->GetAnimationLength();
                    m_finished = true;
                    m_animationCompletionEvent.NotifyConnections(this);
                }
                break;
            }
            case PlaybackType::k_looping:
            {
                while (m_playbackPosition >= m_activeAnimationGroup->GetAnimationLength() && m_activeAnimationGroup->GetAnimationLength() > 0.0f)
                {
                    m_playbackPosition -= m_activeAnimationGroup->GetAnimationLength();
                    m_animationLoopedEvent.NotifyConnections(this);
                }
                break;
            }
        }
        
        //update the fade timer
        if (m_fadingAnimationGroup)
        {
            m_fadeTimer += deltaTime;
        }
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::UpdateAttachedEntities() noexcept
    {
        CS_ASSERT(m_activeAnimationGroup, "Must have an active animation group.");

        for (AttachedEntityList::iterator it = m_attachedEntities.begin(); it != m_attachedEntities.end();)
        {
            if (EntitySPtr pEntity = it->first.lock())
            {
                s32 dwNodeIndex = it->second;
                
                const Matrix4& matTransform = m_activeAnimationGroup->GetMatrixAtIndex(dwNodeIndex);
                pEntity->GetTransform().SetLocalTransform(matTransform);
                ++it;
            }
            else
            {
                it = m_attachedEntities.erase(it);
            }
        }
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::Reset() noexcept
    {
        DetatchAllEntities();
        m_activeAnimationGroup = SkinnedAnimationGroupSPtr(new SkinnedAnimationGroup(m_model->GetSkeleton()));
        m_fadingAnimationGroup.reset();
        m_blendlinePosition = 0.0f;
        m_fadeTimer = 0.0f;
        SetPlaybackPosition(0.0f);
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::OnAddedToScene() noexcept
    {
        SetPlaybackPosition(0.0f);
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::OnUpdate(f32 deltaTime) noexcept
    {
        UpdateAnimation(deltaTime);
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        CS_ASSERT(m_model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(m_model->GetNumMeshes() == m_materials.size(), "Invalid number of materials.");
        CS_ASSERT(m_activeAnimationGroup, "An animated model must always have an active animation group.");
        
        if (m_animationDataDirty == true)
        {
            UpdateAnimation(0.0f);
        }
        
        for (u32 index = 0; index < m_model->GetNumMeshes(); ++index)
        {
            CS_ASSERT(m_materials[index]->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
            
            auto renderMaterialGroup = m_materials[index]->GetRenderMaterialGroup();
            auto renderMesh = m_model->GetRenderMesh(index);
            
            const auto& transform = GetEntity()->GetTransform();
            auto boundingSphere = Sphere::Transform(renderMesh->GetBoundingSphere(), transform.GetWorldPosition(), transform.GetWorldOrientation(), transform.GetWorldScale());
            
            RenderSkinnedAnimationAUPtr renderSkinnedAnimation;
            if (m_activeAnimationGroup->IsPrepared() == true)
            {
                renderSkinnedAnimation = m_activeAnimationGroup->BuildRenderSkinnedAnimation(frameAllocator, renderMesh->GetInverseBindPoseMatrices());
            }
            else if (m_fadingAnimationGroup != nullptr && m_fadingAnimationGroup->IsPrepared() == true)
            {
                renderSkinnedAnimation = m_fadingAnimationGroup->BuildRenderSkinnedAnimation(frameAllocator, renderMesh->GetInverseBindPoseMatrices());
            }
            
            CS_ASSERT(renderSkinnedAnimation, "No render skinned animation.");
            renderSnapshot.AddRenderObject(RenderObject(renderMaterialGroup, renderMesh, renderSkinnedAnimation.get(), GetEntity()->GetTransform().GetWorldTransform(), boundingSphere,
                                                           m_shadowCastingEnabled, RenderLayer::k_standard));
            renderSnapshot.AddRenderSkinnedAnimation(std::move(renderSkinnedAnimation));
        }
    }
    
    //------------------------------------------------------------------------------
    void AnimatedModelComponent::OnRemovedFromScene() noexcept
    {
        DetatchAllEntities();
    }
}
