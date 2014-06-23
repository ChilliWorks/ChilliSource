//
//  Accelerometer.h
//  Chilli Source
//  Created by Ian Copland on 10/06/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_INPUT_ACCELEROMETER_ACCELEROMETER_H_
#define _CSBACKEND_PLATFORM_ANDROID_INPUT_ACCELEROMETER_ACCELEROMETER_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Input/Accelerometer/Accelerometer.h>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------
		/// Listens to and stores the acceleration read from the
		/// hardware accelerometer.
		///
		/// @author Ian Copland
		//------------------------------------------------------------
		class Accelerometer final : public CSInput::Accelerometer
		{
		public:
			CS_DECLARE_NAMEDTYPE(Accelerometer);
			//----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return whether or not this object implements
			/// the given interface.
			//----------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return whether or not the accelerometer is
            /// currently updating.
			//----------------------------------------------------
			bool IsUpdating() const override;
			//----------------------------------------------------
			/// Start listening for accelerometer changes.
			///
			/// @author Ian Copland
			//----------------------------------------------------
			void StartUpdating() override;
			//----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The acceleration applied to the device
			/// measured in "G"s.
			//----------------------------------------------------
			CSCore::Vector3 GetAcceleration() const override;
			//----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return An event that is invoked every time the
			/// acceleration is updated. The acceleration will not
			/// necessarily have changed between updates.
			//----------------------------------------------------
			CSCore::IConnectableEvent<AccelerationUpdatedDelegate>& GetAccelerationUpdatedEvent() override;
			//----------------------------------------------------
			/// Stop listening for accelerometer changes.
			///
			/// @author Ian Copland
			//----------------------------------------------------
			void StopUpdating() override;

		private:
            friend CSInput::AccelerometerUPtr CSInput::Accelerometer::Create();
			//----------------------------------------------------
			/// Constructor. Declared private to force the use of
            /// the factory method.
			///
			/// @author Ian Copland
			//----------------------------------------------------
			Accelerometer();
            //----------------------------------------------------
			/// Initialises the system. This is called at a time
            /// when all systems have already been added.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
            void OnInit() override;
            //----------------------------------------------------
            /// Resume accelerometer events if enabled
            ///
            /// @author S Downie
			//----------------------------------------------------
            void OnResume() override;
            //----------------------------------------------------
            /// Prevents accelerometer events when the app is
            /// backgrounded
            ///
            /// @author S Downie
			//----------------------------------------------------
            void OnSuspend() override;
            //----------------------------------------------------
			/// Destroys the system immediately before systems
            /// are removed from the application.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
            void OnDestroy() override;
			//------------------------------------------------
			/// Called whenever the acceleration changes.
			///
			/// @author Ian Copland
			///
			/// @param The new acceleration.
			//------------------------------------------------
			void OnAccelerationChanged(const CSCore::Vector3& in_acceleration);

			CSCore::Vector3 m_acceleration;
			AccelerometerJavaInterfaceSPtr m_accelerometerJI;
			bool m_isUpdating;
			CSCore::Event<AccelerationUpdatedDelegate> m_accelerationUpdatedEvent;
		};
	}
}

#endif

#endif