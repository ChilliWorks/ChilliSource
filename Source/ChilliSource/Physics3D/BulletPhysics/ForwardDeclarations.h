/*
 *  ForwardDeclarations.h
 *  moFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_PHYSICS3D_BULLETPHYSICS_FORWARDDECLARATIONS_H_
#define _MOFLOW_PHYSICS3D_BULLETPHYSICS_FORWARDDECLARATIONS_H_

namespace moFlo
{
    namespace Physics3D
    {
        //==================================================
        /// MoFlow Bullet Forward Declarations
        //==================================================
        class CBulletPhysicsSystem;
        class CBulletComponentFactory;
        class CBulletPhysicsComponent;
        class CBulletBodyHolder;
        class CBulletCollisionMesh;
        class CBulletCollisionMeshLoader;
        class CBulletCollisionMeshManager;
        //==================================================
        /// MoFlow Bullet Pointer Definitions
        //==================================================
        typedef SHARED_PTR<CBulletPhysicsSystem> BulletPhysicsSystemPtr;
        typedef SHARED_PTR<CBulletComponentFactory> BulletComponenetFactoryPtr;
        typedef SHARED_PTR<CBulletPhysicsComponent> BulletPhysicsComponentPtr;
        typedef SHARED_PTR<CBulletBodyHolder> BulletBodyHolderPtr;
        typedef SHARED_PTR<CBulletCollisionMesh> BulletCollisionMeshPtr;
        typedef SHARED_PTR<CBulletCollisionMeshLoader> BulletCollisionMeshLoaderPtr;
        typedef SHARED_PTR<CBulletCollisionMeshManager> BulletCollisionMeshManagerPtr;
        
    }
}
//==================================================
/// Bullet Forward Declarations
//==================================================
class btDiscreteDynamicsWorld;
class btCollisionShape;
class btCompoundShape;
class btMotionState;
class btRigidBody;
class btTransform;

#endif