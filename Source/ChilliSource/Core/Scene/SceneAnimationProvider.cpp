//
//  SceneAnimationProvider.cpp
//  Chilli Source
//
//  Created by A Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Scene/SceneAnimationProvider.h>
#include <ChilliSource/Core/Scene/SceneAnimation.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            const std::string k_moSceneAnimExtension("moscene.animations");
        }
        
        CS_DEFINE_NAMEDTYPE(SceneAnimationProvider);
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneAnimationProvider::IsA(InterfaceIDType in_interfaceId) const
        {
            return (in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == SceneAnimationProvider::InterfaceID);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneAnimationProvider::CanCreateResourceOfKind(InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == SceneAnimation::InterfaceID;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneAnimationProvider::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
        {
            return (in_extension == k_moSceneAnimExtension);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneAnimationProvider::CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filepath, ResourceSPtr& out_resource)
        {
            SceneAnimation * sceneAnim = reinterpret_cast<SceneAnimation*>(out_resource.get());
            
            SceneAnimationDesc animDesc;
            
            if(!animDesc.LoadFromBinaryFile(in_storageLocation, in_filepath))
                return false;
            
            sceneAnim->SetSceneAnimDesc(animDesc);
            
            return true;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool SceneAnimationProvider::AsyncCreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceSPtr& out_resource)
        {
            CS_LOG_WARNING("Async Loading not implemented by the scene animation provider.");
            
            return false;
        }
    }
}
