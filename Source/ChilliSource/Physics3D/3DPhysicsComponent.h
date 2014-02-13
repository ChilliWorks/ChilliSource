/*
 *  3DPhysicsComponent.h
 *  moFloTest
 *
 *  Created by Scott Downie on 07/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PHYSICS3D_3DPHYSICSCOMPONENT_H_
#define _MOFLO_PHYSICS3D_3DPHYSICSCOMPONENT_H_

#include <moFlo/Physics3D/ForwardDeclarations.h>
#include <moFlo/Core/Component.h>
#include <moFlo/Core/Transform.h>


namespace moFlo
{
	namespace Physics3D
	{
		typedef u32 CollisionMask3D;
		
		struct PhysicsMaterial
		{
			f32 fLinearDamping;		//<Governs how much an object's linear velocity will reduce with time
			f32 fAngularDamping;	//<Governs how much an object's angular velocity will reduce with time
			f32 fFriction;			//<Governs how much an object's speed is lost when it is in collision with another object
			f32 fRestitution;		//< Ranged 0 - 1, at 0 collisions involving this object are completely inelastic, at 1 they are completely elastic
			
			///Default constructor for PhysicsMaterial initialising all members to 0
			PhysicsMaterial()
			: fLinearDamping(0), fAngularDamping(0), fFriction(0), fRestitution(0)
			{
				
			}
			
			///Constructor taking values for all members and initialising them
			PhysicsMaterial(f32 infLinearDamping, f32 infAngularDamping, f32 infFriction, f32 infRestitution)
			: fLinearDamping(infLinearDamping), fAngularDamping(infAngularDamping), fFriction(infFriction), fRestitution(infRestitution)
			{
				
			}
		};
		
		class I3DPhysicsComponent : public Core::IComponent
		{
		public:
			virtual ~I3DPhysicsComponent(){}
			DECLARE_NAMED_INTERFACE(I3DPhysicsComponent);
			//----------------------------------------------------------
			/// Set Position
			///
			/// @param Position of the object in the physics world
			//----------------------------------------------------------
			virtual void SetPosition(Core::CVector3 invPosition) = 0;
			//----------------------------------------------------------
			/// Get Position
			///
			/// @return Position of the object in the physics world
			//----------------------------------------------------------
			virtual const Core::CVector3 GetPosition() const = 0;
            //----------------------------------------------------------
            /// Rotate To
            ///
            /// Set the rotation of the physics object in the physics
            /// world
            ///
            /// @param Euler axis
            /// @param Angle in radians
            //----------------------------------------------------------
            virtual void RotateTo(Core::CVector3 invAxis, f32 infAngle) = 0;
            //----------------------------------------------------------
			/// Get Orientation
			///
			/// @return Orientation of the object in the physics world
			//----------------------------------------------------------
			virtual const Core::CQuaternion GetOrientation() const = 0;
            //----------------------------------------------------------
			/// Get Transform
			///
			/// @return Transform of the object in the physics world
			//----------------------------------------------------------
			virtual const Core::CTransform GetTransform() const = 0;
            //----------------------------------------------------------
			/// Get Transform
			///
			/// @return Transform of the object in the physics world
			//----------------------------------------------------------
			virtual void SetTransform(Core::CTransform invTransform) = 0;                 
			//----------------------------------------------------------
			/// Set Linear Velocity
			///
			/// Apply linear velocity to the physics object
			///
			/// @param Velocity vector
			//----------------------------------------------------------
			virtual void SetLinearVelocity(Core::CVector3 invVelocity) = 0;
			//----------------------------------------------------------
			/// Get Linear Velocity
			///
			/// @return Velocity vector
			//----------------------------------------------------------
			virtual const Core::CVector3 GetLinearVelocity() = 0;
			//----------------------------------------------------------
			/// Get Velocity In Local Point
			///
			/// @return Velocity vector
			//----------------------------------------------------------
			virtual const Core::CVector3 GetVelocityInLocalPoint(Core::CVector3 invPoint) = 0;            
			//-------------------------------------------------------------------------------
			/// Set Angular Velocity
			///
			/// @param Velocity vector
			//-------------------------------------------------------------------------------
			virtual void SetAngularVelocity(Core::CVector3 invVelocity) = 0;
			//-------------------------------------------------------------------------------
			/// Get Angular Velocity
			///
			/// @return Velocity vector
			//-------------------------------------------------------------------------------
			virtual const Core::CVector3 GetAngularVelocity() = 0;
			//-------------------------------------------------------------------------------
			/// Apply Central Impulse
			///
			/// Apply impulse to the centre of mass of the object
			///
			/// @param Force vector
			//-------------------------------------------------------------------------------
			virtual void ApplyCentralImpulse(Core::CVector3 invForce) = 0;            
			//-------------------------------------------------------------------------------
			/// Apply Impulse
			///
			/// Apply impulse to the relative positon of the object
			///
			/// @param Force vector
			/// @param Point of impact relative to origin of object
			//-------------------------------------------------------------------------------
			virtual void ApplyImpulse(Core::CVector3 invForce, Core::CVector3 invImpact) = 0;
			//-------------------------------------------------------------------------------
			/// Apply Torque Impulse
			///
			/// Apply torque impulse to the object
			///
			/// @param Force vector
			//-------------------------------------------------------------------------------
			virtual void ApplyTorqueImpulse(Core::CVector3 invForce) = 0;            
			//-------------------------------------------------------------------------------
			/// Set Sleeping Thresholds
			///
			/// Set the linear and angular energy threshold beneath which the object
			/// will sleep
			///
			/// @param Linear threshold
			/// @param Angular threshold
			//-------------------------------------------------------------------------------
			virtual void SetSleepingThresholds(f32 infLinearThresh, f32 infAngularThresh) = 0;
			//-------------------------------------------------------------------------------
			/// Set Sleeping
			///
			/// @param Whether to sleep the physics object
			//-------------------------------------------------------------------------------
			virtual void SetSleeping(bool inbIsSleeping) = 0;
			//-------------------------------------------------------------------------------
			/// Set Collision Mask
			///
			/// @param Mask of objects to test collisions against
			//-------------------------------------------------------------------------------
			virtual void SetCollisionMask(CollisionMask3D ineMask) = 0;
			//-------------------------------------------------------------------------------
			/// Set Physics Material
			///
			/// @param Material that governs the restitution, dampning etc
			//-------------------------------------------------------------------------------
			virtual void SetPhysicsMaterial(moFlo::Physics3D::PhysicsMaterial insMaterial) = 0;
			//-------------------------------------------------------------------------------
			/// Get Physics Material
			///
			/// @return Material that governs the restitution, dampning etc
			//-------------------------------------------------------------------------------
			virtual const moFlo::Physics3D::PhysicsMaterial GetPhysicsMaterial() const = 0;
            //-------------------------------------------------------------------------------
            /// Set Damping
            /// 
            /// @param Linear Damping
            /// @param Angular Damping
            //-------------------------------------------------------------------------------
            virtual void SetDamping(f32 infLinearDamping, f32 infAngularDamping) = 0;
            //-------------------------------------------------------------------------------
            /// Get Linear Damping
            /// 
            /// @return Linear Damping
            //-------------------------------------------------------------------------------
            virtual const f32 GetLinearDamping() = 0;
            //-------------------------------------------------------------------------------
            /// Get Angular Damping
            /// 
            /// @return Angular Damping
            //-------------------------------------------------------------------------------
            virtual const f32 GetAngularDamping() = 0;
			//----------------------------------------------------------
			/// Update Position from Entity
			///
			/// @param Position of the object in the physics world
			//----------------------------------------------------------
			virtual void UpdatePositionFromEntity() = 0;                   
            
		};
	}
}

#endif