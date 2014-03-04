/*
 * File: GLShader.cpp
 * Date: 22/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Shader/Shader.h>

#include <algorithm>

namespace ChilliSource
{
	namespace OpenGL
	{
        const bool SortCachedLocations(const std::pair<u32, GLint> & inLHS, const std::pair<u32, GLint> & inRHS)
        {
            return inLHS.first < inRHS.first;
        }
        
		Shader::Shader()
		: mGLVertexShader(0), mGLPixelShader(0), mGLProgram(0)
		{
			
		}
		//----------------------------------------------------------
		/// Is A
		///
		/// Returns if it is of the type given
		/// @param Comparison Type
		/// @return Whether the class matches the comparison type
		//----------------------------------------------------------
		bool Shader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Shader::InterfaceID;
		}
		//----------------------------------------------------------
		/// Get Program ID
		///
		/// @return GL identifer of shader program
		//----------------------------------------------------------
		GLuint Shader::GetProgramID() const
		{
			return mGLProgram;
		}
        //----------------------------------------------------------
		/// Get Attribute Location
		///
		/// @param Name of attribute to be located
		/// @return GL identifer of attribute location
		//----------------------------------------------------------
        GLint Shader::GetAttributeLocation(const char * instrAttributeName)
        {
        	// Hash the name string
            LocationLookup sItem( Core::HashCRC32::GenerateHashCode(instrAttributeName), 0);
            
            // Binary search through a sorted array of items
            std::vector<LocationLookup>::iterator it;
            it = std::lower_bound(maAttributes.begin(), maAttributes.end(), sItem, SortCachedLocations);
            
            if(it!=maAttributes.end()  && it->first == sItem.first)
                return it->second;
            
            // Find and add if we don't already have it cached
            sItem.second = glGetAttribLocation(mGLProgram, instrAttributeName);
            maAttributes.push_back(sItem);
            
            // Resort our list
            std::sort(maAttributes.begin(), maAttributes.end(), SortCachedLocations);
            return sItem.second;
        }
        //----------------------------------------------------------
		/// Get Uniform Location
		///
		/// @param Name of uniform to be located
		/// @return GL identifer of uniform location
		//----------------------------------------------------------
        GLint Shader::GetUniformLocation(const char * instrUniformName)
        {
        	// Hash the name string
            LocationLookup sItem( Core::HashCRC32::GenerateHashCode(instrUniformName), 0);
            
            // Binary search through a sorted array of items
            std::vector<LocationLookup>::iterator it;
            it =std::lower_bound(maUniforms.begin(), maUniforms.end(), sItem, SortCachedLocations);
            
            if(it!=maUniforms.end()  && it->first == sItem.first)
                return it->second;
            
            // Find and add if we don't already have it cached
            sItem.second = glGetUniformLocation(mGLProgram, instrUniformName);
            maUniforms.push_back(sItem);
            
            // Resort our list
            std::sort(maUniforms.begin(), maUniforms.end(), SortCachedLocations);
            return sItem.second;
        }
		//----------------------------------------------------------
		/// Read Shader From File
		///
        /// @param The storage location to load from
		/// @param File path
		/// @param Out contents of file
		/// @return Success
		//----------------------------------------------------------
		bool Shader::ReadShaderFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath, std::stringstream& outstrContents)
		{
			//Open the shader file for reading only
			ChilliSource::Core::FileStreamSPtr shaderStream = ChilliSource::Core::Application::Get()->GetFileSystemPtr()->CreateFileStream(ineStorageLocation, instrFilePath, ChilliSource::Core::FileMode::k_read);
            
            if (shaderStream == nullptr || shaderStream->IsBad() == true)
			{
				CS_LOG_ERROR("Cannot open shader file " + instrFilePath + " for reading");
				shaderStream->Close();
				return false;
			}
			
			//Read the contents into a string
			shaderStream->Get(outstrContents);
			
			//Close the shader file
			shaderStream->Close();
			
			return true;
		}
		//----------------------------------------------------------
		/// Load and Compile Shader
		///
		/// Load the shader program from file and compile it for
		/// errors.
		///
        /// @param The storage location to load from
		/// @param The shader file path
		/// @return Success or failure
		//----------------------------------------------------------
		bool Shader::LoadAndCompileShader(Core::StorageLocation ineStorageLocation, const std::string& instrShaderFile, Rendering::ShaderType ineShaderType)
		{
            maAttributes.clear();
            maUniforms.clear();
            
			//Read the shader contents from file
			std::stringstream sstrShaderSource;
			if(!ReadShaderFromFile(ineStorageLocation, instrShaderFile, sstrShaderSource))
			{
				return false;
			}
			
			//Convert from our internal shader type to OpenGL's
			GLuint* pGLShader;
			GLenum GLShaderType;
			
			switch(ineShaderType)
			{
				case Rendering::ShaderType::k_combined:
					CS_LOG_ERROR("OpenGL ES 2.0 does not support combined vertex and pixel shaders");
					return false;
				case Rendering::ShaderType::k_vertex:
					GLShaderType = GL_VERTEX_SHADER;
					pGLShader = &mGLVertexShader;
					break;
				case Rendering::ShaderType::k_fragment:
					GLShaderType = GL_FRAGMENT_SHADER;
					pGLShader = &mGLPixelShader;
					break;
			};
			
			//Create a shader of the correct type
			*pGLShader = glCreateShader(GLShaderType);
			
			if(*pGLShader == 0)
			{
				CS_LOG_FATAL("OpenGL ES 2.0 does not support shader type for " + instrShaderFile);
				return false;
			}
			
			//Load the shader source 
			CS_LOG_DEBUG("Loading shader " + instrShaderFile);
			std::string strSource = sstrShaderSource.str();
			const GLchar* pGLSource = strSource.c_str();
			glShaderSource(*pGLShader, 1, &pGLSource, nullptr);
			
			//Compile the shader and check for errors
			CS_LOG_DEBUG("Compiling shader " + instrShaderFile);
			glCompileShader(*pGLShader);
			
			GLint Compiled;
			glGetShaderiv(*pGLShader, GL_COMPILE_STATUS, &Compiled);
			
			if(!Compiled)
			{
				//We failed to compile. Spit out the errors
				GLint InfoLength = 0;
				glGetShaderiv(*pGLShader, GL_INFO_LOG_LENGTH, &InfoLength);
				
				if(InfoLength > 1)
				{
					char* ShaderLog = (char*)malloc(sizeof(char) * InfoLength);
					glGetShaderInfoLog(*pGLShader, InfoLength, nullptr, ShaderLog);
					CS_LOG_ERROR("Compiling shader " + instrShaderFile + ". " + std::string(ShaderLog));
					CS_LOG_FATAL("Failed to compile shader");
					free(ShaderLog);
					return false;
				}
				
				CS_LOG_FATAL("Compiling shader " + instrShaderFile);
				return false;
			}
			
			CS_LOG_DEBUG("Shader compiled successfully\n");
			return true;
		}
		//----------------------------------------------------------
		/// Compile Shader
		///
		/// compiles a pre-loaded
		///
		/// @param The shader file path
		/// @return Success or failure
		//----------------------------------------------------------
		bool Shader::CompileShader(const std::string& instrShaderSource, Rendering::ShaderType ineShaderType)
		{
			//Convert from our internal shader type to OpenGL's
			GLuint* pGLShader;
			GLenum GLShaderType;
			
			switch(ineShaderType)
			{
				case Rendering::ShaderType::k_combined:
					CS_LOG_ERROR("OpenGL ES 2.0 does not support combined vertex and pixel shaders");
					return false;
				case Rendering::ShaderType::k_vertex:
					GLShaderType = GL_VERTEX_SHADER;
					pGLShader = &mGLVertexShader;
					break;
				case Rendering::ShaderType::k_fragment:
					GLShaderType = GL_FRAGMENT_SHADER;
					pGLShader = &mGLPixelShader;
					break;
			};
			
			//Create a shader of the correct type
			*pGLShader = glCreateShader(GLShaderType);
			
			if(*pGLShader == 0)
			{
				CS_LOG_FATAL("OpenGL ES 2.0 does not support shader type");
				return false;
			}
			
			//Load the shader source
			CS_LOG_DEBUG("Loading shader from pre loaded source.");
			const GLchar* pGLSource = instrShaderSource.c_str();
			glShaderSource(*pGLShader, 1, &pGLSource, nullptr);
			
			//Compile the shader and check for errors
			CS_LOG_DEBUG("Compiling shader...");
			glCompileShader(*pGLShader);
			
			GLint Compiled;
			glGetShaderiv(*pGLShader, GL_COMPILE_STATUS, &Compiled);
			
			if(!Compiled)
			{
				//We failed to compile. Spit out the errors
				GLint InfoLength = 0;
				glGetShaderiv(*pGLShader, GL_INFO_LOG_LENGTH, &InfoLength);
				
				if(InfoLength > 1)
				{
					char* ShaderLog = (char*)malloc(sizeof(char) * InfoLength);
					glGetShaderInfoLog(*pGLShader, InfoLength, nullptr, ShaderLog);
					CS_LOG_ERROR("Compiling shader. " + std::string(ShaderLog));
					CS_LOG_FATAL(std::string(pGLSource));
					free(ShaderLog);
					return false;
				}
				
				CS_LOG_FATAL("Compiling shader.");
				return false;
			}
			
			CS_LOG_DEBUG("Shader compiled successfully\n");
			return true;
		}
		//----------------------------------------------------------
		/// Create Program
		///
		/// Create shader program from the vertex and pixel shaders
		/// @return Success
		//----------------------------------------------------------
		bool Shader::CreateProgram()
		{
			mGLProgram = glCreateProgram();
			
			if(mGLProgram == 0)
			{
				CS_LOG_FATAL("Creating OpenGL ES shader program");
			}
			
			//Now attach our two shaders
			glAttachShader(mGLProgram, mGLVertexShader);
			glAttachShader(mGLProgram, mGLPixelShader);
			
			//Link the shaders together
			glLinkProgram(mGLProgram);
			
			//Check for success
			GLint linked;
			glGetProgramiv(mGLProgram, GL_LINK_STATUS, &linked);
			
			if(!linked)
			{
				//We failed to link. Spit out the errors
				GLint InfoLength = 0;
				glGetProgramiv(mGLProgram, GL_INFO_LOG_LENGTH, &InfoLength);
				
				if(InfoLength > 1)
				{
					char* ShaderLog = (char*)malloc(sizeof(char) * InfoLength);
					glGetProgramInfoLog(mGLProgram, InfoLength, nullptr, ShaderLog);
					CS_LOG_ERROR("Linking shader: " + std::string(ShaderLog));
					free(ShaderLog);
					return false;
				}
			}
			
			CS_LOG_DEBUG("Linking shader complete\n");
			return true;
		}
		//----------------------------------------------------------
		/// Destructor
		///
		//----------------------------------------------------------
		Shader::~Shader()
		{
			if(mGLVertexShader)
			{
				glDetachShader(mGLProgram, mGLVertexShader);
				glDeleteShader(mGLVertexShader);
			}
			if(mGLPixelShader)
			{
				glDetachShader(mGLProgram, mGLPixelShader);
				glDeleteShader(mGLPixelShader);
			}
			if(mGLProgram)
			{
				glDeleteProgram(mGLProgram);
			}
		}
	}
}


