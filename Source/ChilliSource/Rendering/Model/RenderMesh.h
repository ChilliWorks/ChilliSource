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

#ifndef _CHILLISOURCE_RENDERING_MODEL_RENDERMESH_H_
#define _CHILLISOURCE_RENDERING_MODEL_RENDERMESH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

namespace ChilliSource
{
    /// A standard-layout container for information which the renderer needs pertaining to a
    /// single mesh. This doesn't store render data, but acts as a handle which the underlying
    /// render system can use to reference it.
    ///
    /// This is immutable and therefore thread-safe, aside from the extra data pointer
    /// which should only be accessed on the render thread.
    ///
    class RenderMesh final
    {
    public:
        CS_DECLARE_NOCOPY(RenderMesh);
        
        /// Creates a new instance with the given mesh description data.
        ///
        /// SHOULD ONLY BE CREATED VIA RenderMeshManager
        ///
        /// @param polygonType
        ///     The type of polygon the mesh uses.
        /// @param vertexFormat
        ///     The vertex format.
        /// @param indexFormat
        ///     The type of index.
        /// @param numVertices
        ///     The number of vertices in the mesh. The maximum number is determined by the type of index.
        /// @param numIndices
        ///     The number of indices in the mesh.
        /// @param boundingSphere
        ///     A local space sphere enclosing the mesh.
        /// @param shouldBackupData
        ///     If the mesh data should be backed up in main memory for restoring it later.
        /// @param inverseBindPoseMatrices
        ///     (Optional) The inverse bind pose matices for this mesh. Only applies to animated models.
        ///     Should be moved.
        ///
        RenderMesh(PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, u32 numVertices, u32 numIndices, const Sphere& boundingSphere,
                   bool shouldBackupData, std::vector<Matrix4> inverseBindPoseMatrices = std::vector<Matrix4>()) noexcept;
        
        /// @return The type of polygon the mesh uses.
        ///
        PolygonType GetPolygonType() const noexcept { return m_polygonType; }
        
        /// @return The vertex format.
        ///
        const VertexFormat& GetVertexFormat() const noexcept { return m_vertexFormat; }
        
        /// @return The type of index.
        ///
        IndexFormat GetIndexFormat() const noexcept { return m_indexFormat; }
        
        /// @return The number of vertices in the mesh. The maximum number is determined by the type of index.
        ///
        u32 GetNumVertices() const noexcept { return m_numVertices; }
        
        /// @return The number of indices in the mesh.
        ///
        u32 GetNumIndices() const noexcept { return m_numIndices; }
        
        /// @return A local space sphere enclosing the mesh.
        ///
        const Sphere& GetBoundingSphere() const noexcept { return m_boundingSphere; }

        /// @return If the mesh should backup its data.
        ///
        bool ShouldBackupData() const noexcept { return m_shouldBackupData; }
        
        /// @return The inverse bind pose matrices for this mesh. Only applies to animated meshes.
        ///
        const std::vector<Matrix4>& GetInverseBindPoseMatrices() const noexcept { return m_inverseBindPoseMatrices; }

        /// This is not thread safe and should only be called from the render thread.
        ///
        /// @return A pointer to render system specific additional information.
        ///
        void* GetExtraData() const noexcept { return m_extraData; }
        
        /// This is not thread safe and should only be called from the render thread.
        ///
        /// @param extraData
        ///     A pointer to render system specific additional information.
        ///
        void SetExtraData(void* extraData) noexcept { m_extraData = extraData; }
        
    private:
        
        PolygonType m_polygonType;
        VertexFormat m_vertexFormat;
        IndexFormat m_indexFormat;
        u32 m_numVertices;
        u32 m_numIndices;
        Sphere m_boundingSphere;
        bool m_shouldBackupData;
        std::vector<Matrix4> m_inverseBindPoseMatrices;
        
        void* m_extraData = nullptr;
    };
}

#endif
