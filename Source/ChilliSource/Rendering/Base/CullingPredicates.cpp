//
//  CullingPredicates.cpp
//  moFlow
//
//  Created by Scott Downie on 04/10/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Base/CullingPredicates.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Base/RenderComponent.h>

namespace ChilliSource
{
    namespace Rendering
    {
        bool ViewportCullPredicate::CullItem(CameraComponent* inpCamera, RenderComponent* inpItem) const
        {
            return false;
        }
        
        bool FrustumCullPredicate::CullItem(CameraComponent* inpCamera, RenderComponent* inpItem) const
        {
            return !inpCamera->GetFrustumPtr()->SphereCullTest(inpItem->GetBoundingSphere());
        }
    }
}

