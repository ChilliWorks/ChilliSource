//
//  AccelerometerJavaInterface.h
//  moFlow
//
//  Created by Ian Copland on 10/06/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//


#ifndef MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_ACCELEROMETERJAVAINTERFACE_H
#define MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_ACCELEROMETERJAVAINTERFACE_H

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterface.h>
#include <ChilliSource/Core/Main/FastDelegate.h>

namespace ChilliSource
{
	namespace Android
	{
		//======================================================
		/// Accelerometer Java Interface
		///
		/// A Java Interface for communicating with the Android
		/// accelerometer.
		//======================================================
		class CAccelerometerJavaInterface : public IJavaInterface
		{
		public:
			DECLARE_NAMED_INTERFACE();
			//------------------------------------------------
			/// Delegates
			//------------------------------------------------
			typedef fastdelegate::FastDelegate1<const Core::CVector3&> AccelerationChangedDelegate;
			//------------------------------------------------
			/// Constructor
			//------------------------------------------------
			CAccelerometerJavaInterface();
			//------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements
			/// the given interface.
			//------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//------------------------------------------------
			/// Is Available
			///
			/// @return whether or not the accelerometer is
			/// available on this device.
			//------------------------------------------------
			bool IsAvailable();
			//------------------------------------------------
			/// Start Listening
			///
			/// Start listening for acceleration changes from
			/// the accelerometer on device. If the accelerometer
			/// is not available this will do nothing.
			///
			/// @param The delegate which will receive the
			/// acceleration changes.
			//------------------------------------------------
			void StartListening(const AccelerationChangedDelegate& inAccelerationChangedDelegate);
			//------------------------------------------------
			/// Stop Listening
			///
			/// Stops listening for accelerations changes from
			/// the accelerometer.
			//------------------------------------------------
			void StopListening();
			//------------------------------------------------
			/// Update Acceleration
			///
			/// This is called by the Java Native Interface to
			/// inform of acceleration updates.
			///
			/// @param The x component of the acceleration.
			/// @param The y component of the acceleration.
			/// @param The z component of the acceleration.
			//------------------------------------------------
			void UpdateAcceleration(f32 infAccelerationX, f32 infAccelerationY, f32 infAccelerationZ);
		private:
			AccelerationChangedDelegate mAccelerationChangedDelegate;
			bool mbListening;
		};
	}
}

#endif
