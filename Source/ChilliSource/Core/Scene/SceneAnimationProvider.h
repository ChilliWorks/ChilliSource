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
        class CSceneAnimationProvider : public ChilliSource::Core::IResourceProvider
        {
        public:
            DECLARE_NAMED_INTERFACE(CSceneAnimationProvider);
            
            bool IsA(InterfaceIDType inInterfaceID) const;
            
            bool CanCreateResourceOfKind(InterfaceIDType inInterfaceID) const;
            bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
            
            //---Filepath is relative to the resources directory - either the documents or the package
            bool CreateResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource);
            
        private:
            bool LoadMoSceneAnim(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource);
        };
    }
}

#endif
