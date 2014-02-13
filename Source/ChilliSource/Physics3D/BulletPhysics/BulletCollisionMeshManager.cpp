//
//  BulletCollisionMeshManager.cpp
//  moFlow
//
//  Created by Ian Copland on 29/05/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#include <moFlo/Physics3D/BulletPhysics/BulletCollisionMeshManager.h>
#include <moFlo/Physics3D/BulletPhysics/BulletCollisionMesh.h>
#include <moFlo/Core/ResourceProvider.h>
#include <moFlo/Core/Application.h>

namespace moFlo
{
    namespace Physics3D
    {
        DEFINE_NAMED_INTERFACE(CBulletCollisionMeshManager);
        
        //----------------------------------------------------------------
        /// Constructor
        //----------------------------------------------------------------
        CBulletCollisionMeshManager::CBulletCollisionMeshManager()
        {
        }
        //----------------------------------------------------------------
        /// Is A
        //----------------------------------------------------------------
        bool CBulletCollisionMeshManager::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return (inInterfaceID == CBulletCollisionMeshManager::InterfaceID);
        }
        //----------------------------------------------------------------
        /// Get Resource Type
        //----------------------------------------------------------------
        Core::InterfaceIDType CBulletCollisionMeshManager::GetResourceType() const
        {
            return CBulletCollisionMesh::InterfaceID;
        }
        //----------------------------------------------------------------
        /// Get Provider Type
        //----------------------------------------------------------------
        Core::InterfaceIDType CBulletCollisionMeshManager::GetProviderType() const
        {
            return CBulletCollisionMesh::InterfaceID;
        } 
        //----------------------------------------------------------------
        /// Manages Resource Of Type
        //----------------------------------------------------------------
        bool CBulletCollisionMeshManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
        {
            return (inInterfaceID == CBulletCollisionMesh::InterfaceID);
        }
        //-----------------------------------------------------------------
        /// Get Resource From File
        //-----------------------------------------------------------------
        Core::ResourcePtr CBulletCollisionMeshManager::GetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath)
        {
            return GetCollisionMeshFromFile(ineStorageLocation, instrFilePath);
        }
        //-----------------------------------------------------------------
        /// Async Get Resource From File
        //-----------------------------------------------------------------
        Core::ResourcePtr CBulletCollisionMeshManager::AsyncGetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath)
        {
            return AsyncGetCollisionMeshFromFile(ineStorageLocation, instrFilePath);
        }
        //----------------------------------------------------------------
        /// Get Collision Mesh From File
        //----------------------------------------------------------------
        BulletCollisionMeshPtr CBulletCollisionMeshManager::GetCollisionMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath)
        {
            MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CBulletCollisionMesh());
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					//Check if the resource provider can load this extension
					std::string strName, strExt;
					Core::CStringUtils::SplitBaseFilename(instrFilePath, strName, strExt);
					
					if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
					{
						if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, instrFilePath, pResource)) 
						{
							//Add it to the cache
							DEBUG_LOG("Loading bullet collision mesh " + instrFilePath);
							mMapFilenameToResource.insert(std::make_pair(instrFilePath, pResource));
							pResource->SetName(instrFilePath);
							pResource->SetFilename(instrFilePath);
							pResource->SetStorageLocation(ineStorageLocation);
							pResource->SetOwningResourceManager(static_cast<Core::IResourceManager*>(this));
							pResource->SetLoaded(true);
							
							return SHARED_PTR_CAST<CBulletCollisionMesh>(pResource);
						}
					}
				}
			} 
			else 
			{
				return SHARED_PTR_CAST<CBulletCollisionMesh>(pExistingResource->second);
			}
			
			//Resource not found
			ERROR_LOG("Cannot find resource for bullet collision mesh with path " + instrFilePath);
			return BulletCollisionMeshPtr();
        }
        //----------------------------------------------------------------
        /// Async Get Collision Mesh From File
        //----------------------------------------------------------------
        BulletCollisionMeshPtr CBulletCollisionMeshManager::AsyncGetCollisionMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath)
        {
            MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CBulletCollisionMesh());
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					//Check if the resource provider can load this extension
					std::string strName, strExt;
					Core::CStringUtils::SplitBaseFilename(instrFilePath, strName, strExt);
					
					if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
					{
						
						DEBUG_LOG("Loading bullet collision mesh " + instrFilePath);
						
						pResource->SetName(instrFilePath);
						pResource->SetFilename(instrFilePath);
						pResource->SetStorageLocation(ineStorageLocation);
						pResource->SetOwningResourceManager(this);
						pResource->SetLoaded(false);
						
						if(mResourceProviders[nProvider]->AsyncCreateResourceFromFile(ineStorageLocation, instrFilePath, pResource)) 
						{
							//Add it to the cache
							mMapFilenameToResource.insert(std::make_pair(instrFilePath, pResource));
							return SHARED_PTR_CAST<CBulletCollisionMesh>(pResource);
						}
					}
				}
			} 
			else 
			{
				return SHARED_PTR_CAST<CBulletCollisionMesh>(pExistingResource->second);
			}
			
			//Resource not found
			ERROR_LOG("Cannot find resource for bullet collision mesh with path " + instrFilePath);
			return BulletCollisionMeshPtr();
        }
        //----------------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------------
        CBulletCollisionMeshManager::~CBulletCollisionMeshManager()
        {
            
        }
    }
}
