/*
 *  CBulletBodyHolder.cpp
 *  moFlow
 *
 *  Created by Scott Downie on 09/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Physics3D/BulletPhysics/BulletBodyHolder.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

namespace moFlo
{
    namespace Physics3D
    {
        //---------------------------------------------------
        /// Constructor
        ///
        /// Default
        //---------------------------------------------------
        CBulletBodyHolder::CBulletBodyHolder()
        {
        }
        //-------------------------------------------------
        /// Destroy
        ///
        /// Delete the objects allocated by bullet
        //-------------------------------------------------
        void CBulletBodyHolder::Destroy(bool inbDestroyShape)
        {
            SAFE_DELETE(pRigidBody);
            
            if (inbDestroyShape == true)
                SAFE_DELETE(pCollisionShape);
        }
    }
}