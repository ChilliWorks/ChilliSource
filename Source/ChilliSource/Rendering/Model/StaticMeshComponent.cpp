//
//  StaticMeshComponent.cpp
//  Chilli Source
//  Created by Scott Downie on 07/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Rendering/Model/StaticMeshComponent.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Math/Matrix4.h>

#include <ChilliSource/Rendering/Model/SubMesh.h>

#include <algorithm>
#include <limits>

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(StaticMeshComponent);
        
        StaticMeshComponent::StaticMeshComponent()
        : m_isBSValid(false), m_isAABBValid(false), m_isOOBBValid(false)
        {
            mMaterials.push_back(mpMaterial);
        }
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool StaticMeshComponent::IsA(CSCore::InterfaceIDType inInterfaceID) const
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
			if(GetEntity() && !m_isAABBValid)
			{
                m_isAABBValid = true;
                
				//Rebuild the box
                const Core::AABB& cAABB = mpModel->GetAABB();
                const Core::Matrix4& matWorld = GetEntity()->GetTransform().GetWorldTransform();
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
			if(GetEntity() && !m_isOOBBValid)
			{
                m_isOOBBValid = true;
                
				mOBBoundingBox.SetTransform(GetEntity()->GetTransform().GetWorldTransform());
                // Origin and Size updated in AttachMesh
			}
			return mOBBoundingBox;
		}
		//----------------------------------------------------
		/// Get Bounding Sphere
		//----------------------------------------------------
		const Core::Sphere& StaticMeshComponent::GetBoundingSphere()
		{
			if(GetEntity() && !m_isBSValid)
			{
                m_isBSValid = true;
                
                const Core::AABB& sAABB = GetAABB();
				mBoundingSphere.vOrigin = sAABB.GetOrigin();
                mBoundingSphere.fRadius = (sAABB.BackTopRight() - sAABB.FrontBottomLeft()).Length() * 0.5f;
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
				if (mMaterials[i]->IsTransparencyEnabled() == true)
					return true;
			}
			return false;
		}
		//-----------------------------------------------------------
		/// Set Material
		//-----------------------------------------------------------
		void StaticMeshComponent::SetMaterial(const MaterialCSPtr& inpMaterial)
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
		void StaticMeshComponent::SetMaterialForSubMesh(const MaterialCSPtr& inpMaterial, u32 indwSubMeshIndex)
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
        void StaticMeshComponent::SetMaterialForSubMesh(const MaterialCSPtr& inpMaterial, const std::string& instrSubMeshName)
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
		MaterialCSPtr StaticMeshComponent::GetMaterialOfSubMesh(u32 indwSubMeshIndex) const
		{
			if (indwSubMeshIndex < mMaterials.size())
			{
				return mMaterials[indwSubMeshIndex];
			}
			
            CS_LOG_ERROR("Failed to get material from sub mesh " + Core::ToString(indwSubMeshIndex));
			return nullptr;
		}
        //-----------------------------------------------------------
        /// Get Material Of Sub Mesh
        //-----------------------------------------------------------
        MaterialCSPtr StaticMeshComponent::GetMaterialOfSubMesh(const std::string& instrSubMeshName) const
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
			return nullptr;
        }
		//----------------------------------------------------------
		/// Attach Mesh
		//----------------------------------------------------------
		void StaticMeshComponent::AttachMesh(const MeshCSPtr& inpModel)
		{
			mpModel = inpModel;
            
            // Update OOBB
            mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
            mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
		}
        //----------------------------------------------------------
        /// Attach Mesh
        //----------------------------------------------------------
        void StaticMeshComponent::AttachMesh(const MeshCSPtr& inpModel, const MaterialCSPtr& inpMaterial)
        {
            mpModel = inpModel;
			mpMaterial = inpMaterial;
            
            // Update OOBB
            mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
            mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
            
            SetMaterial(inpMaterial);
        }
        //----------------------------------------------------------
        /// Get Mesh
        //----------------------------------------------------------
        const MeshCSPtr& StaticMeshComponent::GetMesh() const
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
                inpRenderSystem->GetDynamicSpriteBatchPtr()->ForceRender();
            }
            
			mpModel->Render(inpRenderSystem, GetEntity()->GetTransform().GetWorldTransform(), mMaterials, ineShaderPass);
		}
        //----------------------------------------------------------
        /// Render Shadow Map
        //----------------------------------------------------------
        void StaticMeshComponent::RenderShadowMap(RenderSystem* inpRenderSystem, CameraComponent* inpCam, const MaterialCSPtr& in_staticShadowMap, const MaterialCSPtr& in_animShadowMap)
		{
			mpModel->Render(inpRenderSystem, GetEntity()->GetTransform().GetWorldTransform(), {in_staticShadowMap}, ShaderPass::k_ambient);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void StaticMeshComponent::OnAddedToScene()
        {
            m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(Core::MakeDelegate(this, &StaticMeshComponent::OnEntityTransformChanged));
            
            OnEntityTransformChanged();
        }
        //----------------------------------------------------
        /// On Entity Transform Changed
        //----------------------------------------------------
        void StaticMeshComponent::OnEntityTransformChanged()
        {
            m_isBSValid = false;
            m_isAABBValid = false;
            m_isOOBBValid = false;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void StaticMeshComponent::OnRemovedFromScene()
        {
            m_transformChangedConnection = nullptr;
        }
	}
}
