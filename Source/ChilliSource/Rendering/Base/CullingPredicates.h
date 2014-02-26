//
//  CullingPredicates.h
//  MoFlow
//
//  Created by Scott Downie on 04/10/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_RENDERING_CULLING_PREDICATES_H_
#define _MOFLOW_RENDERING_CULLING_PREDICATES_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Rendering
    {
        class ICullingPredicate
        {
        public:
            
            virtual ~ICullingPredicate(){}
            
            virtual bool CullItem(CameraComponent* inpCamera, RenderComponent* inpItem) const = 0;

        private:
        };
        
        class ViewportCullPredicate : public ICullingPredicate
        {
        public:
            
            bool CullItem(CameraComponent* inpCamera, RenderComponent* inpItem) const;
        };
        
        class FrustumCullPredicate : public ICullingPredicate
        {
        public:
            
            bool CullItem(CameraComponent* inpCamera, RenderComponent* inpItem) const;
        };
    }
}

#endif

