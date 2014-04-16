//
//  Texture.h
//  Chilli Source
//  Created by Scott Downie on 01/10/2010.
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

#ifndef _CHILLISOURCE_BACKEND_RENDERING_OPENGL_TEXTURE_TEXTURE_H_
#define _CHILLISOURCE_BACKEND_RENDERING_OPENGL_TEXTURE_TEXTURE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
	namespace OpenGL
	{
        //--------------------------------------------------------------
        /// OpenGL texture resource created from an image into a format
        /// that can be used by OpenGL/GPU.
        ///
        /// @author S Downie
        //--------------------------------------------------------------
		class Texture final : public Rendering::Texture
		{
		public:
            
            //--------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface ID type
            ///
			/// @return Whether the object is of this type
			//--------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------
            /// Construct the texture from the given image data.
            /// The texture will take ownership of the image data
            ///
            /// @author S Downie
            ///
            /// @param Texture descriptor
            /// @param Image data
            //--------------------------------------------------
            void Build(const Descriptor& in_desc, TextureDataUPtr in_data) override;
			//--------------------------------------------------------------
            /// Binds this texture to the given texture unit allowing it to
            /// be accessed by the shaders and operations to be performed on it
            ///
            /// @author S Downie
            ///
            /// @param Texture unit
            //--------------------------------------------------------------
			void Bind(u32 in_texUnit = 0) override;
            //--------------------------------------------------------------
            /// Unbind this texture from its current texture unit. This
            /// means it can no longer be used or changed until rebound.
            ///
            /// @author S Downie
            //--------------------------------------------------------------
			void Unbind() override;
			//--------------------------------------------------------------
            /// Future sampling of the texture will use the given filter function
            ///
            /// @author S Downie
            ///
            /// @param Filter mode
            //--------------------------------------------------------------
            void SetFilterMode(FilterMode in_mode) override;
            //--------------------------------------------------------------
            /// Future sampling of the texture will use the given wrap mode
            ///
            /// @author S Downie
            ///
            /// @param Horizontal wrapping
            /// @param Vertical wrapping
            //--------------------------------------------------------------
			void SetWrapMode(WrapMode in_sMode, WrapMode in_tMode) override;
            //--------------------------------------------------------------
            /// Generate mip map levels for the texture to reduce
            /// aliasing. This will bind the texture and generate the
            /// 3 mip map levels.
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            void GenerateMipMaps() override;
			//--------------------------------------------------
			/// @author S Downie
			///
			/// @return The GL generated texture handle
			//--------------------------------------------------
			GLuint GetTextureHandle() const;
            //--------------------------------------------------
            /// @author I Copland
            ///
            /// @return Whether or not this was told to use
            ///			mip maps
            //--------------------------------------------------
            bool HasMipMaps() const;
            //--------------------------------------------------
            /// @author I Copland
            ///
            /// @return The format of the image used to create
            ///			the texture.
            //--------------------------------------------------
            Core::ImageFormat GetFormat() const;
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The width of the texture in texels
            //--------------------------------------------------------------
            u32 GetWidth() const;
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The height of the texture in texels
            //--------------------------------------------------------------
            u32 GetHeight() const;
            //--------------------------------------------------
            /// Destroys the texture and resets it to the state
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
			~Texture();
			
		private:
            
            friend Rendering::TextureUPtr Rendering::Texture::Create();
			//----------------------------------------------------------
            /// Private constructor to enforce the use of the factory
            /// method
            ///
            /// @author S Downie
            //----------------------------------------------------------
			Texture();
			
		private:
            
            Rendering::RenderCapabilities* m_renderCapabilities;
            TextureUnitSystem* m_texUnitSystem;
            
            GLuint m_texHandle = 0;
            
            u32 m_width = 0;
            u32 m_height = 0;
            
            FilterMode m_filterMode = FilterMode::k_bilinear;
            WrapMode m_sWrapMode = WrapMode::k_clamp;
            WrapMode m_tWrapMode = WrapMode::k_clamp;
            
            Core::ImageFormat m_format;
            
            bool m_hasFilterModeChanged = true;
            bool m_hasWrapModeChanged = true;
            bool m_hasMipMaps = false;
		};
	}
}

#endif
