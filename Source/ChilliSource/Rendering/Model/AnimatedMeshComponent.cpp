/*
 *  AnimatedMeshComponent.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 17/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/AnimatedMeshComponent.h>
#include <ChilliSource/Rendering/Model/AnimatedMeshComponentUpdater.h>
#include <ChilliSource/Rendering/Lighting/LightComponent.h>
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>
#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>

#include <ChilliSource/Core/Entity/Entity.h>

#include <ChilliSource/Rendering/Model/SubMesh.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(AnimatedMeshComponent);
        
        MaterialSPtr AnimatedMeshComponent::mspShadowMapMaterial;
        
		//----------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------
		AnimatedMeshComponent::AnimatedMeshComponent() 
        : mfPlaybackPosition(0.0f), mfPlaybackSpeedMultiplier(1.0f), mfBlendlinePosition(0.0f),
        meBlendType(AnimationBlendType::k_linear), mePlaybackType(AnimationPlaybackType::k_once), meFadeType(AnimationBlendType::k_linear), mfFadeTimer(0.0f), mfFadeMaxTime(0.0f), mfFadePlaybackPosition(0.0f),
        mfFadeBlendlinePosition(0.0f), mbFinished(false), mbAnimationDataDirty(true)
		{
            mMaterials.push_back(mpMaterial);
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool AnimatedMeshComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return  (inInterfaceID == AnimatedMeshComponent::InterfaceID) ||
                    (inInterfaceID == RenderComponent::InterfaceID) ||
                    (inInterfaceID == VolumeComponent::InterfaceID);
		}
		//----------------------------------------------------
		/// Get Axis Aligned Bounding Box
		//----------------------------------------------------
		const Core::AABB& AnimatedMeshComponent::GetAABB()
		{
			if(GetEntity())
			{
				//Rebuild the box
                const Core::AABB& cAABB = mpModel->GetAABB();
                const Core::Matrix4x4& matWorld = GetEntity()->GetTransform().GetWorldTransform();
                Core::Vector3 vBackBottomLeft(cAABB.BackBottomLeft() * matWorld);
                Core::Vector3 vBackBottomRight(cAABB.BackBottomRight() * matWorld);
                Core::Vector3 vBackTopLeft(cAABB.BackTopLeft() * matWorld);
                Core::Vector3 vBackTopRight(cAABB.BackTopRight() * matWorld);
                Core::Vector3 vFrontBottomLeft(cAABB.FrontBottomLeft() * matWorld);
                Core::Vector3 vFrontBottomRight(cAABB.FrontBottomRight() * matWorld);
                Core::Vector3 vFrontTopLeft(cAABB.FrontTopLeft() *matWorld);
                Core::Vector3 vFrontTopRight(cAABB.FrontTopRight() * matWorld);
                
                Core::Vector3 vMin(std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity());
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
                
                Core::Vector3 vMax(-std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity());
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
		const Core::OOBB& AnimatedMeshComponent::GetOOBB()
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
		const Core::Sphere& AnimatedMeshComponent::GetBoundingSphere()
		{
			if(GetEntity())
			{
                const Core::AABB& sAABB = GetAABB();
                Core::Vector3 vSize = sAABB.GetSize();
				mBoundingSphere.vOrigin = sAABB.GetOrigin();
				mBoundingSphere.fRadius = std::max(vSize.x, vSize.y) * 0.5f;
			}
			return mBoundingSphere;
		}
		//-----------------------------------------------------------
		/// Is Transparent
		//-----------------------------------------------------------
		bool AnimatedMeshComponent::IsTransparent()
		{
			for (u32 i = 0; i < mMaterials.size(); ++i)
			{
				if (mMaterials[i]->IsTransparent() == true)
					return true;
			}
			return false;
		}
		//-----------------------------------------------------------
		/// Set Material
		//-----------------------------------------------------------
		void AnimatedMeshComponent::SetMaterial(const MaterialSPtr& inpMaterial)
		{
			mpMaterial = inpMaterial;
			
			//apply to all materials
			for (u32 i = 0; i < mMaterials.size(); i++)
            {
				mMaterials[i] = mpMaterial;
            }
		}
		//-----------------------------------------------------------
		/// Set Material For Sub Mesh
		//-----------------------------------------------------------
		void AnimatedMeshComponent::SetMaterialForSubMesh(const MaterialSPtr& inpMaterial, u32 indwSubMeshIndex)
		{
			if (indwSubMeshIndex < mMaterials.size())
			{
				mMaterials[indwSubMeshIndex] = inpMaterial;
				
				if (indwSubMeshIndex == 0)
                {
					mpMaterial = inpMaterial;
                }
			}
		}
        //-----------------------------------------------------------
        /// Set Material For Sub Mesh
        //-----------------------------------------------------------
        void AnimatedMeshComponent::SetMaterialForSubMesh(const MaterialSPtr& inpMaterial, const std::string& instrSubMeshName)
        {
            if (nullptr != mpModel)
            {
                s32 indwIndex = mpModel->GetSubMeshIndexByName(instrSubMeshName);
                if (indwIndex >= 0 && indwIndex < (s32)mMaterials.size())
                {
                    mMaterials[indwIndex] = inpMaterial;
                    
                    if (indwIndex == 0)
                    {
                        mpMaterial = inpMaterial;
                    }
                }
            }
        }
		//-----------------------------------------------------------
		/// Get Material Of Sub Mesh
		//-----------------------------------------------------------
		const MaterialSPtr AnimatedMeshComponent::GetMaterialOfSubMesh(u32 indwSubMeshIndex) const
		{
			if (indwSubMeshIndex < mMaterials.size())
			{
				return mMaterials[indwSubMeshIndex];
			}
			
            CS_LOG_ERROR("Failed to get material from sub mesh " + Core::ToString(indwSubMeshIndex));
			return MaterialSPtr();
		}
        //-----------------------------------------------------------
        /// Get Material Of Sub Mesh
        //-----------------------------------------------------------
        MaterialSPtr AnimatedMeshComponent::GetMaterialOfSubMesh(const std::string& instrSubMeshName) const
        {
            if (nullptr != mpModel)
            {
                s32 indwIndex = mpModel->GetSubMeshIndexByName(instrSubMeshName);
                
                if (indwIndex >= 0 && indwIndex < (s32)mMaterials.size())
                {
                    return mMaterials[indwIndex];
                }
            }
			
            CS_LOG_ERROR("Failed to get material from sub mesh " + instrSubMeshName);
			return MaterialSPtr();
        }
		//----------------------------------------------------------
		/// Attach Mesh
		//----------------------------------------------------------
		void AnimatedMeshComponent::AttachMesh(const MeshSPtr& inpModel)
		{
			mpModel = inpModel;
            // Update OOBB
            mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
            mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
            
            Reset();
            
            //ensure we have the correct number of materials.
            while (mMaterials.size() > inpModel->GetNumSubMeshes() && mMaterials.size() > 1)
            {
                mMaterials.pop_back();
            }
            while (mMaterials.size() < inpModel->GetNumSubMeshes())
            {
                mMaterials.push_back(MaterialSPtr());
            }
			
			ApplyDefaultMaterials();
		}
        //----------------------------------------------------------
        /// Attach Mesh
        ///
        /// Attach a mesh to this component but uses the given 
        /// material
        /// @param Mesh object
        //----------------------------------------------------------
        void AnimatedMeshComponent::AttachMesh(const MeshSPtr& inpModel, const MaterialSPtr& inpMaterial)
        {
            mpModel = inpModel;
			mpMaterial = inpMaterial;
            
            // Update OOBB
            mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
            mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
            Reset();
            
            //ensure we have the correct number of materials.
            while (mMaterials.size() > inpModel->GetNumSubMeshes() && mMaterials.size() > 1)
            {
                mMaterials.pop_back();
            }
            while (mMaterials.size() < inpModel->GetNumSubMeshes())
            {
                mMaterials.push_back(MaterialSPtr());
            }
            
            SetMaterial(inpMaterial);
        }
        //----------------------------------------------------------
        /// Get Mesh
        //----------------------------------------------------------
        const MeshSPtr& AnimatedMeshComponent::GetMesh() const
        {
            return mpModel;
        }
		//----------------------------------------------------------
        /// Attach Animation
        //----------------------------------------------------------
        void AnimatedMeshComponent::AttachAnimation(Core::StorageLocation ineStorageLocation, const std::string& instrAnimation, f32 infBlendlinePosition)
        {
            AttachAnimation(LOAD_RESOURCE(SkinnedAnimation, ineStorageLocation, instrAnimation), infBlendlinePosition);
        }
        //----------------------------------------------------------
        /// Attach Animation
        //----------------------------------------------------------
        void AnimatedMeshComponent::AttachAnimation(const SkinnedAnimationSPtr& inpAnimation, f32 infBlendlinePosition)
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
        void AnimatedMeshComponent::GetAnimations(std::vector<SkinnedAnimationSPtr>& outapSkinnedAnimationList)
        {
            if (nullptr != mActiveAnimationGroup)
            {
                mActiveAnimationGroup->GetAnimations(outapSkinnedAnimationList);
            }
        }
        //----------------------------------------------------------
        /// Detatch Animation
        //----------------------------------------------------------
        void AnimatedMeshComponent::DetatchAnimation(const SkinnedAnimationSPtr& inpAnimation)
        {
            if (nullptr != mActiveAnimationGroup)
            {
                mActiveAnimationGroup->DetatchAnimation(inpAnimation);
            }
        }
        //----------------------------------------------------------
        /// Fade Out
        //----------------------------------------------------------
        void AnimatedMeshComponent::FadeOut(AnimationBlendType ineFadeType, f32 infFadeOutTime)
        {
            if (nullptr != mActiveAnimationGroup && true == mActiveAnimationGroup->IsPrepared())
            {
                mFadingAnimationGroup = mActiveAnimationGroup;
                mActiveAnimationGroup = SkinnedAnimationGroupSPtr(new SkinnedAnimationGroup(mpModel->GetSkeletonPtr()));
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
        void AnimatedMeshComponent::ClearAnimations()
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
        void AnimatedMeshComponent::AttachEntity(const Core::EntitySPtr& inpEntity, const std::string& instrNodeName)
        {
            if (nullptr == GetEntity())
            {
                CS_LOG_ERROR("Could not attach entity to animated mesh becuase the mesh is not yet attached to an entity.");
                return;
            }
            
            if (nullptr != inpEntity->GetParent() || nullptr != inpEntity->GetScene())
            {
                CS_LOG_ERROR("Could not attach entity to animated mesh becuase the entity already has a parent.");
                return;
            }
            
            //check that it has not already been added.
            for (AttachedEntityList::const_iterator it = maAttachedEntities.begin(); it != maAttachedEntities.end(); ++it)
            {
                if (Core::EntitySPtr pEntity = it->first.lock())
                {
                    if (pEntity.get() == inpEntity.get())
                    {
                        return;
                    }
                }
            }
            
            s32 dwNodeIndex = mpModel->GetSkeletonPtr()->GetNodeIndexByName(instrNodeName);
            if (dwNodeIndex == -1)
            {
                CS_LOG_ERROR("Could not attach entity to the animated mesh becuase the skeleton node name could not be found.");
                return;
            }
            
            GetEntity()->Add(inpEntity);
            maAttachedEntities.push_back(std::pair<Core::EntityWPtr, s32>(Core::EntityWPtr(inpEntity), dwNodeIndex));
        }
        //----------------------------------------------------------
        /// Detatch Entity
        //----------------------------------------------------------
        void AnimatedMeshComponent::DetatchEntity(Core::Entity* inpEntity)
        {
            AttachedEntityList::iterator it;
            for (it = maAttachedEntities.begin(); it != maAttachedEntities.end(); ++it)
            {
                if (Core::EntitySPtr pEntity = it->first.lock())
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
        void AnimatedMeshComponent::DetatchAllEntities()
        {
            for (AttachedEntityList::const_iterator it = maAttachedEntities.begin(); it != maAttachedEntities.end(); ++it)
            {
                if (Core::EntitySPtr pEntity = it->first.lock())
                {
                    pEntity->RemoveFromParent();
                }
            }
            
            maAttachedEntities.clear();
        }
        //----------------------------------------------------------
        /// Set Blendline Position
        //----------------------------------------------------------
        void AnimatedMeshComponent::SetBlendlinePosition(f32 infBlendlinePosition)
        {
            mfBlendlinePosition = infBlendlinePosition;
        }
        //----------------------------------------------------------
        /// Set Playback Type
        //----------------------------------------------------------
        void AnimatedMeshComponent::SetPlaybackType(AnimationPlaybackType inePlaybackType)
        {
            mePlaybackType = inePlaybackType;
            mbFinished = false;
        }
        //----------------------------------------------------------
        /// Set Playback Speed Multiplier
        //----------------------------------------------------------
        void AnimatedMeshComponent::SetPlaybackSpeedMultiplier(f32 infSpeedMultiplier)
        {
            mfPlaybackSpeedMultiplier = infSpeedMultiplier;
        }
        //----------------------------------------------------------
        /// Set Playback Position
        //----------------------------------------------------------
        void AnimatedMeshComponent::SetPlaybackPosition(f32 infPosition)
        {
            mfPlaybackPosition = infPosition;
            mbFinished = false;
            mbAnimationDataDirty = true;
        }
        //----------------------------------------------------------
        /// Set Playback Position Normalised
        //----------------------------------------------------------
        void AnimatedMeshComponent::SetPlaybackPositionNormalised(f32 infPosition)
        {
            mfPlaybackPosition = infPosition * GetAnimationLength();
            mbFinished = false;
        }
        //----------------------------------------------------------
        /// Set Blend Type
        //----------------------------------------------------------
        void AnimatedMeshComponent::SetBlendType(AnimationBlendType ineBlendType)
        {
            meBlendType = ineBlendType;
        }
        //----------------------------------------------------------
        /// Get Animation Changed Event
        //----------------------------------------------------------
        AnimationChangedEvent& AnimatedMeshComponent::GetAnimationChangedEvent()
        {
            return mAnimationChangedEvent;
        }
        //----------------------------------------------------------
        /// Get Animation Completion Event
        //----------------------------------------------------------
        AnimationCompletionEvent& AnimatedMeshComponent::GetAnimationCompletionEvent()
        {
            return mAnimationCompletionEvent;
        }
        //----------------------------------------------------------
        /// Get Animation Looped Event
        //----------------------------------------------------------
        AnimationLoopedEvent& AnimatedMeshComponent::GetAnimationLoopedEvent()
        {
            return mAnimationLoopedEvent;
        }
        //----------------------------------------------------------
        /// Get Blendline Position
        //----------------------------------------------------------
        f32 AnimatedMeshComponent::GetBlendlinePosition() const
        {
            return mfBlendlinePosition;
        }
        //----------------------------------------------------------
        /// Get Playback Type
        //----------------------------------------------------------
        AnimationPlaybackType AnimatedMeshComponent::GetPlaybackType() const
        {
            return mePlaybackType;
        }
        //----------------------------------------------------------
        /// Get Animation Length
        //----------------------------------------------------------
        f32 AnimatedMeshComponent::GetAnimationLength()
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
        f32 AnimatedMeshComponent::GetPlaybackSpeedMultiplier() const
        {
            return mfPlaybackSpeedMultiplier;
        }
        //----------------------------------------------------------
        /// Get Playback Position
        //----------------------------------------------------------
        f32 AnimatedMeshComponent::GetPlaybackPosition() const
        {
            return mfPlaybackPosition;
        }
        //----------------------------------------------------------
        /// get Playback Position Normalised
        //----------------------------------------------------------
        f32 AnimatedMeshComponent::GetPlaybackPositionNormalised()
        {
            return mfPlaybackPosition / GetAnimationLength();
        }
        //----------------------------------------------------------
        /// Get Blend Type
        //----------------------------------------------------------
        AnimationBlendType AnimatedMeshComponent::GetBlendType() const
        {
            return meBlendType;
        }
        //----------------------------------------------------------
        /// Has Finished
        //----------------------------------------------------------
        bool AnimatedMeshComponent::HasFinished() const
        {
            return mbFinished;
        }
        //----------------------------------------------------------
        /// Update
        //----------------------------------------------------------
        void AnimatedMeshComponent::Update(f32 infDeltaTime)
        {
            UpdateAnimation(infDeltaTime);
        }
        //----------------------------------------------------------
		/// Destructor
		//----------------------------------------------------------
		AnimatedMeshComponent::~AnimatedMeshComponent()
		{
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void AnimatedMeshComponent::OnAddedToScene()
		{
            SetPlaybackPosition(0.0f);
            Core::Application::Get()->GetSystem_Old<AnimatedMeshComponentUpdater>()->AddComponent(this);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void AnimatedMeshComponent::OnRemovedFromScene()
        {
            DetatchAllEntities();
            Core::Application::Get()->GetSystem_Old<AnimatedMeshComponentUpdater>()->RemoveComponent(this);
        }
		//----------------------------------------------------------
        /// Render
        //----------------------------------------------------------
        void AnimatedMeshComponent::Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass ineShaderPass)
		{
            if (nullptr != mActiveAnimationGroup)
            {
                if (mbAnimationDataDirty == true)
                {
                    UpdateAnimation(0.0f);
                }
                
                if(IsTransparent())
                {
                    //Flush the sprite cache to maintain order
                    inpRenderSystem->GetDynamicSpriteBatchPtr()->ForceRender(inpRenderSystem);
                }
                
                //apply the shader program for the current shader pass.
                for(u32 i=0; i<mMaterials.size(); ++i)
                {
                    mMaterials[i]->SetActiveShaderProgram(ineShaderPass);
                }
                
                //render the model with the animation data.
                if (mActiveAnimationGroup->IsPrepared() == true)
                {
                    mpModel->Render(inpRenderSystem, GetEntity()->GetTransform().GetWorldTransform(), mMaterials, mActiveAnimationGroup);
                }
                else if (mFadingAnimationGroup != nullptr && mFadingAnimationGroup->IsPrepared() == true)
                {
                    mpModel->Render(inpRenderSystem, GetEntity()->GetTransform().GetWorldTransform(), mMaterials, mFadingAnimationGroup);
                }
            }
		}
        //-----------------------------------------------------
        /// Render Shadow Map
        //-----------------------------------------------------
        void AnimatedMeshComponent::RenderShadowMap(RenderSystem* inpRenderSystem, CameraComponent* inpCam)
        {
            if (nullptr != mActiveAnimationGroup)
            {
                if (mspShadowMapMaterial == nullptr)
                {
                    mspShadowMapMaterial = Core::Application::Get()->GetSystem_Old<MaterialFactory>()->CreateAnimatedDirectionalShadowMap();
                }
                
                std::vector<MaterialSPtr> aMaterials;
                mspShadowMapMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
                aMaterials.push_back(mspShadowMapMaterial);
                
                if (mbAnimationDataDirty == true)
                {
                    UpdateAnimation(0.0f);
                }
                
                //render the model with the animation data.
                if (mActiveAnimationGroup->IsPrepared() == true)
                {
                    mpModel->Render(inpRenderSystem, GetEntity()->GetTransform().GetWorldTransform(), aMaterials, mActiveAnimationGroup);
                }
                else if (mFadingAnimationGroup != nullptr && mFadingAnimationGroup->IsPrepared() == true)
                {
                    mpModel->Render(inpRenderSystem, GetEntity()->GetTransform().GetWorldTransform(), aMaterials, mFadingAnimationGroup);
                }
            }
        }
        //----------------------------------------------------------
        /// Update Animation
        //----------------------------------------------------------
        void AnimatedMeshComponent::UpdateAnimation(f32 infDeltaTime)
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
		void AnimatedMeshComponent::UpdateAnimationTimer(f32 infDeltaTime)
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
        void AnimatedMeshComponent::UpdateAttachedEntities()
        {
            if (nullptr != mActiveAnimationGroup)
            {
                for (AttachedEntityList::iterator it = maAttachedEntities.begin(); it != maAttachedEntities.end();)
                {
                    if (Core::EntitySPtr pEntity = it->first.lock())
                    {
                        s32 dwNodeIndex = it->second;
                        
                        const Core::Matrix4x4& matTransform = mActiveAnimationGroup->GetMatrixAtIndex(dwNodeIndex);
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
        void AnimatedMeshComponent::Reset()
        {
            DetatchAllEntities();
			mActiveAnimationGroup = SkinnedAnimationGroupSPtr(new SkinnedAnimationGroup(mpModel->GetSkeletonPtr()));
            mFadingAnimationGroup.reset();
            mfBlendlinePosition = 0.0f;
            mfFadeTimer = 0.0f;
            SetPlaybackPosition(0.0f);
        }
        //----------------------------------------------------
        /// Apply Default Materials
        //----------------------------------------------------
        void AnimatedMeshComponent::ApplyDefaultMaterials()
        {
            // if the first mesh contains a default material name, then get all the default materials.
			if (mpModel->GetNumSubMeshes() > 0 && mpModel->GetSubMeshAtIndex(0)->GetDefaultMaterialName() != "")
			{
				//iterate through each mesh
				for (u32 i = 0; i < mpModel->GetNumSubMeshes(); i++)
				{
					//get the material name
					SubMeshSPtr subMesh = mpModel->GetSubMeshAtIndex(i);
					std::string matName = subMesh->GetDefaultMaterialName();
                    Core::StorageLocation eStorageLocation = subMesh->GetDefaultMaterialStorageLocation();
                    
					//try and load the material
					MaterialSPtr pMaterial;
					if (matName != "")
						pMaterial = LOAD_RESOURCE(Material, eStorageLocation, matName);
                    
					//if the material load has failed, either fall back on the previous material, or stop getting materials if this is the
					//first mesh as theres no material to fall back on.
					if (pMaterial.get() == nullptr)
					{
						if (i == 0)
							break;
						
						mMaterials[i] = mMaterials[0];
					}
					else
					{
                        mMaterials[i] = pMaterial;
					}
                    
				}
			}
            
            mpMaterial = mMaterials[0];
        }
	}
}
