//
//  Material.cpp
//  Chilli Source
//  Created by Scott Downie on 14/10/2010.
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

#include <ChilliSource/Rendering/Material/Material.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Base/BlendMode.h>
#include <ChilliSource/Rendering/Base/CullFace.h>
#include <ChilliSource/Rendering/Material/RenderMaterialGroupManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Material);

    //----------------------------------------------------------
    //----------------------------------------------------------
    MaterialUPtr Material::Create()
    {
        return MaterialUPtr(new Material());
    }
    //------------------------------------------------
    //------------------------------------------------
    Material::Material() 
        : m_srcBlendMode(BlendMode::k_one), m_dstBlendMode(BlendMode::k_oneMinusSourceAlpha), m_cullFace(CullFace::k_back)
    {
        m_renderMaterialGroupManager = Application::Get()->GetSystem<RenderMaterialGroupManager>();
        CS_ASSERT(m_renderMaterialGroupManager, "RenderMaterialGroupManager is required.");
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == Material::InterfaceID;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    Material::ShadingType Material::GetShadingType() const noexcept
    {
        return m_shadingType;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetShadingType(ShadingType in_shadingType) noexcept
    {
        m_shadingType = in_shadingType;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::RemoveAllTextures()
    {
        m_textures.clear();
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::AddTexture(const TextureCSPtr& in_texture)
    {
        CS_ASSERT(in_texture != nullptr, "Cannot add null texture to material");
        m_textures.push_back(in_texture);
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetTexture(const TextureCSPtr& in_texture, u32 in_texIndex)
    {
        CS_ASSERT(in_texIndex < m_textures.size(), "Texture index out of bounds");
        
        m_textures[in_texIndex] = in_texture;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const TextureCSPtr& Material::GetTexture(u32 in_index) const
    {
        CS_ASSERT(in_index < m_textures.size(), "Texture index out of bounds");
        return m_textures[in_index];
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    u32 Material::GetNumTextures() const
    {
        return static_cast<u32>(m_textures.size());
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetCubemap(const CubemapCSPtr& in_cubemap)
    {
        m_cubemap = in_cubemap;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const CubemapCSPtr& Material::GetCubemap() const
    {
        return m_cubemap;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsTransparencyEnabled() const
    {
        return m_isAlphaBlendingEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetTransparencyEnabled(bool in_enable)
    {
        m_isCacheValid = m_isAlphaBlendingEnabled != in_enable;
        
        m_isAlphaBlendingEnabled = in_enable;
        m_isDepthWriteEnabled = !in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsColourWriteEnabled() const
    {
        return m_isColWriteEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetColourWriteEnabled(bool in_enable)
    {
        m_isCacheValid = m_isColWriteEnabled != in_enable;
        
        m_isColWriteEnabled = in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsDepthWriteEnabled() const
    {
        return m_isDepthWriteEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetDepthWriteEnabled(bool in_enable)
    {
        CS_ASSERT(m_isAlphaBlendingEnabled == false, "Cannot enable depth write on transparent object");
        
        m_isCacheValid = m_isDepthWriteEnabled != in_enable;
        
        m_isDepthWriteEnabled = in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsDepthTestEnabled() const
    {
        return m_isDepthTestEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetDepthTestEnabled(bool in_enable)
    {
        m_isCacheValid = m_isDepthTestEnabled != in_enable;
        
        m_isDepthTestEnabled = in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsFaceCullingEnabled() const
    {
        return m_isFaceCullingEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetFaceCullingEnabled(bool in_enable)
    {
        m_isCacheValid = m_isFaceCullingEnabled != in_enable;
        
        m_isFaceCullingEnabled = in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetBlendModes(BlendMode in_source, BlendMode in_dest)
    {
        m_srcBlendMode = in_source;
        m_dstBlendMode = in_dest;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    BlendMode Material::GetSourceBlendMode() const
    {
        return m_srcBlendMode;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    BlendMode Material::GetDestBlendMode() const
    {
        return m_dstBlendMode;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetCullFace(CullFace in_cullFace)
    {
        m_cullFace = in_cullFace;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    CullFace Material::GetCullFace() const
    {
        return m_cullFace;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetEmissive(const Colour& in_emissive)
    {
        m_emissive = in_emissive;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const Colour& Material::GetEmissive() const
    {
        return m_emissive;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetAmbient(const Colour& in_ambient)
    {
        m_ambient = in_ambient;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const Colour& Material::GetAmbient() const
    {
        return m_ambient;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetDiffuse(const Colour& in_diffuse)
    {
        m_diffuse = in_diffuse;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const Colour& Material::GetDiffuse() const
    {
        return m_diffuse;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetSpecular(const Colour& in_specular)
    {
        m_specular = in_specular;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const Colour& Material::GetSpecular() const
    {
        return m_specular;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, f32 in_value)
    {
        m_floatVars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Vector2& in_value)
    {
        m_vec2Vars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Vector3& in_value)
    {
        m_vec3Vars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Vector4& in_value)
    {
        m_vec4Vars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Matrix4& in_value)
    {
        m_mat4Vars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Colour& in_value)
    {
        m_colourVars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    const RenderMaterialGroup* Material::GetRenderMaterialGroup() const noexcept
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread(), "Must be run in main thread.");
        
        if (!m_isCacheValid || !m_isVariableCacheValid || !m_renderMaterialGroup)
        {
            DestroyRenderMaterialGroup();
            
            //TODO: Handle variables
            
            m_isCacheValid = true;
            m_isVariableCacheValid = true;
            
            switch (m_shadingType)
            {
                case ShadingType::k_unlit:
                    m_renderMaterialGroup = CreateUnlitRenderMaterialGroup();
                    break;
                case ShadingType::k_blinn:
                    m_renderMaterialGroup = CreateBlinnRenderMaterialGroup();
                    break;
                default:
                    CS_LOG_FATAL("Invalid shading type.");
                    
            }
        }
        
        return m_renderMaterialGroup;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const RenderMaterialGroup* Material::CreateUnlitRenderMaterialGroup() const noexcept
    {
        CS_ASSERT(m_textures.size() == 1, "Unlit materials must have one texture.");
        
        auto renderTexture = m_textures[0]->GetRenderTexture();
        return m_renderMaterialGroupManager->CreateUnlitRenderMaterialGroup(renderTexture, m_isAlphaBlendingEnabled, m_isColWriteEnabled, m_isDepthWriteEnabled, m_isDepthTestEnabled, m_isFaceCullingEnabled,
                                                                          m_srcBlendMode, m_dstBlendMode, m_cullFace, m_emissive, m_ambient);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const RenderMaterialGroup* Material::CreateBlinnRenderMaterialGroup() const noexcept
    {
        CS_ASSERT(m_textures.size() == 1, "Blinn materials must have one texture.");
        CS_ASSERT(!m_isAlphaBlendingEnabled, "Blinn materials must have transparency disabled.");
        CS_ASSERT(m_isDepthWriteEnabled, "Blinn materials must have depth write enabled.");
        CS_ASSERT(m_isColWriteEnabled, "Blinn materials must have colour write enabled.");
        CS_ASSERT(m_isDepthTestEnabled, "Blinn materials must have depth test enabled.");
        CS_ASSERT(m_isFaceCullingEnabled, "Blinn materials must have face culling enabled.");
        CS_ASSERT(m_cullFace == CullFace::k_back, "Blinn materials must use back-face culling.");
        
        auto renderTexture = m_textures[0]->GetRenderTexture();
        return m_renderMaterialGroupManager->CreateBlinnRenderMaterialGroup(renderTexture, m_emissive, m_ambient, m_diffuse, m_specular);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::DestroyRenderMaterialGroup() const noexcept
    {
        if (m_renderMaterialGroup)
        {
            auto renderMaterialGroupManager = Application::Get()->GetSystem<RenderMaterialGroupManager>();
            CS_ASSERT(renderMaterialGroupManager, "RenderMaterialGroupManager is required.");
            
            renderMaterialGroupManager->DestroyRenderMaterialGroup(m_renderMaterialGroup);
            m_renderMaterialGroup = nullptr;
        }
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    Material::~Material() noexcept
    {
        DestroyRenderMaterialGroup();
    }
}

