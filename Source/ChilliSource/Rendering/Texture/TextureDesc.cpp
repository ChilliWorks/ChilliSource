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

#include <ChilliSource/Rendering/Texture/TextureDesc.h>

namespace ChilliSource
{
    namespace
    {
        /// @param imageFormat
        ///     The image format to check.
        ///
        /// @return Whether or not the given image format can be restored.
        ///
#if CS_ENABLE_DEBUG
        constexpr bool IsImageFormatRestorable(ImageFormat imageFormat) noexcept
        {
            return (imageFormat == ImageFormat::k_RGBA8888 || imageFormat == ImageFormat::k_RGB888 || imageFormat == ImageFormat::k_RGBA4444 || imageFormat == ImageFormat::k_RGB565);
        }
#endif  
    }
    
    //------------------------------------------------------------------------------
    TextureDesc::TextureDesc(const Integer2& dimensions, ImageFormat imageFormat, ImageCompression imageCompression, bool restorable) noexcept
        : m_dimensions(dimensions), m_imageFormat(imageFormat), m_imageCompression(imageCompression)
    {
        SetTextureDataRestoreEnabled(restorable);
    }

    //------------------------------------------------------------------------------
    void TextureDesc::SetTextureDataRestoreEnabled(bool restoreTextureDataEnabled) noexcept
    {
        CS_ASSERT(restoreTextureDataEnabled == false || IsImageFormatRestorable(m_imageFormat), "Only RGBA8888, RGB888, RGBA4444 and RGB565 image formats can have their data restored.");
        
        m_restoreTextureDataEnabled = restoreTextureDataEnabled;
    }
}
