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

#include <vector>

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
            /// @param vertexData
            ///     The vertex data.
            /// @param vertexDataSize
            ///     The size of the vertex data.
            /// @param indexData
            ///     The size of the index data.
            /// @param indexDataSize
            ///     The size of the index data.
            /// @param render
            ///     The size of the index data.
            ///
            GLMesh(const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize, ChilliSource::RenderMesh* renderMesh) noexcept;
            
            /// Binds the mesh for use and applies attibutes to the given shader.
            ///
            /// @param glShader
            ///     The shader to apply attributes to.
            ///
            void Bind(GLShader* glShader) noexcept;
            
            /// Called when we should restore any cached mesh data.
            ///
            /// This will assert if called without having data backed up.
            ///
            void Restore() noexcept;
            
            /// Called when graphics memory is lost, usually through the GLContext being destroyed
            /// on Android. Function will set a flag to handle safe destructing of this object, preventing
            /// us from trying to delete invalid memory.
            ///
            void Invalidate() noexcept { m_invalidData = true; }
            
            /// Destroys the OpenGL mesh that this represents.
            ///
            ~GLMesh() noexcept;
            
        private:
            
            /// Creates a new OpenGL mesh with the given mesh data.
            ///
            /// @param vertexData
            ///     The vertex data.
            /// @param vertexDataSize
            ///     The size of the vertex data.
            /// @param indexData
            ///     The size of the index data.
            /// @param indexDataSize
            ///     The size of the index data.
            ///
            void BuildMesh(const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept;
            
            GLuint m_vertexBufferHandle = 0;
            GLuint m_indexBufferHandle = 0;
            std::vector<std::pair<const GLShader*, GLuint>> m_vaoCache;
            
            std::unique_ptr<const u8[]> m_vertexDataBackup = nullptr;
            std::unique_ptr<const u8[]> m_indexDataBackup = nullptr;
            
            u32 m_indexDataSize = 0;
            u32 m_vertexDataSize = 0;
            u32 m_maxVertexAttributes = 0;
            bool m_areVAOsSupported;
            
            ChilliSource::RenderMesh* m_renderMesh = nullptr;
            
            bool m_invalidData = false;
        };
    }
}

#endif
