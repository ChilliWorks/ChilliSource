//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#include <ChilliSource/Rendering/Skybox/SkyboxComponent.h>

#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Material/Material.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(SkyboxComponent);
    
    //------------------------------------------------------------------------------
    SkyboxComponent::SkyboxComponent(const ModelCSPtr& model, const MaterialCSPtr& material) noexcept
    : m_model(model), m_material(material)
    {
        CS_ASSERT(m_model, "Model cannot be null");
        CS_ASSERT(m_model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(m_model->GetNumMeshes() == 1, "Skybox model should only have a single mesh");
        CS_ASSERT(material, "Material cannot be null");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
    }
    
    //------------------------------------------------------------------------------
    bool SkyboxComponent::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return  (interfaceId == SkyboxComponent::InterfaceID);
    }
    
    //------------------------------------------------------------------------------
    const ModelCSPtr& SkyboxComponent::GetModel() const noexcept
    {
        return m_model;
    }
    
    //------------------------------------------------------------------------------
    void SkyboxComponent::SetModel(const ModelCSPtr& model) noexcept
    {
        CS_ASSERT(model, "Cannot set null model.");
        CS_ASSERT(model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(model->GetNumMeshes() == 1, "Skybox model should only have a single mesh");
        
        m_model = model;
    }
    
    //------------------------------------------------------------------------------
    void SkyboxComponent::SetModel(const ModelCSPtr& model, const MaterialCSPtr& material) noexcept
    {
        CS_ASSERT(model, "Cannot set null model.");
        CS_ASSERT(model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(model->GetNumMeshes() == 1, "Skybox model should only have a single mesh");
        CS_ASSERT(material, "Cannot set null material.");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        m_model = model;
        m_material = material;
    }
    
    //------------------------------------------------------------------------------
    const MaterialCSPtr& SkyboxComponent::GetMaterial() const noexcept
    {
        return m_material;
    }
    
    //------------------------------------------------------------------------------
    void SkyboxComponent::SetMaterial(const MaterialCSPtr& material) noexcept
    {
        CS_ASSERT(m_model, "Cannot set material without a model.");
        CS_ASSERT(material, "Cannot set null material.");
        CS_ASSERT(material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        m_material = material;
    }
    
    //------------------------------------------------------------------------------
    void SkyboxComponent::OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        CS_ASSERT(m_model->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a model that hasn't been loaded yet.");
        CS_ASSERT(m_material->GetLoadState() == Resource::LoadState::k_loaded, "Cannot use a material that hasn't been loaded yet.");
        
        auto renderMaterialGroup = m_material->GetRenderMaterialGroup();
        auto renderMesh = m_model->GetRenderMesh(0);
        
        renderSnapshot.AddRenderObject(RenderObject(renderMaterialGroup, renderMesh, Matrix4::k_identity, renderMesh->GetBoundingSphere(), false, RenderLayer::k_skybox));
    }
}
