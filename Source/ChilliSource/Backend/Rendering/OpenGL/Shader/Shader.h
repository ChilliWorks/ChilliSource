/*
 * File: GLShader.h
 * Date: 22/11/2010 2010 
 * Description: Concrete shader implementation. Once a shader has been compiled by GL it is held here
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLOW_OPENGL_SHADER_H_
#define _MOFLOW_OPENGL_SHADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Rendering/Shader/Shader.h>

namespace ChilliSource
{
	namespace OpenGL
	{
		class CShader : public ChilliSource::Rendering::Shader
		{
		public:
			
			virtual ~CShader();
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------
			/// Get Program ID
			///
			/// @return GL identifer of shader program
			//----------------------------------------------------------
			GLuint GetProgramID() const;
            //----------------------------------------------------------
			/// Get Attribute Location
			///
			/// @param Name of attribute to be located
			/// @return GL identifer of attribute location
			//----------------------------------------------------------
            GLint GetAttributeLocation(const char * instrAttributeName);
            //----------------------------------------------------------
			/// Get Uniform Location
			///
			/// @param Name of uniform to be located
			/// @return GL identifer of uniform location
			//----------------------------------------------------------
            GLint GetUniformLocation(const char * instrUniformName);
		protected:
			
			//No randomly creating this! That means you!
			CShader();
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
			bool LoadAndCompileShader(Core::StorageLocation ineStorageLocation, const std::string& instrShaderFile, Rendering::ShaderType ineShaderType);
			//----------------------------------------------------------
			/// Compile Shader
			///
			/// Compiles a pre-loaded shader
			///
			/// @param The shader file path
			/// @return Success or failure
			//----------------------------------------------------------
			bool CompileShader(const std::string& instrShaderSource, Rendering::ShaderType ineShaderType);
			//----------------------------------------------------------
			/// Create Program
			///
			/// Create shader program from the vertex and pixel shaders
			/// @return Success
			//----------------------------------------------------------
			bool CreateProgram();
			//----------------------------------------------------------
			/// Read Shader From File
			///
            /// @param The storage location to load from
			/// @param File path
			/// @param Out contents of file
			/// @return Success
			//----------------------------------------------------------
			bool ReadShaderFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath, std::stringstream& outstrContents);
			
			friend class CShaderManager;
			
		private:

			GLuint mGLVertexShader;
			GLuint mGLPixelShader;
			GLuint mGLProgram;
            
            // Cached attribute and uniform locations
            typedef std::pair<u32, GLint> LocationLookup;
            std::vector<LocationLookup> maAttributes;
            std::vector<LocationLookup> maUniforms;
		};
		
		typedef std::shared_ptr<CShader> ShaderSPtr;
		typedef std::weak_ptr<CShader> ShaderWeakPtr;
	}
}

#endif
