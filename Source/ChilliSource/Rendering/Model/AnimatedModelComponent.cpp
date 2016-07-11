//
//  AnimatedModelComponent.cpp
//  Chilli Source
//  Created by Ian Copland on 17/10/2011.
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
    
    //----------------------------------------------------------
    /// Constructor
    //----------------------------------------------------------
    AnimatedModelComponent::AnimatedModelComponent(const ModelCSPtr& in_model, const MaterialCSPtr& in_material) noexcept
    : mfPlaybackPosition(0.0f), mfPlaybackSpeedMultiplier(1.0f), mfBlendlinePosition(0.0f),
    meBlendType(AnimationBlendType::k_linear), mePlaybackType(AnimationPlaybackType::k_once), meFadeType(AnimationBlendType::k_linear), mfFadeTimer(0.0f), mfFadeMaxTime(0.0f), mfFadePlaybackPosition(0.0f),
    mfFadeBlendlinePosition(0.0f), mbFinished(false), mbAnimationDataDirty(true), mpModel(in_model)
    {
        CS_ASSERT(mpModel, "Model cannot be null");
        CS_ASSERT(mpModel->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(in_material, "Material cannot be null");
        CS_ASSERT(in_material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        mMaterials.resize(mpModel->GetNumMeshes());
        
        for (u32 i = 0; i < u32(mMaterials.size()); ++i)
        {
            mMaterials[i] = in_material;
        }
    }
    //----------------------------------------------------------
    /// Is A
    //----------------------------------------------------------
    bool AnimatedModelComponent::IsA(InterfaceIDType inInterfaceID) const
    {
        return  (inInterfaceID == VolumeComponent::InterfaceID || inInterfaceID == AnimatedModelComponent::InterfaceID);
    }
    //----------------------------------------------------
    /// Get Axis Aligned Bounding Box
    //----------------------------------------------------
    const AABB& AnimatedModelComponent::GetAABB()
    {
        if(GetEntity())
        {
            //Rebuild the box
            const AABB& cAABB = mpModel->GetAABB();
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
            
            mBoundingBox.SetSize( vMax - vMin );
            mBoundingBox.SetOrigin( cAABB.Centre() * matWorld);
            
        }
        
        return mBoundingBox;
    }
    //----------------------------------------------------
    /// Get Object Oriented Bounding Box
    //----------------------------------------------------
    const OOBB& AnimatedModelComponent::GetOOBB()
    {
        if(GetEntity())
        {
            mOBBoundingBox.SetTransform(GetEntity()->GetTransform().GetWorldTransform());
        }
        return mOBBoundingBox;
    }
    //----------------------------------------------------
    /// Get Bounding Sphere
    //----------------------------------------------------
    const Sphere& AnimatedModelComponent::GetBoundingSphere()
    {
        if(GetEntity())
        {
            const AABB& sAABB = GetAABB();
            mBoundingSphere.vOrigin = sAABB.GetOrigin();
            mBoundingSphere.fRadius = (sAABB.BackTopRight() - sAABB.FrontBottomLeft()).Length() * 0.5f;
        }
        return mBoundingSphere;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void AnimatedModelComponent::SetMaterial(const MaterialCSPtr& in_material)
    {
        CS_ASSERT(mpModel, "Cannot set material without a model.");
        CS_ASSERT(in_material, "Cannot set null material.");
        CS_ASSERT(in_material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        for (u32 i = 0; i < mMaterials.size(); i++)
        {
            mMaterials[i] = in_material;
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void AnimatedModelComponent::SetMaterialForMesh(const MaterialCSPtr& in_material, u32 in_meshIndex)
    {
        CS_ASSERT(mpModel, "Cannot set material without a model.");
        CS_ASSERT(in_material, "Cannot set null material.");
        CS_ASSERT(in_material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        CS_ASSERT(in_meshIndex < s32(mMaterials.size()), "Invalid mesh index.");
        
        mMaterials[in_meshIndex] = in_material;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void AnimatedModelComponent::SetMaterialForMesh(const MaterialCSPtr& in_material, const std::string& in_meshName)
    {
        CS_ASSERT(mpModel, "Cannot set material without a model.");
        CS_ASSERT(in_material, "Cannot set null material.");
        CS_ASSERT(in_material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        auto meshIndex = mpModel->GetMeshIndex(in_meshName);
        CS_ASSERT(meshIndex >= 0 && meshIndex < s32(mMaterials.size()), "Invalid mesh index.");
        
        mMaterials[meshIndex] = in_material;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    const MaterialCSPtr& AnimatedModelComponent::GetMaterialForMesh(u32 in_meshIndex) const
    {
        CS_ASSERT(in_meshIndex < s32(mMaterials.size()), "Invalid mesh index.");
        
        return mMaterials[in_meshIndex];
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    const MaterialCSPtr& AnimatedModelComponent::GetMaterialForMesh(const std::string& in_meshName) const
    {
        CS_ASSERT(mpModel, "Cannot get material without a model.");
        
        auto meshIndex = mpModel->GetMeshIndex(in_meshName);
        CS_ASSERT(meshIndex >= 0 && meshIndex < s32(mMaterials.size()), "Invalid mesh index.");
        
        return mMaterials[meshIndex];
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void AnimatedModelComponent::SetModel(const ModelCSPtr& in_model)
    {
        CS_ASSERT(in_model, "Cannot set null model.");
        CS_ASSERT(in_model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        
        mpModel = in_model;
        mMaterials.resize(mpModel->GetNumMeshes());
        
        // Update OOBB
        mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
        mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
        
        SetMaterial(GetMaterialForMesh(0));
        
        Reset();
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void AnimatedModelComponent::SetModel(const ModelCSPtr& in_model, const MaterialCSPtr& in_material)
    {
        CS_ASSERT(in_model, "Cannot set null model.");
        CS_ASSERT(in_model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(in_material, "Cannot set null material.");
        CS_ASSERT(in_material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        mpModel = in_model;
        mMaterials.resize(mpModel->GetNumMeshes());
        
        // Update OOBB
        mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
        mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
        Reset();
        
        SetMaterial(in_material);
    }
    //----------------------------------------------------------
    /// Get Model
    //----------------------------------------------------------
    const ModelCSPtr& AnimatedModelComponent::GetModel() const
    {
        return mpModel;
    }
    //----------------------------------------------------------
    /// Attach Animation
    //----------------------------------------------------------
    void AnimatedModelComponent::AttachAnimation(const SkinnedAnimationCSPtr& inpAnimation, f32 infBlendlinePosition)
    {
        if (nullptr != mActiveAnimationGroup)
        {
            mActiveAnimationGroup->AttachAnimation(inpAnimation, infBlendlinePosition);
            mbAnimationDataDirty = true;
        }
    }
    //----------------------------------------------------------
    /// Get Animations
    //----------------------------------------------------------
    void AnimatedModelComponent::GetAnimations(std::vector<SkinnedAnimationCSPtr>& outapSkinnedAnimationList)
    {
        if (nullptr != mActiveAnimationGroup)
        {
            mActiveAnimationGroup->GetAnimations(outapSkinnedAnimationList);
        }
    }
    //----------------------------------------------------------
    /// Detatch Animation
    //----------------------------------------------------------
    void AnimatedModelComponent::DetatchAnimation(const SkinnedAnimationCSPtr& inpAnimation)
    {
        if (nullptr != mActiveAnimationGroup)
        {
            mActiveAnimationGroup->DetatchAnimation(inpAnimation);
        }
    }
    //----------------------------------------------------------
    /// Fade Out
    //----------------------------------------------------------
    void AnimatedModelComponent::FadeOut(AnimationBlendType ineFadeType, f32 infFadeOutTime)
    {
        if (nullptr != mActiveAnimationGroup && true == mActiveAnimationGroup->IsPrepared())
        {
            mFadingAnimationGroup = mActiveAnimationGroup;
            mActiveAnimationGroup = SkinnedAnimationGroupSPtr(new SkinnedAnimationGroup(mpModel->GetSkeleton()));
            mfFadePlaybackPosition = mfPlaybackPosition;
            mfFadeBlendlinePosition = mfBlendlinePosition;
            mfFadeMaxTime = infFadeOutTime;
            mfFadeTimer = 0.0f;
            SetPlaybackPosition(0.0f);
            mAnimationChangedEvent.NotifyConnections(this);
        }
        else if (nullptr != mActiveAnimationGroup)
        {
            mActiveAnimationGroup->ClearAnimations();
        }
    }
    //----------------------------------------------------------
    /// Clear Animations
    //----------------------------------------------------------
    void AnimatedModelComponent::ClearAnimations()
    {
        if (mActiveAnimationGroup != nullptr)
        {
            mActiveAnimationGroup->ClearAnimations();
            mFadingAnimationGroup.reset();
            SetPlaybackPosition(0.0f);
            mfBlendlinePosition = 0.0f;
            mfFadeTimer = 0.0f;
            mAnimationChangedEvent.NotifyConnections(this);
        }
    }
    //----------------------------------------------------------
    /// Attach Entity
    //----------------------------------------------------------
    void AnimatedModelComponent::AttachEntity(const EntitySPtr& inpEntity, const std::string& instrNodeName)
    {
        if (nullptr == GetEntity())
        {
            CS_LOG_ERROR("Could not attach entity to animated mesh because the mesh is not yet attached to an entity.");
            return;
        }
        
        if (nullptr != inpEntity->GetParent() || nullptr != inpEntity->GetScene())
        {
            CS_LOG_ERROR("Could not attach entity to animated mesh because the entity already has a parent.");
            return;
        }
        
        //check that it has not already been added.
        for (AttachedEntityList::const_iterator it = maAttachedEntities.begin(); it != maAttachedEntities.end(); ++it)
        {
            if (EntitySPtr pEntity = it->first.lock())
            {
                if (pEntity.get() == inpEntity.get())
                {
                    return;
                }
            }
        }
        
        s32 dwNodeIndex = mpModel->GetSkeleton().GetNodeIndexByName(instrNodeName);
        if (dwNodeIndex == -1)
        {
            CS_LOG_ERROR("Could not attach entity to the animated mesh because the skeleton node name could not be found.");
            return;
        }
        
        GetEntity()->AddEntity(inpEntity);
        maAttachedEntities.push_back(std::pair<EntityWPtr, s32>(EntityWPtr(inpEntity), dwNodeIndex));
    }
    //----------------------------------------------------------
    /// Detatch Entity
    //----------------------------------------------------------
    void AnimatedModelComponent::DetatchEntity(Entity* inpEntity)
    {
        AttachedEntityList::iterator it;
        for (it = maAttachedEntities.begin(); it != maAttachedEntities.end(); ++it)
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
        
        if (it != maAttachedEntities.end())
        {
            maAttachedEntities.erase(it);
        }
    }
    //----------------------------------------------------------
    /// Detatch All Entities
    //----------------------------------------------------------
    void AnimatedModelComponent::DetatchAllEntities()
    {
        for (AttachedEntityList::const_iterator it = maAttachedEntities.begin(); it != maAttachedEntities.end(); ++it)
        {
            if (EntitySPtr pEntity = it->first.lock())
            {
                pEntity->RemoveFromParent();
            }
        }
        
        maAttachedEntities.clear();
    }
    //----------------------------------------------------------
    /// Set Blendline Position
    //----------------------------------------------------------
    void AnimatedModelComponent::SetBlendlinePosition(f32 infBlendlinePosition)
    {
        mfBlendlinePosition = infBlendlinePosition;
    }
    //----------------------------------------------------------
    /// Set Playback Type
    //----------------------------------------------------------
    void AnimatedModelComponent::SetPlaybackType(AnimationPlaybackType inePlaybackType)
    {
        mePlaybackType = inePlaybackType;
        mbFinished = false;
    }
    //----------------------------------------------------------
    /// Set Playback Speed Multiplier
    //----------------------------------------------------------
    void AnimatedModelComponent::SetPlaybackSpeedMultiplier(f32 infSpeedMultiplier)
    {
        mfPlaybackSpeedMultiplier = infSpeedMultiplier;
    }
    //----------------------------------------------------------
    /// Set Playback Position
    //----------------------------------------------------------
    void AnimatedModelComponent::SetPlaybackPosition(f32 infPosition)
    {
        mfPlaybackPosition = infPosition;
        mbFinished = false;
        mbAnimationDataDirty = true;
    }
    //----------------------------------------------------------
    /// Set Playback Position Normalised
    //----------------------------------------------------------
    void AnimatedModelComponent::SetPlaybackPositionNormalised(f32 infPosition)
    {
        mfPlaybackPosition = infPosition * GetAnimationLength();
        mbFinished = false;
    }
    //----------------------------------------------------------
    /// Set Blend Type
    //----------------------------------------------------------
    void AnimatedModelComponent::SetBlendType(AnimationBlendType ineBlendType)
    {
        meBlendType = ineBlendType;
    }
    //----------------------------------------------------------
    /// Get Animation Changed Event
    //----------------------------------------------------------
    AnimationChangedEvent& AnimatedModelComponent::GetAnimationChangedEvent()
    {
        return mAnimationChangedEvent;
    }
    //----------------------------------------------------------
    /// Get Animation Completion Event
    //----------------------------------------------------------
    AnimationCompletionEvent& AnimatedModelComponent::GetAnimationCompletionEvent()
    {
        return mAnimationCompletionEvent;
    }
    //----------------------------------------------------------
    /// Get Animation Looped Event
    //----------------------------------------------------------
    AnimationLoopedEvent& AnimatedModelComponent::GetAnimationLoopedEvent()
    {
        return mAnimationLoopedEvent;
    }
    //----------------------------------------------------------
    /// Get Blendline Position
    //----------------------------------------------------------
    f32 AnimatedModelComponent::GetBlendlinePosition() const
    {
        return mfBlendlinePosition;
    }
    //----------------------------------------------------------
    /// Get Playback Type
    //----------------------------------------------------------
    AnimationPlaybackType AnimatedModelComponent::GetPlaybackType() const
    {
        return mePlaybackType;
    }
    //----------------------------------------------------------
    /// Get Animation Length
    //----------------------------------------------------------
    f32 AnimatedModelComponent::GetAnimationLength()
    {
        if (mActiveAnimationGroup != nullptr)
        {
            return mActiveAnimationGroup->GetAnimationLength();
        }
        
        return 0;
    }
    //----------------------------------------------------------
    /// Get Playback Speed Multiplier
    //----------------------------------------------------------
    f32 AnimatedModelComponent::GetPlaybackSpeedMultiplier() const
    {
        return mfPlaybackSpeedMultiplier;
    }
    //----------------------------------------------------------
    /// Get Playback Position
    //----------------------------------------------------------
    f32 AnimatedModelComponent::GetPlaybackPosition() const
    {
        return mfPlaybackPosition;
    }
    //----------------------------------------------------------
    /// get Playback Position Normalised
    //----------------------------------------------------------
    f32 AnimatedModelComponent::GetPlaybackPositionNormalised()
    {
        return mfPlaybackPosition / GetAnimationLength();
    }
    //----------------------------------------------------------
    /// Get Blend Type
    //----------------------------------------------------------
    AnimationBlendType AnimatedModelComponent::GetBlendType() const
    {
        return meBlendType;
    }
    //----------------------------------------------------------
    /// Has Finished
    //----------------------------------------------------------
    bool AnimatedModelComponent::HasFinished() const
    {
        return mbFinished;
    }
    //-----------------------------------------------------
    //-----------------------------------------------------
    void AnimatedModelComponent::SetShadowCastingEnabled(bool inbEnabled)
    {
        m_shadowCastingEnabled = inbEnabled;
    }
    //-----------------------------------------------------
    //-----------------------------------------------------
    bool AnimatedModelComponent::IsShadowCastingEnabled() const
    {
        return m_shadowCastingEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void AnimatedModelComponent::OnUpdate(f32 infDeltaTime)
    {
        UpdateAnimation(infDeltaTime);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void AnimatedModelComponent::OnRenderSnapshot(RenderSnapshot& in_renderSnapshot) noexcept
    {
        CS_ASSERT(mActiveAnimationGroup, "An animated model must always have an active animation group.");
        CS_ASSERT(mpModel->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(mpModel->GetNumMeshes() == mMaterials.size(), "Invalid number of materials.");
        
        if (mbAnimationDataDirty == true)
        {
            UpdateAnimation(0.0f);
        }
        
        for (u32 index = 0; index < mpModel->GetNumMeshes(); ++index)
        {
            CS_ASSERT(mMaterials[index]->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
            
            auto renderMaterialGroup = mMaterials[index]->GetRenderMaterialGroup();
            auto renderMesh = mpModel->GetRenderMesh(index);
            
            const auto& transform = GetEntity()->GetTransform();
            auto boundingSphere = Sphere::Transform(renderMesh->GetBoundingSphere(), transform.GetWorldPosition(), transform.GetWorldScale());
            
            RenderSkinnedAnimationAUPtr renderSkinnedAnimation;
            if (mActiveAnimationGroup->IsPrepared() == true)
            {
                renderSkinnedAnimation = mActiveAnimationGroup->BuildRenderSkinnedAnimation(in_renderSnapshot.GetFrameAllocator(), renderMesh->GetInverseBindPoseMatrices());
            }
            else if (mFadingAnimationGroup != nullptr && mFadingAnimationGroup->IsPrepared() == true)
            {
                renderSkinnedAnimation = mFadingAnimationGroup->BuildRenderSkinnedAnimation(in_renderSnapshot.GetFrameAllocator(), renderMesh->GetInverseBindPoseMatrices());
            }
            
            CS_ASSERT(renderSkinnedAnimation, "No render skinned animation.");
            in_renderSnapshot.AddRenderObject(RenderObject(renderMaterialGroup, renderMesh, renderSkinnedAnimation.get(), GetEntity()->GetTransform().GetWorldTransform(), boundingSphere,
                                                           m_shadowCastingEnabled, RenderLayer::k_standard));
            in_renderSnapshot.AddRenderSkinnedAnimation(std::move(renderSkinnedAnimation));
        }
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    AnimatedModelComponent::~AnimatedModelComponent()
    {
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void AnimatedModelComponent::OnAddedToScene()
    {
        SetPlaybackPosition(0.0f);
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void AnimatedModelComponent::OnRemovedFromScene()
    {
        DetatchAllEntities();
    }
    //----------------------------------------------------------
    /// Update Animation
    //----------------------------------------------------------
    void AnimatedModelComponent::UpdateAnimation(f32 infDeltaTime)
    {
        if (nullptr != GetEntity() && nullptr != GetEntity()->GetScene() && nullptr != mActiveAnimationGroup && mActiveAnimationGroup->GetAnimationCount() != 0)
        {
            //update the animation timer.
            UpdateAnimationTimer(infDeltaTime);
            
            //calculate the animation data and convert to matrices.
            mActiveAnimationGroup->BuildAnimationData(meBlendType, mfPlaybackPosition, mfBlendlinePosition);
            
            //if there is a group fading out, then apply this to the active data.
            if (nullptr != mFadingAnimationGroup)
            {
                if (mfFadeMaxTime > 0.0f && mfFadeTimer < mfFadeMaxTime)
                {
                    mFadingAnimationGroup->BuildAnimationData(meBlendType, mfFadePlaybackPosition, mfFadeBlendlinePosition);
                    f32 fGroupBlendFactor = 1.0f - (mfFadeTimer / mfFadeMaxTime);
                    mActiveAnimationGroup->BlendGroup(meBlendType, mFadingAnimationGroup, fGroupBlendFactor);
                }
                else
                {
                    mFadingAnimationGroup = SkinnedAnimationGroupSPtr();
                }
            }
            mActiveAnimationGroup->BuildMatrices();
            UpdateAttachedEntities();
            
            mbAnimationDataDirty = false;
        }
    }
    //----------------------------------------------------------
    /// Update Animation Timer
    //----------------------------------------------------------
    void AnimatedModelComponent::UpdateAnimationTimer(f32 infDeltaTime)
    {
        if (nullptr != mActiveAnimationGroup)
        {
            //Update the playback position
            mfPlaybackPosition += infDeltaTime * mfPlaybackSpeedMultiplier;
            
            switch (mePlaybackType)
            {
                case AnimationPlaybackType::k_once:
                {
                    if (mfPlaybackPosition >= mActiveAnimationGroup->GetAnimationLength())
                    {
                        mfPlaybackPosition = mActiveAnimationGroup->GetAnimationLength();
                        mbFinished = true;
                        mAnimationCompletionEvent.NotifyConnections(this);
                    }
                    break;
                }
                case AnimationPlaybackType::k_looping:
                {
                    while (mfPlaybackPosition >= mActiveAnimationGroup->GetAnimationLength() && mActiveAnimationGroup->GetAnimationLength() > 0.0f)
                    {
                        mfPlaybackPosition -= mActiveAnimationGroup->GetAnimationLength();
                        mAnimationLoopedEvent.NotifyConnections(this);
                    }
                    break;
                }
            }
            
            //update the fade timer
            if (nullptr != mFadingAnimationGroup)
            {
                mfFadeTimer += infDeltaTime;
            }
        }
    }
    //----------------------------------------------------------
    /// Update Attached Entities
    //----------------------------------------------------------
    void AnimatedModelComponent::UpdateAttachedEntities()
    {
        if (nullptr != mActiveAnimationGroup)
        {
            for (AttachedEntityList::iterator it = maAttachedEntities.begin(); it != maAttachedEntities.end();)
            {
                if (EntitySPtr pEntity = it->first.lock())
                {
                    s32 dwNodeIndex = it->second;
                    
                    const Matrix4& matTransform = mActiveAnimationGroup->GetMatrixAtIndex(dwNodeIndex);
                    pEntity->GetTransform().SetLocalTransform(matTransform);
                    ++it;
                }
                else
                {
                    it = maAttachedEntities.erase(it);
                }
            }
        }
    }
    //----------------------------------------------------------
    /// Reset
    //----------------------------------------------------------
    void AnimatedModelComponent::Reset()
    {
        DetatchAllEntities();
        mActiveAnimationGroup = SkinnedAnimationGroupSPtr(new SkinnedAnimationGroup(mpModel->GetSkeleton()));
        mFadingAnimationGroup.reset();
        mfBlendlinePosition = 0.0f;
        mfFadeTimer = 0.0f;
        SetPlaybackPosition(0.0f);
    }
}
