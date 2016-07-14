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

#ifndef _CHILLISOURCE_RENDERING_MODEL_RENDERMESHBATCH_H_
#define _CHILLISOURCE_RENDERING_MODEL_RENDERMESHBATCH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Rendering/Model/IndexFormat.h>
#include <ChilliSource/Rendering/Model/PolygonType.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

#include <vector>

namespace ChilliSource
{
    /// Contains the description of a series of meshes which can be combined in a single draw
    /// call. All meshes must have the same polygon type, vertex and index format, and if one
    /// mesh contains indices, then they all must.
    ///
    /// A render mesh batch doesn't directly contain any mesh data, merely holding handles to
    /// the data. This means that the described data must outlive the mesh batch.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class RenderMeshBatch final
    {
    public:
        /// A container for information on a single mesh within the mesh batch.
        ///
        /// This is immutable and therefore thread-safe.
        ///
        class Mesh final
        {
        public:
            /// Creates a new mesh with the given mesh data.
            ///
            /// @param worldMatrix
            ///     The world matrix of the mesh.
            /// @param numVertices
            ///     The number of vertices in the mesh.
            /// @param numIndices
            ///     The number of indicews in the mesh.
            /// @param vertexData
            ///     The vertex data of the mesh.
            /// @param vertexDataSize
            ///     The size of the vertex data in bytes.
            /// @param indexData
            ///     The index data of the mesh. May be null if the mesh has no indices.
            /// @param indexDataSize
            ///     The size of the index data in bytes. May be zero if the mesh has no indices.
            ///
            Mesh(const Matrix4& worldMatrix, u32 numVertices, u32 numIndices, const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept;
            
            /// @return The world matrix of the mesh.
            ///
            const Matrix4& GetWorldMatrix() const noexcept { return m_worldMatrix; }
            
            /// @return The number of vertices.
            ///
            u32 GetNumVertices() const noexcept { return m_numVertices; }
            
            /// @return The number of indices.
            ///
            u32 GetNumIndices() const noexcept { return m_numIndices; }
            
            /// @return The vertex data of the mesh.
            ///
            const u8* GetVertexData() const noexcept { return m_vertexData; }
            
            /// @return The size of the vertex data in bytes.
            ///
            u32 GetVertexDataSize() const noexcept { return m_vertexDataSize; }
            
            /// @return The index data of the mesh. May be null if the mesh has no indices.
            ///
            const u8* GetIndexData() const noexcept { return m_indexData; }
            
            /// @return The size of the index data in bytes. May be zero if the mesh has no indices.
            ///
            u32 GetIndexDataSize() const noexcept { return m_indexDataSize; }
            
        private:
            Matrix4 m_worldMatrix;
            u32 m_numVertices;
            u32 m_numIndices;
            const u8* m_vertexData;
            u32 m_vertexDataSize;
            const u8* m_indexData;
            u32 m_indexDataSize;
        };
        
        /// Creates a new batch with the given mesh type info and meshes to batch.
        ///
        /// @param polygonType
        ///     The polygonType of the batch.
        /// @param vertexFormat
        ////    The vertex format of the batch.
        /// @param indexFormat
        ///     The index format of the batch.
        /// @param meshes
        ///     The meshes included in the batch. Should be moved.
        ///
        RenderMeshBatch(PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, std::vector<Mesh> meshes) noexcept;
        
        /// @return The polygonType of the batch.
        ///
        PolygonType GetPolygonType() const noexcept { return m_polygonType; }
        
        /// @return The vertex format of the batch.
        ///
        const VertexFormat& GetVertexFormat() const noexcept { return m_vertexFormat; }
        
        /// @return The index format of the batch.
        ///
        IndexFormat GetIndexFormat() const noexcept { return m_indexFormat; }
        
        /// @return The total number of vertices in the batch.
        ///
        u32 GetNumVertices() const noexcept { return m_numVertices; }
        
        /// @return The total number of indices in the batch.
        ///
        u32 GetNumIndices() const noexcept { return m_numIndices; }
        
        /// @return The total size of the vertex data in the batch.
        ///
        u32 GetVertexDataSize() const noexcept { return m_vertexDataSize; }
        
        /// @return The total size of the index data in the batch.
        ///
        u32 GetIndexDataSize() const noexcept { return m_indexDataSize; }
        
        /// @return The meshes included in the batch.
        ///
        const std::vector<Mesh>& GetMeshes() const noexcept { return m_meshes; }
        
    private:
        PolygonType m_polygonType;
        VertexFormat m_vertexFormat;
        IndexFormat m_indexFormat;
        u32 m_numVertices = 0;
        u32 m_numIndices = 0;
        u32 m_vertexDataSize = 0;
        u32 m_indexDataSize = 0;
        std::vector<Mesh> m_meshes;
    };
}

#endif
