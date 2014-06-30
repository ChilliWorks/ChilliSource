//
//  Accelerometer.h
//  Chilli Source
//  Created by S McGaw on 15/11/2010
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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
            /// @author Ian Copland
            ///
            /// @param The new acceleration. This wont neccessarily
            /// have changed since the last update.
			//----------------------------------------------------
			typedef std::function<void(const Core::Vector3&)> AccelerationUpdatedDelegate;
            //----------------------------------------------------
			/// Creates a new platform specific instance of the
            /// system.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			static AccelerometerUPtr Create();
			//----------------------------------------------------
            /// @author Ian Copland
			///
			/// @return whether or not the accelerometer is
            /// currently updating.
			//----------------------------------------------------
			virtual bool IsUpdating() const = 0;
            //----------------------------------------------------
			/// Start listening for accelerometer changes.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			virtual void StartUpdating() = 0;
			//----------------------------------------------------
            /// @author Ian Copland
            ///
			/// @return The acceleration applied to the device
			/// measured in "G"s.
			//----------------------------------------------------
			virtual Core::Vector3 GetAcceleration() const = 0;
			//----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return An event that is invoked every time the
			/// acceleration is updated. The acceleration will not
			/// necessarily have changed between updates.
			//----------------------------------------------------
			virtual Core::IConnectableEvent<AccelerationUpdatedDelegate>& GetAccelerationUpdatedEvent() = 0;
			//----------------------------------------------------
			/// Stop listening for accelerometer changes.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			virtual void StopUpdating() = 0;
			//----------------------------------------------------
			/// Destructor
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			virtual ~Accelerometer() {}
		};
	}
}

#endif
