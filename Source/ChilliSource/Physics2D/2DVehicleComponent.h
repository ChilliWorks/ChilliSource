/** 
 * File: 2DVehicleComponent.h
 * Date: 20/10/2010
 * Description: Defines the interface for a 
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - MoFlow
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PHYSICS2D_2DVEHICLECOMPONENT_H_
#define _MOFLO_PHYSICS2D_2DVEHICLECOMPONENT_H_

#include <ChilliSource/Core/Component.h>
#include <ChilliSource/Core/Math/Vector2.h>

namespace moFlo
{
	namespace Physics2D
	{
		
		struct VehicleComponentDescription
		{
			Core::CVector2 vBodySize;
			f32 fLeftWheelSize;
			Core::CVector2 vLeftWheelOffset; //<Offset from main body's centre
			f32 fRightWheelSize;
			Core::CVector2 vRightWheelOffset; //<Offset from main body's centre

			f32 fDensity;
			f32 fAngle;
			Core::CVector2 vPosition;
		};
		
		///A box with wheels
		class I2DVehicleComponent : public Core::IComponent
		{
		public:
			DECLARE_NAMED_INTERFACE(I2DVehicleComponent);
			virtual void SetPosition(const Core::CVector2 & invPosition) = 0;
			virtual const Core::CVector2 & GetPosition() const = 0;
			
			virtual void SetAngle(const f32 infAngle) = 0;
			virtual f32 GetAngle() const = 0;
			
			virtual const moFlo::Core::CVector2 GetLeftWheelPosition() const = 0;
			virtual const moFlo::Core::CVector2 GetRightWheelPosition() const = 0;
			virtual f32 GetLeftWheelAngle() const = 0;
			virtual f32 GetRightWheelAngle() const = 0;
			
			virtual void SetWheelVelocity(const f32 infValue) = 0;
			virtual f32 GetWheelVelocity() const = 0;
		};
		typedef SHARED_PTR<I2DVehicleComponent> I2DVehicleComponentPtr;
	}
}

#endif