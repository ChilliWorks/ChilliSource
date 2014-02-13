/** 
 * File: 2DCharacterComponent.h
 * Date: 27 Sept 2010
 * Description: Interface for a component representing a character in a 3D world
 * which will react to gravity, can be given a velocity and will collide physically.
 * It cannot have forces or impulses applied and will not rotate.
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PHYSICS2D_2DCHARACTERCOMPONENT_H_
#define _MOFLO_PHYSICS2D_2DCHARACTERCOMPONENT_H_

#include <moFlo/Physics2D/ForwardDeclarations.h>
#include <moFlo/Core/Component.h>
#include <moFlo/Core/FastDelegate.h>
#include <moFlo/Physics2D/2DSceneQueryer.h>

namespace moFlo
{
	namespace Physics2D
	{
		
		class I2DCharacterComponent : public Core::IComponent, public I2DSceneQueryer
		{
		public:
			virtual ~I2DCharacterComponent(){}
			DECLARE_NAMED_INTERFACE(I2DCharacterComponent);
			virtual void SetPosition(const Core::CVector2 & invPosition) = 0;
			virtual const Core::CVector2 & GetPosition() const = 0;
			
			virtual void SetAngle(const f32 infAngle) = 0;
			virtual f32 GetAngle() const = 0;
			
			virtual void SetVelocity(const Core::CVector2 & invVelocity) = 0;
			virtual const Core::CVector2 & GetVelocity() = 0;
	
			typedef fastdelegate::FastDelegate2<Core::IComponent*, Core::IComponent*> CollisionDelegate;
			
			virtual void AddCallbackForCollisionWithGroup(moFlo::Physics2D::GroupIDType GroupID, u32 CollisionStagesMask, CollisionDelegate CallbackFunction) = 0;
			virtual void RemoveCallbackForCollisionWithGroup(moFlo::Physics2D::GroupIDType GroupID, u32 CollisionStagesMask, CollisionDelegate CallbackFunction) = 0;
			
		};
		typedef SHARED_PTR<I2DCharacterComponent> I2DCharacterComponentPtr;
	}
}

#endif