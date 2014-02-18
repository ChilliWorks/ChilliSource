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

namespace moFlo
{
	namespace Rendering
	{
		
		DEFINE_NAMED_INTERFACE(IShaderManager);
		
		//----------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//----------------------------------------------------------------
		IShaderManager::IShaderManager() : mpRenderSystem(NULL)
		{
			
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool IShaderManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IShaderManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType IShaderManager::GetResourceType() const
		{
			return IShader::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool IShaderManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IShader::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType IShaderManager::GetProviderType() const
		{
			return IShader::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr IShaderManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
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
		Core::ResourcePtr IShaderManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
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
		ShaderPtr IShaderManager::GetShaderFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath)
		{
            if(mpRenderSystem)
            {
                //Where we load the shader from is based on the render system
                std::string strShaderFilePath = mpRenderSystem->GetPathToShaders();
                //The shader is cached by the concrete loader
                ShaderPtr pShader = CreateShaderResource();

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
			return ShaderPtr();
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
		ShaderPtr IShaderManager::AsyncGetShaderFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath)
		{
            if(mpRenderSystem)
            {
                //Where we load the shader from is based on the render system
                std::string strShaderFilePath = mpRenderSystem->GetPathToShaders();
                //The shader is cached by the concrete loader
                ShaderPtr pShader = CreateShaderResource();

                pShader->SetName(inFilePath);
                pShader->SetOwningResourceManager(this);
                pShader->SetFilename(strShaderFilePath + inFilePath);
                pShader->SetStorageLocation(ineStorageLocation);

                if(AsyncCreateShaderProgramFromFile(ineStorageLocation, strShaderFilePath + inFilePath, pShader))
                {
                    return pShader;
                }
            }

			return ShaderPtr();
		}
        //-----------------------------------------------------------------
        /// Set Render System
        ///
        /// @param Render system 
        //-----------------------------------------------------------------
        void IShaderManager::SetRenderSystem(IRenderSystem* inpRenderSystem)
        {
            mpRenderSystem = inpRenderSystem;
        }
		//----------------------------------------------------------------
		/// Destructor
		///
		//----------------------------------------------------------------
		IShaderManager::~IShaderManager()
		{
			
		}
	}
}
