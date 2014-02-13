//
//  CullingPredicates.cpp
//  moFlow
//
//  Created by Scott Downie on 04/10/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/CullingPredicates.h>
#include <ChilliSource/Rendering/Components/CameraComponent.h>
#include <ChilliSource/Rendering/Components/RenderComponent.h>

namespace moFlo
{
    namespace Rendering
    {
        bool CViewportCullPredicate::CullItem(CCameraComponent* inpCamera, IRenderComponent* inpItem) const
        {
            return false;
        }
        
        bool CFrustumCullPredicate::CullItem(CCameraComponent* inpCamera, IRenderComponent* inpItem) const
        {
            return !inpCamera->GetFrustumPtr()->SphereCullTest(inpItem->GetBoundingSphere());
        }
    }
}

