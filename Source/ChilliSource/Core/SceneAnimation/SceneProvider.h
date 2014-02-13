//
//  SceneProvider.h
//  moFlo
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef MoshiMonsters_SceneProvider_h
#define MoshiMonsters_SceneProvider_h

#include <moFlo/Core/ResourceProvider.h>

namespace moFlo 
{
    namespace Core
    {
        class CSceneProvider : public moFlo::Core::IResourceProvider
        {
        public:
            DECLARE_NAMED_INTERFACE(CSceneProvider);
            
            bool IsA(InterfaceIDType inInterface) const;
            
            bool CanCreateResourceOfKind(InterfaceIDType inInterfaceID) const;
            bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
            
            //---Filepath is relative to the resources directory - either the documents or the package
            bool CreateResourceFromFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource);
            bool AsyncCreateResourceFromFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource);
            
        private:
            bool LoadMoScene(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource);
            void LoadAsyncMoScene(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource);
            
        };
    }
}






#endif
