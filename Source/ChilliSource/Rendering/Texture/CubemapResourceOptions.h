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


#ifndef _CHILLISOURCE_RENDERING_TEXTURE_CUBEMAPRESOURCEOPTIONS_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_CUBEMAPRESOURCEOPTIONS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/IResourceOptions.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureFilterMode.h>
#include <ChilliSource/Rendering/Texture/TextureWrapMode.h>

namespace ChilliSource
{
    //-------------------------------------------------------
    /// Custom options for loading a cubempap
    ///
    /// @author S Downie
    //-------------------------------------------------------
    class CubemapResourceOptions final : public IResourceOptions<Cubemap>
    {
    public:
        //-------------------------------------------------------
        /// Constructor
        ///
        /// @author S Downie
        //-------------------------------------------------------
        CubemapResourceOptions() = default;
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
        CubemapResourceOptions(bool in_mipmaps, TextureFilterMode in_filter, TextureWrapMode in_wrapS, TextureWrapMode in_wrapT);
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
        TextureWrapMode GetWrapModeS() const;
        //-------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Wrap T direction mode to create texture with
        //-------------------------------------------------------
        TextureWrapMode GetWrapModeT() const;
        //-------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Filter mode to create texture with
        //-------------------------------------------------------
        TextureFilterMode GetFilterMode() const;
        
    private:
        
        //-------------------------------------------------------
        /// The options for loading textures. These are held
        /// in a struct to more easily allow hashing of the data
        ///
        /// @author S Downie
        //-------------------------------------------------------
        struct Options
        {
            TextureWrapMode m_wrapModeS = TextureWrapMode::k_clamp;
            TextureWrapMode m_wrapModeT = TextureWrapMode::k_clamp;
            TextureFilterMode m_filterMode = TextureFilterMode::k_bilinear;
            bool m_hasMipMaps = false;
        };
        
        Options m_options;
    };
}

#endif
