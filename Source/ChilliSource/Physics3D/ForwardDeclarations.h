/*
 *  ForwardDeclarations.h
 *  moFloTest
 *
 *  Created by Scott Downie on 07/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PHYSICS3D_FORWARDDECLARATIONS_H
#define _MOFLO_PHYSICS3D_FORWARDDECLARATIONS_H

namespace moFlo
{
	namespace Physics3D
	{
		class I3DPhysicsSystem;
		class I3DPhysicsComponentFactory;
		class I3DPhysicsComponent; 
		
		struct PhysicsMaterial;
		struct Physics3DComponentDesc;
		
		typedef SHARED_PTR<I3DPhysicsComponent> I3DPhysicsComponentPtr;
		typedef WEAK_PTR<I3DPhysicsComponent> I3DPhysicsComponentWeakPtr;
	}
}

#endif