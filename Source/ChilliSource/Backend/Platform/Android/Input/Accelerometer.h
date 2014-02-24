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
		class CAccelerometer : public Input::IAccelerometer
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
			bool IsUpdating() const;
			//------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements
			/// the given interface.
			//------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------
			/// Start Updating
			///
			/// Start listening for accelerometer changes.
			//----------------------------------------------------
			void StartUpdating();
			//------------------------------------------------
			/// Get Acceleration
			///
			/// @return The acceleration applied to the device
			/// measured in "G"s.
			//------------------------------------------------
			const Core::CVector3& GetAcceleration() const;
			//----------------------------------------------------
			/// Stop Updating
			///
			/// Stop listening for accelerometer changes.
			//----------------------------------------------------
			void StopUpdating();
			//------------------------------------------------
			/// Destructor
			//------------------------------------------------
			virtual ~CAccelerometer();
		protected:
			//------------------------------------------------
			/// On Acceleration Changed
			///
			/// Called whenever the acceleration changes.
			///
			/// @param The new acceleration.
			//------------------------------------------------
			void OnAccelerationChanged(const Core::CVector3& invAcceleration);

			Core::CVector3 mvAcceleration;
			AccelerometerJavaInterfacePtr mpAccelerometerJI;
			bool mbIsUpdating;
		};
	}
}

#endif
