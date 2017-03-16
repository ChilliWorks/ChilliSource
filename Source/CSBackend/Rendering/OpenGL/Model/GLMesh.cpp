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

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Model/RenderMesh.h>

#include <CSBackend/Rendering/OpenGL/Base/GLExtensions.h>
#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Model/GLMeshUtils.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>


namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            //Should maintain memory backups on android to restore data when the context
            //is lost when dealing with meshes that are not loaded from file.
            const bool k_shouldBackupMeshDataFromMemory = true;
#else
            const bool k_shouldBackupMeshDataFromMemory = false;
#endif
        }
        
        //------------------------------------------------------------------------------
        GLMesh::GLMesh(const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize, ChilliSource::RenderMesh* renderMesh) noexcept
            : m_vertexDataSize(vertexDataSize), m_indexDataSize(indexDataSize), m_renderMesh(renderMesh)
        {
            BuildMesh(vertexData, vertexDataSize, indexData, indexDataSize);
            
            if(k_shouldBackupMeshDataFromMemory && renderMesh->ShouldBackupData())
            {
                u8* vertextDataCopy = new u8[vertexDataSize];
                memcpy(vertextDataCopy, vertexData, vertexDataSize);
                m_vertexDataBackup = std::unique_ptr<const u8[]>(vertextDataCopy);
                
                u8* indexDataCopy = new u8[indexDataSize];
                memcpy(indexDataCopy, indexData, indexDataSize);
                m_indexDataBackup = std::unique_ptr<const u8[]>(indexDataCopy);
            }
            
            auto renderCapabilities = ChilliSource::Application::Get()->GetSystem<ChilliSource::RenderCapabilities>();
            m_maxVertexAttributes = renderCapabilities->GetNumVertexAttributes();
            CS_ASSERT(m_maxVertexAttributes >= m_renderMesh->GetVertexFormat().GetNumElements(), "Too many vertex elements.");
            
            m_areVAOsSupported = renderCapabilities->IsVAOSupported();
        }
        
        //------------------------------------------------------------------------------
        void GLMesh::Bind(GLShader* glShader) noexcept
        {
            if(m_areVAOsSupported == true)
            {
                //Check if we have been rendererd using this shader before and
                //if so we have a VAO setup for this shader
                GLuint vao = 0;
                for(const auto& pair : m_vaoCache)
                {
                    if(pair.first == glShader)
                    {
                        vao = pair.second;
                        break;
                    }
                }
                
                if(vao > 0)
                {
                    glBindVertexArray(vao);
                    CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding GLMesh.");
                    return;
                }
            
                //Otherwise we need to create a VAO
                GLuint vaoHandle = 0;
                glGenVertexArrays(1, &vaoHandle);
                glBindVertexArray(vaoHandle);
                m_vaoCache.push_back(std::make_pair(glShader, vaoHandle));
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating VAO for GLMesh.");
            
            for (u32 i = 0; i < ChilliSource::VertexFormat::k_maxElements; ++i)
            {
                glDisableVertexAttribArray(i);
            }
            
            auto vertexFormat = m_renderMesh->GetVertexFormat();
            for (u32 i = 0; i < vertexFormat.GetNumElements(); ++i)
            {
                auto elementType = vertexFormat.GetElement(i);
                
                GLint attribHandle = glShader->GetAttributeHandle((u32)elementType);
                if(attribHandle < 0)
                    continue;
                
                glEnableVertexAttribArray(attribHandle);
                
                auto numComponents = ChilliSource::VertexFormat::GetNumComponents(elementType);
                auto type = GLMeshUtils::GetGLType(ChilliSource::VertexFormat::GetDataType(elementType));
                auto normalised = GLMeshUtils::IsNormalised(elementType);
                auto offset = reinterpret_cast<const GLvoid*>(u64(vertexFormat.GetElementOffset(i)));
                
                glShader->SetAttribute((u32)elementType, numComponents, type, normalised, vertexFormat.GetSize(), offset);
            }
        }
        
        //------------------------------------------------------------------------------
        void GLMesh::Restore() noexcept
        {
            if(m_invalidData == true)
            {
                m_vaoCache.clear();
                
                if(m_vertexDataBackup)
                {
                    BuildMesh(m_vertexDataBackup.get(), m_vertexDataSize, m_indexDataBackup.get(), m_indexDataSize);
                }
                
                m_invalidData = false;
            }
        }
        
        //------------------------------------------------------------------------------
        void GLMesh::BuildMesh(const u8* vertexData, u32 vertexDataSize, const u8* indexData, u32 indexDataSize) noexcept
        {
            CS_ASSERT(vertexDataSize > 0 && vertexData, "Cannot build mesh with empty data");
            
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
        }
        
        //------------------------------------------------------------------------------
        GLMesh::~GLMesh() noexcept
        {
            if(!m_invalidData)
            {
                for(auto& vao : m_vaoCache)
                {
                    glDeleteVertexArrays(1, &vao.second);
                }
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
