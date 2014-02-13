/** 
 * File: 2DPhysicsComponent.h
 * Date: 27 Sept 2010
 * Description: Interface for a physically simulated 2D object
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PHYSICS2D_2DPHYSICSCOMPONENT_H_
#define _MOFLO_PHYSICS2D_2DPHYSICSCOMPONENT_H_

#include <ChilliSource/Physics2D/ForwardDeclarations.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Component.h>
#include <ChilliSource/Core/FastDelegate.h>
#include <ChilliSource/Core/Math/Shapes.h>

namespace moFlo
{
	namespace Physics2D
	{
		class I2DPhysicsComponent : public Core::IComponent
		{
		public:
			virtual ~I2DPhysicsComponent(){}
			DECLARE_NAMED_INTERFACE(I2DPhysicsComponent);
			
			virtual moFlo::Core::Rectangle GetAABB() const = 0;
			
			virtual void SetPosition(const Core::CVector2 & invPosition) = 0;
			virtual const Core::CVector2 & GetPosition() const = 0;
			
			virtual void SetAngle(const f32 infAngle) = 0;
			virtual f32 GetAngle() const = 0;
			
			virtual void SetLinearVelocity(const Core::CVector2 & invLinearVelocity) = 0;
			virtual const Core::CVector2 & GetLinearVelocity() = 0;
			
			virtual void SetAngularVelocity(f32 infAngularVelocity) = 0;
			virtual f32 GetAngularVelocity() = 0;

			virtual void ApplyForce(const Core::CVector2 & invForce, const Core::CVector2 & invPoint) = 0;
			virtual void ApplyTorque(f32 torque) = 0;
			
			virtual void ApplyLinearImpulse(const Core::CVector2 & invImpulse, const Core::CVector2 & invPoint) = 0;
			virtual void ApplyAngularImpulse(f32 invImpulse) = 0;
			
			virtual f32 GetMass() const = 0;
			virtual f32 GetInertia() const = 0;
			
			virtual void SetPhysicsMaterial(const PhysicsMaterial & insMaterial) = 0;
			virtual const PhysicsMaterial GetPhysicsMaterial() const = 0;
			
			///The first parameter in a collision delegate call is the component the callback's attached to, the second is the object it's colliding with
			typedef fastdelegate::FastDelegate2<Core::IComponent*, Core::IComponent*> CollisionDelegate;
			
			virtual void AddCallbackForCollisionWithGroup(moFlo::Physics2D::GroupIDType inGroupID, u32 inCollisionStagesMask, CollisionDelegate inCallbackFunction) = 0;
			virtual void RemoveCallbackForCollisionWithGroup(moFlo::Physics2D::GroupIDType inGroupID, u32 inCollisionStagesMask, CollisionDelegate inCallbackFunction) = 0;
															 
			virtual void SetCollisionEnabled(bool inbValue) = 0;
			virtual bool IsCollisionEnabled() const = 0;
			
			virtual void SetFixedRotation(bool inbValue) = 0;
			virtual bool HasFixedRotation() const = 0;
			
			virtual void SetSleepingAllowed(bool flag) = 0;
			virtual bool IsSleepingAllowed() const = 0;
			
			virtual void SetAwake(bool flag) = 0;
			virtual bool IsAwake() const = 0;
		};
	}
}

#endif