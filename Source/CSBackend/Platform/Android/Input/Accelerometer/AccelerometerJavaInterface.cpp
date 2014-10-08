//
//  AccelerometerJavaInterface.cpp
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

#include <CSBackend/Platform/Android/Input/Accelerometer/AccelerometerJavaInterface.h>

#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Math/Vector3.h>

#include <jni.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	void Java_com_chilliworks_chillisource_input_AccelerometerNativeInterface_UpdateAcceleration(JNIEnv* inpEnv, jobject inThis, f32 infAccelerationX, f32 infAccelerationY, f32 infAccelerationZ);
}
//-------------------------------------------
/// Update Acceleration
///
/// Native implemention of the java CAccelerometerNativeInterface
/// method UpdateAcceleration().
///
/// @param The jni environment.
/// @param The java object calling the function.
/// @param The X component of the acceleration.
/// @param The Y component of the acceleration.
/// @param The Z component of the acceleration.
//-------------------------------------------
void Java_com_chilliworks_chillisource_input_AccelerometerNativeInterface_UpdateAcceleration(JNIEnv* inpEnv, jobject inThis, f32 infAccelerationX, f32 infAccelerationY, f32 infAccelerationZ)
{
	CSBackend::Android::AccelerometerJavaInterfaceSPtr pAccelerometerJI = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::AccelerometerJavaInterface>();
	if (nullptr != pAccelerometerJI)
	{
		pAccelerometerJI->UpdateAcceleration(infAccelerationX, infAccelerationY, infAccelerationZ);
	}
}

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(AccelerometerJavaInterface);
		//-----------------------------------------------
		/// Constructor
		//-----------------------------------------------
		AccelerometerJavaInterface::AccelerometerJavaInterface()
			: mbListening(false)
		{
			CreateNativeInterface("com/chilliworks/chillisource/input/AccelerometerNativeInterface");
			CreateMethodReference("IsAvailable", "()Z");
			CreateMethodReference("StartListening", "()V");
			CreateMethodReference("StopListening", "()V");
		}
		//-----------------------------------------------
		/// Is A
		//-----------------------------------------------
		bool AccelerometerJavaInterface::IsA(CSCore::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == AccelerometerJavaInterface::InterfaceID);
		}
		//------------------------------------------------
		/// Is Available
		//------------------------------------------------
		bool AccelerometerJavaInterface::IsAvailable()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("IsAvailable"));
		}
		//------------------------------------------------
		/// Start Listening
		//------------------------------------------------
		void AccelerometerJavaInterface::StartListening(const AccelerationChangedDelegate& inAccelerationChangedDelegate)
		{
			if (true == IsAvailable() && false == mbListening)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StartListening"));

				mAccelerationChangedDelegate = inAccelerationChangedDelegate;
				mbListening = true;
			}
		}
		//------------------------------------------------
		/// Stop Listening
		//------------------------------------------------
		void AccelerometerJavaInterface::StopListening()
		{
			if (true == mbListening)
			{
				mAccelerationChangedDelegate = nullptr;
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StopListening"));
				mbListening = false;
			}
		}
		//------------------------------------------------
		/// Update Acceleration
		//------------------------------------------------
		void AccelerometerJavaInterface::UpdateAcceleration(f32 infAccelerationX, f32 infAccelerationY, f32 infAccelerationZ)
		{
			if (true == mbListening && nullptr != mAccelerationChangedDelegate)
			{
				mAccelerationChangedDelegate(CSCore::Vector3(infAccelerationX, infAccelerationY, infAccelerationZ));
			}
		}
	}
}

#endif
