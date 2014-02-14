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

#include <ChilliSource/Rendering/MaterialManager.h>
#include <ChilliSource/Rendering/Material/Material.h>

#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Main/Application.h>

namespace moFlo
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(IMaterialManager);
		
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool IMaterialManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IMaterialManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType IMaterialManager::GetResourceType() const
		{
			return CMaterial::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool IMaterialManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CMaterial::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType IMaterialManager::GetProviderType() const
		{
			return CMaterial::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr IMaterialManager::GetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath)
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
		Core::ResourcePtr IMaterialManager::AsyncGetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath)
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
		MaterialPtr IMaterialManager::GetMaterialFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CMaterial());
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, inFilePath, pResource))
					{
						DEBUG_LOG("Loading Material " + inFilePath);
						mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));
						
						MaterialPtr pMaterial = SHARED_PTR_CAST<CMaterial>(pResource);
						pMaterial->SetName(inFilePath);
						pMaterial->SetOwningResourceManager(this);
						pMaterial->SetFilename(inFilePath);
						pMaterial->SetStorageLocation(ineStorageLocation);
						pMaterial->SetLoaded(true);
						
						return pMaterial;
					}
				}
			} 
			else 
			{
				return SHARED_PTR_CAST<CMaterial>(pExistingResource->second);
			}
			
			ERROR_LOG("Cannot find resource for Material with path " + inFilePath);
			return Core::CApplication::GetDefaultMaterial();
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
		MaterialPtr IMaterialManager::AsyncGetMaterialFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CMaterial());
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					MaterialPtr pMaterial = SHARED_PTR_CAST<CMaterial>(pResource);
					pMaterial->SetName(inFilePath);
					pMaterial->SetFilename(inFilePath);
					pMaterial->SetStorageLocation(ineStorageLocation);
					pMaterial->SetOwningResourceManager(this);
					pMaterial->SetLoaded(false);
					
					if(mResourceProviders[nProvider]->AsyncCreateResourceFromFile(ineStorageLocation, inFilePath, pResource))
					{
						DEBUG_LOG("Loading Material " + inFilePath);
						mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));

						return pMaterial;
					}
				}
			} 
			else 
			{
				return SHARED_PTR_CAST<CMaterial>(pExistingResource->second);
			}
			
			ERROR_LOG("Cannot find resource for Material with path " + inFilePath);
			return MaterialPtr();
		}
	}
}
