//
//  GyroscopeJavaInterface.h
//  ChilliSource
//  Created by Jordan Brown on 20/01/2017.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_GYROSCOPEJAVAINTERFACE_H
#define _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_GYROSCOPEJAVAINTERFACE_H

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterface.h>

namespace CSBackend
{
	namespace Android
	{
		//======================================================
		/// Gyroscope Java Interface
		///
		/// A Java Interface for communicating with the Android
		/// gyroscope.
		//======================================================
		class GyroscopeJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE();
			//------------------------------------------------
			/// Delegates
			//------------------------------------------------
			typedef std::function<void(const ChilliSource::Quaternion&)> OrientationChangedDelegate;
			//------------------------------------------------
			/// Constructor
			//------------------------------------------------
			GyroscopeJavaInterface();
			//------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements
			/// the given interface.
			//------------------------------------------------
			bool IsA(ChilliSource::InterfaceIDType inInterfaceID) const;
			//------------------------------------------------
			/// Is Available
			///
			/// @return whether or not the gyroscope is
			/// available on this device.
			//------------------------------------------------
			bool IsAvailable();
			//------------------------------------------------
			/// Start Listening
			///
			/// Start listening for orientation changes from
			/// the gyroscope on device. If the gyroscope
			/// is not available this will do nothing.
			///
			/// @param The delegate which will receive the
			/// orientation changes.
			//------------------------------------------------
			void StartListening(const OrientationChangedDelegate& inOrientationChangedDelegate);
			//------------------------------------------------
			/// Stop Listening
			///
			/// Stops listening for orientations changes from
			/// the gyroscope.
			//------------------------------------------------
			void StopListening();
			//------------------------------------------------
			/// Update Orientation
			///
			/// This is called by the Java Native Interface to
			/// inform of orientation updates.
			///
			/// @param The x component of the orientation.
			/// @param The y component of the orientation.
			/// @param The z component of the orientation.
			/// @param The w component of the orientation
			//------------------------------------------------
			void UpdateOrientation(f32 infOrientationX, f32 infOrientationY, f32 infOrientationZ, f32 infOrientationW);
		private:
			OrientationChangedDelegate mOrientationChangedDelegate;
			bool mbListening;
		};
	}
}

#endif

#endif
