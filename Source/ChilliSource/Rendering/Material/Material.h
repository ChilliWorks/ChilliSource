//
//  Material.h
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

#ifndef _CHILLISOURCE_RENDERING_MATERIAL_MATERIAL_H_
#define _CHILLISOURCE_RENDERING_MATERIAL_MATERIAL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Rendering/Base/ShaderPass.h>

#include <array>
#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
        //----------------------------------------------------------
        /// Holds the render state of an object. Used to organise
        /// the rendering of objects and to alter their surface
        /// appearance via lighting, texture, shader, etc.
        ///
        /// @author S Downie
        //----------------------------------------------------------
		class Material final : public Core::Resource
		{
		public:
			CS_DECLARE_NAMEDTYPE(Material);
		
			//----------------------------------------------------------
            /// @author S Downie
            ///
			/// @param Comparison Type
            ///
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return The shader associated with this rendering pass
			//----------------------------------------------------------
			const ShaderCSPtr& GetShader(ShaderPass in_pass) const;
            //----------------------------------------------------------
            /// Associate the given shader with the given render pass
            ///
			/// @author S Downie
            ///
			/// @param Shader pass
            /// @param Shader
			//----------------------------------------------------------
			void SetShader(ShaderPass in_pass, const ShaderCSPtr& in_shader);
            //----------------------------------------------------------
			/// Clear the textures from the slots
            ///
            /// @author S Downie
			//----------------------------------------------------------
			void RemoveAllTextures();
			//----------------------------------------------------------
			/// Add the texture to the end of the list. The index of the
            /// texture in the list corresponds to the texture handle in
            /// the shader.
            ///
            /// NOTE: Due to devices supporting different numbers of
            /// textures it is possible that textures at the end will be
            /// ignored by the renderer. The cubemap can also steal
            /// a texture slot and will take precedence over a texture.
            ///
            /// @author S Downie
			///
			/// @param Texture 
			//----------------------------------------------------------
			void AddTexture(const TextureCSPtr& in_texture);
            //----------------------------------------------------------
            /// Overwrite an exisiting added texture with the given
            /// texture at the given index. If a texture does not
            /// already exist at that slot then it asserts
            ///
            /// @author S Downie
            ///
            /// @param Texture
            /// @param Index
            //----------------------------------------------------------
            void SetTexture(const TextureCSPtr& in_texture, u32 in_texIndex = 0);
			//----------------------------------------------------------
			/// Get Texture at the given index. Will assert
            /// if index is out of bounds.
            ///
            /// @author S Downie
			///
			/// @param Texture index
            ///
			/// @return Texture or null
			//----------------------------------------------------------
			const TextureCSPtr& GetTexture(u32 in_texIndex = 0) const;
			//----------------------------------------------------------
            /// @author S Downie
			///
			/// @return Number of textures set on the material
			//----------------------------------------------------------
			u32 GetNumTextures() const;
            //----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Cubemap
			//----------------------------------------------------------
			void SetCubemap(const CubemapCSPtr& in_cubemap);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Cubemap or null
			//----------------------------------------------------------
			const CubemapCSPtr& GetCubemap() const;
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the object has transparency enabled 
			//----------------------------------------------------------
			bool IsTransparencyEnabled() const;
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Whether transparency is enabled
			//----------------------------------------------------------
			void SetTransparencyEnabled(bool in_enable);
            //----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the object has colour write enabled
			//----------------------------------------------------------
			bool IsColourWriteEnabled() const;
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Whether colour write is enabled
			//----------------------------------------------------------
			void SetColourWriteEnabled(bool in_enable);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the object has depth write enabled 
			//----------------------------------------------------------
			bool IsDepthWriteEnabled() const;
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Whether depth write is enabled
			//----------------------------------------------------------
			void SetDepthWriteEnabled(bool in_enable);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the object has depth test enabled 
			//----------------------------------------------------------
			bool IsDepthTestEnabled() const;
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Whether depth test is enabled
			//----------------------------------------------------------
			void SetDepthTestEnabled(bool in_enable);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the object has face culling enabled 
			//----------------------------------------------------------
			bool IsFaceCullingEnabled() const;
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Whether face culling is enabled
			//----------------------------------------------------------
			void SetFaceCullingEnabled(bool in_enable);
			//----------------------------------------------------------
			/// Tells the render system how to blend pixels based on the
            /// source and destination mode
            ///
            /// @author S Downie
			///
			/// @param Source mode
			/// @param Destination mode
			//----------------------------------------------------------
			void SetBlendModes(BlendMode in_source, BlendMode in_dest);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Source mode of blending functions
			//----------------------------------------------------------
			BlendMode GetSourceBlendMode() const;
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Dest BlendMode of blending functions
			//----------------------------------------------------------
			BlendMode GetDestBlendMode() const;
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The face of the polygon that should be culled
            /// if culling is enabled
			//----------------------------------------------------------
			void SetCullFace(CullFace in_face);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return The face of the polygon that should be culled
            /// if face culling is enabled
			//----------------------------------------------------------
			CullFace GetCullFace() const;
			//----------------------------------------------------------
            /// The emissive colour is used to simulate the light emitted
            /// by the surface of an object (effectively its colour)
            ///
            /// @author S Downie
            ///
			/// @param Emissive colour used in ambient pass
			//----------------------------------------------------------
			void SetEmissive(const Core::Colour& in_emissive);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Emissive colour used in ambient pass
			//----------------------------------------------------------
			const Core::Colour& GetEmissive() const;
            //----------------------------------------------------------
            /// The ambient colour is used to simulate the light absorbed
            /// by the object from light reflections in the scene.
            ///
            /// @author S Downie
            ///
			/// @param Ambient colour used in ambient pass
			//----------------------------------------------------------
			void SetAmbient(const Core::Colour& in_ambient);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Ambient colour used in ambient pass
			//----------------------------------------------------------
			const Core::Colour& GetAmbient() const;
			//----------------------------------------------------------
            /// The diffuse colour is used to simulate the light absorbed
            /// by the object directly from the light source.
            ///
            /// @author S Downie
            ///
			/// @param Diffuse light colour used in subsequent light passes
			//----------------------------------------------------------
			void SetDiffuse(const Core::Colour& in_diffuse);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Diffuse light colour used in subsequent light passes
			//----------------------------------------------------------
			const Core::Colour& GetDiffuse() const;
			//----------------------------------------------------------
            /// The specular colour is used to simulate the light reflected
            /// back by the object creating a highlight.
            ///
			/// @author S Downie
            ///
			/// @param Specular light colour used in subsequent light passes
			//----------------------------------------------------------
			void SetSpecular(const Core::Colour& in_specular);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Specular light colour used in subsequent light passes
			//----------------------------------------------------------
			const Core::Colour& GetSpecular() const;
			//-----------------------------------------------------------
			/// Set the value of the variable with the given name to the
            /// given value
            ///
            /// @author S Downie
			///
			/// @param Variable name
			/// @param Float value
			//-----------------------------------------------------------
			void SetShaderVar(const std::string& in_varName, f32 in_value);
			//-----------------------------------------------------------
			/// Set the value of the variable with the given name to the
            /// given value
            ///
            /// @author S Downie
			///
			/// @param Variable name
			/// @param Vec2 value
			//-----------------------------------------------------------
			void SetShaderVar(const std::string& in_varName, const Core::Vector2& in_value);
			//-----------------------------------------------------------
			/// Set the value of the variable with the given name to the
            /// given value
            ///
            /// @author S Downie
			///
			/// @param Variable name
			/// @param Vec3 value
			//-----------------------------------------------------------
			void SetShaderVar(const std::string& in_varName, const Core::Vector3& in_value);
			//-----------------------------------------------------------
			/// Set the value of the variable with the given name to the
            /// given value
            ///
            /// @author S Downie
			///
			/// @param Variable name
			/// @param Vec4 value
			//-----------------------------------------------------------
			void SetShaderVar(const std::string& in_varName, const Core::Vector4& in_value);
			//-----------------------------------------------------------
			/// Set the value of the variable with the given name to the
            /// given value
            ///
            /// @author S Downie
			///
			/// @param Variable name
			/// @param Mat4 value
			//-----------------------------------------------------------
			void SetShaderVar(const std::string& in_varName, const Core::Matrix4& in_value);
			//-----------------------------------------------------------
			/// Set the value of the variable with the given name to the
            /// given value
            ///
            /// @author S Downie
			///
			/// @param Variable name
			/// @param Colour value
			//-----------------------------------------------------------
			void SetShaderVar(const std::string& in_varName, const Core::Colour& in_value);
            
            //TODO: Remove these once we no longer rely on render system and material to hold our state
            //i.e. when we have a proper render command queue
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether this material has been dirtied
            //----------------------------------------------------------
            bool IsCacheValid() const;
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether this materials shader variables has been dirtied
            //----------------------------------------------------------
            bool IsVariableCacheValid() const;
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param This material has been applied and is no longer
            /// dirty
            //----------------------------------------------------------
            void SetCacheValid();
            
            //TODO: Make private when we rework the render system
            std::unordered_map<std::string, f32> m_floatVars;
			std::unordered_map<std::string, Core::Vector2> m_vec2Vars;
			std::unordered_map<std::string, Core::Vector3> m_vec3Vars;
			std::unordered_map<std::string, Core::Vector4> m_vec4Vars;
			std::unordered_map<std::string, Core::Matrix4> m_mat4Vars;
			std::unordered_map<std::string, Core::Colour> m_colourVars;
            
        private:
            
            friend class Core::ResourcePool;
            //----------------------------------------------------------
            /// Factory method to create an new instance of an empty
            /// material resource. Only called by the resource pool
            ///
            /// @author S Downie
            //----------------------------------------------------------
            static MaterialUPtr Create();
            //----------------------------------------------------------
            /// Private constructor to ensure that the factory method
            /// is used
            ///
            /// @author S Downie
            //----------------------------------------------------------
            Material();

		private:
            
            std::vector<TextureCSPtr> m_textures;
            
            CubemapCSPtr m_cubemap;
            
            std::array<ShaderCSPtr, (u32)ShaderPass::k_total> m_shaders;
			
			Core::Colour m_emissive;
            Core::Colour m_ambient;
			Core::Colour m_diffuse;
			Core::Colour m_specular;
			
			BlendMode m_srcBlendMode;
			BlendMode m_dstBlendMode;
            CullFace m_cullFace;
            
            bool m_isCacheValid = false;
            bool m_isVariableCacheValid = true;

            bool m_isAlphaBlendingEnabled = false;
            bool m_isColWriteEnabled = true;
			bool m_isDepthWriteEnabled = true;
			bool m_isDepthTestEnabled = true;
            bool m_isFaceCullingEnabled = true;
		};
	}
}

#endif
