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

#include <ChilliSource/Rendering/Model/RenderMeshBatch.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderMeshBatch::Mesh::Mesh(const Matrix4& worldMatrix, u32 numVertices, u32 numIndices, const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept
        : m_worldMatrix(worldMatrix), m_numVertices(numVertices), m_numIndices(numIndices), m_vertexData(vertexData), m_vertexDataSize(vertexDataSize), m_indexData(indexData), m_indexDataSize(indexDataSize)
    {
        CS_ASSERT(vertexData, "Must supply vertex data.");
        CS_ASSERT(vertexDataSize > 0, "Vertex data must have a greater than zero size.");
        CS_ASSERT(m_numVertices > 0, "Must have some vertices.");
        CS_ASSERT((!m_indexData && m_indexDataSize == 0 && m_numIndices == 0) || (m_indexData && m_indexDataSize > 0 && m_numIndices > 0),
                  "If there is index data then the size must be greater than zero, other wise in must be zero.");
    }

    //------------------------------------------------------------------------------
    RenderMeshBatch::RenderMeshBatch(PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, std::vector<Mesh> meshes) noexcept
        : m_polygonType(polygonType), m_vertexFormat(vertexFormat), m_indexFormat(indexFormat), m_meshes(std::move(meshes))
    {
        CS_ASSERT(m_meshes.size() > 0, "Cannot create a batch with zero size.");
        
        for (const auto& mesh : m_meshes)
        {
            CS_ASSERT((mesh.GetIndexDataSize() > 0) == (m_meshes[0].GetIndexDataSize() > 0), "Either all meshes must have indices, or all meshes must have no indices.");
            
            m_numVertices += mesh.GetNumVertices();
            m_numIndices += mesh.GetNumIndices();
            m_vertexDataSize += mesh.GetVertexDataSize();
            m_indexDataSize += mesh.GetIndexDataSize();
        }
    }
}
