//
//  SceneAnimDescManager.h
//  MoshiMonsters
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef MoshiMonsters_SceneAnimDescManager_h
#define MoshiMonsters_SceneAnimDescManager_h

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Scene/SceneAnimation.h>

namespace ChilliSource
{
    namespace Core
    {
        class SceneAnimDescManager : public ResourceManager
        {
        public:
            CS_DECLARE_NAMEDTYPE(SceneAnimDescManager);
            
            bool IsA(InterfaceIDType inInterfaceID) const override;
            
            InterfaceIDType GetResourceType() const override;
            InterfaceIDType GetProviderType() const override;
            
            bool ManagesResourceOfType(InterfaceIDType inInterfaceID) const override;
            
            SceneAnimationSPtr GetSceneAnimationFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
            SceneAnimationSPtr AsyncGetSceneAnimationFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath);
            
            ResourceSPtr GetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
            
            ResourceSPtr AsyncGetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
            
        };
    }
}

#endif
