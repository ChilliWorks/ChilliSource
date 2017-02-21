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

#ifndef _CHILLISOURCE_RENDERING_SHADER_RENDERMESHMANAGER_H_
#define _CHILLISOURCE_RENDERING_SHADER_RENDERMESHMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/ObjectPoolAllocator.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Model/RenderMesh.h>

#include <mutex>

namespace ChilliSource
{
    /// Manages the lifecycle of all RenderMesh instances.
    ///
    /// On creation of a RenderMesh a LoadMeshRenderCommand is queued for the next render
    /// snapshot phase, ensuring that related mesh data is processed before the RenderMesh
    /// is used.
    ///
    /// On deletion an UnloadMeshRenderCommand is queued and given ownership of the
    /// RenderMesh. The RenderMesh is then deleted once the command has been processed.
    ///
    /// This is thread-safe and can be called from any thread. If it is called on a background
    /// thread, care needs to be taken to ensure any created RenderMeshes are not used prior
    /// to being loaded.
    ///
    class RenderMeshManager final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(RenderMeshManager);
        
        /// Allows querying of whether or not this system implements the interface described by the
        /// given interface Id. Typically this is not called directly as the templated equivalent
        /// IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;
        
        /// Creates a new RenderMesh and queues a LoadMeshRenderCommand for the next Render Snapshot
        /// stage in the render pipeline.
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
        /// @param vertexData
        ///     The vertex data buffer.
        /// @param vertexDataSize
        ///     The size of the vertex data buffer.
        /// @param indexData
        ///     The index data buffer.
        /// @param indexDataSize
        ///     The size of the index data buffer.
        /// @param shouldBackupData
        ///     If the mesh data should be backed up in main memory for restoring it later.
        /// @param inverseBindPoseMatrices
        ///     (Optional) The inverse bind pose matices for this mesh. Only applies to animated models.
        ///     Should be moved.
        ///
        /// @return The RenderMesh instance.
        ///
        UniquePtr<RenderMesh> CreateRenderMesh(PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, u32 numVertices, u32 numIndices, const Sphere& boundingSphere,
                                           std::unique_ptr<const u8[]> vertexData, u32 vertexDataSize, std::unique_ptr<const u8[]> indexData, u32 indexDataSize, bool shouldBackupData,
                                           std::vector<Matrix4> inverseBindPoseMatrices = std::vector<Matrix4>()) noexcept;
        
        /// Removes the RenderMesh from the manager and queues an UnloadMeshRenderCommand for the
        /// next Render Snapshot stage in the render pipeline. The render command is given ownership
        /// of the RenderMesh, ensuring it won't be destroyed until it is no longer used.
        ///
        /// @param renderMesh
        ///     The RenderMesh which should be destroyed.
        ///
        void DestroyRenderMesh(UniquePtr<RenderMesh> renderMesh) noexcept;

        
    private:
        friend class Application;
        
        /// A container for information relating to pending LoadMeshRenderCommands, such as the
        /// index and vertex buffers and the related RenderMesh.
        ///
        struct PendingLoadCommand final
        {
            RenderMesh* m_renderMesh = nullptr;
            std::unique_ptr<const u8[]> m_vertexData;
            u32 m_vertexDataSize = 0;
            std::unique_ptr<const u8[]> m_indexData;
            u32 m_indexDataSize = 0;
        };
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static RenderMeshManagerUPtr Create() noexcept;
        
        RenderMeshManager();
        
        /// Called during the Render Snapshot stage of the render pipeline. All pending load and
        /// unload commands are added to the render snapshot.
        ///
        /// @param targetType
        ///     Whether the snapshot is for the main screen or an offscreen render target
        /// @param renderSnapshot
        ///     The render shapshot for storing snapshotted data.
        /// @param frameAllocator
        ///     Allocate memory for this render frame from here
        ///
        void OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        std::mutex m_mutex;
        ObjectPoolAllocator<RenderMesh> m_renderMeshPool;
        std::vector<PendingLoadCommand> m_pendingLoadCommands;
        std::vector<UniquePtr<RenderMesh>> m_pendingUnloadCommands;
    };
}

#endif
