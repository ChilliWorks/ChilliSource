/*
 *  BulletComponentFactory.cpp
 *  moFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Physics3D/BulletPhysics/BulletComponentFactory.h>
#include <moFlo/Physics3D/BulletPhysics/BulletPhysicsSystem.h>
#include <moFlo/Physics3D/BulletPhysics/BulletPhysicsComponent.h>
#include <moFlo/Physics3D/BulletPhysics/BulletBodyHolder.h>

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include <moFlo/Physics3D/3DPhysicsSystem.h>
#include <moFlo/Physics3D/BulletPhysics/BulletCollisionMesh.h>
#include <moFlo/Core/ResourceManagerDispenser.h>

namespace moFlo
{
    namespace Physics3D
    {
        //-----------------------------------------------------------------------
        /// Constructor
        ///
        /// @param Physics world
        //-----------------------------------------------------------------------
        CBulletComponentFactory::CBulletComponentFactory(CBulletPhysicsSystem* inpPhysSystem) : mpPhysSystem(inpPhysSystem)
        {
        }
        //-----------------------------------------------------------------------
        /// Is A
        ///
        /// @return Whether this implements the given interface.
        //-----------------------------------------------------------------------
        bool CBulletComponentFactory::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return I3DPhysicsComponentFactory::InterfaceID == inInterfaceID;
        }
        //-----------------------------------------------------------------------
        /// Can Produce Components With Interface
        ///
        /// @return Whether the factory can produce the given type
        //------------------------------------------------------------------------
        bool CBulletComponentFactory::CanProduceComponentWithInterface(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == Physics3D::I3DPhysicsComponent::InterfaceID;
        }
        //-----------------------------------------------------------------------
        /// Can Produce Components With Type Name
        ///
        /// @return Whether the factory can produce the given name
        //------------------------------------------------------------------------
        bool CBulletComponentFactory::CanProduceComponentWithTypeName(const std::string& inTypeName) const
        {
            return inTypeName == Physics3D::I3DPhysicsComponent::TypeName;
        }
        //-----------------------------------------------------------------------
        /// Create Ground Physics Component
        ///
        /// Create a physical plane that acts as the ground
        ///
        /// @param Normal to plane surface
        /// @param Offet from origin along normal
        /// @return Instantiated component
        //------------------------------------------------------------------------
        Physics3D::I3DPhysicsComponentPtr CBulletComponentFactory::CreateGroundPhysicsComponent(Core::CVector3 vNorm, f32 fOffset) const
        {
            //Create a bullet shape as the underlying physics object
            btVector3 vNormBT=btVector3(vNorm.x,vNorm.y,vNorm.z);
            btCollisionShape* pPlaneShape = new btStaticPlaneShape(reinterpret_cast<btVector3&>(vNormBT), fOffset);
            
            CBulletBodyHolder Object;
            
            //We are a component and a motion state
            CBulletPhysicsComponent* pPhysComponent = new CBulletPhysicsComponent(Object, mpPhysSystem->GetPhysicsWorldPtr());
            
            //Create a rigid body from the shape and descriptor
            btRigidBody::btRigidBodyConstructionInfo RigidBodyCI(0, pPhysComponent, pPlaneShape, btVector3(0, 0, 0));
            btRigidBody* pRigidBody = new btRigidBody(RigidBodyCI); 
            
            pPhysComponent->mBulletContainer.pRigidBody = pRigidBody;
            pPhysComponent->mBulletContainer.pCollisionShape = pPlaneShape;
            
            Physics3D::I3DPhysicsComponentPtr pPhysShrdComponent(pPhysComponent);
            
            return pPhysShrdComponent;
        }
        //-----------------------------------------------------------------------
        /// Create Sphere Physics Component
        ///
        /// Create a physical, spherical bounding volume
        ///
        /// @param Radius
        /// @return Instantiated component
        //------------------------------------------------------------------------
        Physics3D::I3DPhysicsComponentPtr CBulletComponentFactory::CreateSpherePhysicsComponent(Physics3D::Physics3DComponentDesc& inDesc, f32 infRadius) const
        {
            //Create a bullet shape as the underlying physics object
            btCollisionShape* pSphereShape = new btSphereShape(infRadius);
            
            //Calculate the inertia of the object from it's initial inertia and mass
            btVector3 vInertiaBT=btVector3(inDesc.vInertia.x,inDesc.vInertia.y,inDesc.vInertia.z);    
            pSphereShape->calculateLocalInertia(inDesc.fMass, vInertiaBT);
            
            CBulletBodyHolder Object;
            
            //We are a component and a motion state
            CBulletPhysicsComponent* pPhysComponent = new CBulletPhysicsComponent(Object, mpPhysSystem->GetPhysicsWorldPtr());
            
            //Create a rigid body from the shape and descriptor
            btRigidBody::btRigidBodyConstructionInfo RigidBodyCI(inDesc.fMass, pPhysComponent, pSphereShape, vInertiaBT);
            btRigidBody* pRigidBody = new btRigidBody(RigidBodyCI); 
            pRigidBody->setActivationState(DISABLE_DEACTIVATION);
            
            pPhysComponent->mBulletContainer.pRigidBody = pRigidBody;
            pPhysComponent->mBulletContainer.pCollisionShape = pSphereShape;
            
            Physics3D::I3DPhysicsComponentPtr pPhysShrdComponent(pPhysComponent);
            
            return pPhysShrdComponent;
        }
        //-----------------------------------------------------------------------
        /// Create Box Physics Component
        ///
        /// Create a physical, cubical bounding volume
        ///
        /// @param Half dimensions
        /// @return Instantiated component
        //------------------------------------------------------------------------
        Physics3D::I3DPhysicsComponentPtr CBulletComponentFactory::CreateBoxPhysicsComponent(Physics3D::Physics3DComponentDesc& inDesc, const Core::CVector3& invHalfExtents) const
        {
            //Create a bullet shape as the underlying physics object
            btCollisionShape* pBoxShape = new btBoxShape(btVector3(invHalfExtents.x, invHalfExtents.y, invHalfExtents.z));
            
            //Calculate the inertia of the object from it's initial inertia and mass
            btVector3 vInertiaBT=btVector3(inDesc.vInertia.x,inDesc.vInertia.y,inDesc.vInertia.z);        
            pBoxShape->calculateLocalInertia(inDesc.fMass, vInertiaBT);
            
            CBulletBodyHolder Object;
            
            //We are a component and a motion state
            CBulletPhysicsComponent* pPhysComponent = new CBulletPhysicsComponent(Object, mpPhysSystem->GetPhysicsWorldPtr());
            
            //Create a rigid body from the shape and descriptor
            btRigidBody::btRigidBodyConstructionInfo RigidBodyCI(inDesc.fMass, pPhysComponent, pBoxShape, vInertiaBT);
            btRigidBody* pRigidBody = new btRigidBody(RigidBodyCI); 
            pRigidBody->setActivationState(DISABLE_DEACTIVATION);
            
            pPhysComponent->mBulletContainer.pRigidBody = pRigidBody;
            pPhysComponent->mBulletContainer.pCollisionShape = pBoxShape;
            
            Physics3D::I3DPhysicsComponentPtr pPhysShrdComponent(pPhysComponent);
            
            return pPhysShrdComponent;
        }
        //-----------------------------------------------------------------------
        /// Create Physics Component From Existing Resources
        ///
        /// Create a Physics Component containing the Collision Shape and Rigid Body passed in
        ///
        /// @param Half dimensions
        /// @return Instantiated component
        //------------------------------------------------------------------------
        Physics3D::I3DPhysicsComponentPtr CBulletComponentFactory::CreatePhysicsComponent(btCollisionShape* inpShape, btRigidBody* inpBody) const
        {
            CBulletBodyHolder Object;
            CBulletPhysicsComponent* pPhysComponent = new CBulletPhysicsComponent(Object, mpPhysSystem->GetPhysicsWorldPtr());
            inpBody->setMotionState(pPhysComponent);
            pPhysComponent->mBulletContainer.pRigidBody = inpBody;
            pPhysComponent->mBulletContainer.pCollisionShape = inpShape;
            
            Physics3D::I3DPhysicsComponentPtr pPhysShrdComponent(pPhysComponent);
            
            return pPhysShrdComponent;
        }
        //-----------------------------------------------------------------------
        /// Create Physics Component
        ///
        /// Create a Physics Component from a BulletCollisionMesh
        ///
        /// @param the collision mesh
        /// @return Instantiated component
        //------------------------------------------------------------------------
        Physics3D::I3DPhysicsComponentPtr CBulletComponentFactory::CreatePhysicsComponent(Physics3D::Physics3DComponentDesc& inDesc, BulletCollisionMeshPtr inpCollisionMesh) const
        {
            CBulletBodyHolder Object;
            CBulletPhysicsComponent* pPhysComponent = new CBulletPhysicsComponent(Object, mpPhysSystem->GetPhysicsWorldPtr());
            pPhysComponent->SetCollisionMesh(inpCollisionMesh);
            
            //create the shape
            btCollisionShape* pShape = inpCollisionMesh->GetCollisionShape();
            
            //Calculate the inertia of the object from it's initial inertia and mass
            btVector3 vInertiaBT=btVector3(inDesc.vInertia.x,inDesc.vInertia.y,inDesc.vInertia.z);
            
            //Create a rigid body from the shape and descriptor
            btRigidBody::btRigidBodyConstructionInfo RigidBodyCI(inDesc.fMass, pPhysComponent, pShape, vInertiaBT);
            btRigidBody* pRigidBody = new btRigidBody(RigidBodyCI); 
            pRigidBody->setActivationState(DISABLE_DEACTIVATION);
            
            //create the component, attach the shape and body and return
            pRigidBody->setMotionState(pPhysComponent);
            pPhysComponent->mBulletContainer.pRigidBody = pRigidBody;
            pPhysComponent->mBulletContainer.pCollisionShape = pShape;
            Physics3D::I3DPhysicsComponentPtr pPhysShrdComponent(pPhysComponent);
            return pPhysShrdComponent;
        }
        //-----------------------------------------------------------------------
        /// Create Physics Component
        ///
        /// Create a Physics Component from a mocollision file.
        ///
        /// @param the storage location
        /// @param the filename
        /// @return Instantiated component
        //------------------------------------------------------------------------
        Physics3D::I3DPhysicsComponentPtr CBulletComponentFactory::CreatePhysicsComponent(Physics3D::Physics3DComponentDesc& inDesc, Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilename) const
        {
            BulletCollisionMeshPtr pMesh = LOAD_RESOURCE(CBulletCollisionMesh, ineStorageLocation, instrFilename);
            return CreatePhysicsComponent(inDesc, pMesh);
        }
    }
}
