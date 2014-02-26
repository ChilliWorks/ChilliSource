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
		DEFINE_NAMED_INTERFACE(CAnimatedMeshComponent);
        
        MaterialPtr CAnimatedMeshComponent::mspShadowMapMaterial;
        
		//----------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------
		CAnimatedMeshComponent::CAnimatedMeshComponent() 
        : mfPlaybackPosition(0.0f), mfPlaybackSpeedMultiplier(1.0f), mfBlendlinePosition(0.0f),
        meBlendType(AnimationBlendType::k_linear), mePlaybackType(AnimationPlaybackType::k_once), meFadeType(AnimationBlendType::k_linear), mfFadeTimer(0.0f), mfFadeMaxTime(0.0f), mfFadePlaybackPosition(0.0f),
        mfFadeBlendlinePosition(0.0f), mbFinished(false), mbAnimationDataDirty(true)
		{
            mMaterials.push_back(mpMaterial);
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool CAnimatedMeshComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return  (inInterfaceID == CAnimatedMeshComponent::InterfaceID) ||
                    (inInterfaceID == IRenderComponent::InterfaceID) ||
                    (inInterfaceID == IVolumeComponent::InterfaceID);
		}
		//----------------------------------------------------
		/// Get Axis Aligned Bounding Box
		//----------------------------------------------------
		const Core::AABB& CAnimatedMeshComponent::GetAABB()
		{
			if(mpEntityOwner)
			{
				//Rebuild the box
                const Core::AABB& cAABB = mpModel->GetAABB();
                const Core::CMatrix4x4& matWorld = mpEntityOwner->Transform().GetWorldTransform();
                Core::CVector3 vBackBottomLeft(cAABB.BackBottomLeft() * matWorld);
                Core::CVector3 vBackBottomRight(cAABB.BackBottomRight() * matWorld);
                Core::CVector3 vBackTopLeft(cAABB.BackTopLeft() * matWorld);
                Core::CVector3 vBackTopRight(cAABB.BackTopRight() * matWorld);
                Core::CVector3 vFrontBottomLeft(cAABB.FrontBottomLeft() * matWorld);
                Core::CVector3 vFrontBottomRight(cAABB.FrontBottomRight() * matWorld);
                Core::CVector3 vFrontTopLeft(cAABB.FrontTopLeft() *matWorld);
                Core::CVector3 vFrontTopRight(cAABB.FrontTopRight() * matWorld);
                
                Core::CVector3 vMin(std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity());
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
                
                Core::CVector3 vMax(-std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity());
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
		const Core::OOBB& CAnimatedMeshComponent::GetOOBB()
		{
			if(mpEntityOwner)
			{
				mOBBoundingBox.SetTransform(mpEntityOwner->Transform().GetWorldTransform());
			}
			return mOBBoundingBox;
		}
		//----------------------------------------------------
		/// Get Bounding Sphere
		//----------------------------------------------------
		const Core::Sphere& CAnimatedMeshComponent::GetBoundingSphere()
		{
			if(mpEntityOwner)
			{
                const Core::AABB& sAABB = GetAABB();
                Core::CVector3 vSize = sAABB.GetSize();
				mBoundingSphere.vOrigin = sAABB.GetOrigin();
				mBoundingSphere.fRadius = std::max(vSize.x, vSize.y) * 0.5f;
			}
			return mBoundingSphere;
		}
		//-----------------------------------------------------------
		/// Is Transparent
		//-----------------------------------------------------------
		bool CAnimatedMeshComponent::IsTransparent()
		{
			for (int i = 0; i < mMaterials.size(); ++i)
			{
				if (mMaterials[i]->IsTransparent() == true)
					return true;
			}
			return false;
		}
		//-----------------------------------------------------------
		/// Set Material
		//-----------------------------------------------------------
		void CAnimatedMeshComponent::SetMaterial(const MaterialPtr& inpMaterial)
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
		void CAnimatedMeshComponent::SetMaterialForSubMesh(const MaterialPtr& inpMaterial, u32 indwSubMeshIndex)
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
        void CAnimatedMeshComponent::SetMaterialForSubMesh(const MaterialPtr& inpMaterial, const std::string& instrSubMeshName)
        {
            if (NULL != mpModel)
            {
                s32 indwIndex = mpModel->GetSubMeshIndexByName(instrSubMeshName);
                if (indwIndex >= 0 && indwIndex < mMaterials.size())
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
		const MaterialPtr CAnimatedMeshComponent::GetMaterialOfSubMesh(u32 indwSubMeshIndex) const
		{
			if (indwSubMeshIndex < mMaterials.size())
			{
				return mMaterials[indwSubMeshIndex];
			}
			
            ERROR_LOG("Failed to get material from sub mesh " + Core::ToString(indwSubMeshIndex));
			return MaterialPtr();
		}
        //-----------------------------------------------------------
        /// Get Material Of Sub Mesh
        //-----------------------------------------------------------
        MaterialPtr CAnimatedMeshComponent::GetMaterialOfSubMesh(const std::string& instrSubMeshName) const
        {
            if (NULL != mpModel)
            {
                s32 indwIndex = mpModel->GetSubMeshIndexByName(instrSubMeshName);
                
                if (indwIndex >= 0 && indwIndex < mMaterials.size())
                {
                    return mMaterials[indwIndex];
                }
            }
			
            ERROR_LOG("Failed to get material from sub mesh " + instrSubMeshName);
			return MaterialPtr();
        }
		//----------------------------------------------------------
		/// Attach Mesh
		//----------------------------------------------------------
		void CAnimatedMeshComponent::AttachMesh(const MeshPtr& inpModel)
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
                mMaterials.push_back(MaterialPtr());
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
        void CAnimatedMeshComponent::AttachMesh(const MeshPtr& inpModel, const MaterialPtr& inpMaterial)
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
                mMaterials.push_back(MaterialPtr());
            }
            
            SetMaterial(inpMaterial);
        }
        //----------------------------------------------------------
        /// Get Mesh
        //----------------------------------------------------------
        const MeshPtr& CAnimatedMeshComponent::GetMesh() const
        {
            return mpModel;
        }
		//----------------------------------------------------------
        /// Attach Animation
        //----------------------------------------------------------
        void CAnimatedMeshComponent::AttachAnimation(Core::StorageLocation ineStorageLocation, const std::string& instrAnimation, f32 infBlendlinePosition)
        {
            AttachAnimation(LOAD_RESOURCE(CSkinnedAnimation, ineStorageLocation, instrAnimation), infBlendlinePosition);
        }
        //----------------------------------------------------------
        /// Attach Animation
        //----------------------------------------------------------
        void CAnimatedMeshComponent::AttachAnimation(const SkinnedAnimationPtr& inpAnimation, f32 infBlendlinePosition)
        {
            if (NULL != mActiveAnimationGroup)
            {
                mActiveAnimationGroup->AttachAnimation(inpAnimation, infBlendlinePosition);
                mbAnimationDataDirty = true;
            }
        }
        //----------------------------------------------------------
        /// Get Animations
        //----------------------------------------------------------
        void CAnimatedMeshComponent::GetAnimations(DYNAMIC_ARRAY<SkinnedAnimationPtr>& outapSkinnedAnimationList)
        {
            if (NULL != mActiveAnimationGroup)
            {
                mActiveAnimationGroup->GetAnimations(outapSkinnedAnimationList);
            }
        }
        //----------------------------------------------------------
        /// Detatch Animation
        //----------------------------------------------------------
        void CAnimatedMeshComponent::DetatchAnimation(const SkinnedAnimationPtr& inpAnimation)
        {
            if (NULL != mActiveAnimationGroup)
            {
                mActiveAnimationGroup->DetatchAnimation(inpAnimation);
            }
        }
        //----------------------------------------------------------
        /// Fade Out
        //----------------------------------------------------------
        void CAnimatedMeshComponent::FadeOut(AnimationBlendType ineFadeType, f32 infFadeOutTime)
        {
            if (NULL != mActiveAnimationGroup && true == mActiveAnimationGroup->IsPrepared())
            {
                mFadingAnimationGroup = mActiveAnimationGroup;
                mActiveAnimationGroup = SkinnedAnimationGroupPtr(new CSkinnedAnimationGroup(mpModel->GetSkeletonPtr()));
                mfFadePlaybackPosition = mfPlaybackPosition;
                mfFadeBlendlinePosition = mfBlendlinePosition;
                mfFadeMaxTime = infFadeOutTime;
                mfFadeTimer = 0.0f;
                SetPlaybackPosition(0.0f);
                mAnimationChangedEvent.Invoke(this);
            }
            else if (NULL != mActiveAnimationGroup)
            {
                mActiveAnimationGroup->ClearAnimations();
            }
            
        }
        //----------------------------------------------------------
        /// Clear Animations
        //----------------------------------------------------------
        void CAnimatedMeshComponent::ClearAnimations()
        {
            if (mActiveAnimationGroup != NULL)
            {
                mActiveAnimationGroup->ClearAnimations();
                mFadingAnimationGroup.reset();
                SetPlaybackPosition(0.0f);
                mfBlendlinePosition = 0.0f;
                mfFadeTimer = 0.0f;
                mAnimationChangedEvent.Invoke(this);
            }
        }
        //----------------------------------------------------------
        /// Attach Entity
        //----------------------------------------------------------
        void CAnimatedMeshComponent::AttachEntity(const Core::EntityPtr& inpEntity, const std::string& instrNodeName)
        {
            if (NULL == mpEntityOwner)
            {
                ERROR_LOG("Could not attach entity to animated mesh becuase the mesh is not yet attached to an entity.");
                return;
            }
            
            if (NULL != inpEntity->GetParent() || NULL != inpEntity->GetOwningScene())
            {
                ERROR_LOG("Could not attach entity to animated mesh becuase the entity already has a parent.");
                return;
            }
            
            //check that it has not already been added.
            for (AttachedEntityList::const_iterator it = maAttachedEntities.begin(); it != maAttachedEntities.end(); ++it)
            {
                if (Core::EntityPtr pEntity = it->first.lock())
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
                ERROR_LOG("Could not attach entity to the animated mesh becuase the skeleton node name could not be found.");
                return;
            }
            
            mpEntityOwner->AddChild(inpEntity);
            maAttachedEntities.push_back(std::pair<Core::EntityWeakPtr, s32>(Core::EntityWeakPtr(inpEntity), dwNodeIndex));
        }
        //----------------------------------------------------------
        /// Detatch Entity
        //----------------------------------------------------------
        void CAnimatedMeshComponent::DetatchEntity(Core::CEntity* inpEntity)
        {
            AttachedEntityList::iterator it;
            for (it = maAttachedEntities.begin(); it != maAttachedEntities.end(); ++it)
            {
                if (Core::EntityPtr pEntity = it->first.lock())
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
        void CAnimatedMeshComponent::DetatchAllEntities()
        {
            for (AttachedEntityList::const_iterator it = maAttachedEntities.begin(); it != maAttachedEntities.end(); ++it)
            {
                if (Core::EntityPtr pEntity = it->first.lock())
                {
                    pEntity->RemoveFromParent();
                }
            }
            
            maAttachedEntities.clear();
        }
        //----------------------------------------------------------
        /// Set Blendline Position
        //----------------------------------------------------------
        void CAnimatedMeshComponent::SetBlendlinePosition(f32 infBlendlinePosition)
        {
            mfBlendlinePosition = infBlendlinePosition;
        }
        //----------------------------------------------------------
        /// Set Playback Type
        //----------------------------------------------------------
        void CAnimatedMeshComponent::SetPlaybackType(AnimationPlaybackType inePlaybackType)
        {
            mePlaybackType = inePlaybackType;
            mbFinished = false;
        }
        //----------------------------------------------------------
        /// Set Playback Speed Multiplier
        //----------------------------------------------------------
        void CAnimatedMeshComponent::SetPlaybackSpeedMultiplier(f32 infSpeedMultiplier)
        {
            mfPlaybackSpeedMultiplier = infSpeedMultiplier;
        }
        //----------------------------------------------------------
        /// Set Playback Position
        //----------------------------------------------------------
        void CAnimatedMeshComponent::SetPlaybackPosition(f32 infPosition)
        {
            mfPlaybackPosition = infPosition;
            mbFinished = false;
            mbAnimationDataDirty = true;
        }
        //----------------------------------------------------------
        /// Set Playback Position Normalised
        //----------------------------------------------------------
        void CAnimatedMeshComponent::SetPlaybackPositionNormalised(f32 infPosition)
        {
            mfPlaybackPosition = infPosition * GetAnimationLength();
            mbFinished = false;
        }
        //----------------------------------------------------------
        /// Set Blend Type
        //----------------------------------------------------------
        void CAnimatedMeshComponent::SetBlendType(AnimationBlendType ineBlendType)
        {
            meBlendType = ineBlendType;
        }
        //----------------------------------------------------------
        /// Get Animation Changed Event
        //----------------------------------------------------------
        AnimationChangedEvent& CAnimatedMeshComponent::GetAnimationChangedEvent()
        {
            return mAnimationChangedEvent;
        }
        //----------------------------------------------------------
        /// Get Animation Completion Event
        //----------------------------------------------------------
        AnimationCompletionEvent& CAnimatedMeshComponent::GetAnimationCompletionEvent()
        {
            return mAnimationCompletionEvent;
        }
        //----------------------------------------------------------
        /// Get Animation Looped Event
        //----------------------------------------------------------
        AnimationLoopedEvent& CAnimatedMeshComponent::GetAnimationLoopedEvent()
        {
            return mAnimationLoopedEvent;
        }
        //----------------------------------------------------------
        /// Get Blendline Position
        //----------------------------------------------------------
        f32 CAnimatedMeshComponent::GetBlendlinePosition() const
        {
            return mfBlendlinePosition;
        }
        //----------------------------------------------------------
        /// Get Playback Type
        //----------------------------------------------------------
        AnimationPlaybackType CAnimatedMeshComponent::GetPlaybackType() const
        {
            return mePlaybackType;
        }
        //----------------------------------------------------------
        /// Get Animation Length
        //----------------------------------------------------------
        f32 CAnimatedMeshComponent::GetAnimationLength()
        {
            if (mActiveAnimationGroup != NULL)
            {
                return mActiveAnimationGroup->GetAnimationLength();
            }
            return 0;
        }
        //----------------------------------------------------------
        /// Get Playback Speed Multiplier
        //----------------------------------------------------------
        f32 CAnimatedMeshComponent::GetPlaybackSpeedMultiplier() const
        {
            return mfPlaybackSpeedMultiplier;
        }
        //----------------------------------------------------------
        /// Get Playback Position
        //----------------------------------------------------------
        f32 CAnimatedMeshComponent::GetPlaybackPosition() const
        {
            return mfPlaybackPosition;
        }
        //----------------------------------------------------------
        /// get Playback Position Normalised
        //----------------------------------------------------------
        f32 CAnimatedMeshComponent::GetPlaybackPositionNormalised()
        {
            return mfPlaybackPosition / GetAnimationLength();
        }
        //----------------------------------------------------------
        /// Get Blend Type
        //----------------------------------------------------------
        AnimationBlendType CAnimatedMeshComponent::GetBlendType() const
        {
            return meBlendType;
        }
        //----------------------------------------------------------
        /// Has Finished
        //----------------------------------------------------------
        bool CAnimatedMeshComponent::HasFinished() const
        {
            return mbFinished;
        }
        //----------------------------------------------------------
        /// Update
        //----------------------------------------------------------
        void CAnimatedMeshComponent::Update(f32 infDeltaTime)
        {
            if (NULL != mpEntityOwner && NULL != mpEntityOwner->GetOwningScene())
            {
                UpdateAnimation(infDeltaTime);
            }
        }
        //----------------------------------------------------------
		/// Destructor
		//----------------------------------------------------------
		CAnimatedMeshComponent::~CAnimatedMeshComponent()
		{
		}
		//----------------------------------------------------
		/// On Attached To Entity
		//----------------------------------------------------
		void CAnimatedMeshComponent::OnAttachedToEntity()
		{
            SetPlaybackPosition(0.0f);
            Core::CApplication::GetSystemImplementing<CAnimatedMeshComponentUpdater>()->AddComponent(this);
		}
        //----------------------------------------------------
        /// On Detached From Entity
        //----------------------------------------------------
        void CAnimatedMeshComponent::OnDetachedFromEntity()
        {
            DetatchAllEntities();
            Core::CApplication::GetSystemImplementing<CAnimatedMeshComponentUpdater>()->RemoveComponent(this);
        }
		//----------------------------------------------------------
        /// Render
        //----------------------------------------------------------
        void CAnimatedMeshComponent::Render(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam, ShaderPass ineShaderPass)
		{
            if (NULL != mActiveAnimationGroup)
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
                    mpModel->Render(inpRenderSystem, mpEntityOwner->Transform().GetWorldTransform(), mMaterials, mActiveAnimationGroup);
                }
                else if (mFadingAnimationGroup != NULL && mFadingAnimationGroup->IsPrepared() == true)
                {
                    mpModel->Render(inpRenderSystem, mpEntityOwner->Transform().GetWorldTransform(), mMaterials, mFadingAnimationGroup);
                }
            }
		}
        //-----------------------------------------------------
        /// Render Shadow Map
        //-----------------------------------------------------
        void CAnimatedMeshComponent::RenderShadowMap(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam)
        {
            if (NULL != mActiveAnimationGroup)
            {
                if (mspShadowMapMaterial == NULL)
                {
                    mspShadowMapMaterial = Core::CApplication::GetSystemImplementing<CMaterialFactory>()->CreateAnimatedDirectionalShadowMap();
                }
                
                DYNAMIC_ARRAY<MaterialPtr> aMaterials;
                mspShadowMapMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
                aMaterials.push_back(mspShadowMapMaterial);
                
                if (mbAnimationDataDirty == true)
                {
                    UpdateAnimation(0.0f);
                }
                
                //render the model with the animation data.
                if (mActiveAnimationGroup->IsPrepared() == true)
                {
                    mpModel->Render(inpRenderSystem, mpEntityOwner->Transform().GetWorldTransform(), aMaterials, mActiveAnimationGroup);
                }
                else if (mFadingAnimationGroup != NULL && mFadingAnimationGroup->IsPrepared() == true)
                {
                    mpModel->Render(inpRenderSystem, mpEntityOwner->Transform().GetWorldTransform(), aMaterials, mFadingAnimationGroup);
                }
            }
        }
        //----------------------------------------------------------
        /// Update Animation
        //----------------------------------------------------------
        void CAnimatedMeshComponent::UpdateAnimation(f32 infDeltaTime)
        {
            if (NULL != mpEntityOwner && NULL != mpEntityOwner->GetOwningScene() && NULL != mActiveAnimationGroup && mActiveAnimationGroup->GetAnimationCount() != 0)
            {
                //update the animation timer.
                UpdateAnimationTimer(infDeltaTime);
                
                //calculate the animation data and convert to matrices.
                mActiveAnimationGroup->BuildAnimationData(meBlendType, mfPlaybackPosition, mfBlendlinePosition);
                
                //if there is a group fading out, then apply this to the active data.
                if (NULL != mFadingAnimationGroup)
                {
                    if (mfFadeMaxTime > 0.0f && mfFadeTimer < mfFadeMaxTime)
                    {
                        mFadingAnimationGroup->BuildAnimationData(meBlendType, mfFadePlaybackPosition, mfFadeBlendlinePosition);
                        f32 fGroupBlendFactor = 1.0f - (mfFadeTimer / mfFadeMaxTime);
                        mActiveAnimationGroup->BlendGroup(meBlendType, mFadingAnimationGroup, fGroupBlendFactor);
                    }
                    else
                    {
                        mFadingAnimationGroup = SkinnedAnimationGroupPtr();
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
		void CAnimatedMeshComponent::UpdateAnimationTimer(f32 infDeltaTime)
		{
            if (NULL != mActiveAnimationGroup)
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
                            mAnimationCompletionEvent.Invoke(this);
                        }
                        break;
                    }
                    case AnimationPlaybackType::k_looping:
                    {
                        while (mfPlaybackPosition >= mActiveAnimationGroup->GetAnimationLength() && mActiveAnimationGroup->GetAnimationLength() > 0.0f)
                        {
                            mfPlaybackPosition -= mActiveAnimationGroup->GetAnimationLength();
                            mAnimationLoopedEvent.Invoke(this);
                        }
                        break;
                    }
                }
                
                //update the fade timer
                if (NULL != mFadingAnimationGroup)
                {
                    mfFadeTimer += infDeltaTime;
                }
            }
		}
        //----------------------------------------------------------
        /// Update Attached Entities
        //----------------------------------------------------------
        void CAnimatedMeshComponent::UpdateAttachedEntities()
        {
            if (NULL != mActiveAnimationGroup)
            {
                for (AttachedEntityList::iterator it = maAttachedEntities.begin(); it != maAttachedEntities.end();)
                {
                    if (Core::EntityPtr pEntity = it->first.lock())
                    {
                        s32 dwNodeIndex = it->second;
                        
                        const Core::CMatrix4x4& matTransform = mActiveAnimationGroup->GetMatrixAtIndex(dwNodeIndex);
                        pEntity->Transform().SetLocalTransform(matTransform);
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
        void CAnimatedMeshComponent::Reset()
        {
            DetatchAllEntities();
			mActiveAnimationGroup = SkinnedAnimationGroupPtr(new CSkinnedAnimationGroup(mpModel->GetSkeletonPtr()));
            mFadingAnimationGroup.reset();
            mfBlendlinePosition = 0.0f;
            mfFadeTimer = 0.0f;
            SetPlaybackPosition(0.0f);
        }
        //----------------------------------------------------
        /// Apply Default Materials
        //----------------------------------------------------
        void CAnimatedMeshComponent::ApplyDefaultMaterials()
        {
            // if the first mesh contains a default material name, then get all the default materials.
			if (mpModel->GetNumSubMeshes() > 0 && mpModel->GetSubMeshAtIndex(0)->GetDefaultMaterialName() != "")
			{
				//iterate through each mesh
				for (u32 i = 0; i < mpModel->GetNumSubMeshes(); i++)
				{
					//get the material name
					SubMeshPtr subMesh = mpModel->GetSubMeshAtIndex(i);
					std::string matName = subMesh->GetDefaultMaterialName();
                    Core::StorageLocation eStorageLocation = subMesh->GetDefaultMaterialStorageLocation();
                    
					//try and load the material
					MaterialPtr pMaterial;
					if (matName != "")
						pMaterial = LOAD_RESOURCE(CMaterial, eStorageLocation, matName);
                    
					//if the material load has failed, either fall back on the previous material, or stop getting materials if this is the
					//first mesh as theres no material to fall back on.
					if (pMaterial.get() == NULL)
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
