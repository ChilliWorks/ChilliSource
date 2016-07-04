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
#include <ChilliSource/Rendering/Material/ForwardRenderMaterialGroupManager.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Base/BlendMode.h>
#include <ChilliSource/Rendering/Base/CullFace.h>
#include <ChilliSource/Rendering/Base/ForwardRenderPasses.h>
#include <ChilliSource/Rendering/Shader/Shader.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(ForwardRenderMaterialGroupManager);
    
    //------------------------------------------------------------------------------
    bool ForwardRenderMaterialGroupManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderMaterialGroupManager::InterfaceID == interfaceId || ForwardRenderMaterialGroupManager::InterfaceID == interfaceId);
    }
    
    //------------------------------------------------------------------------------
    const RenderMaterialGroup* ForwardRenderMaterialGroupManager::CreateUnlitRenderMaterialGroup(const RenderTexture* renderTexture, bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled,
                                                                                                 bool isDepthTestEnabled, bool isFaceCullingEnabled, BlendMode sourceBlendMode, BlendMode destinationBlendMode,
                                                                                                 CullFace cullFace, const Colour& emissiveColour, const Colour& ambientColour) noexcept
    {
        std::vector<const RenderTexture*> renderTextures { renderTexture };
        
        //TODO: Create RenderMaterials from pools.
        auto staticMeshRM = RenderMaterialUPtr(new RenderMaterial(m_staticMeshUnlit->GetRenderShader(), renderTextures, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled,
                                                                  isFaceCullingEnabled, sourceBlendMode, destinationBlendMode, cullFace, emissiveColour, ambientColour, Colour::k_black, Colour::k_black));
        auto spriteRM = RenderMaterialUPtr(new RenderMaterial(m_spriteUnlit->GetRenderShader(), renderTextures, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled,
                                                              isFaceCullingEnabled, sourceBlendMode, destinationBlendMode, cullFace, emissiveColour, ambientColour, Colour::k_black, Colour::k_black));
        
        //TODO: Add support for animated vertex format.
        
        std::array<const RenderMaterial*, RenderMaterialGroup::k_maxPasses> staticMeshRenderMaterials {};
        std::array<const RenderMaterial*, RenderMaterialGroup::k_maxPasses> spriteMeshRenderMaterials {};
        
        if (isTransparencyEnabled)
        {
            staticMeshRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_transparent)] = staticMeshRM.get();
            spriteMeshRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_transparent)] = spriteRM.get();
        }
        else
        {
            staticMeshRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_base)] = staticMeshRM.get();
            spriteMeshRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_base)] = spriteRM.get();
        }
        
        std::vector<RenderMaterialGroup::Collection> collections
        {
            RenderMaterialGroup::Collection(VertexFormat::k_staticMesh, staticMeshRenderMaterials),
            RenderMaterialGroup::Collection(VertexFormat::k_sprite, spriteMeshRenderMaterials)
        };
        
        std::vector<RenderMaterialUPtr> renderMaterials;
        renderMaterials.push_back(std::move(staticMeshRM));
        renderMaterials.push_back(std::move(spriteRM));

        RenderMaterialGroupUPtr renderMaterialGroup(new RenderMaterialGroup(std::move(renderMaterials), collections));
        auto renderMaterialGroupRaw = renderMaterialGroup.get();
        AddRenderMaterialGroup(std::move(renderMaterialGroup));
        
        return renderMaterialGroupRaw;
    }
    
    //------------------------------------------------------------------------------
    const RenderMaterialGroup* ForwardRenderMaterialGroupManager::CreateBlinnRenderMaterialGroup(const RenderTexture* renderTexture, const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour,
                                                                                                 const Colour& specularColour) noexcept
    {
        std::vector<const RenderTexture*> renderTextures { renderTexture };
        
        //TODO: Create RenderMaterials from pools.
        auto staticMeshBaseRM = RenderMaterialUPtr(new RenderMaterial(m_staticMeshBlinnBase->GetRenderShader(), renderTextures, false, true, true, true, true, BlendMode::k_one, BlendMode::k_oneMinusSourceAlpha,
                                                                      CullFace::k_back, emissiveColour, ambientColour, Colour::k_black, Colour::k_black));
        auto staticMeshDirectionalRM = RenderMaterialUPtr(new RenderMaterial(m_staticMeshBlinnDirectional->GetRenderShader(), renderTextures, true, true, false, true, true, BlendMode::k_one, BlendMode::k_one,
                                                                             CullFace::k_back, Colour::k_black, Colour::k_black, diffuseColour, specularColour));
        
        //TODO: Add support for animated vertex format.
        
        std::array<const RenderMaterial*, RenderMaterialGroup::k_maxPasses> staticMeshRenderMaterials {};
        staticMeshRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_base)] = staticMeshBaseRM.get();
        staticMeshRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_directionalLight)] = staticMeshDirectionalRM.get();
        
        std::vector<RenderMaterialGroup::Collection> collections { RenderMaterialGroup::Collection(VertexFormat::k_staticMesh, staticMeshRenderMaterials) };
        
        std::vector<RenderMaterialUPtr> renderMaterials;
        renderMaterials.push_back(std::move(staticMeshBaseRM));
        renderMaterials.push_back(std::move(staticMeshDirectionalRM));
        
        RenderMaterialGroupUPtr renderMaterialGroup(new RenderMaterialGroup(std::move(renderMaterials), collections));
        auto renderMaterialGroupRaw = renderMaterialGroup.get();
        AddRenderMaterialGroup(std::move(renderMaterialGroup));
        
        return renderMaterialGroupRaw;
    }
    
    //------------------------------------------------------------------------------
    const RenderMaterialGroup* ForwardRenderMaterialGroupManager::CreateBlinnShadowedRenderMaterialGroup(const RenderTexture* renderTexture, const Colour& emissiveColour, const Colour& ambientColour,
                                                                                                         const Colour& diffuseColour, const Colour& specularColour) noexcept
    {
        std::vector<const RenderTexture*> renderTextures { renderTexture };
        
        //TODO: Create RenderMaterials from pools.
        auto staticMeshBaseRM = RenderMaterialUPtr(new RenderMaterial(m_staticMeshBlinnBase->GetRenderShader(), renderTextures, false, true, true, true, true, BlendMode::k_one, BlendMode::k_oneMinusSourceAlpha,
                                                                      CullFace::k_back, emissiveColour, ambientColour, Colour::k_black, Colour::k_black));
        auto staticMeshDirectionalRM = RenderMaterialUPtr(new RenderMaterial(m_staticMeshBlinnShadowedDirectional->GetRenderShader(), renderTextures, true, true, false, true, true, BlendMode::k_one, BlendMode::k_one,
                                                                             CullFace::k_back, Colour::k_black, Colour::k_black, diffuseColour, specularColour));
        
        //TODO: Add support for animated vertex format.
        
        std::array<const RenderMaterial*, RenderMaterialGroup::k_maxPasses> staticMeshRenderMaterials {};
        staticMeshRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_base)] = staticMeshBaseRM.get();
        staticMeshRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_directionalLight)] = staticMeshDirectionalRM.get();
        
        std::vector<RenderMaterialGroup::Collection> collections { RenderMaterialGroup::Collection(VertexFormat::k_staticMesh, staticMeshRenderMaterials) };
        
        std::vector<RenderMaterialUPtr> renderMaterials;
        renderMaterials.push_back(std::move(staticMeshBaseRM));
        renderMaterials.push_back(std::move(staticMeshDirectionalRM));
        
        RenderMaterialGroupUPtr renderMaterialGroup(new RenderMaterialGroup(std::move(renderMaterials), collections));
        auto renderMaterialGroupRaw = renderMaterialGroup.get();
        AddRenderMaterialGroup(std::move(renderMaterialGroup));
        
        return renderMaterialGroupRaw;
    }
    
    //------------------------------------------------------------------------------
    void ForwardRenderMaterialGroupManager::OnInit() noexcept
    {
        auto resourcePool = Application::Get()->GetResourcePool();
        
        m_spriteUnlit = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Sprite-Unlit-Base.csshader");
        
        m_staticMeshUnlit = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/StaticMesh-Unlit-Base.csshader");
        m_staticMeshBlinnBase = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/StaticMesh-Blinn-Base.csshader");
        m_staticMeshBlinnDirectional = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/StaticMesh-Blinn-Directional.csshader");
        m_staticMeshBlinnShadowedDirectional = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/StaticMesh-BlinnShadowed-Directional.csshader");
    }
    
    //------------------------------------------------------------------------------
    void ForwardRenderMaterialGroupManager::OnDestroy() noexcept
    {
        m_staticMeshUnlit.reset();
        m_staticMeshBlinnBase.reset();
        m_staticMeshBlinnDirectional.reset();
    }
}
