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
#include <ChilliSource/Core/Memory/LinearAllocator.h>
#include <ChilliSource/Rendering/Model/IndexFormat.h>
#include <ChilliSource/Rendering/Model/PolygonType.h>
#include <ChilliSource/Rendering/Model/RenderMeshBatch.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// A container for all functionality pertaining to a single dynamic OpenGL mesh, including
        /// buffer creation, and updating and binding the mesh. Binding includes setting up the
        /// relevant shader attributes. A dynamic mesh does not have a fixed vertex or index format,
        /// instead this is set when the data is bound.
        ///
        /// This also support batching of meshes.
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
            
            /// @return The current polygon type.
            ///
            ChilliSource::PolygonType GetPolygonType() const noexcept { return m_polygonType; }
            
            /// @return The current vertex format.
            ///
            const ChilliSource::VertexFormat& GetVertexFormat() const noexcept { return m_vertexFormat; }
            
            /// @return The current index format.
            ///
            ChilliSource::IndexFormat GetIndexFormat() const noexcept { return m_indexFormat; }
            
            /// @return The current number of vertices.
            ///
            u32 GetNumVertices() const noexcept { return m_numVertices; }
            
            /// @return The current number of indices.
            ///
            u32 GetNumIndices() const noexcept { return m_numIndices; }
            
            /// Updates the vertex and index data stored in the dynamic mesh, binds the mesh for use
            /// and applies attibutes to the given shader.
            ///
            /// @param glShader
            ///     The shader to apply attributes to.
            /// @param polygonType
            ///     The polygon type of the mesh.
            /// @param vertexFormat
            ///     The vertex format.
            /// @param indexFormat
            ///     The index format of the mesh.
            /// @param numVertices
            ///     The number of vertices in the mesh.
            /// @param numIndices
            ///     The number of indices in the mesh.
            /// @param vertexData
            ///     The vertex data.
            /// @param vertexDataSize
            ///     The size of the vertex data.
            /// @param indexData
            ///     The size of the index data.
            /// @param indexDataSize
            ///     The size of the index data.
            ///
            void Bind(GLShader* glShader, ChilliSource::PolygonType polygonType, const ChilliSource::VertexFormat& vertexFormat, ChilliSource::IndexFormat indexFormat, u32 numVertices, u32 numIndices,
                      const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept;
            
            /// Updates the vertex and index data stored in the dynamic mesh, binds the mesh batch for use
            /// and applies attibutes to the given shader.
            ///
            /// @param glShader
            ///     The shader to apply attributes to.
            /// @param polygonType
            ///     The polygon type of the batch.
            /// @param vertexFormat
            ///     The vertex format of the batch.
            /// @param indexFormat
            ///     The index format of the batch.
            /// @param numVertices
            ///     The number of vertices in the batch.
            /// @param numIndices
            ///     The number of indices in the batch.
            /// @param vertexDataSize
            ///     The total size of the vertex data in the batch.
            /// @param indexDataSize
            ///     The total size of the index data in the batch.
            /// @param batchMeshDescs
            ///     The list of meshes that should be batched.
            ///
            void Bind(GLShader* glShader, ChilliSource::PolygonType polygonType, const ChilliSource::VertexFormat& vertexFormat, ChilliSource::IndexFormat indexFormat, u32 numVertices, u32 numIndices,
                      u32 vertexDataSize, u32 indexDataSize, const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes) noexcept;
            
            /// Called when graphics memory is lost, usually through the GLContext being destroyed
            /// on Android. Function will set a flag to handle safe destructing of this object, preventing
            /// us from trying to delete invalid memory.
            ///
            void Invalidate() noexcept { m_invalidData = true; }
            
            ///
            /// Dynamic buffers are multi buffered. This is called after submitting to the GPu
            /// to allow the CPU to upload to the next buffer
            ///
            void SwitchBuffer() noexcept { m_currentBufferIndex = (m_currentBufferIndex + 1) % k_numBuffers; }
            
            /// Destroys the OpenGL dynamic mesh that this represents.
            ///
            ~GLDynamicMesh() noexcept;
            
        private:
            /// Applies the vertex attributes to the given shader.
            ///
            /// @param glShader
            ///     The shader to apply the attributes to.
            ///
            void ApplyVertexAttributes(GLShader* glShader) const noexcept;
            
            ChilliSource::LinearAllocator m_allocator;
            
            u32 m_maxVertexDataSize;
            u32 m_maxIndexDataSize;
            u32 m_maxVertexAttributes;
            
            bool m_areVAOsSupported = false;
            bool m_useMapBuffer = false;
            
            static const u32 k_numBuffers = 3;
            std::array<GLuint, k_numBuffers> m_vertexBufferHandles;
            std::array<GLuint, k_numBuffers> m_indexBufferHandles;
            u32 m_currentBufferIndex = 0;

            ChilliSource::PolygonType m_polygonType = ChilliSource::PolygonType::k_triangle;
            ChilliSource::VertexFormat m_vertexFormat = ChilliSource::VertexFormat::k_staticMesh;
            ChilliSource::IndexFormat m_indexFormat = ChilliSource::IndexFormat::k_short;
            u32 m_numVertices = 0;
            u32 m_numIndices = 0;
            
            bool m_invalidData = false;
        };
    }
}

#endif
