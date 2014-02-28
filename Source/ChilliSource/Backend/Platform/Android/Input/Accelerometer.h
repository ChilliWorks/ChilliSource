//
//  Accelerometer.h
//  moFlow
//
//  Created by Ian Copland on 10/06/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#ifndef MOFLO_PLATFORM_ANDROID_ACCELEROMETER_H
#define MOFLO_PLATFORM_ANDROID_ACCELEROMETER_H

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Input/Accelerometer/Accelerometer.h>

namespace ChilliSource
{
	namespace Android
	{
		//======================================================
		/// Accelerometer
		///
		/// Listens to and stores the acceleration read from the
		/// hardware accelerometer.
		//======================================================
		class CAccelerometer : public Input::Accelerometer
		{
		public:
			//------------------------------------------------
			/// Constructor
			//------------------------------------------------
			CAccelerometer();
			//----------------------------------------------------
			/// Is Updating
			///
			/// @return whether or not the accelerometer is
            /// currently updating.
			//----------------------------------------------------
			bool IsUpdating() const override;
			//------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements
			/// the given interface.
			//------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------
			/// Start Updating
			///
			/// Start listening for accelerometer changes.
			//----------------------------------------------------
			void StartUpdating() override;
			//------------------------------------------------
			/// Get Acceleration
			///
			/// @return The acceleration applied to the device
			/// measured in "G"s.
			//------------------------------------------------
			Core::Vector3 GetAcceleration() const override;
			//----------------------------------------------------
			/// Stop Updating
			///
			/// Stop listening for accelerometer changes.
			//----------------------------------------------------
			void StopUpdating() override;

		protected:
			//------------------------------------------------
			/// On Acceleration Changed
			///
			/// Called whenever the acceleration changes.
			///
			/// @param The new acceleration.
			//------------------------------------------------
			void OnAccelerationChanged(const Core::Vector3& invAcceleration);

			Core::Vector3 mvAcceleration;
			AccelerometerJavaInterfacePtr mpAccelerometerJI;
			bool mbIsUpdating;
		};
	}
}

#endif
