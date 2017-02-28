//
//  StaticModelComponent.cpp
//  ChilliSource
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
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Material/Material.h>

#include <algorithm>
#include <limits>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(StaticModelComponent);
    
    //------------------------------------------------------------------------------
    StaticModelComponent::StaticModelComponent(const ModelCSPtr& model, const MaterialCSPtr& material) noexcept
    : m_model(model)
    {
        CS_ASSERT(m_model, "Model cannot be null");
        CS_ASSERT(m_model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(material, "Material cannot be null");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        m_materials.resize(model->GetNumMeshes());
        
        for (u32 i = 0; i < u32(m_materials.size()); ++i)
        {
            m_materials[i] = material;
        }
    }
    
    //------------------------------------------------------------------------------
    StaticModelComponent::StaticModelComponent(const ModelCSPtr& model, const std::vector<MaterialCSPtr>& materials) noexcept
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
    }
    
    //------------------------------------------------------------------------------
    bool StaticModelComponent::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return  (interfaceId == VolumeComponent::InterfaceID || interfaceId == StaticModelComponent::InterfaceID);
    }
    
    //------------------------------------------------------------------------------
    const AABB& StaticModelComponent::GetAABB() noexcept
    {
        if(GetEntity() && !m_isAABBValid)
        {
            m_isAABBValid = true;
            
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
    const OOBB& StaticModelComponent::GetOOBB() noexcept
    {
        if(GetEntity() && !m_isOOBBValid)
        {
            m_isOOBBValid = true;
            
            m_oobb.SetTransform(GetEntity()->GetTransform().GetWorldTransform());
            // Origin and Size updated in SetModel
        }
        return m_oobb;
    }
    
    //------------------------------------------------------------------------------
    const Sphere& StaticModelComponent::GetBoundingSphere() noexcept
    {
        if(GetEntity() && !m_isBoundingSphereValid)
        {
            m_isBoundingSphereValid = true;
            
            const AABB& sAABB = GetAABB();
            m_boundingSphere.vOrigin = sAABB.GetOrigin();
            m_boundingSphere.fRadius = (sAABB.BackTopRight() - sAABB.FrontBottomLeft()).Length() * 0.5f;
        }
        return m_boundingSphere;
    }
    
    //------------------------------------------------------------------------------
    const ModelCSPtr& StaticModelComponent::GetModel() const noexcept
    {
        return m_model;
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::SetModel(const ModelCSPtr& model) noexcept
    {
        CS_ASSERT(model, "Cannot set null model.");
        CS_ASSERT(model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        
        m_model = model;
        m_materials.resize(model->GetNumMeshes());
        
        m_oobb.SetSize(m_model->GetAABB().GetSize());
        m_oobb.SetOrigin(m_model->GetAABB().GetOrigin());
        
        SetMaterial(GetMaterialForMesh(0));
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::SetModel(const ModelCSPtr& model, const MaterialCSPtr& material) noexcept
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
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::SetModel(const ModelCSPtr& model, const std::vector<MaterialCSPtr>& materials) noexcept
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
    }
    
    //------------------------------------------------------------------------------
    const MaterialCSPtr& StaticModelComponent::GetMaterialForMesh(u32 meshIndex) const noexcept
    {
        CS_ASSERT(meshIndex < s32(m_materials.size()), "Invalid mesh index.");
        
        return m_materials[meshIndex];
    }
    
    //------------------------------------------------------------------------------
    const MaterialCSPtr& StaticModelComponent::GetMaterialForMesh(const std::string& meshName) const noexcept
    {
        CS_ASSERT(m_model, "Cannot get material without a model.");
        
        auto meshIndex = m_model->GetMeshIndex(meshName);
        CS_ASSERT(meshIndex >= 0 && meshIndex < s32(m_materials.size()), "Invalid mesh index.");
        
        return m_materials[meshIndex];
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::SetMaterial(const MaterialCSPtr& material) noexcept
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
    void StaticModelComponent::SetMaterialForMesh(const MaterialCSPtr& material, u32 meshIndex) noexcept
    {
        CS_ASSERT(m_model, "Cannot set material without a model.");
        CS_ASSERT(material, "Cannot set null material.");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        CS_ASSERT(meshIndex < s32(m_materials.size()), "Invalid mesh index.");
        
        m_materials[meshIndex] = material;
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::SetMaterialForMesh(const MaterialCSPtr& material, const std::string& meshName) noexcept
    {
        CS_ASSERT(m_model, "Cannot set material without a model.");
        CS_ASSERT(material, "Cannot set null material.");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        auto meshIndex = m_model->GetMeshIndex(meshName);
        CS_ASSERT(meshIndex >= 0 && meshIndex < s32(m_materials.size()), "Invalid mesh index.");
        
        m_materials[meshIndex] = material;
    }

    //------------------------------------------------------------------------------
    bool StaticModelComponent::IsShadowCastingEnabled() const noexcept
    {
        return m_shadowCastingEnabled;
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::SetShadowCastingEnabled(bool enabled) noexcept
    {
        m_shadowCastingEnabled = enabled;
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::OnAddedToScene() noexcept
    {
        m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(MakeDelegate(this, &StaticModelComponent::OnEntityTransformChanged));
        
        OnEntityTransformChanged();
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::OnEntityTransformChanged() noexcept
    {
        m_isAABBValid = false;
        m_isOOBBValid = false;
        m_isBoundingSphereValid = false;
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        CS_ASSERT(m_model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(m_model->GetNumMeshes() == m_materials.size(), "Invalid number of materials.");
        
        for (u32 index = 0; index < m_model->GetNumMeshes(); ++index)
        {
            CS_ASSERT(m_materials[index]->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
            
            auto renderMaterialGroup = m_materials[index]->GetRenderMaterialGroup();
            auto renderMesh = m_model->GetRenderMesh(index);
            
            const auto& transform = GetEntity()->GetTransform();
            auto boundingSphere = Sphere::Transform(renderMesh->GetBoundingSphere(), transform.GetWorldPosition(), transform.GetWorldOrientation(), transform.GetWorldScale());
            
            renderSnapshot.AddRenderObject(RenderObject(renderMaterialGroup, renderMesh, GetEntity()->GetTransform().GetWorldTransform(), boundingSphere, m_shadowCastingEnabled, RenderLayer::k_standard));
        }
    }
    
    //------------------------------------------------------------------------------
    void StaticModelComponent::OnRemovedFromScene() noexcept
    {
        m_transformChangedConnection.reset();
    }
}
