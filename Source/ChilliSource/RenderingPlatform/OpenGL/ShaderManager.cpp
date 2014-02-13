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

#include <ChilliSource/RenderingPlatform/OpenGL/ShaderManager.h>

namespace moFlo
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
		moFlo::Rendering::ShaderPtr CShaderManager::CreateShaderResource() const
		{
			return moFlo::Rendering::ShaderPtr(new CShader());
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
		bool CShaderManager::CreateShaderProgramFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, moFlo::Rendering::ShaderPtr& outpShader)
		{
			SHARED_PTR<CShader> pGLShader = SHARED_PTR_CAST<CShader>(outpShader);
			
			//Check if this program already exists
			MapStringToResourcePtr::iterator pShaderResource = mMapFilenameToResource.find(instrFilePath);
			
			//If the shaderptr already exists then just return that
			if(pShaderResource != mMapFilenameToResource.end())
			{
				//It's already linked baby!
				outpShader = SHARED_PTR_CAST<moFlo::Rendering::IShader>(pShaderResource->second);
				return true;
			} 
			
			//Shader does not already exist
			if(!pGLShader->LoadAndCompileShader(ineStorageLocation, instrFilePath + "." + kstrGLVertexShaderExtension, Rendering::SHADER_TYPE_VERTEX))
			{
				return false;
			}

			if(!pGLShader->LoadAndCompileShader(ineStorageLocation, instrFilePath + "." + kstrGLFragmentShaderExtension, Rendering::SHADER_TYPE_FRAGMENT))
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
		bool CShaderManager::AsyncCreateShaderProgramFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, moFlo::Rendering::ShaderPtr& outpShader)
		{
			SHARED_PTR<CShader> pGLShader = SHARED_PTR_CAST<CShader>(outpShader);
			
			//Check if this program already exists
			MapStringToResourcePtr::iterator pShaderResource = mMapFilenameToResource.find(instrFilePath);
			
			//If the program already exists then just return that
			//Check for vertex shader in cache
			if(pShaderResource != mMapFilenameToResource.end())
			{
				//It's already linked baby!
				outpShader = SHARED_PTR_CAST<Rendering::IShader>(pShaderResource->second);
				return true;
			}
			
			//add resources to the lists
			mMapFilenameToResource.insert(std::make_pair(instrFilePath, outpShader));
			
			//create a task for loading the shader in the background
			CTaskScheduler::ScheduleTask(Task4<Core::STORAGE_LOCATION, const std::string&, const std::string&, moFlo::Rendering::ShaderPtr&>(this, &CShaderManager::LoadShaderTask, ineStorageLocation, instrFilePath + "." + kstrGLVertexShaderExtension, instrFilePath + "." + kstrGLFragmentShaderExtension, outpShader));
			
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
		void CShaderManager::LoadShaderTask(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrVSFilePath, const std::string &instrPSFilePath, moFlo::Rendering::ShaderPtr& outpShader)
		{
			SHARED_PTR<CShader> pGLShader = SHARED_PTR_CAST<CShader>(outpShader);
			
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
			CTaskScheduler::ScheduleMainThreadTask(Task3<const std::string&, const std::string&, moFlo::Rendering::ShaderPtr&>(this, &CShaderManager::CompileShaderTask, sstrVS.str(), sstrPS.str(), outpShader));
		}
		//---------------------------------------------------------
		/// Compile Shader Task
		///
		/// @param Out: Shader resource
		//---------------------------------------------------------
		void CShaderManager::CompileShaderTask(const std::string& instrVS,const std::string& instrPS, moFlo::Rendering::ShaderPtr& outpShader)
		{
			SHARED_PTR<CShader> pGLShader = SHARED_PTR_CAST<CShader>(outpShader);
			
			//compile the VS
			if (instrVS != "")
			{
				if (pGLShader->CompileShader(instrVS, Rendering::SHADER_TYPE_VERTEX) == false)
					return;
			}
			//compile the VS
			if (instrPS != "")
			{
				if (pGLShader->CompileShader(instrPS, Rendering::SHADER_TYPE_FRAGMENT) == false)
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
						SHARED_PTR<CShader> pShader = SHARED_PTR_CAST<CShader>(it->second);

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
									FATAL_LOG("Failed to create shader: " + pShader->GetFilename());
								}
							}
							else
							{
								FATAL_LOG("Failed to reload shader: " + pShader->GetFilename());
							}
						}
						else
						{
							FATAL_LOG("Failed to reload shader: " + pShader->GetFilename());
						}
					}
				}
			}
#endif
		}
	}
}

