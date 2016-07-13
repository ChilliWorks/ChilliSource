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

#ifndef _CSBACKEND_RENDERING_OPENGL_TARGET_GLTARGETGROUP_H_
#define _CSBACKEND_RENDERING_OPENGL_TARGET_GLTARGETGROUP_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <ChilliSource/ChilliSource.h>

#include <unordered_map>
#include <vector>

namespace CSBackend
{
    namespace OpenGL
    {
        /// A container for all functionality pertaining to a single OpenGL target group, including
        /// creation, deletion and binding of frame buffers.
        ///
        /// This is not thread-safe and should only be accessed from the render thread.
        ///
        class GLTargetGroup final
        {
        public:
            CS_DECLARE_NOCOPY(GLTargetGroup);
            
            /// Creates a new OpenGL texture with the given texture data and description.
            ///
            /// @param renderTargetGroup
            ///     The render target
            ///
            GLTargetGroup(const ChilliSource::RenderTargetGroup* renderTargetGroup) noexcept;
            
            /// Binds the frame buffer that this target group represents for rendering.
            ///
            void Bind() noexcept;
            
            /// Called when we should restore the target group.
            ///
            void Restore() noexcept;
            
            /// Called when graphics memory is lost, usually through the GLContext being destroyed
            /// on Android. Function will set a flag to handle safe destructing of this object, preventing
            /// us from trying to delete invalid memory.
            ///
            void Invalidate() noexcept { m_invalidData = true; }
            
            /// Destroys the OpenGL texture that this represents.
            ///
            ~GLTargetGroup() noexcept;
            
        private:
            
            /// Creates glTextures for the depth|colour buffers of the stored RenderTargetGroup
            ///
            void BuildTargetGroup() noexcept;
            
            const ChilliSource::RenderTargetGroup* m_renderTargetGroup;
            
            GLuint m_frameBufferHandle;
            GLuint m_colourRenderBufferHandle = 0;
            GLuint m_depthRenderBufferHandle = 0;
            
            bool m_invalidData = false;
        };
    }
}

#endif
