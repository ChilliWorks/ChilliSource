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

#ifndef _MOFLO_PHYSICS2D_2DTRIGGERCOMPONENT_H_
#define _MOFLO_PHYSICS2D_2DTRIGGERCOMPONENT_H_

#include <moFlo/Physics2D/ForwardDeclarations.h>
#include <moFlo/Core/Component.h>
#include <moFlo/Core/FastDelegate.h>


namespace moFlo
{
	namespace Physics2D
	{
		
		///I2DTriggerComponent is a component which does not interact physically with other physics objects but generates notifications when it collides with them
		class I2DTriggerComponent : public Core::IComponent
		{
		public:
			DECLARE_NAMED_INTERFACE(I2DTriggerComponent);
			
			virtual ~I2DTriggerComponent(){}

			
			/// Setting the position with this method is only valid for an object
			/// which is the first physics component attached to an entity.
			virtual void SetPosition(const Core::CVector2 & invPosition) = 0;
			virtual const Core::CVector2 & GetPosition() const = 0;
			
			/// Position offset is used to position this object relative to another
			/// physics object if this is not the first physics component attached to an entity
			virtual void SetPositionOffset(const Core::CVector2 & invPosition) = 0;
			virtual const Core::CVector2 & GetPositionOffset() const = 0;
			
			virtual void SetAngle(const f32 infAngle) = 0;
			virtual f32 GetAngle() const = 0;
						
			virtual void SetTriggerableByMask(GroupIDMaskType GroupIDMask) = 0;
			virtual GroupIDMaskType GetTriggerableByMask() const = 0;
			
			virtual u32 GetTouchingObjectsCount() = 0;
			
			typedef fastdelegate::FastDelegate2<I2DTriggerComponent*, Core::IComponent*> TriggerDelegate;
			
			virtual void AddOnTriggerDelegate(TriggerDelegate inDelegate) = 0;
			virtual void RemoveOnTriggerDelegate(TriggerDelegate inDelegate) = 0;
			
			virtual void AddOnUntriggerDelegate(TriggerDelegate inDelegate) = 0;
			virtual void RemoveOnUntriggerDelegate(TriggerDelegate inDelegate) = 0;

			
		};
		typedef SHARED_PTR<I2DTriggerComponent> I2DTriggerComponentPtr;
	}
}

#endif