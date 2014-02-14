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
#include <ChilliSource/Core/Main/Application.h>

namespace moFlo{
    namespace Core
    {
        DEFINE_NAMED_INTERFACE(CSceneAnimationProvider);
        
        const std::string kMoSceneAnimExtension("moscene.animations");
        
        bool CSceneAnimationProvider::IsA(InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == CSceneAnimationProvider::InterfaceID ||inInterfaceID == IResourceProvider::InterfaceID;
        }
        
        
        bool CSceneAnimationProvider::CanCreateResourceOfKind(InterfaceIDType  inInterfaceID) const
        {
            return inInterfaceID == moFlo::Core::CSceneAnimation::InterfaceID;
        }
        
        bool CSceneAnimationProvider::CanCreateResourceFromFileWithExtension(const std::string &inExtension) const
        {
            return inExtension == kMoSceneAnimExtension;
        }
        
        bool CSceneAnimationProvider::CreateResourceFromFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource)
        {
            return LoadMoSceneAnim(ineStorageLocation, inFilePath, outpResource);
        }
        
        bool CSceneAnimationProvider::LoadMoSceneAnim(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource)
        {
            CSceneAnimation * pSceneAnim = reinterpret_cast<CSceneAnimation*>(outpResource.get());
            
            CSceneAnimationDesc sAnimDesc;
            
            if(!sAnimDesc.LoadFromBinaryFile(ineStorageLocation, inFilePath))
                return false;
            
            pSceneAnim->SetSceneAnimDesc(sAnimDesc);
            
            return true;
        }
        
    }
}
