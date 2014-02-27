/*
 *  StaticMeshComponent.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 07/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/StaticMeshComponent.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Material/MaterialManager.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

#include <ChilliSource/Rendering/Model/SubMesh.h>

#include <limits>

namespace ChilliSource
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(StaticMeshComponent);
    
        MaterialSPtr StaticMeshComponent::mspShadowMapMaterial;
        
        StaticMeshComponent::StaticMeshComponent()
        : mbBoundingSphereValid(false), mbAABBValid(false), mbOOBBValid(false)
        {
            mMaterials.push_back(mpMaterial);
        }
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool StaticMeshComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return  (inInterfaceID == StaticMeshComponent::InterfaceID) || 
                    (inInterfaceID == RenderComponent::InterfaceID) ||
                    (inInterfaceID == VolumeComponent::InterfaceID);
		}
		//----------------------------------------------------
		/// Get Axis Aligned Bounding Box
		//----------------------------------------------------
		const Core::AABB& StaticMeshComponent::GetAABB()
		{
			if(mpEntityOwner && !mbAABBValid)
			{
                mbAABBValid = true;
                
				//Rebuild the box
                const Core::AABB& cAABB = mpModel->GetAABB();
                const Core::Matrix4x4& matWorld = mpEntityOwner->Transform().GetWorldTransform();
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
		const Core::OOBB& StaticMeshComponent::GetOOBB()
		{
			if(mpEntityOwner && !mbOOBBValid)
			{
                mbOOBBValid = true;
                
				mOBBoundingBox.SetTransform(mpEntityOwner->Transform().GetWorldTransform());
                // Origin and Size updated in AttachMesh
			}
			return mOBBoundingBox;
		}
		//----------------------------------------------------
		/// Get Bounding Sphere
		//----------------------------------------------------
		const Core::Sphere& StaticMeshComponent::GetBoundingSphere()
		{
			if(mpEntityOwner && !mbBoundingSphereValid)
			{
                mbBoundingSphereValid = true;
                
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
		bool StaticMeshComponent::IsTransparent()
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
		void StaticMeshComponent::SetMaterial(const MaterialSPtr& inpMaterial)
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
		void StaticMeshComponent::SetMaterialForSubMesh(const MaterialSPtr& inpMaterial, u32 indwSubMeshIndex)
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
        void StaticMeshComponent::SetMaterialForSubMesh(const MaterialSPtr& inpMaterial, const std::string& instrSubMeshName)
        {
            if (nullptr != mpModel)
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
		MaterialSPtr StaticMeshComponent::GetMaterialOfSubMesh(u32 indwSubMeshIndex) const
		{
			if (indwSubMeshIndex < mMaterials.size())
			{
				return mMaterials[indwSubMeshIndex];
			}
			
            CS_ERROR_LOG("Failed to get material from sub mesh " + Core::ToString(indwSubMeshIndex));
			return MaterialSPtr();
		}
        //-----------------------------------------------------------
        /// Get Material Of Sub Mesh
        //-----------------------------------------------------------
        MaterialSPtr StaticMeshComponent::GetMaterialOfSubMesh(const std::string& instrSubMeshName) const
        {
            if (nullptr != mpModel)
            {
                s32 indwIndex = mpModel->GetSubMeshIndexByName(instrSubMeshName);
            
                if (indwIndex >= 0 && indwIndex < mMaterials.size())
                {
                    return mMaterials[indwIndex];
                }
            }
			
            CS_ERROR_LOG("Failed to get material from sub mesh " + instrSubMeshName);
			return MaterialSPtr();
        }
		//----------------------------------------------------------
		/// Attach Mesh
		//----------------------------------------------------------
		void StaticMeshComponent::AttachMesh(const MeshSPtr& inpModel)
		{
			mpModel = inpModel;
            
            // Update OOBB
            mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
            mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
            
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
        //----------------------------------------------------------
        void StaticMeshComponent::AttachMesh(const MeshSPtr& inpModel, const MaterialSPtr& inpMaterial)
        {
            mpModel = inpModel;
			mpMaterial = inpMaterial;
            
            // Update OOBB
            mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
            mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
            
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
        const MeshSPtr& StaticMeshComponent::GetMesh() const
        {
            return mpModel;
        }
        //----------------------------------------------------------
        /// Render
        //----------------------------------------------------------
        void StaticMeshComponent::Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass ineShaderPass)
		{
            if(IsTransparent())
            {
                //Flush the sprite cache to maintain order
                inpRenderSystem->GetDynamicSpriteBatchPtr()->ForceRender(inpRenderSystem);
            }
            
            for(u32 i=0; i<mMaterials.size(); ++i)
            {
                mMaterials[i]->SetActiveShaderProgram(ineShaderPass);
            }

			mpModel->Render(inpRenderSystem, mpEntityOwner->Transform().GetWorldTransform(), mMaterials);
		}
        //----------------------------------------------------------
        /// Render Shadow Map
        //----------------------------------------------------------
        void StaticMeshComponent::RenderShadowMap(RenderSystem* inpRenderSystem, CameraComponent* inpCam)
		{
            if (mspShadowMapMaterial == nullptr)
            {
                mspShadowMapMaterial = Core::Application::GetSystemImplementing<MaterialFactory>()->CreateStaticDirectionalShadowMap();
            }
            
            std::vector<MaterialSPtr> aMaterials;
            mspShadowMapMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            aMaterials.push_back(mspShadowMapMaterial);
            
			mpModel->Render(inpRenderSystem, mpEntityOwner->Transform().GetWorldTransform(), aMaterials);
		}
        //----------------------------------------------------
        /// On Attached To Entity
        //----------------------------------------------------
        void StaticMeshComponent::OnAttachedToEntity()
        {
            mpEntityOwner->Transform().GetTransformChangedEvent() += Core::Transform::TransformChangedDelegate(this, &StaticMeshComponent::OnEntityTransformChanged);
            
            OnEntityTransformChanged();
        }
        //----------------------------------------------------
        /// On Entity Transform Changed
        //----------------------------------------------------
        void StaticMeshComponent::OnEntityTransformChanged()
        {
            mbBoundingSphereValid = false;
            mbAABBValid = false;
            mbOOBBValid = false;
        }
        //----------------------------------------------------
        /// On Detached From Entity
        //----------------------------------------------------
        void StaticMeshComponent::OnDetachedFromEntity()
        {
            mpEntityOwner->Transform().GetTransformChangedEvent() -= Core::Transform::TransformChangedDelegate(this, &StaticMeshComponent::OnEntityTransformChanged);
        }
        //----------------------------------------------------
        /// Apply Default Materials
        //----------------------------------------------------
        void StaticMeshComponent::ApplyDefaultMaterials()
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
                    {
						pMaterial = LOAD_RESOURCE(Material, eStorageLocation, matName);
                    }
                    
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
		//----------------------------------------------------------
		/// Destructor
		//----------------------------------------------------------
		StaticMeshComponent::~StaticMeshComponent()
		{
			if(mpEntityOwner)
            {
                mpEntityOwner->Transform().GetTransformChangedEvent() -= Core::Transform::TransformChangedDelegate(this, &StaticMeshComponent::OnEntityTransformChanged);
            }
		}
	}
}