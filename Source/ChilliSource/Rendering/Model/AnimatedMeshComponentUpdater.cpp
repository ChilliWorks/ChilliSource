//
//  AnimatedMeshComponentUpdater.cpp
//  moFlow
//
//  Created by Ian Copland on 22/04/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/ComponentUpdaters/AnimatedMeshComponentUpdater.h>
#include <ChilliSource/Rendering/Components/AnimatedMeshComponent.h>

namespace moFlo
{
    namespace Rendering
    {
        DEFINE_NAMED_INTERFACE(CAnimatedMeshComponentUpdater);
        //------------------------------------------------
        /// Constructor
        //------------------------------------------------
        CAnimatedMeshComponentUpdater::CAnimatedMeshComponentUpdater()
        {
        }
        //------------------------------------------------
        /// Is A
        //------------------------------------------------
        bool CAnimatedMeshComponentUpdater::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return (CAnimatedMeshComponentUpdater::InterfaceID == inInterfaceID || moCore::IUpdateable::InterfaceID == inInterfaceID);
        }
        //------------------------------------------------
        /// Add Component
        //------------------------------------------------
        void CAnimatedMeshComponentUpdater::AddComponent(CAnimatedMeshComponent* inpAnimatedMesh)
        {
            mapComponentsToUpdate.push_back(inpAnimatedMesh);
        }
        //------------------------------------------------
        /// Update
        //------------------------------------------------
        void CAnimatedMeshComponentUpdater::Update(f32 infDeltaTime)
        {
            for (DYNAMIC_ARRAY<CAnimatedMeshComponent*>::const_iterator it = mapComponentsToUpdate.begin(); it != mapComponentsToUpdate.end(); ++it)
            {
                (*it)->Update(infDeltaTime);
            }
        }
        //------------------------------------------------
        /// Remove Component
        //------------------------------------------------
        void CAnimatedMeshComponentUpdater::RemoveComponent(CAnimatedMeshComponent* inpAnimatedMesh)
        {
            DYNAMIC_ARRAY<CAnimatedMeshComponent*>::iterator it;
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