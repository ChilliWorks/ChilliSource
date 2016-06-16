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

#include <CSBackend/Rendering/OpenGL/Model/GLMesh.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            /// @param dataType
            ///     The vertex element data type.
            ///
            /// @return the OpenGL type for the requested data type.
            ///
            GLenum GetGLType(ChilliSource::VertexFormat::DataType dataType) noexcept
            {
                switch (dataType)
                {
                    case ChilliSource::VertexFormat::DataType::k_byte:
                        return GL_UNSIGNED_BYTE;
                    case ChilliSource::VertexFormat::DataType::k_float:
                        return GL_FLOAT;
                    default:
                        CS_LOG_FATAL("Invalid data type.");
                        return GL_UNSIGNED_BYTE;
                }
            }
            
            /// @param elementType
            ///     The vertex element type.
            ///
            /// @return Whether or not the value is normalised for the given element type.
            ///
            GLboolean IsNormalised(ChilliSource::VertexFormat::ElementType elementType) noexcept
            {
                switch (elementType)
                {
                    case ChilliSource::VertexFormat::ElementType::k_position4:
                        return GL_FALSE;
                    case ChilliSource::VertexFormat::ElementType::k_normal3:
                        return GL_FALSE;
                    case ChilliSource::VertexFormat::ElementType::k_uv2:
                        return GL_FALSE;
                    case ChilliSource::VertexFormat::ElementType::k_colour4:
                        return GL_TRUE;
                    case ChilliSource::VertexFormat::ElementType::k_weight4:
                        return GL_FALSE;
                    case ChilliSource::VertexFormat::ElementType::k_jointIndex4:
                        return GL_FALSE;
                    default:
                        CS_LOG_FATAL("Invalid element type.");
                        return GL_FALSE;
                }
            }
            
            /// @param elementType
            ///     The vertex element type.
            ///
            /// @return The attribute name for the given element type.
            ///
            const char* GetAttributeName(ChilliSource::VertexFormat::ElementType elementType) noexcept
            {
                switch (elementType)
                {
                    case ChilliSource::VertexFormat::ElementType::k_position4:
                        return "a_position";
                    case ChilliSource::VertexFormat::ElementType::k_normal3:
                        return "a_normal";
                    case ChilliSource::VertexFormat::ElementType::k_uv2:
                        return "a_texCoord";
                    case ChilliSource::VertexFormat::ElementType::k_colour4:
                        return "a_colour";
                    case ChilliSource::VertexFormat::ElementType::k_weight4:
                        return "a_weights";
                    case ChilliSource::VertexFormat::ElementType::k_jointIndex4:
                        return "a_jointIndices";
                    default:
                        CS_LOG_FATAL("Invalid element type.");
                        return "";
                }
            }
        }
        
        //------------------------------------------------------------------------------
        GLMesh::GLMesh(ChilliSource::PolygonType polygonType, const ChilliSource::VertexFormat& vertexFormat, ChilliSource::IndexFormat indexFormat,
                       const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept
            : m_vertexFormat(vertexFormat)
        {
            glGenBuffers(1, &m_vertexBufferHandle);
            CS_ASSERT(m_vertexBufferHandle != 0, "Invalid vertex buffer.");
            
            if(indexData)
            {
                glGenBuffers(1, &m_indexBufferHandle);
                CS_ASSERT(m_vertexBufferHandle != 0, "Invalid index buffer.");
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
            glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
            
            if(indexData)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexData, GL_STATIC_DRAW);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating mesh buffer.");
        }
        
        //------------------------------------------------------------------------------
        void GLMesh::Bind(GLShader* glShader) noexcept
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
            
            GLint maxVertexAttributes = 0;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
            CS_ASSERT(u32(maxVertexAttributes) >= m_vertexFormat.GetNumElements(), "Too many vertex elements.");
            
            for (u32 i = 0; i < m_vertexFormat.GetNumElements(); ++i)
            {
                glEnableVertexAttribArray(i);
                
                auto elementType = m_vertexFormat.GetElement(i);
                auto name = GetAttributeName(elementType);
                auto numComponents = ChilliSource::VertexFormat::GetNumComponents(elementType);
                auto type = GetGLType(ChilliSource::VertexFormat::GetDataType(elementType));
                auto normalised = IsNormalised(elementType);
                auto offset = reinterpret_cast<const GLvoid*>(u64(m_vertexFormat.GetElementOffset(i)));
                
                glShader->SetAttribute(name, numComponents, type, normalised, m_vertexFormat.GetSize(), offset);
            }
            
            for (s32 i = m_vertexFormat.GetNumElements(); i < maxVertexAttributes; ++i)
            {
                glDisableVertexAttribArray(i);
            }
        }
        
        //------------------------------------------------------------------------------
        GLMesh::~GLMesh() noexcept
        {
            //TODO: Handle context loss
            
            glDeleteBuffers(1, &m_vertexBufferHandle);
            if(m_indexBufferHandle != 0)
            {
                glDeleteBuffers(1, &m_indexBufferHandle);
            }
        }
    }
}