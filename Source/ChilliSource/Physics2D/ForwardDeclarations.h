/** 
 * File: ForwardDeclarations.h
 * Date: 27 Sept 2010
 * Description: Declares types and typedefs used by moFlo's Physics2D interface
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PHYSICS2D_FORWARDDECLARATIONS_H
#define _MOFLO_PHYSICS2D_FORWARDDECLARATIONS_H

namespace moFlo
{
	namespace Physics2D
	{
		class I2DPhysicsSystem;
		
		class I2DPhysicsComponentFactory;
		
		class I2DPhysicsComponent; /// 
		class I2DCharacterComponent; /// A component which collides with other physics ones but cannot, 
		class I2DTriggerComponent; /// An object which does not move physically
		class I2DVehicleComponent;
		
		struct PhysicsMaterial;
		
		enum ComponentBehaviour
		{
			PHYS_BEHAVIOUR_DYNAMIC,
			PHYS_BEHAVIOUR_KINEMATIC,
			PHYS_BEHAVIOUR_STATIC
		};
		
		//Describes different stages in a collision event
		
		enum CollisionStages
		{
			COLLISION_BEGIN = 0x1,
			COLLISION_END = 0x2
		};
		
		typedef u8 GroupIDType;
		const GroupIDType DefaultGroup = 1;

		typedef u16 GroupIDMaskType;
		const GroupIDType MaxGroupIndex = (sizeof(GroupIDMaskType) * 8) - 1;
		const GroupIDMaskType DefaultMask = 0xFFFF;
		
		bool GroupPartOfGroupMask(GroupIDType inGroup, GroupIDMaskType inGroupMask);
		
		typedef SHARED_PTR<I2DPhysicsComponent> I2DPhysicsComponentPtr;
		typedef WEAK_PTR<I2DPhysicsComponent> I2DPhysicsComponentWeakPtr;
	}
}

#endif