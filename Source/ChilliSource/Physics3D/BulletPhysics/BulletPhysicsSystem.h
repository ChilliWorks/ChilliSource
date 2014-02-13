/*
 *  BulletPhysicsSystem.h
 *  moFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETPHYSICSSYSTEM_H_
#define _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETPHYSICSSYSTEM_H_


#include <ChilliSource/Physics3D/3DPhysicsSystem.h>
#include <ChilliSource/Physics3D/3DPhysicsComponent.h>

#include <ChilliSource/Physics3D/BulletPhysics/ForwardDeclarations.h>
#include <btBulletDynamicsCommon.h>

#include <ChilliSource/Physics3D/BulletPhysics/BulletDebugDraw.h>
#include <ChilliSource/Physics3D/BulletPhysics/BulletCollisionMeshManager.h>

namespace moFlo
{
    namespace Physics3D
    {
        class CBulletPhysicsSystem : public Physics3D::I3DPhysicsSystem
        {
        public:
            CBulletPhysicsSystem(u32 inudwDefaultPoolSizeInKB = 1024);
            ~CBulletPhysicsSystem();
            
            //----------------------------------------------------------
            /// Is A
            ///
            /// @return Whether we are of the given type
            //----------------------------------------------------------
            bool IsA(Core::InterfaceIDType InterfaceID) const;
            //----------------------------------------------------------
            /// Update
            ///
            /// @param Time since last update
            //----------------------------------------------------------
            void Update(f32 infDT);
            //----------------------------------------------------------
            /// Get Component Factory
            ///
            /// @return Factory that this system owns
            //----------------------------------------------------------
            Core::IComponentFactory& GetComponentFactory(u32 index);
            //----------------------------------------------------------
            /// Get Component Factory Pointer
            ///
            /// @return Factory that this system owns
            //----------------------------------------------------------
            Core::IComponentFactory* GetComponentFactoryPtr(u32 index);
            //----------------------------------------------------------
            /// Get Number of Component Factories
            ///
            /// @return Number of factories this system has
            //----------------------------------------------------------
            u32 GetNumComponentFactories() const;
            //-----------------------------------------------------------
            /// Set Gravity
            ///
            /// @param New gravity velocity vector
            //-----------------------------------------------------------
            void SetGravity(const Core::CVector3& invGravity);
            //-----------------------------------------------------------
            /// Get Gravity
            ///
            /// @return New gravity velocity vector
            //-----------------------------------------------------------
            const Core::CVector3 & GetGravity() const;
            //-----------------------------------------------------------
            /// Get Physics World Pointer
            ///
            /// @return Bullet dynamics world
            //-----------------------------------------------------------
            btDiscreteDynamicsWorld* GetPhysicsWorldPtr() const;
            //-----------------------------------------------------------
            /// Get Broadphase Pointer
            ///
            /// @return Bullet broadphase
            //-----------------------------------------------------------
            btDbvtBroadphase* GetBroadphase() const;
            //-----------------------------------------------------------
            /// Get Collision Dispatcher
            ///
            /// @return Bullet collision dispatcher
            //-----------------------------------------------------------
            btCollisionDispatcher* GetCollisionDispatcher() const;    
            //-----------------------------------------------------------
            /// Set a Debug Draw class
            ///
            /// @return Bullet dynamics world
            //-----------------------------------------------------------
            void SetDebugDrawer(CBulletDebugDraw * inpBDD);   
            //-----------------------------------------------------------
            /// Get the Debug Draw class
            ///
            /// @return Bullet dynamics world
            //-----------------------------------------------------------
            CBulletDebugDraw* GetDebugDrawer();
            //-----------------------------------------------------------
            /// Convert a Position from Bullet to MoFlow
            ///
            /// @return Converted position
            //-----------------------------------------------------------
            static Core::CVector3 ConvertBulletPositionToMoFlow(const btVector3& invPos);    
            //-----------------------------------------------------------
            /// Convert a Position from MoFlow to Bullet
            ///
            /// @return Converted position
            //-----------------------------------------------------------    
            static btVector3 ConvertMoFlowPositionToBullet(Core::CVector3 invPos); 
            //-----------------------------------------------------------
            /// Convert a Quaternion from Bullet to MoFlow
            ///
            /// @return Converted position
            //-----------------------------------------------------------  
            static Core::CQuaternion ConvertBulletQuaternionToMoFlow(const btQuaternion& inQuat);
            //-----------------------------------------------------------
            /// Convert a Quaternion from MoFlow to Bullet
            ///
            /// @return Converted position
            //-----------------------------------------------------------  
            static btQuaternion ConvertMoFlowQuaternionToBullet(Core::CQuaternion inQuat);
            //-----------------------------------------------------------
            /// Convert a Transform from Bullet to MoFlow
            ///
            /// @return Converted position
            //-----------------------------------------------------------  
            static Core::CTransform ConvertBulletTransformToMoFlow(const btTransform& inTrans);
            //-----------------------------------------------------------
            /// Convert a Transform from MoFlow to Bullet
            ///
            /// @return Converted position
            //-----------------------------------------------------------  
            static btTransform ConvertMoFlowTransformToBullet(Core::CTransform inTrans);
        private:
            
            btDiscreteDynamicsWorld* mpWorld;
            btDefaultCollisionConfiguration* mpCollisionConfig;
            btCollisionDispatcher* mpDispatcher;
            btDbvtBroadphase* mpBroadphase;
            btSequentialImpulseConstraintSolver* mpSolver;
            
            CBulletComponentFactory* mpComponentFactory;
            CBulletDebugDraw * pDebugDraw;
            CBulletCollisionMeshManager mCollisionMeshManager;
        };
    }
}

#endif