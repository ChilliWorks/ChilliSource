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

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Model/GLMeshUtils.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>

#include <ChilliSource/Core/Base/ByteColour.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            constexpr u32 k_allocatorSize = 1024 * 1024;
            
            /// TODO
            struct SpriteVertex final
            {
                ChilliSource::Vector4 m_position;
                ChilliSource::Vector2 m_uvs;
                ChilliSource::ByteColour m_colour;
            };
            
            /// TODO
            ///
            void ApplySpriteVertices(ChilliSource::LinearAllocator& allocator, u32 numVertices, u32 vertexDataSize, const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes) noexcept
            {
                auto combinedVertices = ChilliSource::MakeUniqueArray<SpriteVertex>(allocator, numVertices);
                
                u32 vertexOffset = 0;
                for (const auto& mesh : meshes)
                {
                    auto meshVertices = reinterpret_cast<const SpriteVertex*>(mesh.GetVertexData());
                    
                    for (u32 i = 0; i < mesh.GetNumVertices(); ++i)
                    {
                        combinedVertices[vertexOffset + i] = meshVertices[i];
                        combinedVertices[vertexOffset + i].m_position *= mesh.GetWorldMatrix();
                    }
                    
                    vertexOffset += mesh.GetNumVertices();
                }
                
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDataSize, reinterpret_cast<const u8*>(combinedVertices.get()));
                
                combinedVertices.reset();
                allocator.Reset();
            }
            
            /// TODO
            ///
            void ApplyVertices(ChilliSource::LinearAllocator& allocator, const ChilliSource::VertexFormat& vertexFormat, u32 numVertices, u32 vertexDataSize,
                               const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes) noexcept
            {
                if (vertexFormat == ChilliSource::VertexFormat::k_sprite)
                {
                    ApplySpriteVertices(allocator, numVertices, vertexDataSize, meshes);
                }
                else if (vertexFormat == ChilliSource::VertexFormat::k_staticMesh)
                {
                    //TODO: handle mesh vertices
                }
                else
                {
                    CS_LOG_FATAL("Unsupported vertex format.");
                }
            }
            
            /// TODO
            ///
            void ApplyIndices(ChilliSource::LinearAllocator& allocator, ChilliSource::IndexFormat indexFormat, u32 numIndices, u32 indexDataSize,
                              const std::vector<ChilliSource::RenderMeshBatch::Mesh>& meshes) noexcept
            {
                CS_ASSERT(indexFormat == ChilliSource::IndexFormat::k_short, "Only short indices are supported at the moment.");
                
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
                
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexDataSize, reinterpret_cast<const u8*>(combinedIndices.get()));
                
                combinedIndices.reset();
                allocator.Reset();
            }
        }
        
        //------------------------------------------------------------------------------
        GLDynamicMesh::GLDynamicMesh(u32 vertexDataSize, u32 indexDataSize) noexcept
           : m_allocator(k_allocatorSize), m_maxVertexDataSize(vertexDataSize), m_maxIndexDataSize(indexDataSize)
        {
            glGenBuffers(1, &m_vertexBufferHandle);
            CS_ASSERT(m_vertexBufferHandle != 0, "Invalid vertex buffer.");
            
            if(indexDataSize > 0)
            {
                glGenBuffers(1, &m_indexBufferHandle);
                CS_ASSERT(m_indexBufferHandle != 0, "Invalid index buffer.");
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
            glBufferData(GL_ARRAY_BUFFER, m_maxVertexDataSize, nullptr, GL_DYNAMIC_DRAW);
            
            if(indexDataSize > 0)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxIndexDataSize, nullptr, GL_DYNAMIC_DRAW);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating GLDynamicMesh.");
        }
        
        //------------------------------------------------------------------------------
        void GLDynamicMesh::Bind(GLShader* glShader, ChilliSource::PolygonType polygonType, const ChilliSource::VertexFormat& vertexFormat, ChilliSource::IndexFormat indexFormat, u32 numVertices, u32 numIndices,
                                 const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept
        {
            m_polygonType = polygonType;
            m_vertexFormat = vertexFormat;
            m_indexFormat = indexFormat;
            m_numVertices = numVertices;
            m_numIndices = m_numIndices;
            
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDataSize, vertexData);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
            if (m_indexBufferHandle != 0)
            {
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexDataSize, indexData);
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
            
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
            ApplyVertices(m_allocator, m_vertexFormat, numVertices, vertexDataSize, meshes);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
            if (m_indexBufferHandle != 0)
            {
                ApplyIndices(m_allocator, indexFormat, numIndices, indexDataSize, meshes);
            }
            
            ApplyVertexAttributes(glShader);
        }
        
        //------------------------------------------------------------------------------
        void GLDynamicMesh::ApplyVertexAttributes(GLShader* glShader) const noexcept
        {
            //TODO: This should be pre-calculated.
            GLint maxVertexAttributes = 0;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
            CS_ASSERT(u32(maxVertexAttributes) >= m_vertexFormat.GetNumElements(), "Too many vertex elements.");
            
            for (u32 i = 0; i < m_vertexFormat.GetNumElements(); ++i)
            {
                glEnableVertexAttribArray(i);
                
                auto elementType = m_vertexFormat.GetElement(i);
                auto name = GLMeshUtils::GetAttributeName(elementType);
                auto numComponents = ChilliSource::VertexFormat::GetNumComponents(elementType);
                auto type = GLMeshUtils::GetGLType(ChilliSource::VertexFormat::GetDataType(elementType));
                auto normalised = GLMeshUtils::IsNormalised(elementType);
                auto offset = reinterpret_cast<const GLvoid*>(u64(m_vertexFormat.GetElementOffset(i)));
                
                glShader->SetAttribute(name, numComponents, type, normalised, m_vertexFormat.GetSize(), offset);
            }
            
            for (s32 i = m_vertexFormat.GetNumElements(); i < maxVertexAttributes; ++i)
            {
                glDisableVertexAttribArray(i);
            }
        }
        
        //------------------------------------------------------------------------------
        GLDynamicMesh::~GLDynamicMesh() noexcept
        {
            if(!m_invalidData)
            {
                glDeleteBuffers(1, &m_vertexBufferHandle);
                if(m_indexBufferHandle != 0)
                {
                    glDeleteBuffers(1, &m_indexBufferHandle);
                }
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while deleting GLDynamicMesh.");
            }
        }
    }
}
