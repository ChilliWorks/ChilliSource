/*
 * File: ShaderManager.cpp
 * Date: 22/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Rendering/Shader/ShaderManager.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>

namespace ChilliSource
{
	namespace Rendering
	{
		
		DEFINE_NAMED_INTERFACE(ShaderManager);
		
		//----------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//----------------------------------------------------------------
		ShaderManager::ShaderManager() : mpRenderSystem(nullptr)
		{
			
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool ShaderManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ShaderManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType ShaderManager::GetResourceType() const
		{
			return Shader::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool ShaderManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Shader::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType ShaderManager::GetProviderType() const
		{
			return Shader::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr ShaderManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetShaderFromFile(ineStorageLocation, instrFilePath);
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		///
		/// Generic call to get the managers resource, loaded in the background
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr ShaderManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return AsyncGetShaderFromFile(ineStorageLocation, instrFilePath);
		}
		//----------------------------------------------------------------
		/// Get Shader From File
		///
		/// Loads the Shader from resource and returns a handle to it.
		/// Alternately if the Shader already exists it will return the
		/// handle without loading
		///
        /// @param The storage location to load from
		/// @param File name
		/// @return A handle to the Shader
		//----------------------------------------------------------------
		ShaderSPtr ShaderManager::GetShaderFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath)
		{
            if(mpRenderSystem)
            {
                //Where we load the shader from is based on the render system
                std::string strShaderFilePath = mpRenderSystem->GetPathToShaders();
                //The shader is cached by the concrete loader
                ShaderSPtr pShader = CreateShaderResource();

                if(CreateShaderProgramFromFile(ineStorageLocation, strShaderFilePath + inFilePath, pShader))
                {
                    pShader->SetName(inFilePath);
                    pShader->SetOwningResourceManager(this);
                    pShader->SetFilename(strShaderFilePath + inFilePath);
                    pShader->SetStorageLocation(ineStorageLocation);
                    pShader->SetLoaded(true);
                    return pShader;
                }
            }
			return ShaderSPtr();
		}
		//----------------------------------------------------------------
		/// Get Shader From File
		///
		/// Loads the Shader from resource and returns a handle to it.
		/// Alternately if the Shader already exists it will return the
		/// handle without loading
		///
        /// @param The storage location to load from
		/// @param File name
		/// @return A handle to the Shader
		//----------------------------------------------------------------
		ShaderSPtr ShaderManager::AsyncGetShaderFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath)
		{
            if(mpRenderSystem)
            {
                //Where we load the shader from is based on the render system
                std::string strShaderFilePath = mpRenderSystem->GetPathToShaders();
                //The shader is cached by the concrete loader
                ShaderSPtr pShader = CreateShaderResource();

                pShader->SetName(inFilePath);
                pShader->SetOwningResourceManager(this);
                pShader->SetFilename(strShaderFilePath + inFilePath);
                pShader->SetStorageLocation(ineStorageLocation);

                if(AsyncCreateShaderProgramFromFile(ineStorageLocation, strShaderFilePath + inFilePath, pShader))
                {
                    return pShader;
                }
            }

			return ShaderSPtr();
		}
        //-----------------------------------------------------------------
        /// Set Render System
        ///
        /// @param Render system 
        //-----------------------------------------------------------------
        void ShaderManager::SetRenderSystem(RenderSystem* inpRenderSystem)
        {
            mpRenderSystem = inpRenderSystem;
        }
		//----------------------------------------------------------------
		/// Destructor
		///
		//----------------------------------------------------------------
		ShaderManager::~ShaderManager()
		{
			
		}
	}
}
