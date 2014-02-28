//
//  SceneProvider.cpp
//  MoshiMonsters
//
//	
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Scene/SceneProvider.h>
#include <ChilliSource/Core/Scene/SceneDescription.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Threading/Task.h>

namespace ChilliSource
{
    namespace Core
    {
        CS_DEFINE_NAMEDTYPE(SceneProvider);
        
        const std::string kMoSceneExtension("moscene");
        
        bool SceneProvider::IsA(InterfaceIDType inInterface) const
        {
            return SceneProvider::InterfaceID == inInterface||(inInterface) == ResourceProvider::InterfaceID;
        }
        
        
        bool SceneProvider::CanCreateResourceOfKind(InterfaceIDType  inInterfaceID) const
        {
            return inInterfaceID == ChilliSource::Core::SceneDesc::InterfaceID;
        }
        
        bool SceneProvider::CanCreateResourceFromFileWithExtension(const std::string &inExtension) const
        {
            return inExtension == kMoSceneExtension;
        }
        
        bool SceneProvider::CreateResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource)
        {
            return LoadMoScene(ineStorageLocation, inFilePath, outpResource);
        }
        
        bool SceneProvider::AsyncCreateResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource)
        {
            Task<StorageLocation, const std::string&, ResourceSPtr&> ReadFileTask(this, &SceneProvider::LoadAsyncMoScene, ineStorageLocation, inFilePath, outpResource);
            TaskScheduler::ScheduleTask(ReadFileTask);
            return true;
        }
        
        bool SceneProvider::LoadMoScene(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource)
        {
            SceneDesc * pSceneDesc = reinterpret_cast<SceneDesc*>(outpResource.get());
            
            return pSceneDesc->LoadFromFile(ineStorageLocation, inFilePath);
        }
        
        void SceneProvider::LoadAsyncMoScene(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource)
        {
            SceneDesc * pSceneDesc = reinterpret_cast<SceneDesc*>(outpResource.get());
            pSceneDesc->LoadFromFile(ineStorageLocation, inFilePath);
            outpResource->SetLoaded(true);
        }
    }
}
