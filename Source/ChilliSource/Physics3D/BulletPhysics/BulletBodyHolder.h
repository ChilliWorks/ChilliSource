/*
 *  CBulletBodyHolder.h
 *  moFlow
 *
 *  Created by Scott Downie on 09/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Physics3D/BulletPhysics/ForwardDeclarations.h>

#ifndef _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETBODYHOLDER_H_
#define _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETBODYHOLDER_H_

namespace moFlo
{
    namespace Physics3D
    {
        class CBulletBodyHolder
        {
        public:
            CBulletBodyHolder();
            
            //-------------------------------------------------
            /// Destroy
            ///
            /// Delete the objects allocated by bullet
            //-------------------------------------------------
            void Destroy(bool inbDestroyShape = true);
            
        public:	
            btCollisionShape* pCollisionShape;
            btRigidBody* pRigidBody;
        };
    }
}
#endif