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

#include <ChilliSource/Rendering/Model/RenderMeshManager.h>

#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Base/TargetType.h>

#include <mutex>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_meshPoolSize = 100;
    }
    
    CS_DEFINE_NAMEDTYPE(RenderMeshManager);

    //------------------------------------------------------------------------------
    RenderMeshManagerUPtr RenderMeshManager::Create() noexcept
    {
        return RenderMeshManagerUPtr(new RenderMeshManager());
    }
    
    //------------------------------------------------------------------------------
    RenderMeshManager::RenderMeshManager()
    : m_renderMeshPool(k_meshPoolSize, ObjectPoolAllocatorLimitPolicy::k_expand)
    {
        
    }

    //------------------------------------------------------------------------------
    bool RenderMeshManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderMeshManager::InterfaceID == interfaceId);
    }

    //------------------------------------------------------------------------------
    UniquePtr<RenderMesh> RenderMeshManager::CreateRenderMesh(PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, u32 numVertices, u32 numIndices, const Sphere& boundingSphere,
                                                          std::unique_ptr<const u8[]> vertexData, u32 vertexDataSize, std::unique_ptr<const u8[]> indexData, u32 indexDataSize, bool shouldBackupData,
                                                          std::vector<Matrix4> inverseBindPoseMatrices) noexcept
    {
        UniquePtr<RenderMesh> renderMesh = MakeUnique<RenderMesh>(m_renderMeshPool, polygonType, vertexFormat, indexFormat, numVertices, numIndices, boundingSphere, shouldBackupData, std::move(inverseBindPoseMatrices));

        auto rawRenderMesh = renderMesh.get();
        
        PendingLoadCommand loadCommand;
        loadCommand.m_renderMesh = rawRenderMesh;
        loadCommand.m_vertexData = std::move(vertexData);
        loadCommand.m_vertexDataSize = vertexDataSize;
        loadCommand.m_indexData = std::move(indexData);
        loadCommand.m_indexDataSize = indexDataSize;
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_pendingLoadCommands.push_back(std::move(loadCommand));
        
        return renderMesh;
    }

    //------------------------------------------------------------------------------
    void RenderMeshManager::DestroyRenderMesh(UniquePtr<RenderMesh> renderMesh) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_pendingUnloadCommands.push_back(std::move(renderMesh));
    }

    //------------------------------------------------------------------------------
    void RenderMeshManager::OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        if(targetType == TargetType::k_main)
        {
            auto preRenderCommandList = renderSnapshot.GetPreRenderCommandList();
            auto postRenderCommandList = renderSnapshot.GetPostRenderCommandList();
            
            std::unique_lock<std::mutex> lock(m_mutex);
            
            for (auto& loadCommand : m_pendingLoadCommands)
            {
                preRenderCommandList->AddLoadMeshCommand(loadCommand.m_renderMesh, std::move(loadCommand.m_vertexData), loadCommand.m_vertexDataSize, std::move(loadCommand.m_indexData), loadCommand.m_indexDataSize);
            }
            m_pendingLoadCommands.clear();
            
            for (auto& unloadCommand : m_pendingUnloadCommands)
            {
                postRenderCommandList->AddUnloadMeshCommand(std::move(unloadCommand));
            }
            m_pendingUnloadCommands.clear();
        }
    }
}
