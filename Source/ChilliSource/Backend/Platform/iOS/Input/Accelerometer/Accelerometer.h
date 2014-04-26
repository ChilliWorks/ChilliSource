//
//  Accelerometer.h
//  Chilli Source
//
//  Created by S Downie on 15/11/2010
//  Copyright ©2010 Tag Games Limited. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_IOS_INPUT_ACCELEROMETER_ACCELEROMETER_H_
#define _CHILLISOURCE_BACKEND_IOS_INPUT_ACCELEROMETER_ACCELEROMETER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Input/Accelerometer/Accelerometer.h>

@class CMMotionManager;

namespace ChilliSource
{
	namespace iOS
	{
        //------------------------------------------------------------
        /// The iOS backend implementation of the Accelerometer.
        ///
        /// @author S Downie
        //------------------------------------------------------------
		class Accelerometer final : public Input::Accelerometer
		{
		public:
            CS_DECLARE_NAMEDTYPE(Accelerometer);
            //----------------------------------------------------
            /// Returns whether or not this is supported on this
            /// device. This should be called prior to allocating
            /// the system.
            ///
            /// @author I Copland
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
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------
            /// @author I Copland
			///
			/// @return whether or not the accelerometer is
            /// currently updating.
			//----------------------------------------------------
			bool IsUpdating() const override;
			//----------------------------------------------------
			/// Start listening for accelerometer changes.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void StartUpdating() override;
			//----------------------------------------------------
			/// @author I Copland
            ///
			/// @return The acceleration in Gs.
			//----------------------------------------------------
            Core::Vector3 GetAcceleration() const override;
            //----------------------------------------------------
			/// @author I Copland
			///
			/// @return An event that is invoked every time the
			/// acceleration is updated. The acceleration will not
			/// necessarily have changed between updates.
			//----------------------------------------------------
			Core::IConnectableEvent<AccelerationUpdatedDelegate>& GetAccelerationUpdatedEvent() override;
			//----------------------------------------------------
			/// Stop listening for accelerometer changes.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void StopUpdating() override;
		protected:
            friend Input::AccelerometerUPtr Input::Accelerometer::Create();
            
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
            /// @author I Copland
			//----------------------------------------------------
            void OnInit() override;
            //----------------------------------------------------
            /// @author I Copland
            ///
            /// @param Time since last update
            //----------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //----------------------------------------------------
			/// Destroys the system immediately before systems
            /// are removed from the application.
            ///
            /// @author I Copland
			//----------------------------------------------------
            void OnDestroy() override;
            
            bool m_isUpdating;
            CMMotionManager* m_motionManager;
            Core::Event<AccelerationUpdatedDelegate> m_accelerationUpdatedEvent;
		};
	}
}

#endif
