//
//  SceneDescManager.cpp
//  MoshiMonsters
//
//	
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Scene/SceneDescManager.h>
#include <ChilliSource/Core/Scene/SceneDescription.h>
#include <ChilliSource/Core/Scene/SceneProvider.h>

namespace ChilliSource
{
    namespace Core
    {
        DEFINE_NAMED_INTERFACE(CSceneDescManager);
        
        bool CSceneDescManager::IsA(InterfaceIDType inInterfaceID) const
        {
            return CSceneDescManager::InterfaceID == inInterfaceID;
        }
        
        InterfaceIDType CSceneDescManager::GetResourceType() const
        {
            return CSceneDesc::InterfaceID;
        }
        
        InterfaceIDType CSceneDescManager::GetProviderType() const
        {
            return CSceneDesc::InterfaceID;
        }
        
        bool CSceneDescManager::ManagesResourceOfType(InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == CSceneDesc::InterfaceID;
        }
        
        ResourcePtr CSceneDescManager::GetResourceFromFile(moFlo::Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
        {
            return GetSceneDescFromFile(ineStorageLocation, instrFilePath);
        }
        
        ResourcePtr CSceneDescManager::AsyncGetResourceFromFile(moFlo::Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
        {
            return AsyncGetSceneDescFromFile(ineStorageLocation, instrFilePath);
        }
        
        SceneDescPtr CSceneDescManager::GetSceneDescFromFile(moFlo::Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
        {
            MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
            
            if(pExistingResource == mMapFilenameToResource.end())
            {
                ResourcePtr pResource(new CSceneDesc());
                for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
                {
                    if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, instrFilePath, pResource))
                    {
                        DEBUG_LOG("Loading Scene Description " + instrFilePath);
                        mMapFilenameToResource.emplace(instrFilePath, pResource);
                        
                        SceneDescPtr pSceneDesc = SHARED_PTR_CAST<CSceneDesc>(pResource);
                        pSceneDesc->SetName(instrFilePath);
                        pSceneDesc->SetOwningResourceManager(this);
                        pSceneDesc->SetFilename(instrFilePath);
                        pSceneDesc->SetStorageLocation(ineStorageLocation);
                        pSceneDesc->SetLoaded(true);
                        
                        return pSceneDesc;
                    }
                }
            }
            else
                return SHARED_PTR_CAST<CSceneDesc>(pExistingResource->second);
            
            ERROR_LOG("Cannot find resource for Scene Description with path " + instrFilePath);
            return SceneDescPtr();
        }
        
        SceneDescPtr CSceneDescManager::AsyncGetSceneDescFromFile(moFlo::Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
        {
            MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
            
            if(pExistingResource == mMapFilenameToResource.end())
            {
                ResourcePtr pResource(new CSceneDesc());
                for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
                {
                    SceneDescPtr pSceneDesc = SHARED_PTR_CAST<CSceneDesc>(pResource);
                    pSceneDesc->SetName(instrFilePath);
                    pSceneDesc->SetOwningResourceManager(this);
                    pSceneDesc->SetFilename(instrFilePath);
                    pSceneDesc->SetStorageLocation(ineStorageLocation);
                    pSceneDesc->SetLoaded(false);

                    if(mResourceProviders[nProvider]->AsyncCreateResourceFromFile(ineStorageLocation, instrFilePath, pResource))
                    {
                        DEBUG_LOG("Loading Scene Description " + instrFilePath);
                        mMapFilenameToResource.emplace(instrFilePath, pResource);
                                                
                        return pSceneDesc;
                    }
                }
            }
            else
                return SHARED_PTR_CAST<CSceneDesc>(pExistingResource->second);
            
            ERROR_LOG("Cannot find resource for Scene Description with path " + instrFilePath);
            return SceneDescPtr();
        }
        
    }
}