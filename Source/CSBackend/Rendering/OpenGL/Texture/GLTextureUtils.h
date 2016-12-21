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

#ifndef _CSBACKEND_RENDERING_OPENGL_TEXTURE_GLTEXTUREUTILS_H_
#define _CSBACKEND_RENDERING_OPENGL_TEXTURE_GLTEXTUREUTILS_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <ChilliSource/ChilliSource.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace GLTextureUtils
        {
            /// Uploads the given uncompressed image data to texture memory.
            ///
            /// @param target
            ///     Texture target - 2D or specific cubemap face, etc
            /// @param format
            ///     The format of the image data.
            /// @param dimensions
            ///     The image dimensions.
            /// @param imageData
            ///     The image data.
            ///
            void UploadImageDataNoCompression(GLenum target, ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData);
            
            /// Uploads the given ETC1 image data to texture memory.
            ///
            /// @param target
            ///     Texture target - 2D or specific cubemap face, etc
            /// @param format
            ///     The format of the image data.
            /// @param dimensions
            ///     The image dimensions.
            /// @param imageData
            ///     The image data.
            /// @param imageDataSize
            ///     The size, in bytes, of the image data.
            ///
            void UploadImageDataETC1(GLenum target, ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData, u32 imageDataSize);
            
            /// Uploads the given PVR2 image data to texture memory.
            ///
            /// @param target
            ///     Texture target - 2D or specific cubemap face, etc
            /// @param format
            ///     The format of the image data.
            /// @param dimensions
            ///     The image dimensions.
            /// @param imageData
            ///     The image data.
            /// @param imageDataSize
            ///     The size, in bytes, of the image data.
            ///
            void UploadImageDataPVR2(GLenum target, ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData, u32 imageDataSize);
            
            /// Uploads the given PVR4 image data to texture memory.
            ///
            /// @param target
            ///     Texture target - 2D or specific cubemap face, etc
            /// @param format
            ///     The format of the image data.
            /// @param dimensions
            ///     The image dimensions.
            /// @param imageData
            ///     The image data.
            /// @param imageDataSize
            ///     The size, in bytes, of the image data.
            ///
            void UploadImageDataPVR4(GLenum target, ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData, u32 imageDataSize);
            
            /// Applies the given wrap mode to the currently bound texture.
            ///
            /// @param target
            ///     Texture target - 2D or specific cubemap face, etc
            /// @param wrapModeS
            ///     The s-coordinate wrap mode of the texture.
            /// @param wrapModeT
            ///     The t-coordinate wrap mode of the texture.
            ///
            void ApplyWrapMode(GLenum target, ChilliSource::TextureWrapMode wrapModeS, ChilliSource::TextureWrapMode wrapModeT);
            
            /// Applies the given texture filter mode to the currently bound texture.
            ///
            /// @param target
            ///     Texture target - 2D or specific cubemap face, etc
            /// @param filterMode
            ///     The filter mode of the texutre
            /// @param mipmappingEnabled
            ///     Whether or not mipmapping is enabled for the texture.
            ///
            void ApplyFilterMode(GLenum target, ChilliSource::TextureFilterMode filterMode, bool mipmappingEnabled);
        }
    }
}

#endif
