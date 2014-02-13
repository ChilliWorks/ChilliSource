/*
 *  BulletPhysicsComponent
 *  moFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <LinearMath/btTransform.h>
#include <LinearMath/btMotionState.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <moFlo/Physics3D/BulletPhysics/BulletPhysicsComponent.h>
#include <moFlo/Physics3D/BulletPhysics/BulletPhysicsSystem.h>

#include <moFlo/Core/Entity.h>

namespace moFlo
{
    namespace Physics3D
    {
        //-----------------------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //-----------------------------------------------------------------------
        CBulletPhysicsComponent::CBulletPhysicsComponent(const CBulletBodyHolder& inObject, btDiscreteDynamicsWorld* inpWorld)
        : mbIsPrimePhysicsComponent(false), mpWorld(inpWorld), mBulletContainer(inObject), mbTransformingSelf(false),
        mwCollisionGroup(btBroadphaseProxy::StaticFilter), mwCollisionFlags(btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter)
        {
        }
        //----------------------------------------------------------
        /// Is A
        ///
        /// @return Whether we are of the given type
        //----------------------------------------------------------
        bool CBulletPhysicsComponent::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return (inInterfaceID == Physics3D::I3DPhysicsComponent::InterfaceID);
        }
        //----------------------------------------------------------
        /// Set Position
        ///
        /// @param Position of the object in the physics world
        //----------------------------------------------------------
        void CBulletPhysicsComponent::SetPosition(Core::CVector3 invPosition)
        {
            btTransform PhysTransform;
            getWorldTransform(PhysTransform);
            btVector3 vPositionBT=CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(invPosition);
            PhysTransform.setOrigin(vPositionBT);
            setWorldTransform(PhysTransform);
            mBulletContainer.pRigidBody->setCenterOfMassTransform(PhysTransform);
            
            mvPhysicsPosition = invPosition;
        }
        //----------------------------------------------------------
        /// Get Position
        ///
        /// @return Position of the object in the physics world
        /// @deprecated Use the entity Transform()
        //----------------------------------------------------------
        const Core::CVector3 CBulletPhysicsComponent::GetPosition() const 
        {
            return mvPhysicsPosition;
        }
        //----------------------------------------------------------
        /// Rotate To
        ///
        /// Set the rotation of the physics object in the physics
        /// world
        ///
        /// @param Euler axis
        /// @param Angle in radians
        //----------------------------------------------------------
        void CBulletPhysicsComponent::RotateTo(Core::CVector3 invAxis, f32 infAngle)
        {
            //TODO: Something here? :S
        }
        //----------------------------------------------------------
        /// Get Orientation
        ///
        /// @return Orientation of the object in the physics world
        //----------------------------------------------------------
        const Core::CQuaternion CBulletPhysicsComponent::GetOrientation() const
        {
            return mqPhysicsOrientation;
        }
        //----------------------------------------------------------
        /// Get Transform
        ///
        /// @return Transform of the object in the physics world
        //----------------------------------------------------------
        const Core::CTransform CBulletPhysicsComponent::GetTransform() const
        {
            Core::CTransform trMF;
            btTransform trBT=mBulletContainer.pRigidBody->getCenterOfMassTransform();
            btVector3 vOriginBT = trBT.getOrigin();
            trMF.SetPosition(CBulletPhysicsSystem::ConvertBulletPositionToMoFlow(vOriginBT));
            btQuaternion qOriBT = trBT.getRotation();
            trMF.SetOrientation(CBulletPhysicsSystem::ConvertBulletQuaternionToMoFlow(qOriBT));
            
            return trMF;
        }
        //----------------------------------------------------------
        /// Get Transform
        ///
        /// @return Transform of the object in the physics world
        //----------------------------------------------------------
        void CBulletPhysicsComponent::SetTransform(Core::CTransform inTrans)
        {
            btTransform trBT;
            Core::CVector3 vOriginMF = inTrans.GetWorldPosition();
            trBT.setOrigin(CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(vOriginMF));
            Core::CQuaternion qOriMF = inTrans.GetWorldOrientation();
            trBT.setRotation(CBulletPhysicsSystem::ConvertMoFlowQuaternionToBullet(qOriMF));
            
            mBulletContainer.pRigidBody->setCenterOfMassTransform(trBT);
        }
        //----------------------------------------------------------
        /// Set Linear Velocity
        ///
        /// Apply linear velocity to the physics object
        ///
        /// @param Velocity vector
        //----------------------------------------------------------
        void CBulletPhysicsComponent::SetLinearVelocity(Core::CVector3 invVelocity)
        {
            mBulletContainer.pRigidBody->setLinearVelocity(CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(invVelocity));
        }
        //----------------------------------------------------------
        /// Get Linear Velocity
        ///
        /// @return Velocity vector
        //----------------------------------------------------------
        const Core::CVector3 CBulletPhysicsComponent::GetLinearVelocity()
        {
            btVector3 vVel=mBulletContainer.pRigidBody->getLinearVelocity();
            return CBulletPhysicsSystem::ConvertBulletPositionToMoFlow(vVel);
        }
        //----------------------------------------------------------
        /// Get Velocity In Local Point
        ///
        /// @return Velocity vector
        //----------------------------------------------------------
        const Core::CVector3 CBulletPhysicsComponent::GetVelocityInLocalPoint(Core::CVector3 invPoint)
        {    
            btVector3 vVel=mBulletContainer.pRigidBody->getVelocityInLocalPoint(btVector3(invPoint.x,invPoint.y,invPoint.z));
            return Core::CVector3(CBulletPhysicsSystem::ConvertBulletPositionToMoFlow(vVel));
        }
        //-------------------------------------------------------------------------------
        /// Set Angular Velocity
        ///
        /// @param Velocity vector
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::SetAngularVelocity(Core::CVector3 invVelocity)
        {
            mBulletContainer.pRigidBody->setAngularVelocity(CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(invVelocity));
        }
        //-------------------------------------------------------------------------------
        /// Get Angular Velocity
        ///
        /// @return Velocity vector
        //-------------------------------------------------------------------------------
        const Core::CVector3 CBulletPhysicsComponent::GetAngularVelocity()
        {
            btVector3 vVel=mBulletContainer.pRigidBody->getAngularVelocity();
            return CBulletPhysicsSystem::ConvertBulletPositionToMoFlow(vVel);
        }
        //-------------------------------------------------------------------------------
        /// Apply Impulse
        ///
        /// Apply impulse to the relative positon of the object
        ///
        /// @param Force vector
        /// @param Point of impact relative to origin of object
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::ApplyCentralImpulse(Core::CVector3 invForce)
        {
            mBulletContainer.pRigidBody->applyCentralImpulse(CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(invForce));
        }
        //-------------------------------------------------------------------------------
        /// Apply Impulse
        ///
        /// Apply impulse to the relative positon of the object
        ///
        /// @param Force vector
        /// @param Point of impact relative to origin of object
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::ApplyImpulse(Core::CVector3 invForce, Core::CVector3 invImpact)
        {
            mBulletContainer.pRigidBody->applyImpulse(CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(invForce), CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(invImpact));
        }
        //-------------------------------------------------------------------------------
        /// Apply Torque Impulse
        ///
        /// Apply torque impulse to the object
        ///
        /// @param Force vector
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::ApplyTorqueImpulse(Core::CVector3 invForce){
            mBulletContainer.pRigidBody->applyTorqueImpulse(CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(invForce));
        }
        //-------------------------------------------------------------------------------
        /// Set Sleeping
        ///
        /// @param Whether to sleep the physics object
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::SetSleeping(bool inbIsSleeping)
        {
            if(inbIsSleeping)
            {
                mBulletContainer.pRigidBody->activate(false);
            }
            else
            {
                mBulletContainer.pRigidBody->activate(true);
            }
        }
        //-------------------------------------------------------------------------------
        /// Set Sleeping Thresholds
        ///
        /// Set the linear and angular energy threshold beneath which the object
        /// will sleep
        ///
        /// @param Linear threshold
        /// @param Angular threshold
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::SetSleepingThresholds(f32 infLinearThresh, f32 infAngularThresh)
        {
            mBulletContainer.pRigidBody->setSleepingThresholds(infLinearThresh, infAngularThresh);
        }
        //-------------------------------------------------------------------------------
        /// Set Physics Material
        ///
        /// @param Material that governs the restitution, dampning etc
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::SetPhysicsMaterial(Physics3D::PhysicsMaterial insMaterial)
        {
            mMaterial = insMaterial;
            
            mBulletContainer.pRigidBody->setDamping(mMaterial.fLinearDamping, mMaterial.fAngularDamping);
            mBulletContainer.pRigidBody->setFriction(mMaterial.fFriction);
            mBulletContainer.pRigidBody->setRestitution(mMaterial.fRestitution);
        }
        //-------------------------------------------------------------------------------
        /// Get Physics Material
        ///
        /// @return Material that governs the restitution, dampning etc
        //-------------------------------------------------------------------------------
        const Physics3D::PhysicsMaterial CBulletPhysicsComponent::GetPhysicsMaterial() const
        {
            return mMaterial;
        }
        //-------------------------------------------------------------------------------
        /// Set Collision Mask
        ///
        /// @param Mask of objects to test collisions against
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::SetCollisionMask(Physics3D::CollisionMask3D ineMask)
        {
            //TODO: Something here? :S
        }
        //-------------------------------------------------------------------------------
        /// Set Damping
        /// 
        /// @param Linear Damping
        /// @param Angular Damping
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::SetDamping(f32 infLinearDamping, f32 infAngularDamping)
        {
            mBulletContainer.pRigidBody->setDamping(infLinearDamping, infAngularDamping);
        }
        //-------------------------------------------------------------------------------
        /// Get Linear Damping
        /// 
        /// @return Linear Damping
        //-------------------------------------------------------------------------------
        const f32 CBulletPhysicsComponent::GetLinearDamping()
        {
            return mBulletContainer.pRigidBody->getLinearDamping();
        }
        //-------------------------------------------------------------------------------
        /// Get Angular Damping
        /// 
        /// @return Angular Damping
        //-------------------------------------------------------------------------------
        const f32 CBulletPhysicsComponent::GetAngularDamping()
        {
            return 0.0f;
        }
        //----------------------------------------------------------
        /// On Attached To Entity
        ///
        /// Called when this component is attached
        //----------------------------------------------------------
        void CBulletPhysicsComponent::OnAttachedToEntity()
        {
            //Add the physics component to the physics world
            mpWorld->addRigidBody(mBulletContainer.pRigidBody);  
            OnAttachedEntityChanged();
        }
        //----------------------------------------------------------
        /// On Detached To Entity
        ///
        /// Called when this component is detached
        //----------------------------------------------------------
        void CBulletPhysicsComponent::OnDetachedFromEntity()
        {
            //Remove the physics component to the physics world 
            mpWorld->removeRigidBody(mBulletContainer.pRigidBody);
            OnAttachedEntityChanged();
        }
        //----------------------------------------------------------
        /// On Attached Entity Changed
        ///
        /// Called when this component is detached or attached
        /// to an entity to work out if it has precedence
        //----------------------------------------------------------
        void CBulletPhysicsComponent::OnAttachedEntityChanged()
        {
            Core::CEntity* pEntity = GetEntityOwner();
            
            if(pEntity) 
            {
                mbIsPrimePhysicsComponent = !(pEntity->GetComponent(I3DPhysicsComponent::InterfaceID));
                //TODO needs awareness of 2D physics interfaces, god help anyone who mixes the two though. I mean really.
            }
            
            UpdatePositionFromEntity();
        }
        //----------------------------------------------------------
        /// On Attached Entity Transformed
        ///
        /// Called when the entity this component is attached to
        /// transforms, requiring the physics component to move
        /// accordingly
        //----------------------------------------------------------
        void CBulletPhysicsComponent::UpdatePositionFromEntity()
        {
            btTransform PhysTransform;
            getWorldTransform(PhysTransform);
            mBulletContainer.pRigidBody->setCenterOfMassTransform(PhysTransform);
            Core::CEntity* pEntity = GetEntityOwner();
            if(pEntity)
            {
                mvPhysicsPosition=pEntity->Transform().GetWorldPosition();
                mqPhysicsOrientation=pEntity->Transform().GetWorldOrientation();
            }
            
        }
        //----------------------------------------------------------
        /// Set World Transform
        ///
        /// @param Bullet object transform
        //----------------------------------------------------------
        void CBulletPhysicsComponent::setWorldTransform(const btTransform& worldTrans)
        {
            //Check attached to an entity attached to a scene node
            Core::CEntity* pEntity = GetEntityOwner();
            if(pEntity)
            {
                //If we are prime we have control of the scene node
                if(mbIsPrimePhysicsComponent) 
                {
                    // get position of physics object in world space
                    btVector3 vPos = worldTrans.getOrigin();
                    
                    // make a moflow vector out of it and invert Z
                    mvPhysicsPosition = CBulletPhysicsSystem::ConvertBulletPositionToMoFlow(vPos);
                    
                    // set the attached entity's position from that moflow vector
                    pEntity->Transform().SetPosition(mvPhysicsPosition);
                    
                    // get quaternion of physics object's world rotation
                    btQuaternion quat = worldTrans.getRotation();
                    
                    // make a moflow quaternion out of it
                    mqPhysicsOrientation = CBulletPhysicsSystem::ConvertBulletQuaternionToMoFlow(quat);
                    
                    pEntity->Transform().SetOrientation(mqPhysicsOrientation);
                    
                } 
                else 
                { 	
                    
                    //TODO: THIS IS OUT OF DATE, NEEDS REWRITING TO MATCH TRANSLATION ABOVE
                    
                    //Set our position to the current scene node position + our offset (i.e. overwrite system)
                    /*
                     Core::CVector3 vPos = pEntity->Transform().GetWorldPosition(); 
                     Core::CQuaternion qRot = pEntity->Transform().GetWorldOrientation();
                     
                     Core::CQuaternion qToBulletSystem;
                     qToBulletSystem.SetAxisAngle(Core::CVector3::Z_UNIT_POSITIVE, 1.57079633f);
                     
                     qRot = qToBulletSystem * qRot;
                     
                     const_cast<btTransform&>(worldTrans).setOrigin(btVector3(vPos.x, vPos.y, vPos.z));
                     const_cast<btTransform&>(worldTrans).setRotation(btQuaternion(qRot.x, qRot.y, qRot.z, qRot.w));
                     
                     mBulletContainer.pRigidBody->setCenterOfMassTransform(worldTrans);
                     
                     mvPhysicsPosition = vPos;
                     mqPhysicsOrientation = qRot;
                     */
                }
            }
        }
        //----------------------------------------------------------
        /// Get World Transform
        ///
        /// @return Bullet object transform set from object
        //----------------------------------------------------------
        void CBulletPhysicsComponent::getWorldTransform(btTransform& worldTrans) const
        {
            //Check attached to an entity attached to a scene node
            Core::CEntity* pEntity = const_cast<Core::CEntity*>(GetEntityOwner());
            if(pEntity)
            {
                // get the position of the attached entity
                Core::CVector3 vPosition = pEntity->Transform().GetWorldPosition();
                // get the orientation of the attached entity
                Core::CQuaternion qRotation = pEntity->Transform().GetWorldOrientation();
                
                // set the world position from the entity position
                worldTrans.setOrigin(CBulletPhysicsSystem::ConvertMoFlowPositionToBullet(vPosition));
                // set the orientation from the entity orientation
                worldTrans.setRotation(CBulletPhysicsSystem::ConvertMoFlowQuaternionToBullet(qRotation));
            }
        }
        //-----------------------------------------------------------------------
        /// Get Body Holder
        ///
        /// @return Bullet container
        //-----------------------------------------------------------------------
        const CBulletBodyHolder& CBulletPhysicsComponent::GetBodyHolder() const
        {
            return mBulletContainer;
        }
        //-------------------------------------------------------------------------------
        /// Set Collision Mesh
        /// 
        /// @param Collision mesh
        //-------------------------------------------------------------------------------
        void CBulletPhysicsComponent::SetCollisionMesh(BulletCollisionMeshPtr inpCollisionMesh)
        {
            mpCollisionMesh = inpCollisionMesh;
        }
        //-----------------------------------------------------------------------
        /// Destructor
        //-----------------------------------------------------------------------
        CBulletPhysicsComponent::~CBulletPhysicsComponent()
        {
            //Remove the physics component to the physics world 
            mpWorld->removeRigidBody(mBulletContainer.pRigidBody);
            
            //We should only delete the collision shape if we are not getting it
            //from a collision mesh.
            if (mpCollisionMesh == NULL)
                mBulletContainer.Destroy();
            else
                mBulletContainer.Destroy(false);
        }
    }
}