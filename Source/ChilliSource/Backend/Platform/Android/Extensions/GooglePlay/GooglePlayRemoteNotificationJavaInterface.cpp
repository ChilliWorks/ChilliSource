//
//  GooglePlayRemoteNotificationJavaInterface.cpp
//  MoFlow
//
//  Created by Robert Henning on 23/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayRemoteNotificationJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayRemoteNotificationSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

extern "C"
{
	void Java_com_chillisource_googleplay_GooglePlayRemoteNotificationNativeInterface_NativeOnRemoteTokenReceived(JNIEnv* inpEnv, jobject inThis, jstring instrToken);
	void Java_com_chillisource_googleplay_GooglePlayRemoteNotificationNativeInterface_NativeOnRemoteNotificationReceived(JNIEnv* inpEnv, jobject inThis, jobjectArray inaKeys, jobjectArray inaValues);
}

void Java_com_chillisource_googleplay_GooglePlayRemoteNotificationNativeInterface_NativeOnRemoteTokenReceived(JNIEnv* inpEnv, jobject inThis, jstring instrToken)
{
	ChilliSource::Android::GooglePlayRemoteNotificationJavaInterfaceSPtr pInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::GooglePlayRemoteNotificationJavaInterface>();
	if(pInterface != nullptr)
	{
		const std::string strToken = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrToken);
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&ChilliSource::Android::GooglePlayRemoteNotificationJavaInterface::OnRemoteTokenReceived, pInterface.get(), strToken));
	}
}

void Java_com_chillisource_googleplay_GooglePlayRemoteNotificationNativeInterface_NativeOnRemoteNotificationReceived(JNIEnv* inpEnv, jobject inThis, jobjectArray inaKeys, jobjectArray inaValues)
{
	ChilliSource::Android::GooglePlayRemoteNotificationJavaInterfaceSPtr pInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::GooglePlayRemoteNotificationJavaInterface>();
	if(pInterface != nullptr)
	{
		//Convert the incoming jarrays to a param dictionary
		CS_ASSERT((inpEnv->GetArrayLength(inaKeys) == inpEnv->GetArrayLength(inaValues)), "CRemoteNotificationJavaInterface::NativeOnRemoteNotificationReceived has received a notification with different quantities of keys and values.");

		u32 dwNumEntries = inpEnv->GetArrayLength(inaKeys);
		ChilliSource::Core::ParamDictionary sParams;

		for(u32 i = 0; i < dwNumEntries; ++i)
		{
			std::string strKey = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(static_cast<jstring>(inpEnv->GetObjectArrayElement(inaKeys, i)));
			std::string strValue = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(static_cast<jstring>(inpEnv->GetObjectArrayElement(inaValues, i)));

			// Bail out on any keys we do not want to end up in the final params.
			if(strKey == "collapse_key" || strKey == "from")
			{
				continue;
			}

			sParams.SetValueForKey(strKey, strValue);
		}

		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&ChilliSource::Android::GooglePlayRemoteNotificationJavaInterface::OnRemoteNotificationReceived, pInterface.get(), sParams));
	}
}

namespace ChilliSource
{
    namespace Android
    {
    	CS_DEFINE_NAMEDTYPE(GooglePlayRemoteNotificationJavaInterface);

    	//--------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------
    	GooglePlayRemoteNotificationJavaInterface::GooglePlayRemoteNotificationJavaInterface()
    	{
    		CreateNativeInterface("com/chillisource/googleplay/GooglePlayRemoteNotificationNativeInterface");
    		CreateMethodReference("RequestRemoteToken", "()V");
    	}
    	//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool GooglePlayRemoteNotificationJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == GooglePlayRemoteNotificationJavaInterface::InterfaceID);
		}
		//---------------------------------------------------------------
		/// Request Remote Token
		//---------------------------------------------------------------
		void GooglePlayRemoteNotificationJavaInterface::RequestRemoteToken()
		{
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestRemoteToken"));
		}
		//-------------------------------------------------------------------------
		/// On Remote Token Received
		//-------------------------------------------------------------------------
		void GooglePlayRemoteNotificationJavaInterface::OnRemoteTokenReceived(const std::string& instrToken)
		{
			ChilliSource::Android::GooglePlayRemoteNotificationSystem* pRemoteNotificationSys = Core::Application::Get()->GetSystem<ChilliSource::Android::GooglePlayRemoteNotificationSystem>();
			CS_ASSERT(pRemoteNotificationSys, "Unable to get remote notification system.");
			pRemoteNotificationSys->OnRemoteTokenReceived(instrToken);
		}
		//-------------------------------------------------------------------------
		/// On Remote Notification Received
		//-------------------------------------------------------------------------
		void GooglePlayRemoteNotificationJavaInterface::OnRemoteNotificationReceived(const Core::ParamDictionary& insParams)
		{
			ChilliSource::Android::GooglePlayRemoteNotificationSystem* pRemoteNotificationSys = Core::Application::Get()->GetSystem<ChilliSource::Android::GooglePlayRemoteNotificationSystem>();
			CS_ASSERT(pRemoteNotificationSys, "Unable to get remote notification system.");
			pRemoteNotificationSys->OnRemoteNotificationReceived(insParams);
		}
    }
}
