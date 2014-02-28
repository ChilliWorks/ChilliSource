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
        class SceneDescManager : public ResourceManager
        {
        public:
            CS_DECLARE_NAMEDTYPE(SceneDescManager);
            
            bool IsA(InterfaceIDType inInterfaceID) const override;
            
            InterfaceIDType GetResourceType() const override;
            InterfaceIDType GetProviderType() const override;
            
            bool ManagesResourceOfType(InterfaceIDType inInterfaceID) const override;
            
            
            SceneDescSPtr GetSceneDescFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
            SceneDescSPtr AsyncGetSceneDescFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
            ResourceSPtr GetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
            
            ResourceSPtr AsyncGetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
            
        };
    }
}


#endif
