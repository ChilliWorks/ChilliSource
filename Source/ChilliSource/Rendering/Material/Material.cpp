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

#include <ChilliSource/Rendering/Base/BlendMode.h>
#include <ChilliSource/Rendering/Base/CullFace.h>

namespace ChilliSource
{
	namespace Rendering
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

		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        bool Material::IsCacheValid() const
        {
            return m_isCacheValid;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        bool Material::IsVariableCacheValid() const
        {
            return m_isVariableCacheValid;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Material::SetCacheValid()
        {
            m_isCacheValid = true;
            m_isVariableCacheValid = true;
        }
		//----------------------------------------------------------
		//----------------------------------------------------------
		bool Material::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == Material::InterfaceID;
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        const ShaderCSPtr& Material::GetShader(ShaderPass in_pass) const
        {
            CS_ASSERT(in_pass != ShaderPass::k_total, "Invalid shader pass when fetching material shader");
            return m_shaders[(u32)in_pass];
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Material::SetShader(ShaderPass in_pass, const ShaderCSPtr& in_shader)
        {
            CS_ASSERT(in_pass != ShaderPass::k_total, "Invalid shader pass when setting material shader");
            
            m_isCacheValid = false;
            m_shaders[(u32)in_pass] = in_shader;
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
			return m_textures.size();
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
		void Material::SetEmissive(const Core::Colour& in_emissive)
		{
			m_emissive = in_emissive;
            
            m_isCacheValid = false;
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		const Core::Colour& Material::GetEmissive() const
		{
			return m_emissive;
		}
        //----------------------------------------------------------
		//----------------------------------------------------------
		void Material::SetAmbient(const Core::Colour& in_ambient)
		{
			m_ambient = in_ambient;
            
            m_isCacheValid = false;
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		const Core::Colour& Material::GetAmbient() const
		{
			return m_ambient;
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		void Material::SetDiffuse(const Core::Colour& in_diffuse)
		{
			m_diffuse = in_diffuse;
            
            m_isCacheValid = false;
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		const Core::Colour& Material::GetDiffuse() const
		{
			return m_diffuse;
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		void Material::SetSpecular(const Core::Colour& in_specular)
		{
			m_specular = in_specular;
            
            m_isCacheValid = false;
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		const Core::Colour& Material::GetSpecular() const
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
		void Material::SetShaderVar(const std::string& in_varName, const Core::Vector2& in_value)
		{
			m_vec2Vars[in_varName] = in_value;
            
            m_isCacheValid = false;
            m_isVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Material::SetShaderVar(const std::string& in_varName, const Core::Vector3& in_value)
		{
			m_vec3Vars[in_varName] = in_value;
            
            m_isCacheValid = false;
            m_isVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Material::SetShaderVar(const std::string& in_varName, const Core::Vector4& in_value)
		{
            m_vec4Vars[in_varName] = in_value;
            
            m_isCacheValid = false;
            m_isVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Material::SetShaderVar(const std::string& in_varName, const Core::Matrix4& in_value)
		{
			m_mat4Vars[in_varName] = in_value;
            
            m_isCacheValid = false;
            m_isVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Material::SetShaderVar(const std::string& in_varName, const Core::Colour& in_value)
		{
            m_colourVars[in_varName] = in_value;
            
            m_isCacheValid = false;
            m_isVariableCacheValid = false;
		}
	}
}

