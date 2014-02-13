//
//  SceneAnimationLoader.h
//  MoshiMonsters
//
//  Created by Andrew Glass on 19/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef MoshiMonsters_SceneAnimationProvider_h
#define MoshiMonsters_SceneAnimationProvider_h

#include <moFlo/Core/ResourceProvider.h>

namespace moFlo {
    namespace Core
    {
        class CSceneAnimationProvider : public moFlo::Core::IResourceProvider
        {
        public:
            DECLARE_NAMED_INTERFACE(CSceneAnimationProvider);
            
            bool IsA(InterfaceIDType inInterfaceID) const;
            
            bool CanCreateResourceOfKind(InterfaceIDType inInterfaceID) const;
            bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
            
            //---Filepath is relative to the resources directory - either the documents or the package
            bool CreateResourceFromFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource);
            
        private:
            bool LoadMoSceneAnim(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource);
        };
    }
}

#endif
