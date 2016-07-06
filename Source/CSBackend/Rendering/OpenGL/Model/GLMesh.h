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

#ifndef _CSBACKEND_RENDERING_OPENGL_MODEL_GLMESH_H_
#define _CSBACKEND_RENDERING_OPENGL_MODEL_GLMESH_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// A container for all functionality pertaining to a single OpenGL mesh, including
        /// loading, unloading and binding the mesh. Binding includes setting up the relevant
        /// shader attributes.
        ///
        /// This is not thread-safe and should only be accessed from the render thread.
        ///
        class GLMesh final
        {
        public:
            CS_DECLARE_NOCOPY(GLMesh);
            
            /// Creates a new OpenGL mesh with the given mesh data and description.
            ///
            /// @param vertexFormat
            ///     The vertex format.
            /// @param vertexData
            ///     The vertex data.
            /// @param vertexDataSize
            ///     The size of the vertex data.
            /// @param indexData
            ///     The size of the index data.
            /// @param indexDataSize
            ///     The size of the index data.
            ///
            GLMesh(const ChilliSource::VertexFormat& vertexFormat, const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept;
            
            /// Binds the mesh for use and applies attibutes to the given shader.
            ///
            /// @param glShader
            ///     The shader to apply attributes to.
            ///
            void Bind(GLShader* glShader) noexcept;
            
            /// Called when the GLContext has been lost. Function will set a flag to handle safe
            /// destructing of this object, preventing us from trying to delete invalid memory.
            ///
            void InvalidateContext() noexcept { m_contextInvalid = true; }
            
            /// Destroys the OpenGL mesh that this represents.
            ///
            ~GLMesh() noexcept;
            
        private:
            ChilliSource::VertexFormat m_vertexFormat;
            GLuint m_vertexBufferHandle = 0;
            GLuint m_indexBufferHandle = 0;
            
            bool m_contextInvalid = false;
        };
    }
}

#endif
