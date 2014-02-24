//
//  SceneDescManager.h
//  MoshiMonsters
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef MoshiMonsters_SceneDescManager_h
#define MoshiMonsters_SceneDescManager_h

#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Scene/SceneDescription.h>

namespace ChilliSource
{
    namespace Core
    {
        class CSceneDescManager : public IResourceManager
        {
        public:
            DECLARE_NAMED_INTERFACE(CSceneDescManager);
            
            bool IsA(InterfaceIDType inInterfaceID) const;
            
            InterfaceIDType GetResourceType() const;
            InterfaceIDType GetProviderType() const;
            
            bool ManagesResourceOfType(InterfaceIDType inInterfaceID) const;
            
            
            SceneDescPtr GetSceneDescFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
            SceneDescPtr AsyncGetSceneDescFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
            ResourcePtr GetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
            ResourcePtr AsyncGetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
        };
    }
}


#endif
