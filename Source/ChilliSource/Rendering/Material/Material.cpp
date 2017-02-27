//
//  Material.cpp
//  ChilliSource
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
#include <ChilliSource/Rendering/Base/StencilOp.h>
#include <ChilliSource/Rendering/Base/TestFunc.h>
#include <ChilliSource/Rendering/Material/RenderMaterialGroupManager.h>
#include <ChilliSource/Rendering/Shader/RenderShaderVariables.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>

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
    Material::Material() noexcept
    :   m_srcBlendMode(BlendMode::k_one), m_dstBlendMode(BlendMode::k_oneMinusSourceAlpha),
        m_depthTestFunc(TestFunc::k_lessEqual),
        m_cullFace(CullFace::k_back),
        m_stencilPassOp(StencilOp::k_keep), m_stencilFailOp(StencilOp::k_keep), m_stencilDepthFailOp(StencilOp::k_keep)
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
    MaterialShadingType Material::GetShadingType() const noexcept
    {
        return m_shadingType;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetShadingType(MaterialShadingType in_shadingType) noexcept
    {
        m_shadingType = in_shadingType;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::RemoveAllTextures() noexcept
    {
        m_textures.clear();
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::AddTexture(const TextureCSPtr& in_texture) noexcept
    {
        CS_ASSERT(in_texture != nullptr, "Cannot add null texture to material");
        m_textures.push_back(in_texture);
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetTexture(const TextureCSPtr& in_texture, u32 in_texIndex) noexcept
    {
        CS_ASSERT(in_texIndex < m_textures.size(), "Texture index out of bounds");
        
        m_textures[in_texIndex] = in_texture;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const TextureCSPtr& Material::GetTexture(u32 in_index) const noexcept
    {
        CS_ASSERT(in_index < m_textures.size(), "Texture index out of bounds");
        return m_textures[in_index];
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    u32 Material::GetNumTextures() const noexcept
    {
        return static_cast<u32>(m_textures.size());
    }
    
    //----------------------------------------------------------
    void Material::RemoveAllCubemaps() noexcept
    {
        m_cubemaps.clear();
        m_isCacheValid = false;
    }
    
    //----------------------------------------------------------
    void Material::AddCubemap(const CubemapCSPtr& cubemap) noexcept
    {
        CS_ASSERT(cubemap != nullptr, "Cannot add null cubemap to material");
        m_cubemaps.push_back(cubemap);
        
        m_isCacheValid = false;
    }
    
    //----------------------------------------------------------
    void Material::SetCubemap(const CubemapCSPtr& cubemap, u32 index) noexcept
    {
        CS_ASSERT(index < m_cubemaps.size(), "Cubemap index out of bounds");
        
        m_cubemaps[index] = cubemap;
        
        m_isCacheValid = false;
    }
    
    //----------------------------------------------------------
    const CubemapCSPtr& Material::GetCubemap(u32 index) const noexcept
    {
        CS_ASSERT(index < m_cubemaps.size(), "Cubemap index out of bounds");
        return m_cubemaps[index];
    }
    
    //----------------------------------------------------------
    u32 Material::GetNumCubemaps() const noexcept
    {
        return static_cast<u32>(m_cubemaps.size());
    }
    
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsTransparencyEnabled() const noexcept
    {
        return m_isAlphaBlendingEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetTransparencyEnabled(bool in_enable) noexcept
    {
        m_isCacheValid = m_isAlphaBlendingEnabled != in_enable;
        
        m_isAlphaBlendingEnabled = in_enable;
        m_isDepthWriteEnabled = !in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsColourWriteEnabled() const noexcept
    {
        return m_isColWriteEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetColourWriteEnabled(bool in_enable) noexcept
    {
        m_isCacheValid = m_isColWriteEnabled != in_enable;
        
        m_isColWriteEnabled = in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsDepthWriteEnabled() const noexcept
    {
        return m_isDepthWriteEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetDepthWriteEnabled(bool in_enable) noexcept
    {
        CS_ASSERT(m_isAlphaBlendingEnabled == false, "Cannot enable depth write on transparent object");
        
        m_isCacheValid = m_isDepthWriteEnabled != in_enable;
        
        m_isDepthWriteEnabled = in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsDepthTestEnabled() const noexcept
    {
        return m_isDepthTestEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetDepthTestEnabled(bool in_enable) noexcept
    {
        m_isCacheValid = m_isDepthTestEnabled != in_enable;
        
        m_isDepthTestEnabled = in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::IsFaceCullingEnabled() const noexcept
    {
        return m_isFaceCullingEnabled;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetFaceCullingEnabled(bool in_enable) noexcept
    {
        m_isCacheValid = m_isFaceCullingEnabled != in_enable;
        
        m_isFaceCullingEnabled = in_enable;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetBlendModes(BlendMode in_source, BlendMode in_dest) noexcept
    {
        m_srcBlendMode = in_source;
        m_dstBlendMode = in_dest;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    BlendMode Material::GetSourceBlendMode() const noexcept
    {
        return m_srcBlendMode;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    BlendMode Material::GetDestBlendMode() const noexcept
    {
        return m_dstBlendMode;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetStencilPostTestOps(StencilOp stencilFail, StencilOp depthFail, StencilOp pass) noexcept
    {
        m_stencilFailOp = stencilFail;
        m_stencilDepthFailOp = depthFail;
        m_stencilPassOp = pass;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetStencilTestFunc(TestFunc testFunc, s32 ref, u32 mask) noexcept
    {
        m_stencilTestFunc = testFunc;
        m_stencilTestFuncRef = ref;
        m_stencilTestFuncMask = mask;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetCullFace(CullFace in_cullFace) noexcept
    {
        m_cullFace = in_cullFace;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    CullFace Material::GetCullFace() const noexcept
    {
        return m_cullFace;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetEmissive(const Colour& in_emissive) noexcept
    {
        m_emissive = in_emissive;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const Colour& Material::GetEmissive() const noexcept
    {
        return m_emissive;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetAmbient(const Colour& in_ambient) noexcept
    {
        m_ambient = in_ambient;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const Colour& Material::GetAmbient() const noexcept
    {
        return m_ambient;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetDiffuse(const Colour& in_diffuse) noexcept
    {
        m_diffuse = in_diffuse;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const Colour& Material::GetDiffuse() const noexcept
    {
        return m_diffuse;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::SetSpecular(const Colour& in_specular) noexcept
    {
        m_specular = in_specular;
        
        m_isCacheValid = false;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    const Colour& Material::GetSpecular() const noexcept
    {
        return m_specular;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::PrepCustomShaders(const VertexFormat& vertexFormat, MaterialShadingType fallbackType) noexcept
    {
        m_customShaderVertexFormat = vertexFormat;
        m_customShaderFallbackType = fallbackType;
        m_customShaders.clear();
        
        m_isCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::AddCustomShader(const ShaderCSPtr& shader, RenderPasses pass) noexcept
    {
        m_customShaders.push_back(std::make_pair(shader, pass));
        
        m_isCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, f32 in_value) noexcept
    {
        m_floatVars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Vector2& in_value) noexcept
    {
        m_vec2Vars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Vector3& in_value) noexcept
    {
        m_vec3Vars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Vector4& in_value) noexcept
    {
        m_vec4Vars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Matrix4& in_value) noexcept
    {
        m_mat4Vars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void Material::SetShaderVar(const std::string& in_varName, const Colour& in_value) noexcept
    {
        m_colourVars[in_varName] = in_value;
        
        m_isCacheValid = false;
        m_isVariableCacheValid = false;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    const RenderMaterialGroup* Material::GetRenderMaterialGroup() const noexcept
    {
        CS_RELEASE_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread(), "Must be run in main thread.");
        
        if (!m_isCacheValid || !m_isVariableCacheValid || !m_renderMaterialGroup || !VerifyTexturesAreValid())
        {
            DestroyRenderMaterialGroup();
            
            m_cachedRenderTextures.clear();
            for(const auto& texture : m_textures)
            {
                m_cachedRenderTextures.push_back(texture->GetRenderTexture());
            }
            
            for(const auto& cubemap : m_cubemaps)
            {
                m_cachedRenderTextures.push_back(cubemap->GetRenderTexture());
            }
            
            m_isCacheValid = true;
            m_isVariableCacheValid = true;
            
            switch (m_shadingType)
            {
                case MaterialShadingType::k_unlit:
                    CreateUnlitRenderMaterialGroup();
                    break;
                case MaterialShadingType::k_blinn:
                    CreateBlinnRenderMaterialGroup();
                    break;
                case MaterialShadingType::k_custom:
                    CreateCustomRenderMaterialGroup();
                    break;
                case MaterialShadingType::k_skybox:
                    CreateSkyboxRenderMaterialGroup();
                    break;
                default:
                    CS_LOG_FATAL("Invalid shading type.");
                    
            }
        }
        
        return m_renderMaterialGroup.get();
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::CreateUnlitRenderMaterialGroup() const noexcept
    {
        CS_ASSERT(!m_renderMaterialGroup, "Render material group must be null.");
        CS_ASSERT(m_textures.size() == 1, "Unlit materials must have one texture.");
        CS_ASSERT(m_cubemaps.size() == 0, "Unlit materials must have no cubemaps.");
        CS_ASSERT(m_customShaders.size() == 0, "Unlit materials cannot have custom shaders.");
        CS_ASSERT(m_floatVars.size() == 0, "Unlit materials cannot have custom shader variables.");
        CS_ASSERT(m_vec2Vars.size() == 0, "Unlit materials cannot have custom shader variables.");
        CS_ASSERT(m_vec3Vars.size() == 0, "Unlit materials cannot have custom shader variables.");
        CS_ASSERT(m_vec4Vars.size() == 0, "Unlit materials cannot have custom shader variables.");
        CS_ASSERT(m_mat4Vars.size() == 0, "Unlit materials cannot have custom shader variables.");
        CS_ASSERT(m_colourVars.size() == 0, "Unlit materials cannot have custom shader variables.");
        
        auto renderTexture = m_textures[0]->GetRenderTexture();
        m_renderMaterialGroup = m_renderMaterialGroupManager->CreateUnlitRenderMaterialGroup(renderTexture,
                                                                                             m_isAlphaBlendingEnabled, m_isColWriteEnabled, m_isDepthWriteEnabled, m_isDepthTestEnabled, m_isFaceCullingEnabled, m_isStencilTestEnabled,
                                                                                             m_depthTestFunc,
                                                                                             m_srcBlendMode, m_dstBlendMode,
                                                                                             m_stencilFailOp, m_stencilDepthFailOp, m_stencilPassOp, m_stencilTestFunc, m_stencilTestFuncRef, m_stencilTestFuncMask,
                                                                                             m_cullFace, m_emissive, m_ambient);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::CreateSkyboxRenderMaterialGroup() const noexcept
    {
        CS_ASSERT(!m_renderMaterialGroup, "Render material group must be null.");
        CS_ASSERT(m_textures.size() == 0, "Skybox materials must have no textures.");
        CS_ASSERT(m_cubemaps.size() == 1, "Skybox materials must have 1 cubemap.");
        CS_ASSERT(m_customShaders.size() == 0, "Skybox materials cannot have custom shaders.");
        CS_ASSERT(m_floatVars.size() == 0, "Skybox materials cannot have custom shader variables.");
        CS_ASSERT(m_vec2Vars.size() == 0, "Skybox materials cannot have custom shader variables.");
        CS_ASSERT(m_vec3Vars.size() == 0, "Skybox materials cannot have custom shader variables.");
        CS_ASSERT(m_vec4Vars.size() == 0, "Skybox materials cannot have custom shader variables.");
        CS_ASSERT(m_mat4Vars.size() == 0, "Skybox materials cannot have custom shader variables.");
        CS_ASSERT(m_colourVars.size() == 0, "Skybox materials cannot have custom shader variables.");
        
        auto renderCubemap = m_cubemaps[0]->GetRenderTexture();
        m_renderMaterialGroup = m_renderMaterialGroupManager->CreateSkyboxRenderMaterialGroup(renderCubemap);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::CreateBlinnRenderMaterialGroup() const noexcept
    {
        CS_ASSERT(!m_renderMaterialGroup, "Render material group must be null.");
        CS_ASSERT(m_textures.size() == 1, "Blinn materials must have one texture.");
        CS_ASSERT(m_cubemaps.size() == 0, "Blinn materials must have no cubemaps.");
        CS_ASSERT(!m_isAlphaBlendingEnabled, "Blinn materials must have transparency disabled.");
        CS_ASSERT(m_isDepthWriteEnabled, "Blinn materials must have depth write enabled.");
        CS_ASSERT(m_isColWriteEnabled, "Blinn materials must have colour write enabled.");
        CS_ASSERT(m_isDepthTestEnabled, "Blinn materials must have depth test enabled.");
        CS_ASSERT(m_isFaceCullingEnabled, "Blinn materials must have face culling enabled.");
        CS_ASSERT(m_cullFace == CullFace::k_back, "Blinn materials must use back-face culling.");
        CS_ASSERT(m_customShaders.size() == 0, "Blinn materials cannot have custom shaders.");
        CS_ASSERT(m_floatVars.size() == 0, "Blinn materials cannot have custom shader variables.");
        CS_ASSERT(m_vec2Vars.size() == 0, "Blinn materials cannot have custom shader variables.");
        CS_ASSERT(m_vec3Vars.size() == 0, "Blinn materials cannot have custom shader variables.");
        CS_ASSERT(m_vec4Vars.size() == 0, "Blinn materials cannot have custom shader variables.");
        CS_ASSERT(m_mat4Vars.size() == 0, "Blinn materials cannot have custom shader variables.");
        CS_ASSERT(m_colourVars.size() == 0, "Blinn materials cannot have custom shader variables.");
        
        auto renderTexture = m_textures[0]->GetRenderTexture();
        m_renderMaterialGroup = m_renderMaterialGroupManager->CreateBlinnRenderMaterialGroup(renderTexture, m_emissive, m_ambient, m_diffuse, m_specular);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::CreateCustomRenderMaterialGroup() const noexcept
    {
        CS_ASSERT(!m_renderMaterialGroup, "Render material group must be null.");
        CS_ASSERT(m_customShaders.size() > 0, "Custom material must have at least one custom shader.");
        
        
        std::vector<const RenderTexture*> renderTextures;
        renderTextures.reserve(m_textures.size());
        for (const auto& texture : m_textures)
        {
            CS_ASSERT(texture, "Cannot have a null texture in material");
            renderTextures.push_back(texture->GetRenderTexture());
        }
        
        std::vector<const RenderTexture*> renderCubemaps;
        renderCubemaps.reserve(m_cubemaps.size());
        for (const auto& cubemap : m_cubemaps)
        {
            CS_ASSERT(cubemap, "Cannot have a null cubemap in material");
            renderCubemaps.push_back(cubemap->GetRenderTexture());
        }
        
        std::vector<std::pair<const RenderShader*, RenderPasses>> renderShaders;
        renderShaders.reserve(m_customShaders.size());
        for(const auto& shader : m_customShaders)
        {
            renderShaders.push_back(std::make_pair(shader.first->GetRenderShader(), shader.second));
        }
        
        RenderShaderVariablesUPtr renderShaderVariables(new RenderShaderVariables(m_floatVars, m_vec2Vars, m_vec3Vars, m_vec4Vars, m_mat4Vars, m_colourVars));
        
        m_renderMaterialGroup = m_renderMaterialGroupManager->CreateCustomRenderMaterialGroup(m_customShaderFallbackType, m_customShaderVertexFormat, renderShaders, renderTextures, renderCubemaps,
                                                                                              m_isAlphaBlendingEnabled, m_isColWriteEnabled, m_isDepthWriteEnabled, m_isDepthTestEnabled, m_isFaceCullingEnabled, m_isStencilTestEnabled,
                                                                                              m_depthTestFunc,
                                                                                              m_srcBlendMode, m_dstBlendMode,
                                                                                              m_stencilFailOp, m_stencilDepthFailOp, m_stencilPassOp, m_stencilTestFunc, m_stencilTestFuncRef, m_stencilTestFuncMask,
                                                                                              m_cullFace, m_emissive, m_ambient, m_diffuse, m_specular, std::move(renderShaderVariables));
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void Material::DestroyRenderMaterialGroup() const noexcept
    {
        if (m_renderMaterialGroup)
        {
            auto renderMaterialGroupManager = Application::Get()->GetSystem<RenderMaterialGroupManager>();
            CS_ASSERT(renderMaterialGroupManager, "RenderMaterialGroupManager is required.");
            
            renderMaterialGroupManager->DestroyRenderMaterialGroup(std::move(m_renderMaterialGroup));
        }
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool Material::VerifyTexturesAreValid() const noexcept
    {
        if(m_textures.size() != m_cachedRenderTextures.size())
        {
            return false;
        }
        else
        {
            u32 cacheIndex = 0;
            for(u32 i = 0; i < m_textures.size(); ++i, ++cacheIndex)
            {
                const auto& texture = m_textures[i];
                if(texture->GetRenderTexture() != m_cachedRenderTextures[cacheIndex])
                {
                    return false;
                }
            }
            
            for(u32 i = 0; i < m_cubemaps.size(); ++i, ++cacheIndex)
            {
                const auto& cubemap = m_cubemaps[i];
                if(cubemap->GetRenderTexture() != m_cachedRenderTextures[cacheIndex])
                {
                    return false;
                }
            }
        }
        
        return true;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    Material::~Material() noexcept
    {
        DestroyRenderMaterialGroup();
    }
}

