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

namespace ChilliSource
{
    namespace Core
    {
        CS_DEFINE_NAMEDTYPE(SceneAnimDescManager);
        
        bool SceneAnimDescManager::IsA(InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == SceneAnimDescManager::InterfaceID;
        }
        
        InterfaceIDType SceneAnimDescManager::GetResourceType() const
        {
            return SceneAnimation::InterfaceID;
        }
        
        InterfaceIDType SceneAnimDescManager::GetProviderType() const
        {
            return SceneAnimation::InterfaceID;
        }
        
        bool SceneAnimDescManager::ManagesResourceOfType(InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == SceneAnimation::InterfaceID;
        }
        
        ResourceSPtr SceneAnimDescManager::GetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath)
        {
            return GetSceneAnimationFromFile(ineStorageLocation, instrFilePath);
        }
        
        ResourceSPtr SceneAnimDescManager::AsyncGetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath)
        {
            return AsyncGetSceneAnimationFromFile(ineStorageLocation, instrFilePath);
        }
        
        SceneAnimationSPtr SceneAnimDescManager::GetSceneAnimationFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath)
        {
            MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
            
            if(pExistingResource == mMapFilenameToResource.end())
            {
                ResourceSPtr pResource(new SceneAnimation());
                for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
                {
                    if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, instrFilePath, pResource))
                    {
                        CS_LOG_DEBUG("Loading Scene Animation " + instrFilePath);
                        mMapFilenameToResource.emplace(instrFilePath, pResource);
                        
                        SceneAnimationSPtr pSceneAnim = std::static_pointer_cast<SceneAnimation>(pResource);
                        pSceneAnim->SetName(instrFilePath);
                        pSceneAnim->SetOwningResourceManager(this);
                        pSceneAnim->SetFilename(instrFilePath);
                        pSceneAnim->SetStorageLocation(ineStorageLocation);
                        
                        return pSceneAnim;
                    }
                }
            }
            else
            {
                return std::static_pointer_cast<SceneAnimation>(pExistingResource->second);
            }
            
            CS_LOG_ERROR("Cannot find resource for Scene Description with path " + instrFilePath);
            return nullptr;
        }
        
        SceneAnimationSPtr SceneAnimDescManager::AsyncGetSceneAnimationFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath)
        {
            MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
            
            if(pExistingResource == mMapFilenameToResource.end())
            {
                ResourceSPtr pResource(new SceneAnimation());
                for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
                {
                    
                    SceneAnimationSPtr pSceneAnim = std::static_pointer_cast<SceneAnimation>(pResource);
                    pSceneAnim->SetName(instrFilePath);
                    pSceneAnim->SetOwningResourceManager(this);
                    pSceneAnim->SetFilename(instrFilePath);
                    pSceneAnim->SetStorageLocation(ineStorageLocation);
                    
                    if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, instrFilePath, pResource))
                    {
                        CS_LOG_DEBUG("Loading Scene Description " + instrFilePath);
                        mMapFilenameToResource.emplace(instrFilePath, pResource);
                        return pSceneAnim;
                    }
                }
            }
            else
            {
                return std::static_pointer_cast<SceneAnimation>(pExistingResource->second);
            }
            
            CS_LOG_ERROR("Cannot find resource for Scene Description with path " + instrFilePath);
            return nullptr;
        }
    }
}