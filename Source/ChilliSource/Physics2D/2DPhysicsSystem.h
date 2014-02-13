/** 
 * File: I2DPhysicsSystem.h
 * Date: 27 Sept 2010
 * Description: Defines the interface for a moFlo 2D physics system
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PHYSICS2D_I2DPHYSICSSYSTEM_H
#define _MOFLO_PHYSICS2D_I2DPHYSICSSYSTEM_H

#include <ChilliSource/Physics2D/ForwardDeclarations.h>
#include <ChilliSource/Core/System.h>
#include <ChilliSource/Core/SystemConcepts.h>

namespace moFlo
{
	namespace Physics2D
	{
		class I2DPhysicsSystem : public Core::ISystem, public Core::IUpdateable, public Core::IComponentProducer
		{
		public:
			
			virtual ~I2DPhysicsSystem(){};
			DECLARE_NAMED_INTERFACE(I2DPhysicsSystem);
			//Sets the strength and direction of gravity applied to physical objects in the system
			virtual void SetGravity(const Core::CVector2 & value) = 0;
			
			//Sets the strength and direction of gravity applied to physical objects in the system
			virtual const Core::CVector2 & GetGravity() const = 0;
			
			virtual u32 GetComponentCount() const = 0;

			
			virtual void SetTimeScaleFactor(f32 infFactor) = 0;
			virtual float GetTimeScaleFactor() const = 0;
		};
	}	
}

#endif