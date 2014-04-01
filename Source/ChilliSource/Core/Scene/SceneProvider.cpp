//
//  SceneProvider.cpp
//  Chilli Source
//
//  Created by A Glass on 20/04/2012.
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
        namespace
        {
            const std::string k_sceneExtension("moscene");
        }
        
        CS_DEFINE_NAMEDTYPE(SceneProvider);
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneProvider::IsA(InterfaceIDType in_interfaceId) const
        {
            return (ResourceProvider::InterfaceID == in_interfaceId || SceneProvider::InterfaceID == in_interfaceId);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneProvider::CanCreateResourceOfKind(InterfaceIDType in_interfaceId) const
        {
            return (in_interfaceId == ChilliSource::Core::SceneDesc::InterfaceID);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneProvider::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
        {
            return (in_extension == k_sceneExtension);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneProvider::CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filepath, ResourceSPtr& out_resource)
        {
            SceneDesc* sceneDesc = reinterpret_cast<SceneDesc*>(out_resource.get());
            bool success = sceneDesc->LoadFromFile(in_storageLocation, in_filepath);
            out_resource->SetLoaded(success);
            return success;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneProvider::AsyncCreateResourceFromFile(StorageLocation in_storageLocation, const std::string & in_filepath, ResourceSPtr& out_resource)
        {
            Task<StorageLocation, const std::string&, ResourceSPtr&> ReadFileTask(this, &SceneProvider::LoadAsyncScene, in_storageLocation, in_filepath, out_resource);
            TaskScheduler::ScheduleTask(ReadFileTask);
            return true;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void SceneProvider::LoadAsyncScene(StorageLocation in_storageLocation, const std::string & in_filepath, ResourceSPtr& out_resource)
        {
            SceneDesc * sceneDesc = reinterpret_cast<SceneDesc*>(out_resource.get());
            sceneDesc->LoadFromFile(in_storageLocation, in_filepath);
            out_resource->SetLoaded(true);
        }
    }
}
