//
//  CubeMap.h
//  Chilli Source
//  Created by Scott Downie on 15/07/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#ifndef _CHILLISOURCE_BACKEND_RENDERING_OPENGL_TEXTURE_CUBEMAP_H_
#define _CHILLISOURCE_BACKEND_RENDERING_OPENGL_TEXTURE_CUBEMAP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>

#include <array>

namespace ChilliSource
{
	namespace OpenGL
	{
        //--------------------------------------------------------------
        /// OpenGL cubemap resource created from 6 images into a format
        /// that can be used by OpenGL/GPU.
        ///
        /// @author S Downie
        //--------------------------------------------------------------
		class Cubemap final : public Rendering::Cubemap
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(Cubemap);
            //--------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface ID type
            ///
			/// @return Whether the object is of this type
			//--------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------
            /// Construct the cubemap from the given image data.
            /// The texture will take ownership of the image data
            ///
            /// Order is as follows:
            /// - Pos X
            /// - Neg X
            /// - Pos Y
            /// - Neg Y
            /// - Pos Z
            /// - Neg Z
            ///
            /// @author S Downie
            ///
            /// @param Texture descriptors (6)
            /// @param Image datas (6)
            //--------------------------------------------------
            void Build(const std::array<Rendering::Texture::Descriptor, 6>& in_descs, const std::array<Rendering::Texture::TextureDataUPtr, 6>& in_datas) override;
			//--------------------------------------------------------------
            /// Binds this cubemap to the given texture unit allowing it to
            /// be accessed by the shaders and operations to be performed on it
            ///
            /// @author S Downie
            ///
            /// @param Texture unit
            //--------------------------------------------------------------
			void Bind(u32 in_texUnit = 0) override;
            //--------------------------------------------------------------
            /// Unbind this cubemap from its current texture unit. This
            /// means it can no longer be used or changed until rebound.
            ///
            /// @author S Downie
            //--------------------------------------------------------------
			void Unbind() override;
			//--------------------------------------------------------------
            /// Future sampling of the cubemap will use the given filter function
            ///
            /// @author S Downie
            ///
            /// @param Filter mode
            //--------------------------------------------------------------
            void SetFilterMode(Rendering::Texture::FilterMode in_mode) override;
            //--------------------------------------------------------------
            /// Future sampling of the cubemap will use the given wrap mode
            ///
            /// @author S Downie
            ///
            /// @param Horizontal wrapping
            /// @param Vertical wrapping
            //--------------------------------------------------------------
			void SetWrapMode(Rendering::Texture::WrapMode in_sMode, Rendering::Texture::WrapMode in_tMode) override;
            //--------------------------------------------------------------
            /// Generate mip map levels for the cubemap to reduce
            /// aliasing. This will bind the cubemap and generate the
            /// 3 mip map levels.
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            void GenerateMipMaps() override;
			//--------------------------------------------------
			/// @author S Downie
			///
			/// @return The GL generated cubemap handle
			//--------------------------------------------------
			GLuint GetCubemapHandle() const;
            //--------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether or not this was told to use
            ///			mip maps
            //--------------------------------------------------
            bool HasMipMaps() const;
            //--------------------------------------------------
            /// @author S Downie
            ///
            /// @param Face
            ///
            /// @return The format of the image used to create
            ///			the cubemap face.
            //--------------------------------------------------
            Core::ImageFormat GetFormat(Face in_face) const;
            //--------------------------------------------------
            /// Destroys the cubemap and resets it to the state
            /// prior to build being called.
            ///
            /// @author S Downie
            //--------------------------------------------------
            void Destroy();
            //--------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //--------------------------------------------------
			~Cubemap();
			
		private:
            
            friend Rendering::CubemapUPtr Rendering::Cubemap::Create();
			//----------------------------------------------------------
            /// Private constructor to enforce the use of the factory
            /// method
            ///
            /// @author S Downie
            //----------------------------------------------------------
			Cubemap();
			
		private:
            
            Rendering::RenderCapabilities* m_renderCapabilities;
            TextureUnitSystem* m_texUnitSystem;
            
            GLuint m_cubemapHandle = 0;
            
            Rendering::Texture::FilterMode m_filterMode = Rendering::Texture::FilterMode::k_bilinear;
            Rendering::Texture::WrapMode m_sWrapMode = Rendering::Texture::WrapMode::k_clamp;
            Rendering::Texture::WrapMode m_tWrapMode = Rendering::Texture::WrapMode::k_clamp;
            
            std::array<Core::ImageFormat, 6> m_formats;
            
            bool m_hasFilterModeChanged = true;
            bool m_hasWrapModeChanged = true;
            bool m_hasMipMaps = false;
        };
	}
}

#endif
