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

#ifndef _CSBACKEND_RENDERING_OPENGL_TEXTURE_GLCUBEMAP_H_
#define _CSBACKEND_RENDERING_OPENGL_TEXTURE_GLCUBEMAP_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Math/Vector2.h>

#include <array>

namespace CSBackend
{
    namespace OpenGL
    {
        /// A container for all functionality pertaining to a single OpenGL cubemaps consisting of 6 textures, including
        /// loading, unloading and binding of textures.
        ///
        /// This is not thread-safe and should only be accessed from the render thread.
        ///
        class GLCubemap final
        {
        public:
            CS_DECLARE_NOCOPY(GLCubemap);
            
            /// Creates a new OpenGL cubemap with the given texture data and description.
            /// NOTE: Only one description is given as all face textures must have the same settings
            ///
            /// @param textureData
            ///     Texture data for each face
            /// @param dataSize
            ///     Size of the data for any face (as face textures are all the same dimensions and format)
            /// @param renderTexture
            ///     Description of the settings for all cubemap faces
            ///
            GLCubemap(const std::array<std::unique_ptr<const u8[]>, 6>& textureData, u32 dataSize, const ChilliSource::RenderTexture* renderTexture) noexcept;
            
            /// @return The OpenGL texture handle.
            ///
            GLuint GetHandle() noexcept { return m_handle; }
            
            /// @return The OpenGL texture handle.
            ///
            bool IsDataInvalid() const noexcept { return m_invalidData; }
            
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
            
            /// Destroys the OpenGL cubemap that this represents.
            ///
            ~GLCubemap() noexcept;
            
        private:
            
            GLuint m_handle = 0;
            
            const ChilliSource::RenderTexture* m_renderTexture;
            u32 m_imageDataSize = 0;
            
            std::array<std::unique_ptr<const u8[]>, 6> m_imageDataBackup;
            
            bool m_invalidData = false;
        };
    }
}

#endif
