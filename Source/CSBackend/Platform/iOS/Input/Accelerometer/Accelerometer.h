//
//  Accelerometer.h
//  Chilli Source
//  Created by Scott Downie on 15/11/2010
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

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CSBACKEND_IOS_INPUT_ACCELEROMETER_ACCELEROMETER_H_
#define _CSBACKEND_IOS_INPUT_ACCELEROMETER_ACCELEROMETER_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Input/Accelerometer/Accelerometer.h>

@class CMMotionManager;

namespace CSBackend
{
	namespace iOS
	{
        //------------------------------------------------------------
        /// The iOS backend implementation of the Accelerometer.
        ///
        /// @author S Downie
        //------------------------------------------------------------
		class Accelerometer final : public CSInput::Accelerometer
		{
		public:
            CS_DECLARE_NAMEDTYPE(Accelerometer);
            //----------------------------------------------------
            /// Returns whether or not this is supported on this
            /// device. This should be called prior to allocating
            /// the system.
            ///
            /// @author Ian Copland
            ///
            /// @return Whether or not the accelerometer is supported
            /// on this device.
            //----------------------------------------------------
            static bool IsSupportedByDevice();
            //----------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given ID.
            ///
            /// @author S McGaw
            ///
            /// @param The interface Id.
            ///
			/// @return Whether or not the interface is implemented.
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
			/// @return The acceleration in Gs.
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
            /// @author S Downie
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
            /// @author Ian Copland
            ///
            /// @param Time since last update
            //----------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //----------------------------------------------------
			/// Destroys the system immediately before systems
            /// are removed from the application.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
            void OnDestroy() override;
            
            bool m_isUpdating;
            CMMotionManager* m_motionManager;
            CSCore::Event<AccelerationUpdatedDelegate> m_accelerationUpdatedEvent;
		};
	}
}

#endif

#endif