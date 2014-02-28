/*
 * File: GLShaderManager.cpp
 * Date: 22/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Backend/Rendering/OpenGL/Shader/ShaderManager.h>

namespace ChilliSource
{
	namespace OpenGL
	{
		const std::string kstrGLVertexShaderExtension("vs");
		const std::string kstrGLFragmentShaderExtension("fs");
		
		//----------------------------------------------------------------
		/// Create Shader Resource
		///
		/// @return Concrete shader resource object based on the render
		/// system
		//----------------------------------------------------------------
		ChilliSource::Rendering::ShaderSPtr CShaderManager::CreateShaderResource() const
		{
			return ChilliSource::Rendering::ShaderSPtr(new CShader());
		}
		//----------------------------------------------------------------
		/// Manages Resource With Extension
		///
		/// @param Extension
		/// @return Whether this object manages object with extension
		//----------------------------------------------------------------
		bool CShaderManager::ManagesResourceWithExtension(const std::string &instrExtension) const
		{
			return (instrExtension == kstrGLVertexShaderExtension) || (instrExtension == kstrGLFragmentShaderExtension);
		}
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
		bool CShaderManager::CreateShaderProgramFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath, ChilliSource::Rendering::ShaderSPtr& outpShader)
		{
			std::shared_ptr<CShader> pGLShader = std::static_pointer_cast<CShader>(outpShader);
			
			//Check if this program already exists
			MapStringToResourcePtr::iterator pShaderResource = mMapFilenameToResource.find(instrFilePath);
			
			//If the shaderptr already exists then just return that
			if(pShaderResource != mMapFilenameToResource.end())
			{
				//It's already linked baby!
				outpShader = std::static_pointer_cast<ChilliSource::Rendering::Shader>(pShaderResource->second);
				return true;
			} 
			
			//Shader does not already exist
			if(!pGLShader->LoadAndCompileShader(ineStorageLocation, instrFilePath + "." + kstrGLVertexShaderExtension, Rendering::ShaderType::k_vertex))
			{
				return false;
			}

			if(!pGLShader->LoadAndCompileShader(ineStorageLocation, instrFilePath + "." + kstrGLFragmentShaderExtension, Rendering::ShaderType::k_fragment))
			{
				return false;
			}
			
			//Now link the two shaders together
			if(!pGLShader->CreateProgram())
			{
				return false;
			}
			
			pGLShader->SetLoaded(true);

			mMapFilenameToResource.insert(std::make_pair(instrFilePath, outpShader));
			return true;
		}
		//---------------------------------------------------------
		/// Async Create Shader Program From File
		///
		/// Load and compile a shader program from the given
		/// file, in background.
		///
        /// @param The storage location to load from
		/// @param Shader file path
		/// @param Out: Shader resource
		/// @return Success
		//---------------------------------------------------------
		bool CShaderManager::AsyncCreateShaderProgramFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath, ChilliSource::Rendering::ShaderSPtr& outpShader)
		{
			std::shared_ptr<CShader> pGLShader = std::static_pointer_cast<CShader>(outpShader);
			
			//Check if this program already exists
			MapStringToResourcePtr::iterator pShaderResource = mMapFilenameToResource.find(instrFilePath);
			
			//If the program already exists then just return that
			//Check for vertex shader in cache
			if(pShaderResource != mMapFilenameToResource.end())
			{
				//It's already linked baby!
				outpShader = std::static_pointer_cast<Rendering::Shader>(pShaderResource->second);
				return true;
			}
			
			//add resources to the lists
			mMapFilenameToResource.insert(std::make_pair(instrFilePath, outpShader));
			
			//create a task for loading the shader in the background
            Core::TaskScheduler::ScheduleTask(Core::Task<Core::StorageLocation, const std::string&, const std::string&, ChilliSource::Rendering::ShaderSPtr&>(this, &CShaderManager::LoadShaderTask, ineStorageLocation, instrFilePath + "." + kstrGLVertexShaderExtension, instrFilePath + "." + kstrGLFragmentShaderExtension, outpShader));
			
			return true;
		}
		//---------------------------------------------------------
		/// Load Shader Task
		///
        /// @param The storage location to load from
		/// @param Vertex shader file path
		/// @param Pixel shader file path
		/// @param Out: Shader resource
		//---------------------------------------------------------
		void CShaderManager::LoadShaderTask(Core::StorageLocation ineStorageLocation, const std::string &instrVSFilePath, const std::string &instrPSFilePath, ChilliSource::Rendering::ShaderSPtr& outpShader)
		{
			std::shared_ptr<CShader> pGLShader = std::static_pointer_cast<CShader>(outpShader);
			
			//load the VS
			std::stringstream sstrVS;
			if (instrVSFilePath != "")
			{
				//Read the shader contents from file
				if(!pGLShader->ReadShaderFromFile(ineStorageLocation, instrVSFilePath, sstrVS))
					return;
			}
			else
			{
				pGLShader->WaitTilLoaded();
			}
			
			//load the VS
			std::stringstream sstrPS;
			if (instrPSFilePath != "")
			{
				//Read the shader contents from file
				if(!pGLShader->ReadShaderFromFile(ineStorageLocation, instrPSFilePath, sstrPS))
					return;
			}
			else
			{
				pGLShader->WaitTilLoaded();
			}
			
			//schedule a task for the main thread to compile the shader
			Core::TaskScheduler::ScheduleMainThreadTask(Core::Task<const std::string&, const std::string&, ChilliSource::Rendering::ShaderSPtr&>(this, &CShaderManager::CompileShaderTask, sstrVS.str(), sstrPS.str(), outpShader));
		}
		//---------------------------------------------------------
		/// Compile Shader Task
		///
		/// @param Out: Shader resource
		//---------------------------------------------------------
		void CShaderManager::CompileShaderTask(const std::string& instrVS,const std::string& instrPS, ChilliSource::Rendering::ShaderSPtr& outpShader)
		{
			std::shared_ptr<CShader> pGLShader = std::static_pointer_cast<CShader>(outpShader);
			
			//compile the VS
			if (instrVS != "")
			{
				if (pGLShader->CompileShader(instrVS, Rendering::ShaderType::k_vertex) == false)
					return;
			}
			//compile the VS
			if (instrPS != "")
			{
				if (pGLShader->CompileShader(instrPS, Rendering::ShaderType::k_fragment) == false)
					return;
			}

			//Now link the two shaders together
			if(!pGLShader->CreateProgram())
			{
				return;
			}
			
			//set the shader as ready
			outpShader->SetLoaded(true);
		}
		//-----------------------------------------------------------------
		/// Restore
		///
		/// Restore all the cached shaders after a context loss
		//-----------------------------------------------------------------
		void CShaderManager::Restore()
		{
#ifdef TARGET_ANDROID
			if(mpRenderSystem)
			{
				for(MapStringToResourcePtr::iterator it = mMapFilenameToResource.begin(); it != mMapFilenameToResource.end(); ++it)
				{
					if(it->second->IsLoaded())
					{
						std::shared_ptr<CShader> pShader = std::static_pointer_cast<CShader>(it->second);

						if(pShader->LoadAndCompileShader(pShader->GetStorageLocation(), pShader->GetFilename() + "." + kstrGLVertexShaderExtension, Rendering::SHADER_TYPE_VERTEX))
						{
							if(pShader->LoadAndCompileShader(pShader->GetStorageLocation(), pShader->GetFilename() + "." + kstrGLFragmentShaderExtension, Rendering::SHADER_TYPE_FRAGMENT))
							{
								if(pShader->CreateProgram())
								{
									pShader->SetLoaded(true);
								}
								else
								{
									CS_FATAL_LOG("Failed to create shader: " + pShader->GetFilename());
								}
							}
							else
							{
								CS_FATAL_LOG("Failed to reload shader: " + pShader->GetFilename());
							}
						}
						else
						{
							CS_FATAL_LOG("Failed to reload shader: " + pShader->GetFilename());
						}
					}
				}
			}
#endif
		}
	}
}

