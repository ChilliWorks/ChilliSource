//
//  AccelerometerJavaInterface.cpp
//  moFlow
//
//  Created by Ian Copland on 10/06/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Platform/Android/JavaInterface/AccelerometerJavaInterface.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceUtils.h>
#include <jni.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	void Java_com_taggames_moflow_nativeinterface_CAccelerometerNativeInterface_UpdateAcceleration(JNIEnv* inpEnv, jobject inThis, f32 infAccelerationX, f32 infAccelerationY, f32 infAccelerationZ);
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
void Java_com_taggames_moflow_nativeinterface_CAccelerometerNativeInterface_UpdateAcceleration(JNIEnv* inpEnv, jobject inThis, f32 infAccelerationX, f32 infAccelerationY, f32 infAccelerationZ)
{
	moFlo::AndroidPlatform::AccelerometerJavaInterfacePtr pAccelerometerJI = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CAccelerometerJavaInterface>();
	if (NULL != pAccelerometerJI)
	{
		pAccelerometerJI->UpdateAcceleration(infAccelerationX, infAccelerationY, infAccelerationZ);
	}
}

namespace moFlo
{
	namespace AndroidPlatform
	{
		DEFINE_NAMED_INTERFACE(CAccelerometerJavaInterface);
		//-----------------------------------------------
		/// Constructor
		//-----------------------------------------------
		CAccelerometerJavaInterface::CAccelerometerJavaInterface()
			: mbListening(false)
		{
			CreateNativeInterface("com/taggames/moflow/nativeinterface/CAccelerometerNativeInterface");
			CreateMethodReference("IsAvailable", "()Z");
			CreateMethodReference("StartListening", "()V");
			CreateMethodReference("StopListening", "()V");
		}
		//-----------------------------------------------
		/// Is A
		//-----------------------------------------------
		bool CAccelerometerJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CAccelerometerJavaInterface::InterfaceID);
		}
		//------------------------------------------------
		/// Is Available
		//------------------------------------------------
		bool CAccelerometerJavaInterface::IsAvailable()
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("IsAvailable"));
		}
		//------------------------------------------------
		/// Start Listening
		//------------------------------------------------
		void CAccelerometerJavaInterface::StartListening(const AccelerationChangedDelegate& inAccelerationChangedDelegate)
		{
			if (true == IsAvailable() && false == mbListening)
			{
				JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StartListening"));

				mAccelerationChangedDelegate = inAccelerationChangedDelegate;
				mbListening = true;
			}
		}
		//------------------------------------------------
		/// Stop Listening
		//------------------------------------------------
		void CAccelerometerJavaInterface::StopListening()
		{
			if (true == mbListening)
			{
				mAccelerationChangedDelegate = NULL;
				JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StopListening"));
				mbListening = false;
			}
		}
		//------------------------------------------------
		/// Update Acceleration
		//------------------------------------------------
		void CAccelerometerJavaInterface::UpdateAcceleration(f32 infAccelerationX, f32 infAccelerationY, f32 infAccelerationZ)
		{
			if (true == mbListening && NULL != mAccelerationChangedDelegate)
			{
				mAccelerationChangedDelegate(Core::CVector3(infAccelerationX, infAccelerationY, infAccelerationZ));
			}
		}
	}
}
