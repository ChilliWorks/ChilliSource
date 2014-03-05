/*
 *  LocalNotificationJavaInterface.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 28/09/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Core/Notifications/LocalNotificationJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Backend/Platform/Android/Core/Notifications/LocalNotificationScheduler.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <jni.h>

extern "C"
{
	void Java_com_chillisource_core_LocalNotificationNativeInterface_ApplicationDidReceiveLocalNotification(JNIEnv* inpEnv, jobject thiz, jobjectArray inastrKeys, jobjectArray inastrValues);
}

void Java_com_chillisource_core_LocalNotificationNativeInterface_ApplicationDidReceiveLocalNotification(JNIEnv* inpEnv, jobject thiz, jobjectArray inastrKeys, jobjectArray inastrValues)
{
	CSCore::NotificationSPtr notification(std::make_shared<CSCore::Notification>());
	notification->bDismissed = false;
	notification->eType = CSCore::NotificationType::k_system;

	u32 udwNumberOfParams = inpEnv->GetArrayLength(inastrKeys);
	for(u32 udwKey = 0; udwKey < udwNumberOfParams; ++udwKey)
	{
		jstring jstrKey = (jstring)inpEnv->GetObjectArrayElement(inastrKeys,udwKey);
		jstring jstrValue =  (jstring)inpEnv->GetObjectArrayElement(inastrValues,udwKey);

		//Convert this myself because json to stdstring method requires being on same thread as game
		std::string strKey;
		{
			const char * cString = inpEnv->GetStringUTFChars(jstrKey, JNI_FALSE);
			std::string stdString = std::string(cString);
			inpEnv->ReleaseStringUTFChars(jstrKey, cString);
			strKey =  stdString;
		}
		std::string strValue;
		{
			const char * cString = inpEnv->GetStringUTFChars(jstrValue, JNI_FALSE);
			std::string stdString = std::string(cString);
			inpEnv->ReleaseStringUTFChars(jstrValue, cString);
			strValue =  stdString;
		}
		CS_LOG_DEBUG(strKey + " - " + strValue);
		if(strKey == "NotificationID")
		{
			notification->ID = CSCore::ParseS32(strValue);
			continue;
		}
		if(strKey == "TriggerTime")
		{
			notification->TriggerTime = CSCore::ParseU64(strValue);
			continue;
		}
		if(strKey == "Priority")
		{
			notification->ePriority = (CSCore::NotificationPriority)CSCore::ParseS32(strValue);
			continue;
		}
		notification->sParams.SetValueForKey(strKey,strValue);
	}
	ChilliSource::Android::LocalNotificationScheduler::ApplicationDidReceiveLocalNotification(notification);
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(LocalNotificationJavaInterface);
		//--------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------
		LocalNotificationJavaInterface::LocalNotificationJavaInterface()
		{
			CreateNativeInterface("com/chillisource/core/LocalNotificationNativeInterface");
			CreateMethodReference("ScheduleNotification", "(JLjava/lang/String;II[Ljava/lang/String;[Ljava/lang/String;)V");
			CreateMethodReference("CancelByID","(I)V");
			CreateMethodReference("CancelAll","()V");
		}
		//--------------------------------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------------------------------
		bool LocalNotificationJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == LocalNotificationJavaInterface::InterfaceID);
		}
		//-------------------------------------------------------------------------
		/// Schedule Notification
		//-------------------------------------------------------------------------
		void LocalNotificationJavaInterface::ScheduleNotification(const Core::NotificationSPtr& insNotification)
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

			//Convert Trigger time from seconds to milliseconds as this is what will be used on the Android end;
			u64 uqwFireDate = insNotification->TriggerTime * 1000;

			jstring jstrAlertBody = JavaInterfaceUtils::CreateJStringFromSTDString(insNotification->sParams.ValueForKey("Body"));
			u32 udwID = insNotification->ID;
			u32 udwPriority = (u32)insNotification->ePriority;

			//Create 2 arrays from ParamDictionary as this is easier to pass to Java
			jobjectArray ajstrKey = pEnv->NewObjectArray(insNotification->sParams.size(), pEnv->FindClass("java/lang/String"), pEnv->NewStringUTF(""));
			jobjectArray ajstrValue = pEnv->NewObjectArray(insNotification->sParams.size(), pEnv->FindClass("java/lang/String"), pEnv->NewStringUTF(""));;

			u32 udwParamCount = 0;
			for(Core::StringToStringMap::const_iterator it = insNotification->sParams.begin(); it != insNotification->sParams.end(); ++it)
			{
				jstring jstrFirst = JavaInterfaceUtils::CreateJStringFromSTDString(it->first);
				jstring jstrSecond = JavaInterfaceUtils::CreateJStringFromSTDString(it->second);
				pEnv->SetObjectArrayElement(ajstrKey, udwParamCount, jstrFirst);
				pEnv->SetObjectArrayElement(ajstrValue, udwParamCount, jstrSecond);
				pEnv->DeleteLocalRef(jstrFirst);
				pEnv->DeleteLocalRef(jstrSecond);
				udwParamCount++;
			}

			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("ScheduleNotification"), uqwFireDate,jstrAlertBody,udwID,udwPriority,ajstrKey,ajstrValue);

			//delete local references
			pEnv->DeleteLocalRef(jstrAlertBody);
			pEnv->DeleteLocalRef(ajstrKey);
			pEnv->DeleteLocalRef(ajstrValue);
		}
		//-------------------------------------------------------------------------
		/// Cancel By ID
		//-------------------------------------------------------------------------
		void LocalNotificationJavaInterface::CancelByID(u32 inID)
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("CancelByID"),inID);
		}
		//-------------------------------------------------------------------------
		/// Cancel All
		//-------------------------------------------------------------------------
		void LocalNotificationJavaInterface::CancelAll()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("CancelAll"));
		}
	}
}
