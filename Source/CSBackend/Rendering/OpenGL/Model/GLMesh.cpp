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
#include <CSBackend/Rendering/OpenGL/Model/GLMeshUtils.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>

namespace CSBackend
{
    namespace OpenGL
    {
        //------------------------------------------------------------------------------
        GLMesh::GLMesh(const ChilliSource::VertexFormat& vertexFormat, const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize, bool storeMemoryBackup) noexcept
            : m_vertexFormat(vertexFormat), m_vertextDataSize(vertexDataSize), m_indexDataSize(indexDataSize), m_hasMemoryBackup(storeMemoryBackup)
        {
            glGenBuffers(1, &m_vertexBufferHandle);
            CS_ASSERT(m_vertexBufferHandle != 0, "Invalid vertex buffer.");
            
            if(indexData)
            {
                glGenBuffers(1, &m_indexBufferHandle);
                CS_ASSERT(m_indexBufferHandle != 0, "Invalid index buffer.");
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
            glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
            
            if(indexData)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexData, GL_STATIC_DRAW);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating GLMesh.");
            
            if(storeMemoryBackup)
            {
                u8* vertextDataCopy = new u8[vertexDataSize];
                memcpy(vertextDataCopy, vertexData, vertexDataSize);
                m_vertexDataBackup = std::unique_ptr<const u8[]>(vertextDataCopy);
                
                u8* indexDataCopy = new u8[indexDataSize];
                memcpy(indexDataCopy, indexData, indexDataSize);
                m_indexDataBackup = std::unique_ptr<const u8[]>(indexDataCopy);
            }
        }
        
        //------------------------------------------------------------------------------
        void GLMesh::Bind(GLShader* glShader) noexcept
        {
            //TODO: This should be pre-calculated.
            GLint maxVertexAttributes = 0;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
            CS_ASSERT(u32(maxVertexAttributes) >= m_vertexFormat.GetNumElements(), "Too many vertex elements.");
            
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding GLMesh.");
            
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
        void GLMesh::RestoreContext() noexcept
        {
            if(m_hasMemoryBackup && m_contextInvalid)
            {
                glGenBuffers(1, &m_vertexBufferHandle);
                CS_ASSERT(m_vertexBufferHandle != 0, "Invalid vertex buffer.");
                
                if(m_indexDataBackup)
                {
                    glGenBuffers(1, &m_indexBufferHandle);
                    CS_ASSERT(m_indexBufferHandle != 0, "Invalid index buffer.");
                }
                
                glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
                glBufferData(GL_ARRAY_BUFFER, m_vertextDataSize, m_vertexDataBackup.get(), GL_STATIC_DRAW);
                
                if(m_indexDataBackup)
                {
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexDataSize, m_indexDataBackup.get(), GL_STATIC_DRAW);
                }
                
                m_contextInvalid = false;
            }
        }
        
        //------------------------------------------------------------------------------
        GLMesh::~GLMesh() noexcept
        {
            if(!m_contextInvalid)
            {
                glDeleteBuffers(1, &m_vertexBufferHandle);
                if(m_indexBufferHandle != 0)
                {
                    glDeleteBuffers(1, &m_indexBufferHandle);
                }
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while deleting GLMesh.");
            }
        }
    }
}