//
//  Accelerometer.h
//  Chilli Source
//
//  Created by S McGaw on 15/11/2010
//  Copyright ©2010 Tag Games Limited. All rights reserved
//

#ifndef _CHILLISOURCE_INPUT_ACCELEROMETER_ACCELEROMETER_H_
#define _CHILLISOURCE_INPUT_ACCELEROMETER_ACCELEROMETER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>

namespace ChilliSource
{
	namespace Input
	{
		//------------------------------------------------------------
		/// Listens to and stores the acceleration read from the
        /// hardware accelerometer.
        ///
        /// @author S McGaw
		//------------------------------------------------------------
		class Accelerometer : public Core::AppSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(Accelerometer);
            //----------------------------------------------------
			/// A delegate for receiving acceleratation updates
            /// from the accerometer.
            ///
            /// @author I Copland
            ///
            /// @param The new acceleration. This wont neccessarily
            /// have changed since the last update.
			//----------------------------------------------------
			typedef std::function<void(const Core::Vector3&)> AccelerationUpdatedDelegate;
            //----------------------------------------------------
			/// Creates a new platform specific instance of the
            /// system.
            ///
            /// @author I Copland
			//----------------------------------------------------
			static AccelerometerUPtr Create();
			//----------------------------------------------------
            /// @author I Copland
			///
			/// @return whether or not the accelerometer is
            /// currently updating.
			//----------------------------------------------------
			virtual bool IsUpdating() const = 0;
            //----------------------------------------------------
			/// Start listening for accelerometer changes.
            ///
            /// @author I Copland
			//----------------------------------------------------
			virtual void StartUpdating() = 0;
			//----------------------------------------------------
            /// @author I Copland
            ///
			/// @return The acceleration applied to the device
			/// measured in "G"s.
			//----------------------------------------------------
			virtual Core::Vector3 GetAcceleration() const = 0;
			//----------------------------------------------------
			/// @author I Copland
			///
			/// @return An event that is invoked every time the
			/// acceleration is updated. The acceleration will not
			/// necessarily have changed between updates.
			//----------------------------------------------------
			virtual Core::IConnectableEvent<AccelerationUpdatedDelegate>& GetAccelerationUpdatedEvent() = 0;
			//----------------------------------------------------
			/// Stop listening for accelerometer changes.
            ///
            /// @author I Copland
			//----------------------------------------------------
			virtual void StopUpdating() = 0;
			//----------------------------------------------------
			/// Destructor
            ///
            /// @author I Copland
			//----------------------------------------------------
			virtual ~Accelerometer() {}
		};
	}
}

#endif
