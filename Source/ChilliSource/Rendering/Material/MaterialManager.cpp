/*
 * File: MaterialManager.cpp
 * Date: 22/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Rendering/Material/MaterialManager.h>
#include <ChilliSource/Rendering/Material/Material.h>

#include <ChilliSource/Core/Resource/ResourceProviderOld.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(MaterialManager);
		
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool MaterialManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == MaterialManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType MaterialManager::GetResourceType() const
		{
			return Material::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool MaterialManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Material::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType MaterialManager::GetProviderType() const
		{
			return Material::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourceOldSPtr MaterialManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetMaterialFromFile(ineStorageLocation, instrFilePath);
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourceOldSPtr MaterialManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return AsyncGetMaterialFromFile(ineStorageLocation, instrFilePath);
		}
		//----------------------------------------------------------------
		/// Get Material From File
		///
		/// Loads the Material from resource and returns a handle to it.
		/// Alternately if the Material already exists it will return the
		/// handle without loading
		///
        /// @param The storage location to load from
		/// @param File name
		/// @param Material type
		/// @return A handle to the Material
		//----------------------------------------------------------------
		MaterialSPtr MaterialManager::GetMaterialFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath)
		{
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourceOldSPtr pResource(new Material());
				for(u32 nProvider = 0; nProvider < mResourceProviderOlds.size(); nProvider++) 
				{
					if(mResourceProviderOlds[nProvider]->CreateResourceFromFile(ineStorageLocation, inFilePath, pResource))
					{
						CS_LOG_VERBOSE("Loading Material " + inFilePath);
						mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));
						
						MaterialSPtr pMaterial = std::static_pointer_cast<Material>(pResource);
						pMaterial->SetName(inFilePath);
						pMaterial->SetOwningResourceManager(this);
						pMaterial->SetFilename(inFilePath);
						pMaterial->SetStorageLocation(ineStorageLocation);
						
						return pMaterial;
					}
				}
			} 
			else 
			{
				return std::static_pointer_cast<Material>(pExistingResource->second);
			}
			
			CS_LOG_FATAL("Cannot find resource for Material with path " + inFilePath);
			return Core::Application::Get()->GetDefaultMaterial();
		}
		//----------------------------------------------------------------
		/// Async Get Material From File
		///
		/// starts loading Material from resource and returns a handle to it.
		/// Alternately if the Material already exists it will return the
		/// handle without loading
		///
        /// @param The storage location to load from
		/// @param File name
		/// @param Material type
		/// @return A handle to the Material
		//----------------------------------------------------------------
		MaterialSPtr MaterialManager::AsyncGetMaterialFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath)
		{
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourceOldSPtr pResource(new Material());
				for(u32 nProvider = 0; nProvider < mResourceProviderOlds.size(); nProvider++) 
				{
					MaterialSPtr pMaterial = std::static_pointer_cast<Material>(pResource);
					pMaterial->SetName(inFilePath);
					pMaterial->SetFilename(inFilePath);
					pMaterial->SetStorageLocation(ineStorageLocation);
					pMaterial->SetOwningResourceManager(this);
					pMaterial->SetLoaded(false);
					
					if(mResourceProviderOlds[nProvider]->AsyncCreateResourceFromFile(ineStorageLocation, inFilePath, pResource))
					{
						CS_LOG_VERBOSE("Loading Material " + inFilePath);
						mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));

						return pMaterial;
					}
				}
			} 
			else 
			{
				return std::static_pointer_cast<Material>(pExistingResource->second);
			}
			
			CS_LOG_FATAL("Cannot find resource for Material with path " + inFilePath);
			return nullptr;
		}
	}
}
