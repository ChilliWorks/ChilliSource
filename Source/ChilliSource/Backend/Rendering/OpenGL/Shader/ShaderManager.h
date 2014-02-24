/*
 * File: GLShaderManager.h
 * Date: 22/11/2010 2010 
 * Description: Handles creation of OpenGL ES shader programs
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLOW_OPENGL_SHADERMANAGER_H_
#define _MOFLOW_OPENGL_SHADERMANAGER_H_

#include <ChilliSource/Rendering/Shader/ShaderManager.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Shader/Shader.h>

namespace ChilliSource
{
	namespace OpenGL
	{
		class CShaderManager : public ChilliSource::Rendering::IShaderManager
		{
		public:
			//----------------------------------------------------------------
			/// Create Shader Resource
			///
			/// @return Concrete shader resource object based on the render
			/// system
			//----------------------------------------------------------------
			ChilliSource::Rendering::ShaderPtr CreateShaderResource() const;
			//----------------------------------------------------------------
			/// Manages Resource With Extension
			///
			/// @param Extension
			/// @return Whether this object manages object with extension
			//----------------------------------------------------------------
			bool ManagesResourceWithExtension(const std::string &instrExtension) const;
			//---------------------------------------------------------
			/// Create Shader Program From File
			///
			/// Load and compile a shader program from the given
			/// file.
			///
            /// @param The storage location to load from
			/// @param Shader file path
			/// @param Out: Shader resource
			/// @return Success
			//---------------------------------------------------------
			bool CreateShaderProgramFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath, ChilliSource::Rendering::ShaderPtr& outpShader);
			//---------------------------------------------------------
			/// Async Create Shader Program From File
			///
			/// Load and compile a shader program from the given
			/// file, in background
			///
            /// @param The storage location to load from
			/// @param Shader file path
			/// @param Out: Shader resource
			/// @return Success
			//---------------------------------------------------------
			bool AsyncCreateShaderProgramFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath, ChilliSource::Rendering::ShaderPtr& outpShader);
			//---------------------------------------------------------
			/// Load Shader Task
			///
            /// @param The storage location to load from
			/// @param Vertex shader file path
            /// @param Pixel shader file path
			/// @param Out: Shader resource
			//---------------------------------------------------------
			void LoadShaderTask(Core::StorageLocation ineStorageLocation, const std::string &instrVSFilePath, const std::string &instrPSFilePath, ChilliSource::Rendering::ShaderPtr& outpShader);
			//---------------------------------------------------------
			/// Compile Shader Task
			///
			/// @param Vertex shader
			/// @param Pixel shader
			/// @param Out: Shader resource
			//---------------------------------------------------------
			void CompileShaderTask(const std::string& insstrVS, const std::string& insstrPS, ChilliSource::Rendering::ShaderPtr& outpShader);
			//-----------------------------------------------------------------
			/// Restore
			///
			/// Restore all the cached shaders after a context loss
			//-----------------------------------------------------------------
			void Restore();
		};
	}
}

#endif
