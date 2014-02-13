/** 
 * File: I2DPhysicsComponentFactory.h
 * Date: 27 Sept 2010
 * Description: Defines the interface for a factory which can produce I2DPhysicsComponent, I2DCharacterComponent and I2DTriggerComponent objects
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PHYSICS2D_I2DPHYSICSCOMPONENTFACTORY_H
#define _MOFLO_PHYSICS2D_I2DPHYSICSCOMPONENTFACTORY_H

#include <moFlo/Physics2D/ForwardDeclarations.h>
#include <moFlo/Physics2D/PhysicsMaterial.h>
#include <moFlo/Core/ComponentFactory.h>
#include <moFlo/Core/Math/Vector2.h>
#include <vector>

#include <moFlo/Physics2D/2DPhysicsComponent.h>
#include <moFlo/Physics2D/2DCharacterComponent.h>
#include <moFlo/Physics2D/2DTriggerComponent.h>
#include <moFlo/Physics2D/2DVehicleComponent.h>

namespace moFlo
{

	namespace Physics2D
	{
		
		//A standard set of data about a physical object (not all component types use all variables)
		struct PhysicsComponentInfo 
		{
			Core::CVector2 vPosition; //< Initial position of the physics object
			f32 fAngle; //< Initial clockwise rotation in radians of the object
			Core::CVector2 vLinearVelocity; //<Initial linear velocity of the object
			f32 fAngularVelocity; //<Initial angular velocity of the object
			f32 fDensity; //<Object density
			ComponentBehaviour eBehaviour; //<What variety of object this should be simmed as
			PhysicsMaterial sMaterial; //< Physical properties of the object
			GroupIDType Group; //<Physics group this belongs to (DefaultGroup - MaxGroup)
			
			///Default 
			PhysicsComponentInfo()
			: fAngle(0),fAngularVelocity(0), fDensity(0), eBehaviour(PHYS_BEHAVIOUR_DYNAMIC), Group(DefaultGroup)
			{
				
				
			}
			
		};
		
		class I2DPhysicsComponentFactory : public Core::IComponentFactory
		{
		public:
			DECLARE_NAMED_INTERFACE(I2DPhysicsComponentFactory);
			///Produce an I2DPhysicsComponent with the given properties and with a box shape of the given dimensions
			///@param 
			///@return The created component
			virtual I2DPhysicsComponentPtr CreateBoxPhysicsComponent(const PhysicsComponentInfo & insPhysicsInfo, const Core::CVector2 & invDimensions) const = 0;
			///Produce an I2DPhysicsComponent with the given properties and with a circular shape of the given radius
			virtual I2DPhysicsComponentPtr CreateCirclePhysicsComponent(const PhysicsComponentInfo & insPhysicsInfo, f32 infRadius) const = 0;
			///Produce an I2DPhysicsComponent with the given properties and with a convex polygonal shape defined by the passed vertices
			virtual I2DPhysicsComponentPtr CreatePolygonPhysicsComponent(const PhysicsComponentInfo & insPhysicsInfo, const DYNAMIC_ARRAY<Core::CVector2> & inPoints) const = 0;
			
			///Produce an I2DCharacterComponent with the given properties and with a box shape of the given dimensions
			virtual I2DCharacterComponentPtr CreateBoxCharacterComponent(const PhysicsComponentInfo & insPhysicsInfo, const Core::CVector2 & invDimensions) const = 0;
			///Produce an I2DCharacterComponent with the given properties and with a box shape of the given dimensions
			virtual I2DCharacterComponentPtr CreateCapsuleCharacterComponent(const PhysicsComponentInfo & insPhysicsInfo, f32 infWidth, f32 infHeight) const = 0;

			virtual I2DTriggerComponentPtr CreateBoxTriggerComponent(const PhysicsComponentInfo & insPhysicsInfo, const Core::CVector2 & invDimensions) const = 0;
			virtual I2DTriggerComponentPtr CreateCircleTriggerComponent(const PhysicsComponentInfo & insPhysicsInfo, f32 infRadius) const = 0;
			virtual I2DTriggerComponentPtr CreatePolygonTriggerComponent(const PhysicsComponentInfo & insPhysicsInfo, const DYNAMIC_ARRAY<Core::CVector2> & inPoints) const = 0;

			virtual I2DVehicleComponentPtr CreateVehicleWithDescription(const VehicleComponentDescription & insVehicleDesc) const = 0;
		};
	}
	
}

#endif