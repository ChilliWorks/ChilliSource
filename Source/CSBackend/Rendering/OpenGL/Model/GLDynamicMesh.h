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

#ifndef _CSBACKEND_RENDERING_OPENGL_MODEL_GLDYNAMICMESH_H_
#define _CSBACKEND_RENDERING_OPENGL_MODEL_GLDYNAMICMESH_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Model/IndexFormat.h>
#include <ChilliSource/Rendering/Model/PolygonType.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// A container for all functionality pertaining to a single dynamic OpenGL mesh, including
        /// buffer creation, and updating and binding the mesh. Binding includes setting up the
        /// relevant shader attributes. A dynamic mesh does not have a fixed vertex or index format,
        /// instead this is set when the data is bound.
        ///
        /// This is not thread-safe and should only be accessed from the render thread.
        ///
        class GLDynamicMesh final
        {
        public:
            CS_DECLARE_NOCOPY(GLDynamicMesh);
            
            /// Creates a new OpenGL dynamic mesh of the given size.
            ///
            /// @param vertexDataSize
            ///     The size of the vertex data.
            /// @param indexDataSize
            ///     The size of the index data.
            ///
            GLDynamicMesh(u32 vertexDataSize, u32 indexDataSize) noexcept;
            
            /// Updates the vertex and index data stored in the dynamic mesh, binds the mesh for use
            /// and applies attibutes to the given shader.
            ///
            /// @param glShader
            ///     The shader to apply attributes to.
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
            void Bind(GLShader* glShader, const ChilliSource::VertexFormat& vertexFormat, const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept;
            
            /// Called when graphics memory is lost, usually through the GLContext being destroyed
            /// on Android. Function will set a flag to handle safe destructing of this object, preventing
            /// us from trying to delete invalid memory.
            ///
            void Invalidate() noexcept { m_invalidData = true; }
            
            /// Destroys the OpenGL dynamic mesh that this represents.
            ///
            ~GLDynamicMesh() noexcept;
            
        private:
            u32 m_maxVertexDataSize;
            u32 m_maxIndexDataSize;
            GLuint m_vertexBufferHandle = 0;
            GLuint m_indexBufferHandle = 0;
            
            bool m_invalidData = false;
        };
    }
}

#endif
