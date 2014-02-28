//
//  Accelerometer.h
//  moFlow
//
// Created by Stuat McGaw on 15/11/2010
// Copyright �2010 Tag Games Limited. All rights reserved
//

#ifndef MOFLOW_INPUT_ACCELEROMETER_H
#define MOFLOW_INPUT_ACCELEROMETER_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Input/Base/InputDevice.h>

#include <functional>

namespace ChilliSource
{
	namespace Input
	{
		//======================================================
		/// Accelerometer
		///
		/// Listens to and stores the acceleration read from the
		/// hardware accelerometer.
		//======================================================
		class Accelerometer : public InputDevice
		{
		public:
			CS_DECLARE_NAMEDTYPE(Accelerometer);
			//----------------------------------------------------
			/// Delegates
			//----------------------------------------------------
			typedef std::function<void(const Core::Vector3&)> AccelerationUpdatedDelegate;
			//----------------------------------------------------
			/// Constructor
			//----------------------------------------------------
			Accelerometer();
			//----------------------------------------------------
			/// Is Updating
			///
			/// @return whether or not the accelerometer is
            /// currently updating.
			//----------------------------------------------------
			virtual bool IsUpdating() const = 0;
            //----------------------------------------------------
			/// Start Updating
			///
			/// Start listening for accelerometer changes.
			//----------------------------------------------------
			virtual void StartUpdating() = 0;
			//----------------------------------------------------
			/// Get Acceleration
			///
			/// @return The acceleration applied to the device
			/// measured in "G"s.
			//----------------------------------------------------
			virtual Core::Vector3 GetAcceleration() const = 0;
			//----------------------------------------------------
			/// Get Acceleration Updated Event
			///
			/// @return An event that is invoked every time the
			/// acceleration is updated. The acceleration will not
			/// necessarily have changed between updates.
			//----------------------------------------------------
			Core::IConnectableEvent<AccelerationUpdatedDelegate>& GetAccelerationUpdatedEvent();
			//----------------------------------------------------
			/// Stop Updating
			///
			/// Stop listening for accelerometer changes.
			//----------------------------------------------------
			virtual void StopUpdating() = 0;
			//----------------------------------------------------
			/// Destructor
			//----------------------------------------------------
			virtual ~Accelerometer();
		protected:
			Core::Event<AccelerationUpdatedDelegate> mAccelerationUpdatedEvent;
		};
	}
}

#endif
