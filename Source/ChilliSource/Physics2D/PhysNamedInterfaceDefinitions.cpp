/** 
 * File: PhysNamedInterfaceDefinitions.cpp
 * Date: 27 Sept 2010
 * Description: Defines the named interfaces for the moFlo Physics2D subsystem
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */


#include <moFlo/Physics2D/2DPhysicsSystem.h>
#include <moFlo/Physics2D/2DPhysicsComponent.h>
#include <moFlo/Physics2D/2DTriggerComponent.h>
#include <moFlo/Physics2D/2DCharacterComponent.h>
#include <moFlo/Physics2D/2DVehicleComponent.h>
#include <moFlo/Physics2D/2DPhysicsComponentFactory.h>

namespace moFlo
{

	namespace Physics2D
	{
		DEFINE_NAMED_INTERFACE(I2DPhysicsComponentFactory);
		DEFINE_NAMED_INTERFACE(I2DPhysicsSystem);
		DEFINE_NAMED_INTERFACE(I2DPhysicsComponent);
		DEFINE_NAMED_INTERFACE(I2DCharacterComponent);
		DEFINE_NAMED_INTERFACE(I2DTriggerComponent);
		DEFINE_NAMED_INTERFACE(I2DVehicleComponent);

		bool GroupPartOfGroupMask(GroupIDType inGroup, GroupIDMaskType inGroupMask){
			return ((1<<inGroup) & inGroupMask) != 0;
		}

	}

}