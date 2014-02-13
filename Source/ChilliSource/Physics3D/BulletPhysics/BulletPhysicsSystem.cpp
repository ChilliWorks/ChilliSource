/*
 *  BulletPhysicsSystem.cpp
 *  moFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Physics3D/BulletPhysics/BulletPhysicsSystem.h>
#include <moFlo/Physics3D/BulletPhysics/BulletComponentFactory.h>
#include <moFlo/Physics3D/BulletPhysics/BulletPhysicsComponent.h>
#include <moFlo/Core/ResourceManagerDispenser.h>
#include <moFlo/Core/Component.h>

namespace moFlo
{
    namespace Physics3D
    {
        //--------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //--------------------------------------------------------
        CBulletPhysicsSystem::CBulletPhysicsSystem(u32 inudwDefaultPoolSizeInKB) : pDebugDraw(NULL)
        {
            Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mCollisionMeshManager);
            
            //Create the default configuration for memory etc of collidable objects
            btDefaultCollisionConstructionInfo sCollisionDesc;
            sCollisionDesc.m_defaultMaxPersistentManifoldPoolSize = inudwDefaultPoolSizeInKB;
            sCollisionDesc.m_defaultMaxCollisionAlgorithmPoolSize = inudwDefaultPoolSizeInKB;
            
            mpCollisionConfig = new btDefaultCollisionConfiguration(sCollisionDesc);
            
            //Create a collision dispatcher to notify us of collisions in the physics world
            mpDispatcher = new btCollisionDispatcher(mpCollisionConfig);
            
            //Create a broadphase collision detector which will just to a naive check against all volumes in the world
            mpBroadphase = new btDbvtBroadphase();
            mpBroadphase->optimize();
            
            //The default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
            mpSolver = new btSequentialImpulseConstraintSolver();
            
            //Create the physics world
            mpWorld = new btDiscreteDynamicsWorld(mpDispatcher, mpBroadphase, mpSolver, mpCollisionConfig);
            
            //Enable default gravity
            btVector3 vGravityBT = btVector3(mvGravity.x,mvGravity.y,mvGravity.z);
            mpWorld->setGravity(vGravityBT);
            
            //Create a factory to generate physics objects for us
            mpComponentFactory = new CBulletComponentFactory(this);
        }
        //----------------------------------------------------------
        /// Is A
        ///
        /// @return Whether we are of the given type
        //----------------------------------------------------------
        bool CBulletPhysicsSystem::IsA(Core::InterfaceIDType InterfaceID) const
        {
            return (InterfaceID == I3DPhysicsSystem::InterfaceID) || (InterfaceID == IUpdateable::InterfaceID)  || (InterfaceID == IComponentProducer::InterfaceID);
        }
        //----------------------------------------------------------
        /// Update
        ///
        /// @param Time since last update
        //----------------------------------------------------------
        void CBulletPhysicsSystem::Update(f32 infDT)
        {
            //Bullet works in milliseconds
            mpWorld->stepSimulation(1000.0/30.0);
            mpWorld->debugDrawWorld();
        }
        //-----------------------------------------------------------
        /// Get Physics World Pointer
        ///
        /// @return Bullet dynamics world
        //-----------------------------------------------------------
        btDiscreteDynamicsWorld* CBulletPhysicsSystem::GetPhysicsWorldPtr() const
        {
            return mpWorld;
        }
        //----------------------------------------------------------
        /// Get Component Factory
        ///
        /// @return Factory that this system owns
        //----------------------------------------------------------
        Core::IComponentFactory& CBulletPhysicsSystem::GetComponentFactory(u32 index)
        {
            return *mpComponentFactory;
        }
        //----------------------------------------------------------
        /// Get Component Factory Pointer
        ///
        /// @return Factory that this system owns
        //----------------------------------------------------------
        Core::IComponentFactory* CBulletPhysicsSystem::GetComponentFactoryPtr(u32 index)
        {
            return mpComponentFactory;
        }
        //----------------------------------------------------------
        /// Get Number of Component Factories
        ///
        /// @return Number of factories this system has
        //----------------------------------------------------------
        u32 CBulletPhysicsSystem::GetNumComponentFactories() const
        {
            return 1;
        }
        //-----------------------------------------------------------
        /// Set Gravity
        ///
        /// @param New gravity velocity vector
        //-----------------------------------------------------------
        void CBulletPhysicsSystem::SetGravity(const Core::CVector3 &invGravity)
        {
            mvGravity = invGravity;
            btVector3 vGravityBT = btVector3(mvGravity.x,mvGravity.y,mvGravity.z);
            mpWorld->setGravity(vGravityBT);
        }
        //-----------------------------------------------------------
        /// Get Gravity
        ///
        /// @return New gravity velocity vector
        //-----------------------------------------------------------
        const Core::CVector3& CBulletPhysicsSystem::GetGravity() const
        {
            return mvGravity;
        }
        //-----------------------------------------------------------
        /// Get Broadphase Pointer
        ///
        /// @return Bullet broadphase
        //-----------------------------------------------------------
        btDbvtBroadphase* CBulletPhysicsSystem::GetBroadphase() const 
        {
            return mpBroadphase;
        }
        //-----------------------------------------------------------
        /// Get Collision Dispatcher
        ///
        /// @return Bullet collision dispatcher
        //-----------------------------------------------------------
        btCollisionDispatcher* CBulletPhysicsSystem::GetCollisionDispatcher() const 
        {
            return mpDispatcher;
        }   
        //-----------------------------------------------------------
        /// Set a Debug Draw class
        ///
        /// @return Bullet dynamics world
        //-----------------------------------------------------------
        void CBulletPhysicsSystem::SetDebugDrawer(CBulletDebugDraw * inpBDD)
        {
            pDebugDraw=inpBDD;
            mpWorld->setDebugDrawer(inpBDD);   
        }
        //-----------------------------------------------------------
        /// Get the Debug Draw class
        ///
        /// @return Bullet dynamics world
        //-----------------------------------------------------------
        CBulletDebugDraw* CBulletPhysicsSystem::GetDebugDrawer()
        {
            return pDebugDraw;
        }
        //--------------------------------------------------------
        /// Destructor
        //--------------------------------------------------------
        CBulletPhysicsSystem::~CBulletPhysicsSystem()
        {
            SAFE_DELETE(mpWorld);
            SAFE_DELETE(mpBroadphase);
            SAFE_DELETE(mpCollisionConfig);
            SAFE_DELETE(mpSolver);
            SAFE_DELETE(mpComponentFactory);
        }
        //-----------------------------------------------------------
        /// Convert a Position from Bullet to MoFlow
        ///
        /// @return Converted position
        //-----------------------------------------------------------
        Core::CVector3 CBulletPhysicsSystem::ConvertBulletPositionToMoFlow(const btVector3& invPos)
        {
            return Core::CVector3(invPos.x(),invPos.y(),-invPos.z());
        }
        //-----------------------------------------------------------
        /// Convert a Position from MoFlow to Bullet
        ///
        /// @return Converted position
        //-----------------------------------------------------------  
        btVector3 CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(Core::CVector3 invPos)
        {
            // this function is the same in reverse
            return btVector3(invPos.x,invPos.y,-invPos.z);
        }
        //-----------------------------------------------------------
        /// Convert a Quaternion from Bullet to MoFlow
        ///
        /// @return Converted position
        //-----------------------------------------------------------  
        Core::CQuaternion CBulletPhysicsSystem::ConvertBulletQuaternionToMoFlow(const btQuaternion& invQuat)
        {
            btVector3 vAxis=invQuat.getAxis();
            f32 fAngle=invQuat.getAngle();
            vAxis.setZ(-vAxis.z());
            fAngle=-fAngle;
            return Core::CQuaternion(Core::CVector3(vAxis.x(),vAxis.y(),vAxis.z()),fAngle);
        }
        //-----------------------------------------------------------
        /// Convert a Quaternion from MoFlow to Bullet
        ///
        /// @return Converted position
        //-----------------------------------------------------------  
        btQuaternion CBulletPhysicsSystem::ConvertMoFlowQuaternionToBullet(Core::CQuaternion invQuat)
        {
            // this function is the same in reverse    
            Core::CVector3 vAxis=invQuat.GetAxis();
            f32 fAngle=invQuat.GetAngle();
            vAxis.z=-vAxis.z;
            fAngle=-fAngle;
            return btQuaternion(btVector3(vAxis.x,vAxis.y,vAxis.z),fAngle);
        }
        //-----------------------------------------------------------
        /// Convert a Transform from Bullet to MoFlow
        ///
        /// @return Converted position
        //-----------------------------------------------------------  
        Core::CTransform CBulletPhysicsSystem::ConvertBulletTransformToMoFlow(const btTransform& invTrans)
        {
            Core::CTransform tr;
            tr.SetPosition(ConvertBulletPositionToMoFlow(invTrans.getOrigin()));
            tr.SetOrientation(ConvertBulletQuaternionToMoFlow(invTrans.getRotation()));
            return tr;
        }
        //-----------------------------------------------------------
        /// Convert a Transform from MoFlow to Bullet
        ///
        /// @return Converted position
        //-----------------------------------------------------------  
        btTransform CBulletPhysicsSystem::ConvertMoFlowTransformToBullet(Core::CTransform invTrans)
        {
            btTransform tr;
            tr.setOrigin(ConvertMoFlowPositionToBullet(invTrans.GetWorldPosition()));
            tr.setRotation(ConvertMoFlowQuaternionToBullet(invTrans.GetWorldOrientation()));
            return tr;
        }
    }
}