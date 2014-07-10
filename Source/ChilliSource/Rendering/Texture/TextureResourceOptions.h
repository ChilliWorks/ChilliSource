//
//  TextureResourceOptions.h
//  Chilli Source
//  Created by Scott Downie on 16/05/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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


#ifndef _CHILLISOURCE_RENDERING_TEXTURE_TEXTURERESOURCEOPTIONS_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_TEXTURERESOURCEOPTIONS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/IResourceOptions.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //-------------------------------------------------------
        /// Custom options for loading a texture
        ///
        /// @author S Downie
        //-------------------------------------------------------
		class TextureResourceOptions final : public Core::IResourceOptions<Rendering::Texture>
		{
		public:
            //-------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
            TextureResourceOptions() = default;
            //-------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param MipMaps enabled
            /// @param Filter mode
            /// @param Wrap mode S
            /// @param Wrap mode T
            /// @param Whether or not the texture data should be
            /// restored after a context loss. This involves maintaining
            /// a copy of the texture data in memory which is costly
            /// so this should be disabled for any textures that can
            /// easily be recreated, i.e any texture that is rendered
            /// into every frame. This has no effect on textures that
            /// are loaded from file as they are always restored from
            /// disk. This will only work for RGBA8888, RGB888, RGBA4444
            /// and RGB565 textures.
            //-------------------------------------------------------
            TextureResourceOptions(bool in_mipmaps, Texture::FilterMode in_filter, Texture::WrapMode in_wrapS, Texture::WrapMode in_wrapT, bool in_restoreTextureDataEnabled);
            //-------------------------------------------------------
            /// Generate a unique hash based on the
            /// currently set options
            ///
            /// @author S Downie
            ///
            /// @return Hash of the options contents
            //-------------------------------------------------------
            u32 GenerateHash() const override;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the texture should be created with
            /// mip-maps
            //-------------------------------------------------------
            bool IsMipMapsEnabled() const;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Wrap S direction mode to create texture with
            //-------------------------------------------------------
            Texture::WrapMode GetWrapModeS() const;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Wrap T direction mode to create texture with
            //-------------------------------------------------------
            Texture::WrapMode GetWrapModeT() const;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Filter mode to create texture with
            //-------------------------------------------------------
            Texture::FilterMode GetFilterMode() const;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not the texture data should be
            /// restored after a context loss. This involves maintaining
            /// a copy of the texture data in memory which is costly
            /// so this should be disabled for any textures that can
            /// easily be recreated, i.e any texture that is rendered
            /// into every frame. This has no effect on textures that
            /// are loaded from file as they are always restored from
            /// disk. This will only work for RGBA8888, RGB888, RGBA4444
            /// and RGB565 textures.
            //-------------------------------------------------------
            bool IsRestoreTextureDataEnabled() const;
        private:
            
            //-------------------------------------------------------
            /// The options for loading textures. These are held
            /// in a struct to more easily allow hashing of the data
            ///
            /// @author S Downie
            //-------------------------------------------------------
            struct Options
            {
                Texture::WrapMode m_wrapModeS = Texture::WrapMode::k_clamp;
                Texture::WrapMode m_wrapModeT = Texture::WrapMode::k_clamp;
                Texture::FilterMode m_filterMode = Texture::FilterMode::k_bilinear;
                bool m_hasMipMaps = false;
                bool m_restoreTextureDataEnabled = true;
            };
            
            Options m_options;
		};
	}
}

#endif
