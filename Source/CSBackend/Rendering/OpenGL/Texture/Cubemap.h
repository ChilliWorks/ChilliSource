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

#ifndef _CSBACKEND_RENDERING_OPENGL_TEXTURE_CUBEMAP_H_
#define _CSBACKEND_RENDERING_OPENGL_TEXTURE_CUBEMAP_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>

#include <array>

namespace CSBackend
{
	namespace OpenGL
	{
        //--------------------------------------------------------------
        /// OpenGL cubemap resource created from 6 images into a format
        /// that can be used by OpenGL/GPU.
        ///
        /// @author S Downie
        //--------------------------------------------------------------
		class Cubemap final : public CSRendering::Cubemap
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
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
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
            /// @param Whether the cubemap should be mip-mapped
            /// @param Whether or not the cubemap data should be
            /// restored after a context loss. This involves maintaining
            /// a copy of the cubemap data in memory which is costly
            /// so this should be disabled for any cubemaps that can
            /// easily be recreated. This has no effect on cubemaps that
            /// are loaded from file as they are always restored from
            /// disk. This will only work for RGBA8888, RGB888, RGBA4444
            /// and RGB565 cubemaps.
            //--------------------------------------------------
            void Build(const std::array<CSRendering::Texture::Descriptor, 6>& in_descs, std::array<CSRendering::Texture::TextureDataUPtr, 6>&& in_datas, bool in_mipMap, bool in_restoreCubemapDataEnabled) override;
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
            void SetFilterMode(CSRendering::Texture::FilterMode in_mode) override;
            //--------------------------------------------------------------
            /// Future sampling of the cubemap will use the given wrap mode
            ///
            /// @author S Downie
            ///
            /// @param Horizontal wrapping
            /// @param Vertical wrapping
            //--------------------------------------------------------------
			void SetWrapMode(CSRendering::Texture::WrapMode in_sMode, CSRendering::Texture::WrapMode in_tMode) override;
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
            CSCore::ImageFormat GetFormat(Face in_face) const;
            
#ifdef CS_TARGETPLATFORM_ANDROID
            //--------------------------------------------------
            /// Restores the cubemap after context loss if the
            /// cubemap was not loaded from file. If the cubemap
            /// has restoration data this will be used to
            /// re-populate the cubemap. Otherwise it will be
            /// re-created empty. This should not be called for
            /// any cubemap that was loaded from file.
            ///
            /// @author Ian Copland
            //--------------------------------------------------
            void Restore();
#endif
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
            
            friend CSRendering::CubemapUPtr CSRendering::Cubemap::Create();
			//----------------------------------------------------------
            /// Private constructor to enforce the use of the factory
            /// method
            ///
            /// @author S Downie
            //----------------------------------------------------------
			Cubemap();
			
		private:
            
            CSRendering::RenderCapabilities* m_renderCapabilities;
            TextureUnitSystem* m_texUnitSystem;
            
            GLuint m_cubemapHandle = 0;
            
            CSRendering::Texture::FilterMode m_filterMode = CSRendering::Texture::FilterMode::k_bilinear;
            CSRendering::Texture::WrapMode m_sWrapMode = CSRendering::Texture::WrapMode::k_clamp;
            CSRendering::Texture::WrapMode m_tWrapMode = CSRendering::Texture::WrapMode::k_clamp;
            
            std::array<u32, 6> m_widths;
            std::array<u32, 6> m_heights;
            std::array<CSCore::ImageFormat, 6> m_formats;
            std::array<CSCore::ImageCompression, 6> m_compressions;
            
            bool m_hasFilterModeChanged = true;
            bool m_hasWrapModeChanged = true;
            bool m_hasMipMaps = false;
            
#ifdef CS_TARGETPLATFORM_ANDROID
            bool m_restoreCubemapDataEnabled = false;
            std::array<CSRendering::Texture::TextureDataUPtr, 6> m_restorationDatas;
            std::array<u32, 6> m_restorationDataSizes;
#endif
        };
	}
}

#endif
