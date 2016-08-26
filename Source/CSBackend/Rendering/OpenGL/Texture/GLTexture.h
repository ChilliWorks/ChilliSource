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
            ///
            GLTexture(const u8* data, u32 dataSize, ChilliSource::RenderTexture* renderTexture) noexcept;
            
            /// @return The OpenGL texture handle.
            ///
            GLuint GetHandle() noexcept { return m_handle; }
            
            /// @return The OpenGL texture handle.
            ///
            bool IsDataInvalid() const noexcept { return m_invalidData; }
            
            /// Called prior to the app suspending and losing context to update
            /// the restorable copy of the image data. This is used primarily
            /// or render textures which are changed after creation
            ///
            void UpdateRestorableBackup() noexcept;
            
            /// Called when we should restore any cached texture data.
            ///
            /// This will assert if called without having data backed up.
            ///
            void Restore() noexcept;
            
            /// Called when graphics memory is lost, usually through the GLContext being destroyed
            /// on Android. Function will set a flag to handle safe destructing of this object, preventing
            /// us from trying to delete invalid memory.
            ///
            void Invalidate() noexcept { m_invalidData = true; }
            
            /// Destroys the OpenGL texture that this represents.
            ///
            ~GLTexture() noexcept;
            
        private:
            
            GLuint m_handle = 0;
            
            ChilliSource::RenderTexture* m_renderTexture;
            
            std::unique_ptr<const u8[]> m_imageDataBackup = nullptr;
            
            u32 m_imageDataSize = 0;
            
            bool m_invalidData = false;
        };
    }
}

#endif
