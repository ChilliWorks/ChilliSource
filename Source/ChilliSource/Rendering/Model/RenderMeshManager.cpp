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

#include <mutex>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(RenderMeshManager);

    //------------------------------------------------------------------------------
    RenderMeshManagerUPtr RenderMeshManager::Create() noexcept
    {
        return RenderMeshManagerUPtr(new RenderMeshManager());
    }

    //------------------------------------------------------------------------------
    bool RenderMeshManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderMeshManager::InterfaceID == interfaceId);
    }

    //------------------------------------------------------------------------------
    const RenderMesh* RenderMeshManager::CreateRenderMesh(PolygonType polygonType, const VertexFormat& vertexFormat, IndexFormat indexFormat, u32 numVertices, u32 numIndices, const Sphere& boundingSphere,
                                                          std::unique_ptr<const u8[]> vertexData, u32 vertexDataSize, std::unique_ptr<const u8[]> indexData, u32 indexDataSize) noexcept
    {
        RenderMeshUPtr renderMesh(new RenderMesh(polygonType, vertexFormat, indexFormat, numVertices, numIndices, boundingSphere));
        auto rawRenderMesh = renderMesh.get();
        
        PendingLoadCommand loadCommand;
        loadCommand.m_renderMesh = rawRenderMesh;
        loadCommand.m_vertexData = std::move(vertexData);
        loadCommand.m_vertexDataSize = vertexDataSize;
        loadCommand.m_indexData = std::move(indexData);
        loadCommand.m_indexDataSize = indexDataSize;
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_renderMeshes.push_back(std::move(renderMesh));
        m_pendingLoadCommands.push_back(std::move(loadCommand));
        
        return rawRenderMesh;
    }

    //------------------------------------------------------------------------------
    void RenderMeshManager::DestroyRenderMesh(const RenderMesh* renderMesh) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        for (auto it = m_renderMeshes.begin(); it != m_renderMeshes.end(); ++it)
        {
            if (it->get() == renderMesh)
            {
                m_pendingUnloadCommands.push_back(std::move(*it));
                
                it->swap(m_renderMeshes.back());
                m_renderMeshes.pop_back();
                return;
            }
        }
        
        CS_LOG_FATAL("RenderMesh does not exist.");
    }

    //------------------------------------------------------------------------------
    void RenderMeshManager::OnRenderSnapshot(RenderSnapshot& renderSnapshot) noexcept
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
    
    //------------------------------------------------------------------------------
    RenderMeshManager::~RenderMeshManager() noexcept
    {
        CS_ASSERT(m_renderMeshes.size() == 0, "Render meshes have not been correctly released.");
    }
}
