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

#ifndef _CHILLISOURCE_RENDERING_TEXTURE_TEXTUREDESC_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_TEXTUREDESC_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Rendering/Texture/TextureFilterMode.h>
#include <ChilliSource/Rendering/Texture/TextureWrapMode.h>

namespace ChilliSource
{
    /// A description of a texture, containing information such as the image format, image
    /// compression type and filter mode. This is used to build new texture instances.
    ///
    /// This is not thread safe and should only be accessed from one thread at a time.
    ///
    class TextureDesc final
    {
    public:
        TextureDesc() = default;
        
        /// Constructs a new instance with the given texture data, dimensions, format and compression.
        ///
        /// @param dimensions
        ///     The texture dimensions.
        /// @param imageFormat
        ///     The format of the image.
        /// @param imageCompression
        ///     The compression type applied to the texture data.
        /// @param restorable
        ///     Whether or not the texture is restored automatically after context loss
        ///
        TextureDesc(const Integer2& dimensions, ImageFormat imageFormat, ImageCompression imageCompression, bool restorable) noexcept;
        
        /// Sets the texture filter mode that will be used when rendering the texture.
        ///
        /// @param filterMode
        ///     The texture filter mode.
        ///
        void SetFilterMode(TextureFilterMode filterMode) noexcept { m_filterMode = filterMode; };
        
        /// Sets the s-coordinate texture wrap mode that will be used when rendering the texture.
        ///
        /// @param wrapModeS
        ///     The s-coordinate texture wrap mode.
        ///
        void SetWrapModeS(TextureWrapMode wrapModeS) noexcept { m_wrapModeS = wrapModeS; };
        
        /// Sets the t-coordinate texture wrap mode that will be used when rendering the texture.
        ///
        /// @param wrapModeS
        ///     The t-coordinate texture wrap mode.
        ///
        void SetWrapModeT(TextureWrapMode wrapModeT) noexcept { m_wrapModeT = wrapModeT; };
        
        /// Sets whether or not the texture should be mipmapped.
        ///
        /// @param mipmappingEnabled
        ///     Whether or not the texture should be mipmapped.
        ///
        void SetMipmappingEnabled(bool mipmappingEnabled) noexcept { m_mipmappingEnabled = mipmappingEnabled; };
        
        /// @return The texture dimensions.
        ///
        const Integer2& GetDimensions() const noexcept { return m_dimensions; }
        
        /// @return The format of the image.
        ///
        ImageFormat GetImageFormat() const noexcept { return m_imageFormat; }
        
        /// @return The compression type of the image.
        ///
        ImageCompression GetImageCompression() const noexcept { return m_imageCompression; }
        
        /// @return The texture filter mode used when rendering the texture.
        ///
        TextureFilterMode GetFilterMode() const noexcept { return m_filterMode; }
        
        /// @return The s-coordinate texture wrap mode used when rendering the texture.
        ///
        TextureWrapMode GetWrapModeS() const noexcept { return m_wrapModeS; }
        
        /// @return The t-coordinate texture wrap mode used when rendering the texture.
        ///
        TextureWrapMode GetWrapModeT() const noexcept { return m_wrapModeT; }
        
        /// @return Whether or not the texture should be mipmapped.
        ///
        bool IsMipmappingEnabled() const noexcept { return m_mipmappingEnabled; }
        
        /// @return Whether or not texture data should be restored on context loss.
        ///
        bool IsRestoreTextureDataEnabled() const noexcept { return m_restoreTextureDataEnabled; }
        
    private:
        
        /// Sets whether or not the texture data should be restored after a context loss. This involves
        /// maintaining a copy of the texture data in memory which is costly so this should be disabled
        /// for any textures that can easily be recreated, i.e any texture that is rendered into every
        /// frame.
        ///
        /// This has no effect on textures that are loaded from file as they are always restored from
        /// disk.
        ///
        /// This will only work for RGBA8888, RGB888, RGBA4444 and RGB565 textures.
        ///
        /// @param restoreTextureDataEnabled
        ///     Whether or not texture data should be restored on context loss.
        ///
        void SetTextureDataRestoreEnabled(bool restoreTextureDataEnabled) noexcept;
        
    private:
        Integer2 m_dimensions;
        ImageFormat m_imageFormat = ImageFormat::k_RGBA8888;
        ImageCompression m_imageCompression = ImageCompression::k_none;
        TextureFilterMode m_filterMode = TextureFilterMode::k_bilinear;
        TextureWrapMode m_wrapModeS = TextureWrapMode::k_clamp;
        TextureWrapMode m_wrapModeT = TextureWrapMode::k_clamp;
        bool m_mipmappingEnabled = false;
        bool m_restoreTextureDataEnabled = true;
    };
}

#endif
