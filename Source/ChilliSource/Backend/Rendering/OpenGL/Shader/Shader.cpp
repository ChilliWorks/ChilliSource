//
//  Shader.cpp
//  Chilli Source
//  Created by Scott Downie on 22/11/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Backend/Rendering/OpenGL/Shader/Shader.h>

#include <ChilliSource/Core/Math/Matrix4x4.h>

#include <array>

namespace ChilliSource
{
	namespace OpenGL
	{
        CS_DEFINE_NAMEDTYPE(Shader);
        
        //----------------------------------------------------------
        //----------------------------------------------------------
		Shader::Shader()
		: m_programId(0), m_vertexShaderId(0), m_fragmentShaderId(0)
		{
			
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		bool Shader::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == Shader::InterfaceID || in_interfaceId == Rendering::Shader::InterfaceID;
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::Build(const std::string& in_vs, const std::string& in_fs)
        {
            Destroy();
            
            CompileShader(in_vs, GL_VERTEX_SHADER);
            CompileShader(in_fs, GL_FRAGMENT_SHADER);
            CreateProgram(m_vertexShaderId, m_fragmentShaderId);
            PopulateAttributeHandles();
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::Bind()
        {
            glUseProgram(m_programId);
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::PopulateAttributeHandles()
        {
            const std::array<std::string, 6> attribNames =
            {{
                "a_position",
                "a_colour",
                "a_normal",
                "a_texCoord",
                "a_weights",
                "a_jointIndices"
            }};
            
            for(const auto& name : attribNames)
            {
                GLint handle = glGetAttribLocation(m_programId, name.c_str());
                if(handle >= 0)
                {
                    m_attribHandles.insert(std::make_pair(name, handle));
                }
            }
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::SetUniform(const std::string& in_varName, s32 in_value, UniformNotFoundPolicy in_notFoundPolicy)
        {
            GLint handle = GetUniformHandle(in_varName);
            
            if(handle < 0)
            {
                switch (in_notFoundPolicy)
                {
                    case UniformNotFoundPolicy::k_failHard:
                        CS_LOG_FATAL("Cannot find shader uniform: " + in_varName);
                        return;
                    case UniformNotFoundPolicy::k_failSilent:
                        return;
                }
            }
            
            glUniform1i(handle, in_value);
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::SetUniform(const std::string& in_varName, f32 in_value, UniformNotFoundPolicy in_notFoundPolicy)
        {
            GLint handle = GetUniformHandle(in_varName);
            
            if(handle < 0)
            {
                switch (in_notFoundPolicy)
                {
                    case UniformNotFoundPolicy::k_failHard:
                        CS_LOG_FATAL("Cannot find shader uniform: " + in_varName);
                        return;
                    case UniformNotFoundPolicy::k_failSilent:
                        return;
                }
            }
            
            glUniform1f(handle, in_value);
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::SetUniform(const std::string& in_varName, const Core::Vector2& in_value, UniformNotFoundPolicy in_notFoundPolicy)
        {
            GLint handle = GetUniformHandle(in_varName);
            
            if(handle < 0)
            {
                switch (in_notFoundPolicy)
                {
                    case UniformNotFoundPolicy::k_failHard:
                        CS_LOG_FATAL("Cannot find shader uniform: " + in_varName);
                        return;
                    case UniformNotFoundPolicy::k_failSilent:
                        return;
                }
            }
            
            glUniform2fv(handle, 1, (GLfloat*)(&in_value));
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::SetUniform(const std::string& in_varName, const Core::Vector3& in_value, UniformNotFoundPolicy in_notFoundPolicy)
        {
            GLint handle = GetUniformHandle(in_varName);
            
            if(handle < 0)
            {
                switch (in_notFoundPolicy)
                {
                    case UniformNotFoundPolicy::k_failHard:
                        CS_LOG_FATAL("Cannot find shader uniform: " + in_varName);
                        return;
                    case UniformNotFoundPolicy::k_failSilent:
                        return;
                }
            }
            
            glUniform3fv(handle, 1, (GLfloat*)(&in_value));
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::SetUniform(const std::string& in_varName, const Core::Vector4& in_value, UniformNotFoundPolicy in_notFoundPolicy)
        {
            GLint handle = GetUniformHandle(in_varName);
            
            if(handle < 0)
            {
                switch (in_notFoundPolicy)
                {
                    case UniformNotFoundPolicy::k_failHard:
                        CS_LOG_FATAL("Cannot find shader uniform: " + in_varName);
                        return;
                    case UniformNotFoundPolicy::k_failSilent:
                        return;
                }
            }
            
            glUniform4fv(handle, 1, (GLfloat*)(&in_value));
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::SetUniform(const std::string& in_varName, const Core::Matrix4x4& in_value, UniformNotFoundPolicy in_notFoundPolicy)
        {
            GLint handle = GetUniformHandle(in_varName);
            
            if(handle < 0)
            {
                switch (in_notFoundPolicy)
                {
                    case UniformNotFoundPolicy::k_failHard:
                        CS_LOG_FATAL("Cannot find shader uniform: " + in_varName);
                        return;
                    case UniformNotFoundPolicy::k_failSilent:
                        return;
                }
            }
            
            glUniformMatrix4fv(handle, 1, GL_FALSE, (GLfloat*)(&in_value.m));
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::SetUniform(const std::string& in_varName, const Core::Colour& in_value, UniformNotFoundPolicy in_notFoundPolicy)
        {
            GLint handle = GetUniformHandle(in_varName);
            
            if(handle < 0)
            {
                switch (in_notFoundPolicy)
                {
                    case UniformNotFoundPolicy::k_failHard:
                        CS_LOG_FATAL("Cannot find shader uniform: " + in_varName);
                        return;
                    case UniformNotFoundPolicy::k_failSilent:
                        return;
                }
            }
            
            glUniform4fv(handle, 1, (GLfloat*)(&in_value));
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::SetUniform(const std::string& in_varName, const std::vector<Core::Vector4>& in_vec4Values, UniformNotFoundPolicy in_notFoundPolicy)
        {
            GLint handle = GetUniformHandle(in_varName);
            
            if(handle < 0)
            {
                switch (in_notFoundPolicy)
                {
                    case UniformNotFoundPolicy::k_failHard:
                        CS_LOG_FATAL("Cannot find shader uniform: " + in_varName);
                        return;
                    case UniformNotFoundPolicy::k_failSilent:
                        return;
                }
            }
            
            glUniform4fv(handle, in_vec4Values.size(), (GLfloat*)(&in_vec4Values[0]));
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        bool Shader::HasUniform(const std::string& in_varName)
        {
            return GetUniformHandle(in_varName) >= 0;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::SetAttribute(const std::string& in_varName, GLint in_size, GLenum in_type, GLboolean in_isNormalized, GLsizei in_stride, const GLvoid* in_offset)
        {
            auto it = m_attribHandles.find(in_varName);
            
            if(it == m_attribHandles.end())
            {
                return;
            }
            
            glVertexAttribPointer(it->second, in_size, in_type, in_isNormalized, in_stride, in_offset);
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        GLint Shader::GetUniformHandle(const std::string& in_name)
        {
            auto it = m_uniformHandles.find(in_name);
            
            if(it != m_uniformHandles.end())
            {
                return it->second;
            }
            
            GLint handle = glGetUniformLocation(m_programId, in_name.c_str());
            m_uniformHandles.insert(std::make_pair(in_name, handle));
            return handle;
        }
		//----------------------------------------------------------
		//----------------------------------------------------------
		void Shader::CompileShader(const std::string& in_shaderSource, GLenum in_shaderType)
		{
            GLuint* shaderId = nullptr;
            
			switch(in_shaderType)
			{
				case GL_VERTEX_SHADER:
					shaderId = &m_vertexShaderId;
					break;
				case GL_FRAGMENT_SHADER:
					shaderId = &m_fragmentShaderId;
					break;
			};
			
			*shaderId = glCreateShader(in_shaderType);
            CS_ASSERT(*shaderId > 0, "OpenGL ES 2.0 does not support this shader type");
			
            const GLchar* glSource = in_shaderSource.c_str();
			glShaderSource(*shaderId, 1, &glSource, nullptr);
			glCompileShader(*shaderId);
			
			GLint compilationStatus;
			glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &compilationStatus);
			
			if(compilationStatus == 0)
			{
				GLint errorLength = 0;
				glGetShaderiv(*shaderId, GL_INFO_LOG_LENGTH, &errorLength);
                
				if(errorLength > 1)
				{
					char* errorLog = (char*)malloc(sizeof(char) * errorLength);
					glGetShaderInfoLog(*shaderId, errorLength, nullptr, errorLog);
					
					CS_LOG_ERROR("GLSL compilation error: " + std::string(errorLog));
					free(errorLog);
				}
                
                CS_LOG_FATAL("Failed to compile GLSL shader");
			}
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::CreateProgram(GLuint in_vs, GLuint in_fs)
        {
            m_programId = glCreateProgram();
            CS_ASSERT(m_programId > 0, "Failed to create GLSL program");
            
            glAttachShader(m_programId, in_vs);
            glAttachShader(m_programId, in_fs);
            glLinkProgram(m_programId);
            
            //Check for success
			GLint linkStatus;
			glGetProgramiv(m_programId, GL_LINK_STATUS, &linkStatus);
			
			if(linkStatus == 0)
			{
				GLint errorLength = 0;
				glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &errorLength);
				
				if(errorLength > 1)
				{
					char* errorLog = (char*)malloc(sizeof(char) * errorLength);
					glGetProgramInfoLog(m_programId, errorLength, nullptr, errorLog);
					CS_LOG_ERROR("GLSL link error: " + std::string(errorLog));
					free(errorLog);
				}
                
                CS_LOG_FATAL("Failed to link GLSL shader");
			}
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Shader::Destroy()
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
            
            m_attribHandles.clear();
            m_uniformHandles.clear();
        }
		//----------------------------------------------------------
		//----------------------------------------------------------
		Shader::~Shader()
		{
            Destroy();
		}
	}
}


