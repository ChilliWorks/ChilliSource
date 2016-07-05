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

#ifndef _CSBACKEND_RENDERING_OPENGL_TEXTURE_GLTEXTURE_H_
#define _CSBACKEND_RENDERING_OPENGL_TEXTURE_GLTEXTURE_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Math/Vector2.h>

#include <unordered_map>
#include <vector>

namespace CSBackend
{
    namespace OpenGL
    {
        /// A container for all functionality pertaining to a single OpenGL texture, including
        /// loading, unloading and binding of textures.
        ///
        /// This is not thread-safe and should only be accessed from the render thread.
        ///
        class GLTexture final
        {
        public:
            CS_DECLARE_NOCOPY(GLTexture);
            
            /// Creates a new OpenGL texture with the given texture data and description.
            ///
            /// @param data
            ///     The texture data.
            /// @param dataSize
            ///     The size of the texture data.
            /// @param dimensions
            ///     The dimensions of the texture.
            /// @param format
            ///     The image format of the texture.
            /// @param compression
            ///     The compression type of the texture.
            /// @param filterMode
            ///     The filter mode of the texture.
            /// @param wrapModeS
            ///     The s-component wrap mode of the texture.
            /// @param wrapModeT
            ///     The t-component wrap mode of the texture.
            /// @param enableMipmapping
            ///     Whether or not mipmapping should be enabled for the texture.
            ///
            GLTexture(const u8* data, u32 dataSize, const ChilliSource::Integer2& dimensions, ChilliSource::ImageFormat format, ChilliSource::ImageCompression compression,
                      ChilliSource::TextureFilterMode filterMode, ChilliSource::TextureWrapMode wrapModeS, ChilliSource::TextureWrapMode wrapModeT, bool enableMipmapping) noexcept;
            
            /// @return The OpenGL texture handle.
            ///
            GLuint GetHandle() noexcept { return m_handle; }
            
            
            /// Called when the GLContext has been lost. Function will set a flag to handle safe
            /// destructing of this object, preventing us from trying to delete invalid memory.
            ///
            void OnGLContextLost() noexcept { m_contextLost = true; }
            
            /// Destroys the OpenGL texture that this represents.
            ///
            ~GLTexture() noexcept;
            
        private:
            GLuint m_handle = 0;
            bool m_contextLost = false;
        };
    }
}

#endif
