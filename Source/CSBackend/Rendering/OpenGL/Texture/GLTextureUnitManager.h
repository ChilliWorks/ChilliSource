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

#ifndef _CSBACKEND_RENDERING_OPENGL_TEXTURE_TEXTUREUNITMANAGER_H_
#define _CSBACKEND_RENDERING_OPENGL_TEXTURE_TEXTUREUNITMANAGER_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <ChilliSource/ChilliSource.h>

#include <vector>

namespace CSBackend
{
    namespace OpenGL
    {
        /// Provides the ability to bind a number of textures to different opengl texture
        /// units.
        ///
        /// This is not thread-safe and should only be accessed from the render thread.
        ///
        class GLTextureUnitManager final
        {
        public:
            CS_DECLARE_NOCOPY(GLTextureUnitManager);
            
            GLTextureUnitManager() noexcept;
            
            /// @return The number of available texture slots.
            ///
            u32 GetNumTextureSlots() noexcept { return u32(m_boundTextures.size()); }
            
            /// Binds the given list of textures to texture units, in order. If the list is
            /// greater in size than the number of available texture units this will assert.
            ///
            /// @param target
            ///     Texture2D or Cubemap
            /// @param textures
            ///     The list of textures which should be bound.
            /// @param startingBindIdx
            ///     The index to start binding from
            ///
            void Bind(GLenum target, const std::vector<const ChilliSource::RenderTexture*>& textures, u32 startingBindIdx) noexcept;
            
            /// Binds an additional texture to the first free slot, returning the slot index.
            ///
            /// @param target
            ///     Texture2D or Cubemap
            /// @param texture
            ///     The additional texture which should be bound.
            ///
            /// @return The slot index of the additional texture.
            ///
            GLuint BindAdditional(GLenum target, const ChilliSource::RenderTexture* texture) noexcept;
            
            /// Clears all texture slots. This doesn't unbind them from the OpenGL context, it
            /// simply clears the cache to force them to be re-bound the next time Bind() is called
            ///
            void Reset() noexcept;
            
        private:
            /// Finds a free texture unit or if the texture is already bound then the unit it is bound to, asserting if they are all currently full.
            ///
            /// @return A free texture unit (or the unit of the given texture if already bound).
            ///
            GLint GetBoundOrAvailableUnit(const ChilliSource::RenderTexture* texture) const noexcept;
            
            std::vector<const ChilliSource::RenderTexture*> m_boundTextures;
        };
    }
}

#endif
