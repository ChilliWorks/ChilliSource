/*
 *  BulletPhysicsComponent.h
 *  moFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETPHYSICSCOMPONENT_H_
#define _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETPHYSICSCOMPONENT_H_

#include <moFlo/Physics3D/3DPhysicsComponent.h>
#include <moFlo/Physics3D/BulletPhysics/ForwardDeclarations.h>
#include <moFlo/Physics3D/BulletPhysics/BulletBodyHolder.h>
#include <LinearMath/btMotionState.h>
#include <moFlo/Core/Transform.h>

namespace moFlo
{
    namespace Physics3D
    {
        class CBulletPhysicsComponent  : public Physics3D::I3DPhysicsComponent, public btMotionState
        {
        public:
            CBulletPhysicsComponent(const CBulletBodyHolder& inObject, btDiscreteDynamicsWorld* inpWorld);
            ~CBulletPhysicsComponent();
            
            //----------------------------------------------------------
            /// Is A
            ///
            /// @return Whether we are of the given type
            //----------------------------------------------------------
            bool IsA(Core::InterfaceIDType InterfaceID) const;
            //----------------------------------------------------------
            /// Set Position
            ///
            /// @param Position of the object in the physics world
            //----------------------------------------------------------
            void SetPosition(Core::CVector3 invPosition);
            //----------------------------------------------------------
            /// Get Position
            ///
            /// @return Position of the object in the physics world
            //----------------------------------------------------------
            const Core::CVector3 GetPosition() const;
            //----------------------------------------------------------
            /// Rotate To
            ///
            /// Set the rotation of the physics object in the physics
            /// world
            ///
            /// @param Euler axis
            /// @param Angle in radians
            //----------------------------------------------------------
            void RotateTo(Core::CVector3 invAxis, f32 infAngle);
            //----------------------------------------------------------
            /// Get Orientation
            ///
            /// @return Orientation of the object in the physics world
            //----------------------------------------------------------
            const Core::CQuaternion GetOrientation() const;
            //----------------------------------------------------------
            /// Get Transform
            ///
            /// @return Transform of the object in the physics world
            //----------------------------------------------------------
            const Core::CTransform GetTransform() const;
            //----------------------------------------------------------
            /// Get Transform
            ///
            /// @return Transform of the object in the physics world
            //----------------------------------------------------------
            void SetTransform(Core::CTransform invTransform);     
            //----------------------------------------------------------
            /// Set Linear Velocity
            ///
            /// Apply linear velocity to the physics object
            ///
            /// @param Velocity vector
            //----------------------------------------------------------
            void SetLinearVelocity(Core::CVector3 invVelocity);
            //----------------------------------------------------------
            /// Get Linear Velocity
            ///
            /// @return Velocity vector
            //----------------------------------------------------------
            const Core::CVector3 GetLinearVelocity();
            //----------------------------------------------------------
            /// Get Velocity In Local Point
            ///
            /// @return Velocity vector
            //----------------------------------------------------------
            const Core::CVector3 GetVelocityInLocalPoint(Core::CVector3 invPoint);         
            //-------------------------------------------------------------------------------
            /// Set Angular Velocity
            ///
            /// @param Velocity vector
            //-------------------------------------------------------------------------------
            void SetAngularVelocity(Core::CVector3 invVelocity);
            //-------------------------------------------------------------------------------
            /// Get Angular Velocity
            ///
            /// @return Velocity vector
            //-------------------------------------------------------------------------------
            const Core::CVector3 GetAngularVelocity();
            /// Apply Central Impulse
            ///
            /// Apply impulse to the centre of mass of the object
            ///
            /// @param Force vector
            //-------------------------------------------------------------------------------
            void ApplyCentralImpulse(Core::CVector3 invForce);        
            //-------------------------------------------------------------------------------
            /// Apply Impulse
            ///
            /// Apply impulse to the relative positon of the object
            ///
            /// @param Force vector
            /// @param Point of impact relative to origin of object
            //-------------------------------------------------------------------------------
            void ApplyImpulse(Core::CVector3 invForce, Core::CVector3 invImpact);
            //-------------------------------------------------------------------------------
            /// Apply Torque Impulse
            ///
            /// Apply torque impulse to the object
            ///
            /// @param Force vector
            //-------------------------------------------------------------------------------
            void ApplyTorqueImpulse(Core::CVector3 invForce);      
            //-------------------------------------------------------------------------------
            /// Set Sleeping Thresholds
            ///
            /// Set the linear and angular energy threshold beneath which the object
            /// will sleep
            ///
            /// @param Linear threshold
            /// @param Angular threshold
            //-------------------------------------------------------------------------------
            void SetSleepingThresholds(f32 infLinearThresh, f32 infAngularThresh);
            //-------------------------------------------------------------------------------
            /// Set Sleeping
            ///
            /// @param Whether to sleep the physics object
            //-------------------------------------------------------------------------------
            void SetSleeping(bool inbIsSleeping);
            //-------------------------------------------------------------------------------
            /// Set Physics Material
            ///
            /// @param Material that governs the restitution, dampning etc
            //-------------------------------------------------------------------------------
            void SetPhysicsMaterial(Physics3D::PhysicsMaterial insMaterial);
            //-------------------------------------------------------------------------------
            /// Get Physics Material
            ///
            /// @return Material that governs the restitution, dampning etc
            //-------------------------------------------------------------------------------
            const Physics3D::PhysicsMaterial GetPhysicsMaterial() const;
            //-------------------------------------------------------------------------------
            /// Set Collision Mask
            ///
            /// @param Mask of objects to test collisions against
            //-------------------------------------------------------------------------------
            void SetCollisionMask(Physics3D::CollisionMask3D ineMask);
            //-------------------------------------------------------------------------------
            /// Set Damping
            /// 
            /// @param Linear Damping
            /// @param Angular Damping
            //-------------------------------------------------------------------------------
            void SetDamping(f32 infLinearDamping, f32 infAngularDamping);
            //-------------------------------------------------------------------------------
            /// Set Collision Mesh
            /// 
            /// @param Collision mesh
            //-------------------------------------------------------------------------------
            void SetCollisionMesh(BulletCollisionMeshPtr inpCollisionMesh);
            //-------------------------------------------------------------------------------
            /// Get Linear Damping
            /// 
            /// @return Linear Damping
            //-------------------------------------------------------------------------------
            const f32 GetLinearDamping();
            //-------------------------------------------------------------------------------
            /// Get Angular Damping
            /// 
            /// @return Angular Damping
            //-------------------------------------------------------------------------------
            const f32 GetAngularDamping();
            //-----------------------------------------------------------------------
            /// Get Body Holder
            ///
            /// @return Bullet container
            //-----------------------------------------------------------------------
            const CBulletBodyHolder& GetBodyHolder() const;
            
            void SetBulletCollisionFilterGroup(s32 indwValue){
                mwCollisionGroup=indwValue;
            }
            void SetBulletCollisionMask(s32 indwValue){
                mwCollisionFlags=indwValue;
            }
            s16 GetBulletCollisionFilterGroup(){return mwCollisionGroup;}
            s16 GetBulletCollisionMask(){return mwCollisionFlags;}
            
            
        protected:
            //----------------------------------------------------------
            /// Set World Transform
            ///
            /// @param Bullet object transform
            //----------------------------------------------------------
            void setWorldTransform(const btTransform& worldTrans);
            //----------------------------------------------------------
            /// Get World Transform
            ///
            /// @return Bullet object transform set from object
            //----------------------------------------------------------
            void getWorldTransform(btTransform& worldTrans) const;
            //----------------------------------------------------------
            /// On Attached To Entity
            ///
            /// Called when this component is attached
            //----------------------------------------------------------
            void OnAttachedToEntity();
            //----------------------------------------------------------
            /// On Detached To Entity
            ///
            /// Called when this component is detached
            //----------------------------------------------------------
            void OnDetachedFromEntity();
            //----------------------------------------------------------
            /// On Attached Entity Changed
            ///
            /// Called when this component is detached or attached
            /// to an entity to work out if it has precedence
            //----------------------------------------------------------
            void OnAttachedEntityChanged();	
            //----------------------------------------------------------
            /// On Attached Entity Transformed
            ///
            /// Called when the entity this component is attached to
            /// transforms, requiring the physics component to move
            /// accordingly
            //----------------------------------------------------------
            void UpdatePositionFromEntity();
            
            
            
        private:
            
            Core::CVector3 mvPhysicsPosition;
            Core::CQuaternion mqPhysicsOrientation;
            
            CBulletBodyHolder mBulletContainer;
            Physics3D::PhysicsMaterial mMaterial;
            
            bool mbIsPrimePhysicsComponent;
            
            btDiscreteDynamicsWorld* mpWorld;
            BulletCollisionMeshPtr mpCollisionMesh;
            
            bool mbTransformingSelf;
            
            s16 mwCollisionGroup;
            s16 mwCollisionFlags;
            
            friend class CBulletComponentFactory;
        };
    }
}
#endif

