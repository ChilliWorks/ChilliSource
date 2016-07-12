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

#include <ChilliSource/Rendering/Model/SmallMeshBatcher.h>

#include <ChilliSource/Rendering/Base/RenderFrameData.h>
#include <ChilliSource/Rendering/Base/RenderPassObject.h>
#include <ChilliSource/Rendering/Model/RenderDynamicMesh.h>
#include <ChilliSource/Rendering/Model/RenderMeshBatch.h>
#include <ChilliSource/Rendering/Model/RenderMesh.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandList.h>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_batchVertexCountThreshold = 100;
    }
    
    //------------------------------------------------------------------------------
    bool SmallMeshBatcher::CanBatch(const RenderPassObject& renderPassObject) noexcept
    {
//        if (renderPassObject.GetType() == RenderPassObject::Type::k_static)
//        {
//            auto renderMesh = renderPassObject.GetRenderMesh();
//            CS_ASSERT(renderMesh, "Must have render mesh.");
//            
//            if (renderMesh->GetVertexFormat() == VertexFormat::k_sprite || renderMesh->GetVertexFormat() == VertexFormat::k_staticMesh)
//            {
//                if (renderMesh->GetNumVertices() <= k_batchVertexCountThreshold)
//                {
//                    return true;
//                }
//            }
//        }
        
        if (renderPassObject.GetType() == RenderPassObject::Type::k_dynamic)
        {
            auto renderDynamicMesh = renderPassObject.GetRenderDynamicMesh();
            CS_ASSERT(renderDynamicMesh, "Must have render dynamic mesh.");
            
            if (renderDynamicMesh->GetVertexFormat() == VertexFormat::k_sprite || renderDynamicMesh->GetVertexFormat() == VertexFormat::k_staticMesh)
            {
                if (renderDynamicMesh->GetNumVertices() <= k_batchVertexCountThreshold)
                {
                    return true;
                }
            }
        }
        
        return false;
    }

    //------------------------------------------------------------------------------
    SmallMeshBatcher::SmallMeshBatcher(RenderCommandList* renderCommandList) noexcept
        : m_renderCommandList(renderCommandList)
    {
    }

    //------------------------------------------------------------------------------
    void SmallMeshBatcher::Batch(const RenderPassObject& renderPassObject) noexcept
    {
        CS_ASSERT(CanBatch(renderPassObject), "Cannot batch the given object.");
        
        if (TryUpdateRenderState(renderPassObject))
        {
            Flush();
        }
        
        AddToBatch(renderPassObject);
    }

    //------------------------------------------------------------------------------
    void SmallMeshBatcher::Flush() noexcept
    {
        if (!m_currentMeshes.empty())
        {
            auto renderMeshBatch = RenderMeshBatchUPtr(new RenderMeshBatch(m_currentPolygonType, m_currentVertexFormat, m_currentIndexFormat, std::move(m_currentMeshes)));
            
            m_renderCommandList->AddApplyMeshBatchCommand(std::move(renderMeshBatch));
            m_renderCommandList->AddRenderInstanceCommand(Matrix4::k_identity);
            
            m_currentVertexDataSize = 0;
            m_currentIndexDataSize = 0;
        }
    }
    
    //------------------------------------------------------------------------------
    bool SmallMeshBatcher::TryUpdateRenderState(const RenderPassObject& renderPassObject) noexcept
    {
        PolygonType newPolygonType;
        const VertexFormat* newVertexFormat;
        IndexFormat newIndexFormat;
        bool newHasIndices;
        
        switch (renderPassObject.GetType())
        {
//            case RenderPassObject::Type::k_static:
//            {
//                auto renderMesh = renderPassObject.GetRenderMesh();
//                CS_ASSERT(renderMesh, "Must have render mesh.");
//                
//                newPolygonType = renderMesh->GetPolygonType();
//                newVertexFormat = &renderMesh->GetVertexFormat();
//                newIndexFormat = renderMesh->GetIndexFormat();
//                newHasIndices = (renderMesh->GetNumIndices() > 0);
//                break;
//            }
            case RenderPassObject::Type::k_dynamic:
            {
                auto renderDynamicMesh = renderPassObject.GetRenderDynamicMesh();
                CS_ASSERT(renderDynamicMesh, "Must have render dynamic mesh.");
                
                newPolygonType = renderDynamicMesh->GetPolygonType();
                newVertexFormat = &renderDynamicMesh->GetVertexFormat();
                newIndexFormat = renderDynamicMesh->GetIndexFormat();
                newHasIndices = (renderDynamicMesh->GetNumIndices() > 0);
                break;
            }
            default:
                CS_LOG_FATAL("Invalid render pass object type.");
                break;
        }
        
        if (m_currentPolygonType != newPolygonType || m_currentVertexFormat != *newVertexFormat || m_currentIndexFormat != newIndexFormat || m_hasIndices != newHasIndices)
        {
            m_currentPolygonType = newPolygonType;
            m_currentVertexFormat = *newVertexFormat;
            m_currentIndexFormat = newIndexFormat;
            m_hasIndices = newHasIndices;
            
            return true;
        }
        else
        {
            return false;
        }
    }
    
    //------------------------------------------------------------------------------
    void SmallMeshBatcher::AddToBatch(const RenderPassObject& renderPassObject) noexcept
    {
        u32 numVertices = 0;
        u32 numIndices = 0;
        const u8* vertexData = nullptr;
        u32 vertexDataSize = 0;
        const u8* indexData = nullptr;
        u32 indexDataSize = 0;
        
        switch (renderPassObject.GetType())
        {
//            case RenderPassObject::Type::k_static:
//            {
//                auto renderMesh = renderPassObject.GetRenderMesh();
//                CS_ASSERT(renderMesh, "Must have render mesh.");
//                
//                //TODO: !?
//                break;
//            }
            case RenderPassObject::Type::k_dynamic:
            {
                auto renderDynamicMesh = renderPassObject.GetRenderDynamicMesh();
                CS_ASSERT(renderDynamicMesh, "Must have render dynamic mesh.");
                
                numVertices = renderDynamicMesh->GetNumVertices();
                numIndices = renderDynamicMesh->GetNumIndices();
                vertexData = renderDynamicMesh->GetVertexData();
                vertexDataSize = renderDynamicMesh->GetVertexDataSize();
                indexData = renderDynamicMesh->GetIndexData();
                indexDataSize = renderDynamicMesh->GetIndexDataSize();
                break;
            }
            default:
                CS_LOG_FATAL("Invalid render pass object type.");
                break;
        }
        
        if (m_currentVertexDataSize + vertexDataSize >= RenderDynamicMesh::k_maxVertexDataSize || m_currentIndexDataSize + indexDataSize >= RenderDynamicMesh::k_maxIndexDataSize)
        {
            Flush();
        }
        
        m_currentVertexDataSize += vertexDataSize;
        m_currentIndexDataSize += indexDataSize;
        m_currentMeshes.push_back(RenderMeshBatch::Mesh(renderPassObject.GetWorldMatrix(), numVertices, numIndices, vertexData, vertexDataSize, indexData, indexDataSize));
    }

    //------------------------------------------------------------------------------
    SmallMeshBatcher::~SmallMeshBatcher() noexcept
    {
        CS_ASSERT(m_currentMeshes.empty(), "Deleting small mesh batcher without flushing.");
    }
}