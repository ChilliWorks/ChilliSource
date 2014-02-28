//
//  GooglePlayRemoteNotificationJavaInterface.cpp
//  MoFlow
//
//  Created by Robert Henning on 23/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/GooglePlay/GooglePlayRemoteNotificationJavaInterface.h>

#include <jni.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceUtils.h>
#include <ChilliSource/Backend/Platform/Android/GooglePlay/GooglePlayRemoteNotificationSystem.h>

extern "C"
{
	void Java_com_taggames_moflow_googleplay_remotenotifications_CGooglePlayRemoteNotificationNativeInterface_NativeOnRemoteTokenReceived(JNIEnv* inpEnv, jobject inThis, jstring instrToken);
	void Java_com_taggames_moflow_googleplay_remotenotifications_CGooglePlayRemoteNotificationNativeInterface_NativeOnRemoteNotificationReceived(JNIEnv* inpEnv, jobject inThis, jobjectArray inaKeys, jobjectArray inaValues);
}

void Java_com_taggames_moflow_googleplay_remotenotifications_CGooglePlayRemoteNotificationNativeInterface_NativeOnRemoteTokenReceived(JNIEnv* inpEnv, jobject inThis, jstring instrToken)
{
	ChilliSource::Android::GooglePlayRemoteNotificationJavaInterfacePtr pInterface
		= ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CGooglePlayRemoteNotificationJavaInterface>();
	if(pInterface != NULL)
	{
		const std::string strToken = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrToken);
		ChilliSource::CTaskScheduler::ScheduleMainThreadTask(ChilliSource::Task1<const std::string&>(pInterface.get(), &ChilliSource::Android::CGooglePlayRemoteNotificationJavaInterface::OnRemoteTokenReceived, strToken));
	}
}

void Java_com_taggames_moflow_googleplay_remotenotifications_CGooglePlayRemoteNotificationNativeInterface_NativeOnRemoteNotificationReceived(JNIEnv* inpEnv, jobject inThis, jobjectArray inaKeys, jobjectArray inaValues)
{
	ChilliSource::Android::GooglePlayRemoteNotificationJavaInterfacePtr pInterface
		= ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CGooglePlayRemoteNotificationJavaInterface>();
	if(pInterface != NULL)
	{
		//Convert the incoming jarrays to a param dictionary
		MOFLOW_ASSERT((inpEnv->GetArrayLength(inaKeys) == inpEnv->GetArrayLength(inaValues)), "CRemoteNotificationJavaInterface::NativeOnRemoteNotificationReceived has received a notification with different quantities of keys and values.");

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

		ChilliSource::CTaskScheduler::ScheduleMainThreadTask(ChilliSource::Task1<const ChilliSource::Core::ParamDictionary&>(pInterface.get(), &ChilliSource::Android::CGooglePlayRemoteNotificationJavaInterface::OnRemoteNotificationReceived, sParams));
	}
}

namespace ChilliSource
{
    namespace Android
    {
    	CS_DEFINE_NAMEDTYPE(CGooglePlayRemoteNotificationJavaInterface);

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
			ChilliSource::Android::CGooglePlayRemoteNotificationSystem* pRemoteNotificationSys = Core::Application::GetSystemImplementing<ChilliSource::Android::CGooglePlayRemoteNotificationSystem>();
			MOFLOW_ASSERT(pRemoteNotificationSys, "Unable to get remote notification system.");
			pRemoteNotificationSys->OnRemoteTokenReceived(instrToken);
		}
		//-------------------------------------------------------------------------
		/// On Remote Notification Received
		//-------------------------------------------------------------------------
		void CGooglePlayRemoteNotificationJavaInterface::OnRemoteNotificationReceived(const Core::ParamDictionary& insParams)
		{
			ChilliSource::Android::CGooglePlayRemoteNotificationSystem* pRemoteNotificationSys = Core::Application::GetSystemImplementing<ChilliSource::Android::CGooglePlayRemoteNotificationSystem>();
			MOFLOW_ASSERT(pRemoteNotificationSys, "Unable to get remote notification system.");
			pRemoteNotificationSys->OnRemoteNotificationReceived(insParams);
		}
    }
}
