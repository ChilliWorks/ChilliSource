//
//  SceneAnimDescManager.cpp
//  MoshiMonsters
//
//	
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Scene/SceneAnimDescManager.h>
#include <ChilliSource/Core/Scene/SceneAnimation.h>
#include <ChilliSource/Core/Scene/SceneAnimationProvider.h>

namespace moFlo
{
    namespace Core
    {
        DEFINE_NAMED_INTERFACE(CSceneAnimDescManager);
        
        bool CSceneAnimDescManager::IsA(InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == CSceneAnimDescManager::InterfaceID;
        }
        
        InterfaceIDType CSceneAnimDescManager::GetResourceType() const
        {
            return CSceneAnimation::InterfaceID;
        }
        
        InterfaceIDType CSceneAnimDescManager::GetProviderType() const
        {
            return CSceneAnimation::InterfaceID;
        }
        
        bool CSceneAnimDescManager::ManagesResourceOfType(InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == CSceneAnimation::InterfaceID;
        }
        
        ResourcePtr CSceneAnimDescManager::GetResourceFromFile(STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath)
        {
            return GetSceneAnimationFromFile(ineStorageLocation, instrFilePath);
        }
        
        ResourcePtr CSceneAnimDescManager::AsyncGetResourceFromFile(STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath)
        {
            return AsyncGetSceneAnimationFromFile(ineStorageLocation, instrFilePath);
        }
        
        SceneAnimationPtr CSceneAnimDescManager::GetSceneAnimationFromFile(STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath)
        {
            MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
            
            if(pExistingResource == mMapFilenameToResource.end())
            {
                ResourcePtr pResource(new CSceneAnimation());
                for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
                {
                    if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, instrFilePath, pResource))
                    {
                        DEBUG_LOG("Loading Scene Animation " + instrFilePath);
                        mMapFilenameToResource.emplace(instrFilePath, pResource);
                        
                        SceneAnimationPtr pSceneAnim = SHARED_PTR_CAST<CSceneAnimation>(pResource);
                        pSceneAnim->SetName(instrFilePath);
                        pSceneAnim->SetOwningResourceManager(this);
                        pSceneAnim->SetFilename(instrFilePath);
                        pSceneAnim->SetStorageLocation(ineStorageLocation);
                        pSceneAnim->SetLoaded(true);
                        
                        return pSceneAnim;
                    }
                }
            }
            else
                return SHARED_PTR_CAST<CSceneAnimation>(pExistingResource->second);
            
            ERROR_LOG("Cannot find resource for Scene Description with path " + instrFilePath);
            return SceneAnimationPtr();
        }
        
        SceneAnimationPtr CSceneAnimDescManager::AsyncGetSceneAnimationFromFile(STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath)
        {
            MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
            
            if(pExistingResource == mMapFilenameToResource.end())
            {
                ResourcePtr pResource(new CSceneAnimation());
                for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
                {
                    
                    SceneAnimationPtr pSceneAnim = SHARED_PTR_CAST<CSceneAnimation>(pResource);
                    pSceneAnim->SetName(instrFilePath);
                    pSceneAnim->SetOwningResourceManager(this);
                    pSceneAnim->SetFilename(instrFilePath);
                    pSceneAnim->SetStorageLocation(ineStorageLocation);
                    pSceneAnim->SetLoaded(false);
                    
                    if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, instrFilePath, pResource))
                    {
                        DEBUG_LOG("Loading Scene Description " + instrFilePath);
                        mMapFilenameToResource.emplace(instrFilePath, pResource);
                        return pSceneAnim;
                    }
                }
            }
            else
                return SHARED_PTR_CAST<CSceneAnimation>(pExistingResource->second);
            
            ERROR_LOG("Cannot find resource for Scene Description with path " + instrFilePath);
            return SceneAnimationPtr();
        }
    }
}