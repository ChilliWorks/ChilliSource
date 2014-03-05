//
//  AccelerometerJavaInterface.cpp
//  moFlow
//
//  Created by Ian Copland on 10/06/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Input/Accelerometer/AccelerometerJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Math/Vector3.h>

#include <jni.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	void Java_com_chillisource_input_AccelerometerNativeInterface_UpdateAcceleration(JNIEnv* inpEnv, jobject inThis, f32 infAccelerationX, f32 infAccelerationY, f32 infAccelerationZ);
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
void Java_com_chillisource_input_AccelerometerNativeInterface_UpdateAcceleration(JNIEnv* inpEnv, jobject inThis, f32 infAccelerationX, f32 infAccelerationY, f32 infAccelerationZ)
{
	ChilliSource::Android::AccelerometerJavaInterfacePtr pAccelerometerJI = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::AccelerometerJavaInterface>();
	if (NULL != pAccelerometerJI)
	{
		pAccelerometerJI->UpdateAcceleration(infAccelerationX, infAccelerationY, infAccelerationZ);
	}
}

namespace ChilliSource
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
			CreateNativeInterface("com/chillisource/input/AccelerometerNativeInterface");
			CreateMethodReference("IsAvailable", "()Z");
			CreateMethodReference("StartListening", "()V");
			CreateMethodReference("StopListening", "()V");
		}
		//-----------------------------------------------
		/// Is A
		//-----------------------------------------------
		bool AccelerometerJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
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
				mAccelerationChangedDelegate = NULL;
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
			if (true == mbListening && NULL != mAccelerationChangedDelegate)
			{
				mAccelerationChangedDelegate(Core::Vector3(infAccelerationX, infAccelerationY, infAccelerationZ));
			}
		}
	}
}
