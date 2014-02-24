//
//  GooglePlayRemoteNotificationJavaInterface.cpp
//  MoFlow
//
//  Created by Robert Henning on 23/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Platform/Android/GooglePlay/GooglePlayRemoteNotificationJavaInterface.h>

#include <jni.h>

#include <ChilliSource/Core/Main/Application.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceUtils.h>
#include <ChilliSource/Platform/Android/GooglePlay/GooglePlayRemoteNotificationSystem.h>

extern "C"
{
	void Java_com_taggames_moflow_googleplay_remotenotifications_CGooglePlayRemoteNotificationNativeInterface_NativeOnRemoteTokenReceived(JNIEnv* inpEnv, jobject inThis, jstring instrToken);
	void Java_com_taggames_moflow_googleplay_remotenotifications_CGooglePlayRemoteNotificationNativeInterface_NativeOnRemoteNotificationReceived(JNIEnv* inpEnv, jobject inThis, jobjectArray inaKeys, jobjectArray inaValues);
}

void Java_com_taggames_moflow_googleplay_remotenotifications_CGooglePlayRemoteNotificationNativeInterface_NativeOnRemoteTokenReceived(JNIEnv* inpEnv, jobject inThis, jstring instrToken)
{
	moFlo::AndroidPlatform::GooglePlayRemoteNotificationJavaInterfacePtr pInterface
		= moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CGooglePlayRemoteNotificationJavaInterface>();
	if(pInterface != NULL)
	{
		const std::string strToken = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrToken);
		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task1<const std::string&>(pInterface.get(), &moFlo::AndroidPlatform::CGooglePlayRemoteNotificationJavaInterface::OnRemoteTokenReceived, strToken));
	}
}

void Java_com_taggames_moflow_googleplay_remotenotifications_CGooglePlayRemoteNotificationNativeInterface_NativeOnRemoteNotificationReceived(JNIEnv* inpEnv, jobject inThis, jobjectArray inaKeys, jobjectArray inaValues)
{
	moFlo::AndroidPlatform::GooglePlayRemoteNotificationJavaInterfacePtr pInterface
		= moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CGooglePlayRemoteNotificationJavaInterface>();
	if(pInterface != NULL)
	{
		//Convert the incoming jarrays to a param dictionary
		MOFLOW_ASSERT((inpEnv->GetArrayLength(inaKeys) == inpEnv->GetArrayLength(inaValues)), "CRemoteNotificationJavaInterface::NativeOnRemoteNotificationReceived has received a notification with different quantities of keys and values.");

		u32 dwNumEntries = inpEnv->GetArrayLength(inaKeys);
		moFlo::Core::ParamDictionary sParams;

		for(u32 i = 0; i < dwNumEntries; ++i)
		{
			std::string strKey = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(static_cast<jstring>(inpEnv->GetObjectArrayElement(inaKeys, i)));
			std::string strValue = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(static_cast<jstring>(inpEnv->GetObjectArrayElement(inaValues, i)));

			// Bail out on any keys we do not want to end up in the final params.
			if(strKey == "collapse_key" || strKey == "from")
			{
				continue;
			}

			sParams.SetValueForKey(strKey, strValue);
		}

		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task1<const moFlo::Core::ParamDictionary&>(pInterface.get(), &moFlo::AndroidPlatform::CGooglePlayRemoteNotificationJavaInterface::OnRemoteNotificationReceived, sParams));
	}
}

namespace ChilliSource
{
    namespace Android
    {
    	DEFINE_NAMED_INTERFACE(CGooglePlayRemoteNotificationJavaInterface);

    	//--------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------
    	CGooglePlayRemoteNotificationJavaInterface::CGooglePlayRemoteNotificationJavaInterface()
    	{
    		CreateNativeInterface("com/taggames/moflow/googleplay/remotenotifications/CGooglePlayRemoteNotificationNativeInterface");
    		CreateMethodReference("RequestRemoteToken", "()V");
    	}
    	//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool CGooglePlayRemoteNotificationJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CGooglePlayRemoteNotificationJavaInterface::InterfaceID);
		}
		//---------------------------------------------------------------
		/// Request Remote Token
		//---------------------------------------------------------------
		void CGooglePlayRemoteNotificationJavaInterface::RequestRemoteToken()
		{
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestRemoteToken"));
		}
		//-------------------------------------------------------------------------
		/// On Remote Token Received
		//-------------------------------------------------------------------------
		void CGooglePlayRemoteNotificationJavaInterface::OnRemoteTokenReceived(const std::string& instrToken)
		{
			moFlo::AndroidPlatform::CGooglePlayRemoteNotificationSystem* pRemoteNotificationSys = moCore::CApplication::GetSystemImplementing<moFlo::AndroidPlatform::CGooglePlayRemoteNotificationSystem>();
			MOFLOW_ASSERT(pRemoteNotificationSys, "Unable to get remote notification system.");
			pRemoteNotificationSys->OnRemoteTokenReceived(instrToken);
		}
		//-------------------------------------------------------------------------
		/// On Remote Notification Received
		//-------------------------------------------------------------------------
		void CGooglePlayRemoteNotificationJavaInterface::OnRemoteNotificationReceived(const Core::ParamDictionary& insParams)
		{
			moFlo::AndroidPlatform::CGooglePlayRemoteNotificationSystem* pRemoteNotificationSys = moCore::CApplication::GetSystemImplementing<moFlo::AndroidPlatform::CGooglePlayRemoteNotificationSystem>();
			MOFLOW_ASSERT(pRemoteNotificationSys, "Unable to get remote notification system.");
			pRemoteNotificationSys->OnRemoteNotificationReceived(insParams);
		}
    }
}
