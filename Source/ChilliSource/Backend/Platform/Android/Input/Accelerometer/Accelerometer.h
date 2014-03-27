//
//  Accelerometer.h
//  Chilli Source
//
//  Created by I Copland on 10/06/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_INPUT_ACCELEROMETER_ACCELEROMETER_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_INPUT_ACCELEROMETER_ACCELEROMETER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Input/Accelerometer/Accelerometer.h>

namespace ChilliSource
{
	namespace Android
	{
		//------------------------------------------------------------
		/// Listens to and stores the acceleration read from the
		/// hardware accelerometer.
		///
		/// @author I Copland
		//------------------------------------------------------------
		class Accelerometer final : public Input::Accelerometer
		{
		public:
			CS_DECLARE_NAMEDTYPE(Accelerometer);
			//----------------------------------------------------
			/// @author I Copland
			///
			/// @return whether or not this object implements
			/// the given interface.
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
			/// @return The acceleration applied to the device
			/// measured in "G"s.
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

		private:
            friend Input::AccelerometerUPtr Input::Accelerometer::Create();
			//----------------------------------------------------
			/// Constructor. Declared private to force the use of
            /// the factory method.
			///
			/// @author I Copland
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
            /// @author I Copland
			//----------------------------------------------------
            void OnDestroy() override;
			//------------------------------------------------
			/// Called whenever the acceleration changes.
			///
			/// @author I Copland
			///
			/// @param The new acceleration.
			//------------------------------------------------
			void OnAccelerationChanged(const Core::Vector3& in_acceleration);

			Core::Vector3 m_acceleration;
			AccelerometerJavaInterfaceSPtr m_accelerometerJI;
			bool m_isUpdating;
			Core::Event<AccelerationUpdatedDelegate> m_accelerationUpdatedEvent;
		};
	}
}

#endif
