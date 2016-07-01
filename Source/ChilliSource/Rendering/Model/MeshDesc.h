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

#ifndef _CHILLISOURCE_RENDERING_MODEL_MESHDESC_H_
#define _CHILLISOURCE_RENDERING_MODEL_MESHDESC_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Model/IndexFormat.h>
#include <ChilliSource/Rendering/Model/PolygonType.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

namespace ChilliSource
{
    /// A description of a single mesh within a model. This can only be used to create
    /// a single mesh as the mesh data is moved.
    ///
    /// This is not thread safe and should only be accessed by one thread at a time.
    ///
    class MeshDesc final
    {
    public:
        CS_DECLARE_NOCOPY(MeshDesc);
        
        MeshDesc(MeshDesc&&) = default;
        MeshDesc& operator=(MeshDesc&&) = default;
        
        /// Creates a new mesh description with the given format and mesh data.
        ///
        /// @param name
        ///     The name of the mesh.
        /// @param polygonType
        ///     The type of polygon that this mesh uses.
        /// @param vertexFormat
        ///     The format of a single vertex.
        /// @param indexFormat
        ///     The format of a single index.
        /// @param aabb
        ///     The local AABB of the mesh.
        /// @parma boundingSphere
        ///     The local bounding sphere of the mesh.
        /// @param numVertices
        ///     The number of vertices in the mesh.
        /// @param numIndices
        ///     The number of indices in the mesh.
        /// @param vertexData
        ///     The vertex data for the mesh.
        /// @param indexData
        ///     The index data for the mesh.
        ///
        MeshDesc(const std::string& name, PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, const AABB& aabb, const Sphere& boundingSphere, u32 numVertices, u32 numIndices,
                 std::unique_ptr<const u8[]> vertexData, std::unique_ptr<const u8[]> indexData) noexcept;
        
        /// Creates a new mesh description with the given format, mesh data and inverse bind pose
        /// matrices.
        ///
        /// @param name
        ///     The name of the mesh.
        /// @param polygonType
        ///     The type of polygon that this mesh uses.
        /// @param vertexFormat
        ///     The format of a single vertex.
        /// @param indexFormat
        ///     The format of a single index.
        /// @param aabb
        ///     The local AABB of the mesh.
        /// @param numVertices
        ///     The number of vertices in the mesh.
        /// @param numIndices
        ///     The number of indices in the mesh.
        /// @param vertexData
        ///     The vertex data for the mesh.
        /// @param indexData
        ///     The index data for the mesh.
        /// @param inverseBindPoseMatrices
        ///     The inverse bind pose matices.
        ///
        MeshDesc(const std::string& name, PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, const AABB& aabb, const Sphere& boundingSphere, u32 numVertices, u32 numIndices,
                 std::unique_ptr<const u8[]> vertexData, std::unique_ptr<const u8[]> indexData, std::vector<Matrix4> inverseBindPoseMatrices) noexcept;
        
        /// @return The name of the mesh
        ///
        const std::string& GetName() const noexcept { return m_name; }
        
        /// @return The type of polygon that this mesh uses.
        ///
        PolygonType GetPolygonType() const noexcept { return m_polygonType; }
        
        /// @return The format of single vertex.
        ///
        const VertexFormat& GetVertexFormat() const noexcept { return m_vertexFormat; }
        
        /// @return The format of a single index.
        ///
        IndexFormat GetIndexFormat() const noexcept { return m_indexFormat; }
        
        /// @return The local AABB of the mesh.
        ///
        const AABB& GetAABB() const noexcept { return m_aabb; }
        
        /// @return The local bounding sphere of the mesh.
        ///
        const Sphere& GetBoundingSphere() const noexcept { return m_boundingSphere; }
        
        /// @return The number of vertices in the mesh.
        ///
        u32 GetNumVertices() const noexcept { return m_numVertices; }
        
        /// @return The number of indices in the mesh.
        ///
        u32 GetNumIndices() const noexcept { return m_numIndices; }
        
        /// Moves the vertex data from the description to a new owner. This must not
        /// be called twice, otherwise it will assert.
        ///
        /// @return The vertex data for the mesh.
        ///
        std::unique_ptr<const u8[]> ClaimVertexData() noexcept;
        
        /// Moves the index data from the description to a new owner. This must not
        /// be called twice, otherwise it will assert.
        ///
        /// @return The index data for the mesh.
        ///
        std::unique_ptr<const u8[]> ClaimIndexData() noexcept;
        
        /// Moves the inverse bind pose matrices from the description to a new owner.
        ///
        /// @return The index data for the mesh.
        ///
        std::vector<Matrix4> ClaimInverseBindPoseMatrices() noexcept;
        
    private:
        std::string m_name;
        PolygonType m_polygonType;
        VertexFormat m_vertexFormat;
        IndexFormat m_indexFormat;
        AABB m_aabb;
        Sphere m_boundingSphere;
        u32 m_numVertices;
        u32 m_numIndices;
        std::unique_ptr<const u8[]> m_vertexData;
        std::unique_ptr<const u8[]> m_indexData;
        std::vector<Matrix4> m_inverseBindPoseMatrixes;
    };
}

#endif
