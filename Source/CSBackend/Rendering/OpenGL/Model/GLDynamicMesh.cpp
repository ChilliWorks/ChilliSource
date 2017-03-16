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

#include <CSBackend/Rendering/OpenGL/Model/GLDynamicMesh.h>

#include <CSBackend/Rendering/OpenGL/Base/GLExtensions.h>
#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Model/GLMeshUtils.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>

#include <ChilliSource/Core/Base/ByteColour.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Rendering/Model/RenderDynamicMesh.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            constexpr u32 k_allocatorSize = ChilliSource::RenderDynamicMesh::k_maxVertexDataSize;
            
            /// Applies each of the sprite meshes' vertices to the combined mesh batch vertex buffer. The vertices
            /// are converted into world space before being added to the new buffer. The vertex data is applied to
            /// the currently bound vertex buffer.
            ///
            /// This version makes use of the map buffer extension which means the data can be copied straight to GL without an additional copy being made
            ///
            /// @param numVertices
            ///     The total number of vertices in the batch.
            /// @param vertexDataSize
            ///     The total vertex data size of the batch.
            /// @param meshes
            ///     The meshes which should be added to the batch.
            ///
            void ApplySpriteBatchVerticesMapBuffer(u32 numVertices, u32 vertexDataSize, const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes) noexcept
            {
                CS_ASSERT(numVertices * ChilliSource::VertexFormat::k_sprite.GetSize() == vertexDataSize, "Vertex data size and number of vertices is out of sync.");
                CS_ASSERT(!meshes.empty(), "Batch must contain at least one mesh.");
                
                glBufferData(GL_ARRAY_BUFFER, vertexDataSize, nullptr, GL_DYNAMIC_DRAW);
                auto combinedVertices = static_cast<ChilliSource::SpriteVertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

                u32 vertexOffset = 0;
                for (const auto& mesh : meshes)
                {
                    auto meshVertices = reinterpret_cast<const ChilliSource::SpriteVertex*>(mesh.GetVertexData());
                    
                    for (u32 i = 0; i < mesh.GetNumVertices(); ++i)
                    {
                        combinedVertices[vertexOffset + i] = meshVertices[i];
                        combinedVertices[vertexOffset + i].m_position *= mesh.GetWorldMatrix();
                    }
                    
                    vertexOffset += mesh.GetNumVertices();
                }
                
                glUnmapBuffer(GL_ARRAY_BUFFER);
            }
            
            /// Applies each of the sprite meshes' vertices to the combined mesh batch vertex buffer. The vertices
            /// are converted into world space before being added to the new buffer. The vertex data is applied to
            /// the currently bound vertex buffer.
            ///
            /// This version is used when map buffer is unavailable and will copy the data to a temp buffer that is then
            /// uploaded to GL
            ///
            /// @param allocator
            ///     The allocator which should be used to generate the temporary buffer. Note that the allocator
            ///     will be reset, meaning it should have no other active allocations.
            /// @param numVertices
            ///     The total number of vertices in the batch.
            /// @param vertexDataSize
            ///     The total vertex data size of the batch.
            /// @param meshes
            ///     The meshes which should be added to the batch.
            ///
            void ApplySpriteBatchVertices(ChilliSource::LinearAllocator& allocator, u32 numVertices, u32 vertexDataSize, const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes) noexcept
            {
                CS_ASSERT(numVertices * ChilliSource::VertexFormat::k_sprite.GetSize() == vertexDataSize, "Vertex data size and number of vertices is out of sync.");
                CS_ASSERT(!meshes.empty(), "Batch must contain at least one mesh.");
                
                auto combinedVertices = ChilliSource::MakeUniqueArray<ChilliSource::SpriteVertex>(allocator, numVertices);
                
                u32 vertexOffset = 0;
                for (const auto& mesh : meshes)
                {
                    auto meshVertices = reinterpret_cast<const ChilliSource::SpriteVertex*>(mesh.GetVertexData());
                    
                    for (u32 i = 0; i < mesh.GetNumVertices(); ++i)
                    {
                        combinedVertices[vertexOffset + i] = meshVertices[i];
                        combinedVertices[vertexOffset + i].m_position *= mesh.GetWorldMatrix();
                    }
                    
                    vertexOffset += mesh.GetNumVertices();
                }

                glBufferData(GL_ARRAY_BUFFER, vertexDataSize, reinterpret_cast<const u8*>(combinedVertices.get()), GL_DYNAMIC_DRAW);

                combinedVertices.reset();
                allocator.Reset();
            }
            
            /// Applies each of the meshes' vertices to the combined mesh batch vertex buffer. The vertices are
            /// converted into world space before being added to the new buffer. The vertex data is applied to
            /// the currently bound vertex buffer.
            ///
            /// @param allocator
            ///     The allocator which should be used to generate the temporary buffer. Note that the allocator
            ///     will be reset, meaning it should have no other active allocations.
            /// @param vertexFormat
            ///     The vertex format of the batch.
            /// @param numVertices
            ///     The total number of vertices in the batch.
            /// @param vertexDataSize
            ///     The total vertex data size of the batch.
            /// @param meshes
            ///     The meshes which should be added to the batch.
            /// @param useMapBuffer
            ///     If the hardware supports it we can use map buffer to eliminate a copy
            ///
            void ApplyBatchVertices(ChilliSource::LinearAllocator& allocator, const ChilliSource::VertexFormat& vertexFormat, u32 numVertices, u32 vertexDataSize,
                               const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes, bool useMapBuffer) noexcept
            {
                //TODO: Add support for static mesh vertex formats
                if (vertexFormat == ChilliSource::VertexFormat::k_sprite)
                {
                    useMapBuffer ? ApplySpriteBatchVerticesMapBuffer(numVertices, vertexDataSize, meshes) : ApplySpriteBatchVertices(allocator, numVertices, vertexDataSize, meshes);
                }
                else
                {
                    CS_LOG_FATAL("Unsupported vertex format.");
                }
            }
            
            /// Applies each of the meshes' indices to the combined mesh batch index buffer. The indices are
            /// updated to reflect the new position of the vertices in the combined vertex buffer. The index
            /// data is applied to the currently bound index buffer.
            ///
            /// This version makes use of the map buffer extension which means the data can be copied straight to GL without an additional copy being made
            ///
            /// @param indexFormat
            ///     The format of the indices. Currently only short indices are supported.
            /// @param numIndices
            ///     The total number of indices in the batch.
            /// @param indexDataSize
            ///     The total index data size of the batch.
            /// @param meshes
            ///     The list of meshes which should be batched.
            ///
            void ApplyBatchIndicesMapBuffer(ChilliSource::IndexFormat indexFormat, u32 numIndices, u32 indexDataSize,
                              const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes) noexcept
            {
                CS_ASSERT(indexFormat == ChilliSource::IndexFormat::k_short, "Only short indices are supported at the moment.");
                CS_ASSERT(numIndices * ChilliSource::GetIndexSize(indexFormat) == indexDataSize, "Index data size and number of indices is out of sync.");
                CS_ASSERT(!meshes.empty(), "Batch must contain at least one mesh.");
                
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, nullptr, GL_DYNAMIC_DRAW);
                auto combinedIndices = static_cast<u16*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
                
                u32 vertexOffset = 0;
                u32 indexOffset = 0;
                for (const auto& mesh : meshes)
                {
                    auto meshIndices = reinterpret_cast<const u16*>(mesh.GetIndexData());
                    
                    for (u32 i = 0; i < mesh.GetNumIndices(); ++i)
                    {
                        combinedIndices[indexOffset + i] = vertexOffset + meshIndices[i];
                    }
                    
                    vertexOffset += mesh.GetNumVertices();
                    indexOffset += mesh.GetNumIndices();
                }
                
                glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
            }
            
            /// Applies each of the meshes' indices to the combined mesh batch index buffer. The indices are
            /// updated to reflect the new position of the vertices in the combined vertex buffer. The index
            /// data is applied to the currently bound index buffer.
            ///
            /// Used when map buffer is unavailable. The temporary buffer is allocated from the given linear allocator.
            ///
            /// @param allocator
            ///     The allocator which should be used to generate the temporary buffer. Note that the allocator
            ///     will be reset, meaning it should have no other active allocations.
            /// @param indexFormat
            ///     The format of the indices. Currently only short indices are supported.
            /// @param numIndices
            ///     The total number of indices in the batch.
            /// @param indexDataSize
            ///     The total index data size of the batch.
            /// @param meshes
            ///     The list of meshes which should be batched.
            ///
            void ApplyBatchIndices(ChilliSource::LinearAllocator& allocator, ChilliSource::IndexFormat indexFormat, u32 numIndices, u32 indexDataSize,
                                   const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes) noexcept
            {
                CS_ASSERT(indexFormat == ChilliSource::IndexFormat::k_short, "Only short indices are supported at the moment.");
                CS_ASSERT(numIndices * ChilliSource::GetIndexSize(indexFormat) == indexDataSize, "Index data size and number of indices is out of sync.");
                CS_ASSERT(!meshes.empty(), "Batch must contain at least one mesh.");
                
                auto combinedIndices = ChilliSource::MakeUniqueArray<u16>(allocator, numIndices);
                
                u32 vertexOffset = 0;
                u32 indexOffset = 0;
                for (const auto& mesh : meshes)
                {
                    auto meshIndices = reinterpret_cast<const u16*>(mesh.GetIndexData());
                    
                    for (u32 i = 0; i < mesh.GetNumIndices(); ++i)
                    {
                        combinedIndices[indexOffset + i] = vertexOffset + meshIndices[i];
                    }
                    
                    vertexOffset += mesh.GetNumVertices();
                    indexOffset += mesh.GetNumIndices();
                }
                
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, reinterpret_cast<const u8*>(combinedIndices.get()), GL_DYNAMIC_DRAW);

                combinedIndices.reset();
                allocator.Reset();
            }
        }
        
        //------------------------------------------------------------------------------
        GLDynamicMesh::GLDynamicMesh(u32 vertexDataSize, u32 indexDataSize) noexcept
           : m_allocator(k_allocatorSize), m_maxVertexDataSize(vertexDataSize), m_maxIndexDataSize(indexDataSize)
        {
            for(u32 i=0; i<k_numBuffers; ++i)
            {
                glGenBuffers(1, &m_vertexBufferHandles[i]);
                CS_ASSERT(m_vertexBufferHandles[i] != 0, "Invalid vertex buffer.");
                
                if(indexDataSize > 0)
                {
                    glGenBuffers(1, &m_indexBufferHandles[i]);
                    CS_ASSERT(m_indexBufferHandles[i] != 0, "Invalid index buffer.");
                }
                
                glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandles[i]);
                glBufferData(GL_ARRAY_BUFFER, m_maxVertexDataSize, nullptr, GL_DYNAMIC_DRAW);
                
                if(indexDataSize > 0)
                {
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandles[i]);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxIndexDataSize, nullptr, GL_DYNAMIC_DRAW);
                }
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating GLDynamicMesh.");
            
            auto renderCapabilities = ChilliSource::Application::Get()->GetSystem<ChilliSource::RenderCapabilities>();
            m_maxVertexAttributes = renderCapabilities->GetNumVertexAttributes();
            m_useMapBuffer = renderCapabilities->IsMapBufferSupported();
            m_areVAOsSupported = renderCapabilities->IsVAOSupported();
        }
        
        //------------------------------------------------------------------------------
        void GLDynamicMesh::Bind(GLShader* glShader, ChilliSource::PolygonType polygonType, const ChilliSource::VertexFormat& vertexFormat, ChilliSource::IndexFormat indexFormat, u32 numVertices, u32 numIndices,
                                 const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept
        {
            m_polygonType = polygonType;
            m_vertexFormat = vertexFormat;
            m_indexFormat = indexFormat;
            m_numVertices = numVertices;
            m_numIndices = numIndices;
        
            if(m_areVAOsSupported == true)
            {
                glBindVertexArray(0);
            }
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandles[m_currentBufferIndex]);
            glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_DYNAMIC_DRAW);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandles[m_currentBufferIndex]);
            if (m_indexBufferHandles[m_currentBufferIndex] != 0)
            {
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexData, GL_DYNAMIC_DRAW);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding GLDynamicMesh.");
            
            ApplyVertexAttributes(glShader);
        }
        
        //------------------------------------------------------------------------------
        void GLDynamicMesh::Bind(GLShader* glShader, ChilliSource::PolygonType polygonType, const ChilliSource::VertexFormat& vertexFormat, ChilliSource::IndexFormat indexFormat,
                                 u32 numVertices, u32 numIndices, u32 vertexDataSize, u32 indexDataSize, const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes) noexcept
        {
            m_polygonType = polygonType;
            m_vertexFormat = vertexFormat;
            m_indexFormat = indexFormat;
            m_numVertices = numVertices;
            m_numIndices = numIndices;
            
            if(m_areVAOsSupported == true)
            {
                glBindVertexArray(0);
            }
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandles[m_currentBufferIndex]);
            ApplyBatchVertices(m_allocator, m_vertexFormat, numVertices, vertexDataSize, meshes, m_useMapBuffer);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandles[m_currentBufferIndex]);
            if (m_indexBufferHandles[m_currentBufferIndex] != 0)
            {
                m_useMapBuffer ? ApplyBatchIndicesMapBuffer(indexFormat, numIndices, indexDataSize, meshes) : ApplyBatchIndices(m_allocator, indexFormat, numIndices, indexDataSize, meshes);
            }
            
            ApplyVertexAttributes(glShader);
        }
        
        //------------------------------------------------------------------------------
        void GLDynamicMesh::ApplyVertexAttributes(GLShader* glShader) const noexcept
        {
            CS_ASSERT(m_maxVertexAttributes >= m_vertexFormat.GetNumElements(), "Too many vertex elements.");
            
            for (u32 i = 0; i < ChilliSource::VertexFormat::k_maxElements; ++i)
            {
                glDisableVertexAttribArray(i);
            }
            
            for (u32 i = 0; i < m_vertexFormat.GetNumElements(); ++i)
            {
                auto elementType = m_vertexFormat.GetElement(i);
                
                GLint attribHandle = glShader->GetAttributeHandle((u32)elementType);
                if(attribHandle < 0)
                    continue;
                
                glEnableVertexAttribArray(i);
                
                auto numComponents = ChilliSource::VertexFormat::GetNumComponents(elementType);
                auto type = GLMeshUtils::GetGLType(ChilliSource::VertexFormat::GetDataType(elementType));
                auto normalised = GLMeshUtils::IsNormalised(elementType);
                auto offset = reinterpret_cast<const GLvoid*>(u64(m_vertexFormat.GetElementOffset(i)));
                
                glShader->SetAttribute((u32)elementType, numComponents, type, normalised, m_vertexFormat.GetSize(), offset);
            }
        }
        
        //------------------------------------------------------------------------------
        GLDynamicMesh::~GLDynamicMesh() noexcept
        {
            if(!m_invalidData)
            {
                for(u32 i=0; i<k_numBuffers; ++i)
                {
                    glDeleteBuffers(1, &m_vertexBufferHandles[i]);
                    if(m_indexBufferHandles[i] != 0)
                    {
                        glDeleteBuffers(1, &m_indexBufferHandles[i]);
                    }
                }
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while deleting GLDynamicMesh.");
            }
        }
    }
}
