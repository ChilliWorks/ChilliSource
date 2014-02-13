/*
 *  3DPhysicsComponentFactory.h
 *  moFloTest
 *
 *  Created by Scott Downie on 07/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PHYSICS3D_I3DPHYSICSCOMPONENTFACTORY_H
#define _MOFLO_PHYSICS3D_I3DPHYSICSCOMPONENTFACTORY_H

#include <moFlo/Physics3D/ForwardDeclarations.h>

#include <moFlo/Core/ComponentFactory.h>

namespace moFlo
{
	namespace Physics3D
	{
		class I3DPhysicsComponentFactory : public Core::IComponentFactory
		{
		public:
			DECLARE_NAMED_INTERFACE(I3DPhysicsComponentFactory);
			virtual ~I3DPhysicsComponentFactory(){}
			//-----------------------------------------------------------------------
			/// Create Ground Physics Component
			///
			/// Create a physical plane that acts as the ground
			///
			/// @param Normal to plane surface
			/// @param Offet from origin along normal
			/// @return Instantiated component
			//------------------------------------------------------------------------
			virtual moFlo::Physics3D::I3DPhysicsComponentPtr CreateGroundPhysicsComponent(moFlo::Core::CVector3 vNorm, f32 fOffset) const = 0;
			//-----------------------------------------------------------------------
			/// Create Sphere Physics Component
			///
			/// Create a physical, spherical bounding volume
			///
			/// @param Radius
			/// @return Instantiated component
			//------------------------------------------------------------------------
			virtual moFlo::Physics3D::I3DPhysicsComponentPtr CreateSpherePhysicsComponent(Physics3DComponentDesc& inDesc, f32 infRadius) const = 0;
			//-----------------------------------------------------------------------
			/// Create Box Physics Component
			///
			/// Create a physical, cubical bounding volume
			///
			/// @param Half dimensions
			/// @return Instantiated component
			//------------------------------------------------------------------------
			virtual moFlo::Physics3D::I3DPhysicsComponentPtr CreateBoxPhysicsComponent(Physics3DComponentDesc& inDesc, const Core::CVector3& invHalfExtents) const = 0;
		};
	}
	
}

#endif