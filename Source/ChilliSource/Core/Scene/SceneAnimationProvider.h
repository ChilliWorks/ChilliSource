//
//  SceneAnimationLoader.h
//  MoshiMonsters
//
//  Created by Andrew Glass on 19/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef MoshiMonsters_SceneAnimationProvider_h
#define MoshiMonsters_SceneAnimationProvider_h

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource {
    namespace Core
    {
        class SceneAnimationProvider : public ChilliSource::Core::ResourceProvider
        {
        public:
            DECLARE_NAMED_INTERFACE(SceneAnimationProvider);
            
            bool IsA(InterfaceIDType inInterfaceID) const override;
            
            bool CanCreateResourceOfKind(InterfaceIDType inInterfaceID) const override;
            bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const override;
            
            //---Filepath is relative to the resources directory - either the documents or the package
            bool CreateResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource) override;
            
        private:
            bool LoadMoSceneAnim(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource);
        };
    }
}

#endif
