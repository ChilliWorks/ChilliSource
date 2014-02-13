/** 
 * File: PhysicsMaterial.h
 * Date: 27 Sept 2010
 * Description: Defines a physical material
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PHYSICS2D_PHYSICSMATERIAL_H
#define _MOFLO_PHYSICS2D_PHYSICSMATERIAL_H

namespace moFlo
{

	namespace Physics2D
	{
		
		struct PhysicsMaterial
		{
			f32 fLinearDamping; //<Governs how much an object's linear velocity will reduce with time
			f32 fAngularDamping; //<Governs how much an object's angular velocity will reduce with time
			f32 fFriction; //<Governs how much an object's speed is lost when it is in collision with another object
			f32 fRestitution; //< Ranged 0 - 1, at 0 collisions involving this object are completely inelastic, at 1 they are completely elastic
			
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
	}
	
}

#endif