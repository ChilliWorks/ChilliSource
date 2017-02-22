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

#ifndef _CHILLISOURCE_RENDERING_TEXTURE_RENDERTEXTURE_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_RENDERTEXTURE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Texture/TextureFilterMode.h>
#include <ChilliSource/Rendering/Texture/TextureWrapMode.h>

namespace ChilliSource
{
    /// A standard-layout container for all information needed by the renderer pertaining
    /// to a single texture. This includes things like dimensions, imagine format and
    /// filtering. The render texture does not directly contain texture data, but acts
    /// as a handle which the underlying render system (i.e OpenGL) can use to access the
    /// data.
    ///
    /// This is immutable and therefore thread-safe, aside from the extra data pointer
    /// which should only be accessed on the render thread.
    ///
    class RenderTexture final
    {
    public:
        CS_DECLARE_NOCOPY(RenderTexture);
        
        /// Creates a new instance with the given texture information.
        ///
        /// SHOULD ONLY BE CREATED VIA RenderTextureManager
        ///
        /// @param dimensions
        ///     The texture dimensions.
        /// @param imageFormat
        ///     The image format.
        /// @param imageCompression
        ///     The image compression type.
        /// @param filterMode
        ///     The texture filter mode.
        /// @param wrapModeS
        ///     The s-coordinate wrap mode.
        /// @param wrapModeT
        ///     The t-coordinate wrap mode.
        /// @param isMipmapped
        ///     Whether or not mipmaps are generated for the texture.
        /// @param shouldBackupData
        ///     If the mesh data should be backed up in main memory for restoring it later.
        ///
        RenderTexture(const Integer2& dimensions, ImageFormat imageFormat, ImageCompression imageCompression, TextureFilterMode filterMode, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, bool isMipmapped, bool shouldBackupData) noexcept;
        
        
        /// @return The texture dimensions.
        ///
        const Integer2& GetDimensions() const noexcept { return m_dimensions; }
        
        /// @return The image format.
        ///
        ImageFormat GetImageFormat() const noexcept { return m_imageFormat; }
        
        /// @return The image compression type.
        ///
        ImageCompression GetImageCompression() const noexcept { return m_imageCompression; }
        
        /// @return The texture filter mode.
        ///
        TextureFilterMode GetFilterMode() const noexcept { return m_filterMode; }
        
        /// @return The s-coordinate wrap mode.
        ///
        TextureWrapMode GetWrapModeS() const noexcept { return m_wrapModeS; }
        
        /// @return The t-coordinate wrap mode.
        ///
        TextureWrapMode GetWrapModeT() const noexcept { return m_wrapModeT; }
        
        /// @return Whether or not mipmaps are generated for the texture.
        ///
        bool IsMipmapped() const noexcept { return m_isMipmapped; }
        
        /// @return If the mesh should backup its data.
        ///
        bool ShouldBackupData() const noexcept { return m_shouldBackupData; }
        
        /// This is not thread safe and should only be called from the render thread.
        ///
        /// @return A pointer to render system specific additional information.
        ///
        void* GetExtraData() const noexcept { return m_extraData; }
        
        /// This is not thread safe and should only be called from the render thread.
        ///
        /// @param extraData
        ///     A pointer to render system specific additional information.
        ///
        void SetExtraData(void* extraData) noexcept { m_extraData = extraData; }
        
    private:

        Integer2 m_dimensions;
        ImageFormat m_imageFormat;
        ImageCompression m_imageCompression;
        TextureFilterMode m_filterMode;
        TextureWrapMode m_wrapModeS;
        TextureWrapMode m_wrapModeT;
        bool m_isMipmapped;
        bool m_shouldBackupData = true;
        void* m_extraData = nullptr;
    };
}

#endif
