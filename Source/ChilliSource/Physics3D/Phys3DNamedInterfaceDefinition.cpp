/*
 *  NamedInterfaceDefinition.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 07/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Physics3D/3DPhysicsSystem.h>
#include <ChilliSource/Physics3D/3DPhysicsComponent.h>
#include <ChilliSource/Physics3D/3DPhysicsComponentFactory.h>

namespace moFlo
{
	
	namespace Physics3D
	{
		DEFINE_NAMED_INTERFACE(I3DPhysicsSystem);
		DEFINE_NAMED_INTERFACE(I3DPhysicsComponent);
		DEFINE_NAMED_INTERFACE(I3DPhysicsComponentFactory);
	}
}