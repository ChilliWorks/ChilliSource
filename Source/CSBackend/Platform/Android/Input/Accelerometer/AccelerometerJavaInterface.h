//
//  AccelerometerJavaInterface.h
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

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_ACCELEROMETERJAVAINTERFACE_H
#define _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_ACCELEROMETERJAVAINTERFACE_H

#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

namespace CSBackend
{
	namespace Android
	{
		//======================================================
		/// Accelerometer Java Interface
		///
		/// A Java Interface for communicating with the Android
		/// accelerometer.
		//======================================================
		class AccelerometerJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE();
			//------------------------------------------------
			/// Delegates
			//------------------------------------------------
			typedef std::function<void(const CSCore::Vector3&)> AccelerationChangedDelegate;
			//------------------------------------------------
			/// Constructor
			//------------------------------------------------
			AccelerometerJavaInterface();
			//------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements
			/// the given interface.
			//------------------------------------------------
			bool IsA(CSCore::InterfaceIDType inInterfaceID) const;
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

#endif