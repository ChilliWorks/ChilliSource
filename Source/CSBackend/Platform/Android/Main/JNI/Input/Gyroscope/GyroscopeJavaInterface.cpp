//
//  GyroscopeJavaInterface.cpp
//  ChilliSource
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

#include <CSBackend/Platform/Android/Main/JNI/Input/Gyroscope/GyroscopeJavaInterface.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <ChilliSource/Core/Math/Quaternion.h>

#include <jni.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	void Java_com_chilliworks_chillisource_input_GyroscopeNativeInterface_UpdateOrientation(JNIEnv* inpEnv, jobject inThis, f32 infOrientationX, f32 infOrientationY, f32 infOrientationZ, f32 infOrientationW);
}
//-------------------------------------------
/// Update Orientation
///
/// Native implemention of the java CGyroscopeNativeInterface
/// method UpdateOrientation().
///
/// @param The jni environment.
/// @param The java object calling the function.
/// @param The X component of the orientation.
/// @param The Y component of the orientation.
/// @param The Z component of the orientation.
/// @param The W component of the orientation
//-------------------------------------------
void Java_com_chilliworks_chillisource_input_GyroscopeNativeInterface_UpdateOrientation(JNIEnv* inpEnv, jobject inThis, f32 infOrientationX, f32 infOrientationY, f32 infOrientationZ, f32 infOrientationW)
{
	CSBackend::Android::GyroscopeJavaInterfaceSPtr pGyroscopeJI = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::GyroscopeJavaInterface>();
	if (nullptr != pGyroscopeJI)
	{
		pGyroscopeJI->UpdateOrientation(infOrientationX, infOrientationY, infOrientationZ, infOrientationW);
	}
}

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(GyroscopeJavaInterface);
		//-----------------------------------------------
		/// Constructor
		//-----------------------------------------------
		GyroscopeJavaInterface::GyroscopeJavaInterface()
			: mbListening(false)
		{
			CreateNativeInterface("com/chilliworks/chillisource/input/GyroscopeNativeInterface");
			CreateMethodReference("IsAvailable", "()Z");
			CreateMethodReference("StartListening", "()V");
			CreateMethodReference("StopListening", "()V");
		}
		//-----------------------------------------------
		/// Is A
		//-----------------------------------------------
		bool GyroscopeJavaInterface::IsA(ChilliSource::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == GyroscopeJavaInterface::InterfaceID);
		}
		//------------------------------------------------
		/// Is Available
		//------------------------------------------------
		bool GyroscopeJavaInterface::IsAvailable()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("IsAvailable"));
		}
		//------------------------------------------------
		/// Start Listening
		//------------------------------------------------
		void GyroscopeJavaInterface::StartListening(const OrientationChangedDelegate& inOrientationChangedDelegate)
		{
			if (true == IsAvailable() && false == mbListening)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StartListening"));

				mOrientationChangedDelegate = inOrientationChangedDelegate;
				mbListening = true;
			}
		}
		//------------------------------------------------
		/// Stop Listening
		//------------------------------------------------
		void GyroscopeJavaInterface::StopListening()
		{
			if (true == mbListening)
			{
				mOrientationChangedDelegate = nullptr;
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StopListening"));
				mbListening = false;
			}
		}
		//------------------------------------------------
		/// Update Orientation
		//------------------------------------------------
		void GyroscopeJavaInterface::UpdateOrientation(f32 infOrientationX, f32 infOrientationY, f32 infOrientationZ, f32 infOrientationW)
		{
			if (true == mbListening && nullptr != mOrientationChangedDelegate)
			{
				mOrientationChangedDelegate(ChilliSource::Quaternion(infOrientationX, infOrientationY, infOrientationZ, infOrientationW));
			}
		}
	}
}

#endif
