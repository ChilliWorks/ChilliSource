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
            
            virtual bool CullItem(CCameraComponent* inpCamera, IRenderComponent* inpItem) const = 0;

        private:
        };
        
        class CViewportCullPredicate : public ICullingPredicate
        {
        public:
            
            bool CullItem(CCameraComponent* inpCamera, IRenderComponent* inpItem) const;
        };
        
        class CFrustumCullPredicate : public ICullingPredicate
        {
        public:
            
            bool CullItem(CCameraComponent* inpCamera, IRenderComponent* inpItem) const;
        };
    }
}

#endif

