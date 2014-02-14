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

#include <ChilliSource/Core/Main/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Main/Application.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Shader/Shader.h>

namespace moFlo
{
	namespace OpenGL
	{
        const bool SortCachedLocations(const std::pair<u32, GLint> & inLHS, const std::pair<u32, GLint> & inRHS)
        {
            return inLHS.first < inRHS.first;
        }
        
		CShader::CShader()
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
		bool CShader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IShader::InterfaceID;
		}
		//----------------------------------------------------------
		/// Get Program ID
		///
		/// @return GL identifer of shader program
		//----------------------------------------------------------
		GLuint CShader::GetProgramID() const
		{
			return mGLProgram;
		}
        //----------------------------------------------------------
		/// Get Attribute Location
		///
		/// @param Name of attribute to be located
		/// @return GL identifer of attribute location
		//----------------------------------------------------------
        GLint CShader::GetAttributeLocation(const char * instrAttributeName)
        {
        	// Hash the name string
            LocationLookup sItem( moFlo::CHashCRC32::GenerateHashCode(instrAttributeName), 0);
            
            // Binary search through a sorted array of items
            DYNAMIC_ARRAY<LocationLookup>::iterator it;
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
        GLint CShader::GetUniformLocation(const char * instrUniformName)
        {
        	// Hash the name string
            LocationLookup sItem( moFlo::CHashCRC32::GenerateHashCode(instrUniformName), 0);
            
            // Binary search through a sorted array of items
            DYNAMIC_ARRAY<LocationLookup>::iterator it;
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
		bool CShader::ReadShaderFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, std::stringstream& outstrContents)
		{
			//Open the shader file for reading only
			moFlo::Core::FileStreamPtr shaderStream = moFlo::Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, instrFilePath, moFlo::Core::FM_READ);
            
            if (shaderStream == NULL || shaderStream->IsBad() == true)
			{
				ERROR_LOG("Cannot open shader file " + instrFilePath + " for reading");
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
		bool CShader::LoadAndCompileShader(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrShaderFile, Rendering::ShaderType ineShaderType)
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
				case Rendering::SHADER_TYPE_COMBINED:
					ERROR_LOG("OpenGL ES 2.0 does not support combined vertex and pixel shaders");
					return false;
				case Rendering::SHADER_TYPE_VERTEX:
					GLShaderType = GL_VERTEX_SHADER;
					pGLShader = &mGLVertexShader;
					break;
				case Rendering::SHADER_TYPE_FRAGMENT:
					GLShaderType = GL_FRAGMENT_SHADER;
					pGLShader = &mGLPixelShader;
					break;
			};
			
			//Create a shader of the correct type
			*pGLShader = glCreateShader(GLShaderType);
			
			if(*pGLShader == 0)
			{
				FATAL_LOG("OpenGL ES 2.0 does not support shader type for " + instrShaderFile);
				return false;
			}
			
			//Load the shader source 
			DEBUG_LOG("Loading shader " + instrShaderFile);
			std::string strSource = sstrShaderSource.str();
			const GLchar* pGLSource = strSource.c_str();
			glShaderSource(*pGLShader, 1, &pGLSource, NULL);
			
			//Compile the shader and check for errors
			DEBUG_LOG("Compiling shader " + instrShaderFile);
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
					glGetShaderInfoLog(*pGLShader, InfoLength, NULL, ShaderLog);
					ERROR_LOG("Compiling shader " + instrShaderFile + ". " + std::string(ShaderLog));
					FATAL_LOG("Failed to compile shader");
					free(ShaderLog);
					return false;
				}
				
				FATAL_LOG("Compiling shader " + instrShaderFile);
				return false;
			}
			
			DEBUG_LOG("Shader compiled successfully\n");
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
		bool CShader::CompileShader(const std::string& instrShaderSource, Rendering::ShaderType ineShaderType)
		{
			//Convert from our internal shader type to OpenGL's
			GLuint* pGLShader;
			GLenum GLShaderType;
			
			switch(ineShaderType)
			{
				case Rendering::SHADER_TYPE_COMBINED:
					ERROR_LOG("OpenGL ES 2.0 does not support combined vertex and pixel shaders");
					return false;
				case Rendering::SHADER_TYPE_VERTEX:
					GLShaderType = GL_VERTEX_SHADER;
					pGLShader = &mGLVertexShader;
					break;
				case Rendering::SHADER_TYPE_FRAGMENT:
					GLShaderType = GL_FRAGMENT_SHADER;
					pGLShader = &mGLPixelShader;
					break;
			};
			
			//Create a shader of the correct type
			*pGLShader = glCreateShader(GLShaderType);
			
			if(*pGLShader == 0)
			{
				FATAL_LOG("OpenGL ES 2.0 does not support shader type");
				return false;
			}
			
			//Load the shader source
			DEBUG_LOG("Loading shader from pre loaded source.");
			const GLchar* pGLSource = instrShaderSource.c_str();
			glShaderSource(*pGLShader, 1, &pGLSource, NULL);
			
			//Compile the shader and check for errors
			DEBUG_LOG("Compiling shader...");
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
					glGetShaderInfoLog(*pGLShader, InfoLength, NULL, ShaderLog);
					ERROR_LOG("Compiling shader. " + std::string(ShaderLog));
					FATAL_LOG(std::string(pGLSource));
					free(ShaderLog);
					return false;
				}
				
				FATAL_LOG("Compiling shader.");
				return false;
			}
			
			DEBUG_LOG("Shader compiled successfully\n");
			return true;
		}
		//----------------------------------------------------------
		/// Create Program
		///
		/// Create shader program from the vertex and pixel shaders
		/// @return Success
		//----------------------------------------------------------
		bool CShader::CreateProgram()
		{
			mGLProgram = glCreateProgram();
			
			if(mGLProgram == 0)
			{
				FATAL_LOG("Creating OpenGL ES shader program");
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
					glGetProgramInfoLog(mGLProgram, InfoLength, NULL, ShaderLog);
					ERROR_LOG("Linking shader: " + std::string(ShaderLog));
					free(ShaderLog);
					return false;
				}
			}
			
			DEBUG_LOG("Linking shader complete\n");
			return true;
		}
		//----------------------------------------------------------
		/// Destructor
		///
		//----------------------------------------------------------
		CShader::~CShader()
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


