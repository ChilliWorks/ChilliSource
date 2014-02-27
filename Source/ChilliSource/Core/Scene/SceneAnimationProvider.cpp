//
//  SceneAnimationProvider.cpp
//  moFlo
//
//	
//
//  Created by Andrew Glass on 20/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Scene/SceneAnimationProvider.h>
#include <ChilliSource/Core/Scene/SceneAnimation.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource{
    namespace Core
    {
        DEFINE_NAMED_INTERFACE(SceneAnimationProvider);
        
        const std::string kMoSceneAnimExtension("moscene.animations");
        
        bool SceneAnimationProvider::IsA(InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == SceneAnimationProvider::InterfaceID ||inInterfaceID == ResourceProvider::InterfaceID;
        }
        
        
        bool SceneAnimationProvider::CanCreateResourceOfKind(InterfaceIDType  inInterfaceID) const
        {
            return inInterfaceID == ChilliSource::Core::SceneAnimation::InterfaceID;
        }
        
        bool SceneAnimationProvider::CanCreateResourceFromFileWithExtension(const std::string &inExtension) const
        {
            return inExtension == kMoSceneAnimExtension;
        }
        
        bool SceneAnimationProvider::CreateResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource)
        {
            return LoadMoSceneAnim(ineStorageLocation, inFilePath, outpResource);
        }
        
        bool SceneAnimationProvider::LoadMoSceneAnim(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourceSPtr& outpResource)
        {
            SceneAnimation * pSceneAnim = reinterpret_cast<SceneAnimation*>(outpResource.get());
            
            SceneAnimationDesc sAnimDesc;
            
            if(!sAnimDesc.LoadFromBinaryFile(ineStorageLocation, inFilePath))
                return false;
            
            pSceneAnim->SetSceneAnimDesc(sAnimDesc);
            
            return true;
        }
        
    }
}
