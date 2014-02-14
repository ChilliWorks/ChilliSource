/** 
 * File: Accelerometer.h
 * Date: 15/11/2010
 * Description:
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - MoFlow
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef MOFLOW_PLATFORM_IOS_ACCELEROMETER_H
#define MOFLOW_PLATFORM_IOS_ACCELEROMETER_H

#include <ChilliSource/Input/Accelerometer/Accelerometer.h>
#include <UIKit/UIKit.h>

namespace moFlo
{
	namespace iOSPlatform
	{
		class CAccelerometer : public Input::IAccelerometer
		{
		public:
			//----------------------------------------------------
			/// Constructor
			//----------------------------------------------------
			CAccelerometer();
            //----------------------------------------------------
			/// Is A
			///
			/// @return Whether this object is of given type
			//----------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //----------------------------------------------------
			/// Is Updating
			///
			/// @return whether or not the accelerometer is
            /// currently updating.
			//----------------------------------------------------
			bool IsUpdating() const;
			//----------------------------------------------------
			/// Start Updating
			///
			/// Start listening for accelerometer changes.
			//----------------------------------------------------
			void StartUpdating();
			//----------------------------------------------------
			/// Get Acceleration
			///
			/// @return The acceleration in Gs.
			//----------------------------------------------------
			const Core::CVector3& GetAcceleration() const;
			//----------------------------------------------------
			/// Stop Updating
			///
			/// Stop listening for accelerometer changes.
			//----------------------------------------------------
			void StopUpdating();
            //----------------------------------------------------
			/// Set Acceleration
			///
			/// @param The new acceleration.
			//----------------------------------------------------
			void SetAcceleration(const Core::CVector3& invAcceleration);
			//----------------------------------------------------
			/// Destructor
			//----------------------------------------------------
			~CAccelerometer();
		protected:
			Core::CVector3 mvAcceleration;
            bool mbIsUpdating;
		};
	}
}

extern "C"
{
	void InitAccelerometer();
	void SetAccelerometerUpdateFrequency(f32 infFrequency);
};

@interface moFloAccelerometerDelegate : NSObject <UIAccelerometerDelegate>
{
}
- (void)setUpdateFrequency:(f32) frequency;
- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration;
@end

#endif
