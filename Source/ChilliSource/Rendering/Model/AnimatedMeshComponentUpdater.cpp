//
//  AnimatedMeshComponentUpdater.cpp
//  moFlow
//
//  Created by Ian Copland on 22/04/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/Model/AnimatedMeshComponentUpdater.h>
#include <ChilliSource/Rendering/Model/AnimatedMeshComponent.h>

namespace ChilliSource
{
    namespace Rendering
    {
        CS_DEFINE_NAMEDTYPE(AnimatedMeshComponentUpdater);
        //-------------------------------------------------------
        //-------------------------------------------------------
        AnimatedMeshComponentUpdaterUPtr AnimatedMeshComponentUpdater::Create()
        {
            return AnimatedMeshComponentUpdaterUPtr(new AnimatedMeshComponentUpdater());
        }
        //------------------------------------------------
        /// Is A
        //------------------------------------------------
        bool AnimatedMeshComponentUpdater::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return (AnimatedMeshComponentUpdater::InterfaceID == inInterfaceID || Core::IUpdateable::InterfaceID == inInterfaceID);
        }
        //------------------------------------------------
        /// Add Component
        //------------------------------------------------
        void AnimatedMeshComponentUpdater::AddComponent(AnimatedMeshComponent* inpAnimatedMesh)
        {
            mapComponentsToUpdate.push_back(inpAnimatedMesh);
        }
        //------------------------------------------------
        /// Update
        //------------------------------------------------
        void AnimatedMeshComponentUpdater::Update(f32 infDeltaTime)
        {
            for (std::vector<AnimatedMeshComponent*>::const_iterator it = mapComponentsToUpdate.begin(); it != mapComponentsToUpdate.end(); ++it)
            {
                (*it)->Update(infDeltaTime);
            }
        }
        //------------------------------------------------
        /// Remove Component
        //------------------------------------------------
        void AnimatedMeshComponentUpdater::RemoveComponent(AnimatedMeshComponent* inpAnimatedMesh)
        {
            std::vector<AnimatedMeshComponent*>::iterator it;
            for (it = mapComponentsToUpdate.begin(); it != mapComponentsToUpdate.end(); ++it)
            {
                if ((*it) == inpAnimatedMesh)
                {
                    break;
                }
            }
            
            if (it != mapComponentsToUpdate.end())
            {
                mapComponentsToUpdate.erase(it);
            }
        }
    }
}