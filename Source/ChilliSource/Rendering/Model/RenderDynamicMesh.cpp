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

#include <ChilliSource/Rendering/Model/RenderDynamicMesh.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderDynamicMesh::RenderDynamicMesh(PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, u32 numVertices, u32 numIndices, const Sphere& boundingSphere,
                      UniquePtr<u8[]> vertexData, u32 vertexDataSize, UniquePtr<u8[]> indexData, u32 indexDataSize) noexcept
    : m_polygonType(polygonType), m_vertexFormat(vertexFormat), m_indexFormat(indexFormat), m_numVertices(numVertices), m_numIndices(numIndices), m_boundingSphere(boundingSphere),
      m_vertexData(std::move(vertexData)), m_vertexDataSize(vertexDataSize), m_indexData(std::move(indexData)), m_indexDataSize(indexDataSize)
    {
        CS_ASSERT(vertexDataSize <= k_maxVertexDataSize, "Vertex data too large.");
        CS_ASSERT(indexDataSize <= k_maxIndexDataSize, "Index data too large.");
    }
}
