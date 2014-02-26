//
//  SceneDescManager.h
//  MoshiMonsters
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef MoshiMonsters_SceneDescManager_h
#define MoshiMonsters_SceneDescManager_h

#include <ChilliSource/ChilliSource.h>
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
            
            bool IsA(InterfaceIDType inInterfaceID) const override;
            
            InterfaceIDType GetResourceType() const override;
            InterfaceIDType GetProviderType() const override;
            
            bool ManagesResourceOfType(InterfaceIDType inInterfaceID) const override;
            
            
            SceneDescPtr GetSceneDescFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
            SceneDescPtr AsyncGetSceneDescFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
            ResourcePtr GetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
            
            ResourcePtr AsyncGetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
            
        };
    }
}


#endif
