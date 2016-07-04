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

#include <ChilliSource/Rendering/Model/MeshDesc.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    MeshDesc::MeshDesc(const std::string& name, PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, const AABB& aabb, const Sphere& boundingSphere,
                       u32 numVertices, u32 numIndices, std::unique_ptr<const u8[]> vertexData, std::unique_ptr<const u8[]> indexData) noexcept
        : m_name(name), m_polygonType(polygonType), m_vertexFormat(vertexFormat), m_indexFormat(indexFormat), m_aabb(aabb), m_boundingSphere(boundingSphere), m_numVertices(numVertices),
          m_numIndices(numIndices), m_vertexData(std::move(vertexData)), m_indexData(std::move(indexData))
    {
        CS_ASSERT(m_vertexData, "Mesh must not have vertex data.");
        CS_ASSERT(m_indexData, "Mesh must not have vertex data.");
    }
    
    //------------------------------------------------------------------------------
    MeshDesc::MeshDesc(const std::string& name, PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, const AABB& aabb, const Sphere& boundingSphere,
                       u32 numVertices, u32 numIndices, std::unique_ptr<const u8[]> vertexData, std::unique_ptr<const u8[]> indexData, std::vector<Matrix4> inverseBindPoseMatrices) noexcept
        : m_name(name), m_polygonType(polygonType), m_vertexFormat(vertexFormat), m_indexFormat(indexFormat), m_aabb(aabb), m_boundingSphere(boundingSphere), m_numVertices(numVertices),
          m_numIndices(numIndices), m_vertexData(std::move(vertexData)), m_indexData(std::move(indexData)), m_inverseBindPoseMatrixes(std::move(inverseBindPoseMatrices))
    {
        CS_ASSERT(m_vertexData, "Mesh must not have vertex data.");
        CS_ASSERT(m_indexData, "Mesh must not have vertex data.");
    }
    
    //------------------------------------------------------------------------------
    std::unique_ptr<const u8[]> MeshDesc::ClaimVertexData() noexcept
    {
        CS_ASSERT(m_vertexData, "Vertex data has already been claimed.");
        
        return std::move(m_vertexData);
    }
    
    //------------------------------------------------------------------------------
    std::unique_ptr<const u8[]> MeshDesc::ClaimIndexData() noexcept
    {
        CS_ASSERT(m_indexData, "Index data has already been claimed.");
        
        return std::move(m_indexData);
    }
    
    //------------------------------------------------------------------------------
    std::vector<Matrix4> MeshDesc::ClaimInverseBindPoseMatrices() noexcept
    {
        return std::move(m_inverseBindPoseMatrixes);
    }
}
