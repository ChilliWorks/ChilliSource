//
//  StaticModelComponent.cpp
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

#include <ChilliSource/Rendering/Model/StaticModelComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>

#include <algorithm>
#include <limits>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(StaticModelComponent);
    
    StaticModelComponent::StaticModelComponent()
    : m_isBSValid(false), m_isAABBValid(false), m_isOOBBValid(false)
    {
        mMaterials.push_back(nullptr);
    }
    //----------------------------------------------------------
    /// Is A
    //----------------------------------------------------------
    bool StaticModelComponent::IsA(InterfaceIDType inInterfaceID) const
    {
        return  (inInterfaceID == VolumeComponent::InterfaceID || inInterfaceID == StaticModelComponent::InterfaceID);
    }
    //----------------------------------------------------
    /// Get Axis Aligned Bounding Box
    //----------------------------------------------------
    const AABB& StaticModelComponent::GetAABB()
    {
        if(GetEntity() && !m_isAABBValid)
        {
            m_isAABBValid = true;
            
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
    const OOBB& StaticModelComponent::GetOOBB()
    {
        if(GetEntity() && !m_isOOBBValid)
        {
            m_isOOBBValid = true;
            
            mOBBoundingBox.SetTransform(GetEntity()->GetTransform().GetWorldTransform());
            // Origin and Size updated in SetModel
        }
        return mOBBoundingBox;
    }
    //----------------------------------------------------
    /// Get Bounding Sphere
    //----------------------------------------------------
    const Sphere& StaticModelComponent::GetBoundingSphere()
    {
        if(GetEntity() && !m_isBSValid)
        {
            m_isBSValid = true;
            
            const AABB& sAABB = GetAABB();
            mBoundingSphere.vOrigin = sAABB.GetOrigin();
            mBoundingSphere.fRadius = (sAABB.BackTopRight() - sAABB.FrontBottomLeft()).Length() * 0.5f;
        }
        return mBoundingSphere;
    }
    //-----------------------------------------------------------
    /// Set Material
    //-----------------------------------------------------------
    void StaticModelComponent::SetMaterial(const MaterialCSPtr& inpMaterial)
    {
        for (u32 i = 0; i < mMaterials.size(); i++)
        {
            mMaterials[i] = inpMaterial;
        }
    }
    //-----------------------------------------------------------
    /// Set Material For Sub Model
    //-----------------------------------------------------------
    void StaticModelComponent::SetMaterialForSubMesh(const MaterialCSPtr& inpMaterial, u32 indwSubMeshIndex)
    {
        CS_ASSERT(mpModel, "Cannot set material without a model.");
        CS_ASSERT(indwSubMeshIndex < s32(mMaterials.size()), "Invalid mesh index.");
        
        mMaterials[indwSubMeshIndex] = inpMaterial;
    }
    //-----------------------------------------------------------
    /// Set Material For Sub Model
    //-----------------------------------------------------------
    void StaticModelComponent::SetMaterialForSubMesh(const MaterialCSPtr& inpMaterial, const std::string& instrSubMeshName)
    {
        CS_ASSERT(mpModel, "Cannot set material without a model.");
        
        auto meshIndex = mpModel->GetMeshIndex(instrSubMeshName);
        CS_ASSERT(meshIndex >= 0 && meshIndex < s32(mMaterials.size()), "Invalid mesh index.");
        
        mMaterials[meshIndex] = inpMaterial;
    }
    //-----------------------------------------------------------
    /// Get Material Of Sub Model
    //-----------------------------------------------------------
    MaterialCSPtr StaticModelComponent::GetMaterialOfSubMesh(u32 indwSubMeshIndex) const
    {
        CS_ASSERT(mpModel, "Cannot set material without a model.");
        CS_ASSERT(indwSubMeshIndex < s32(mMaterials.size()), "Invalid mesh index.");
        
        return mMaterials[indwSubMeshIndex];
    }
    //-----------------------------------------------------------
    /// Get Material Of Sub Model
    //-----------------------------------------------------------
    MaterialCSPtr StaticModelComponent::GetMaterialOfSubMesh(const std::string& instrSubMeshName) const
    {
        CS_ASSERT(mpModel, "Cannot get material without a model.");
        
        auto meshIndex = mpModel->GetMeshIndex(instrSubMeshName);
        CS_ASSERT(meshIndex >= 0 && meshIndex < s32(mMaterials.size()), "Invalid mesh index.");
        
        return mMaterials[meshIndex];
    }
    //----------------------------------------------------------
    /// Attach Model
    //----------------------------------------------------------
    void StaticModelComponent::SetModel(const ModelCSPtr& inpModel)
    {
        mpModel = inpModel;
        mMaterials.resize(mpModel->GetNumMeshes());
        
        mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
        mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
    }
    //----------------------------------------------------------
    /// Attach Model
    //----------------------------------------------------------
    void StaticModelComponent::SetModel(const ModelCSPtr& inpModel, const MaterialCSPtr& inpMaterial)
    {
        mpModel = inpModel;
        mMaterials.resize(mpModel->GetNumMeshes());
        
        mOBBoundingBox.SetSize(mpModel->GetAABB().GetSize());
        mOBBoundingBox.SetOrigin(mpModel->GetAABB().GetOrigin());
        
        SetMaterial(inpMaterial);
    }
    //----------------------------------------------------------
    /// Get Model
    //----------------------------------------------------------
    const ModelCSPtr& StaticModelComponent::GetModel() const
    {
        return mpModel;
    }
    //-----------------------------------------------------
    //-----------------------------------------------------
    void StaticModelComponent::SetShadowCastingEnabled(bool inbEnabled)
    {
        m_shadowCastingEnabled = inbEnabled;
    }
    //-----------------------------------------------------
    //-----------------------------------------------------
    bool StaticModelComponent::IsShadowCastingEnabled() const
    {
        return m_shadowCastingEnabled;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void StaticModelComponent::OnAddedToScene()
    {
        m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(MakeDelegate(this, &StaticModelComponent::OnEntityTransformChanged));
        
        OnEntityTransformChanged();
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void StaticModelComponent::OnEntityTransformChanged()
    {
        m_isBSValid = false;
        m_isAABBValid = false;
        m_isOOBBValid = false;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void StaticModelComponent::OnRenderSnapshot(RenderSnapshot& in_renderSnapshot) noexcept
    {
        //TODO: Re-add once materials have been set up.
//        CS_ASSERT(mpModel, "Static model component should not be in scene without a model set.");
//        CS_ASSERT(mpModel->GetNumMeshes() == mMaterials.size(), "Invalid number of materials.");
//        
//        for (u32 index = 0; index < mpModel->GetNumMeshes(); ++index)
//        {
//            CS_ASSERT(mMaterials[index], "Material cannot be null.");
//            
//            auto renderMaterialGroup = mMaterials[index]->GetRenderMaterialGroup();
//            auto renderMesh = mpModel->GetRenderMesh(index);
//            
//            in_renderSnapshot.AddRenderObject(RenderObject(renderMaterialGroup, renderMesh, GetEntity()->GetTransform().GetWorldTransform()));
//        }
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void StaticModelComponent::OnRemovedFromScene()
    {
        m_transformChangedConnection = nullptr;
    }
}
