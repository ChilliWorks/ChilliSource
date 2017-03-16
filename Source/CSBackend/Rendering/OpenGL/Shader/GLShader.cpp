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

#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>

#include <array>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            /// Compiles the given GLSL shader source.
            ///
            /// @param source
            ///     The shader source string.
            /// @param type
            ///     The shader type.
            ///
            /// @return The shader handle.
            ///
            GLuint CompileShader(const std::string& source, GLenum type)
            {
                auto shaderId = glCreateShader(type);
                CS_ASSERT(shaderId > 0, "OpenGL ES 2.0 does not support this shader type");
                
                const GLchar* glSource = source.c_str();
                glShaderSource(shaderId, 1, &glSource, nullptr);
                glCompileShader(shaderId);
                
                GLint compilationStatus;
                glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationStatus);
                
                if(compilationStatus == 0)
                {
                    GLint errorLength = 0;
                    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorLength);
                    
                    if(errorLength > 1)
                    {
                        char* errorLog = (char*)malloc(sizeof(char) * errorLength);
                        glGetShaderInfoLog(shaderId, errorLength, nullptr, errorLog);
                        
                        CS_LOG_ERROR("GLSL compilation error: " + std::string(errorLog));
                        free(errorLog);
                    }
                    
                    CS_LOG_FATAL("Failed to compile GLSL shader");
                }
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while compiling shader.");
                
                return shaderId;
            }
            
            /// Combines the given vertex and fragment shader to create a shader program.
            ///
            /// @param vsHandle
            ///     The vertex shader handle
            /// @param fsHandle
            ///     The fragment shader handle.
            ///
            GLuint CreateProgram(GLuint vsHandle, GLuint fsHandle)
            {
                auto programId = glCreateProgram();
                CS_ASSERT(programId > 0, "Failed to create GLSL program");
                
                glAttachShader(programId, vsHandle);
                glAttachShader(programId, fsHandle);
                glLinkProgram(programId);
                
                //Check for success
                GLint linkStatus;
                glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
                
                if(linkStatus == 0)
                {
                    GLint errorLength = 0;
                    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &errorLength);
                    
                    if(errorLength > 1)
                    {
                        char* errorLog = (char*)malloc(sizeof(char) * errorLength);
                        glGetProgramInfoLog(programId, errorLength, nullptr, errorLog);
                        CS_LOG_ERROR("GLSL link error: " + std::string(errorLog));
                        free(errorLog);
                    }
                    
                    CS_LOG_FATAL("Failed to link GLSL shader");
                }
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating shader program.");
                
                return programId;
            }
        }
        
        const std::string GLShader::k_attributePosition = "a_position";
        const std::string GLShader::k_attributeNormal = "a_normal";
        const std::string GLShader::k_attributeTangent = "a_tangent";
        const std::string GLShader::k_attributeBitangent = "a_bitangent";
        const std::string GLShader::k_attributeTexCoord = "a_texCoord";
        const std::string GLShader::k_attributeColour = "a_colour";
        const std::string GLShader::k_attributeWeights = "a_weights";
        const std::string GLShader::k_attributeJointIndices = "a_jointIndices";
    
        //------------------------------------------------------------------------------
        GLShader::GLShader(const std::string& vertexShader, const std::string& fragmentShader) noexcept
        {
            m_vertexShaderId = CompileShader(vertexShader, GL_VERTEX_SHADER);
            m_fragmentShaderId = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
            m_programId = CreateProgram(m_vertexShaderId, m_fragmentShaderId);
            
            BuildAttributeHandleMap();
        }
    
        //------------------------------------------------------------------------------
        void GLShader::Bind() noexcept
        {
            glUseProgram(m_programId);
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding shader.");
        }
        
        //------------------------------------------------------------------------------
        void GLShader::SetUniform(const std::string& name, s32 value, FailurePolicy failurePolicy) noexcept
        {
            GLint uniformHandle = GetUniformHandle(name, failurePolicy);
            
            if(uniformHandle >= 0)
            {
                glUniform1i(uniformHandle, value);
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting uniform.");
            }
        }
        
        //------------------------------------------------------------------------------
        void GLShader::SetUniform(const std::string& name, f32 value, FailurePolicy failurePolicy) noexcept
        {
            GLint uniformHandle = GetUniformHandle(name, failurePolicy);
            
            if(uniformHandle >= 0)
            {
                glUniform1f(uniformHandle, value);
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting uniform.");
            }
        }
        
        //------------------------------------------------------------------------------
        void GLShader::SetUniform(const std::string& name, const ChilliSource::Vector2& value, FailurePolicy failurePolicy) noexcept
        {
            GLint uniformHandle = GetUniformHandle(name, failurePolicy);
            
            if(uniformHandle >= 0)
            {
                glUniform2fv(uniformHandle, 1, reinterpret_cast<const GLfloat*>(&value));
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting uniform.");
            }
        }
        
        //------------------------------------------------------------------------------
        void GLShader::SetUniform(const std::string& name, const ChilliSource::Vector3& value, FailurePolicy failurePolicy) noexcept
        {
            GLint uniformHandle = GetUniformHandle(name, failurePolicy);
            
            if(uniformHandle >= 0)
            {
                glUniform3fv(uniformHandle, 1, reinterpret_cast<const GLfloat*>(&value));
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting uniform.");
            }
        }
        
        //------------------------------------------------------------------------------
        void GLShader::SetUniform(const std::string& name, const ChilliSource::Vector4& value, FailurePolicy failurePolicy) noexcept
        {
            GLint uniformHandle = GetUniformHandle(name, failurePolicy);
            
            if(uniformHandle >= 0)
            {
                glUniform4fv(uniformHandle, 1, reinterpret_cast<const GLfloat*>(&value));
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting uniform.");
            }
        }
        
        //------------------------------------------------------------------------------
        void GLShader::SetUniform(const std::string& name, const ChilliSource::Matrix4& value, FailurePolicy failurePolicy) noexcept
        {
            GLint uniformHandle = GetUniformHandle(name, failurePolicy);
            
            if(uniformHandle >= 0)
            {
                glUniformMatrix4fv(uniformHandle, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value.m));
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting uniform.");
            }
        }
        
        //------------------------------------------------------------------------------
        void GLShader::SetUniform(const std::string& name, const ChilliSource::Colour& value, FailurePolicy failurePolicy) noexcept
        {
            GLint uniformHandle = GetUniformHandle(name, failurePolicy);
            
            if(uniformHandle >= 0)
            {
                glUniform4fv(uniformHandle, 1, reinterpret_cast<const GLfloat*>(&value));
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting uniform.");
            }
        }
        
        //------------------------------------------------------------------------------
        void GLShader::SetUniform(const std::string& name, const ChilliSource::Vector4* values, u32 numValues, FailurePolicy failurePolicy) noexcept
        {
            GLint uniformHandle = GetUniformHandle(name, failurePolicy);
            
            if(uniformHandle >= 0)
            {
                glUniform4fv(uniformHandle, numValues, reinterpret_cast<const GLfloat*>(values));
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting uniform.");
            }
        }
        
        //------------------------------------------------------------------------------
        void GLShader::SetAttribute(u32 index, GLint size, GLenum type, GLboolean isNormalised, GLsizei stride, const GLvoid* offset) noexcept
        {
			GLint handle = m_attributeHandles[index]; 
            if(handle < 0)
            {
                return;
            }

            glVertexAttribPointer(handle, size, type, isNormalised, stride, offset);

            CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting attribute.");
        }

        
        //------------------------------------------------------------------------------
        void GLShader::BuildAttributeHandleMap() noexcept
        {
			// This list must be kept in the same order as the ElementType enum in VertexFormat.h
            static const std::array<std::string, 8> attribNames =
            {{
                k_attributePosition, 
                k_attributeNormal,
                k_attributeTangent,
                k_attributeBitangent,
                k_attributeTexCoord,
                k_attributeColour,
                k_attributeWeights,
                k_attributeJointIndices
            }};
            
            for(std::size_t i = 0; i < attribNames.size(); ++i)
            {
                GLint handle = glGetAttribLocation(m_programId, attribNames[i].c_str());
                
                if(handle >= 0)
                {
					m_attributeHandles[i] = handle;
                }
				else
				{
					m_attributeHandles[i] = -1;
				}
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while populating attribute handles.");
        }
        
        //------------------------------------------------------------------------------
        GLint GLShader::GetUniformHandle(const std::string& name, FailurePolicy failurePolicy) noexcept
        {
            GLint uniformHandle = -1;
            
            auto it = m_uniformHandles.find(name);
            if(it != m_uniformHandles.end())
            {
                uniformHandle = it->second;
            }
            else
            {
                uniformHandle = glGetUniformLocation(m_programId, name.c_str());
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while getting uniform handle.");
                
                m_uniformHandles.insert(std::make_pair(name, uniformHandle));
            }

            if (uniformHandle < 0 && failurePolicy == FailurePolicy::k_hard)
            {
                CS_LOG_FATAL("Cannot find shader uniform: " + name);
            }
            
            return uniformHandle;
        }
        
        //------------------------------------------------------------------------------
        GLShader::~GLShader() noexcept
        {
            if(!m_invalidData)
            {
                if(m_vertexShaderId > 0)
                {
                    glDetachShader(m_programId, m_vertexShaderId);
                    glDeleteShader(m_vertexShaderId);
                }
                if(m_fragmentShaderId > 0)
                {
                    glDetachShader(m_programId, m_fragmentShaderId);
                    glDeleteShader(m_fragmentShaderId);
                }
                if(m_programId > 0)
                {
                    glDeleteProgram(m_programId);
                }
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while destroying shader.");
            }
        }

    }
}
