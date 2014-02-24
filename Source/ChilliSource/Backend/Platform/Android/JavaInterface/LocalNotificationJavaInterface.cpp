/*
 *  LocalNotificationJavaInterface.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 28/09/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Platform/Android/JavaInterface/LocalNotificationJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceUtils.h>
#include <ChilliSource/Platform/Android/LocalNotificationScheduler.h>

#include <jni.h>

extern "C"
{
	void Java_com_taggames_moflow_nativeinterface_CLocalNotificationNativeInterface_ApplicationDidReceiveLocalNotification(JNIEnv* inpEnv, jobject thiz, jobjectArray inastrKeys, jobjectArray inastrValues);
}

void Java_com_taggames_moflow_nativeinterface_CLocalNotificationNativeInterface_ApplicationDidReceiveLocalNotification(JNIEnv* inpEnv, jobject thiz, jobjectArray inastrKeys, jobjectArray inastrValues)
{
	DEBUG_LOG("GETTING NOTIFICATION");
	ChilliSource::Notification sNotification;
	sNotification.bDismissed = false;
	sNotification.eType = ChilliSource::NOTICE_SYSTEM;

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
		DEBUG_LOG(strKey + " - " + strValue);
		if(strKey == "NotificationID")
		{
			sNotification.ID = ChilliSource::Core::CStringConverter::ParseInt(strValue);
			continue;
		}
		if(strKey == "TriggerTime")
		{
			sNotification.TriggerTime = ChilliSource::Core::CStringConverter::ParseUnsignedLong(strValue);
			continue;
		}
		if(strKey == "Priority")
		{
			sNotification.ePriority = (ChilliSource::NotificationPriority)ChilliSource::Core::CStringConverter::ParseInt(strValue);
			continue;
		}
		sNotification.sParams.SetValueForKey(strKey,strValue);
	}
	ChilliSource::Android::CLocalNotificationScheduler::ApplicationDidReceiveLocalNotification(sNotification);
}

namespace ChilliSource
{
	namespace Android
	{
		DEFINE_NAMED_INTERFACE(CLocalNotificationJavaInterface);
		//--------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------
		CLocalNotificationJavaInterface::CLocalNotificationJavaInterface()
		{
			CreateNativeInterface("com/taggames/moflow/nativeinterface/CLocalNotificationNativeInterface");
			CreateMethodReference("ScheduleNotification", "(JLjava/lang/String;II[Ljava/lang/String;[Ljava/lang/String;)V");
			CreateMethodReference("CancelByID","(I)V");
			CreateMethodReference("CancelAll","()V");
		}
		//--------------------------------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------------------------------
		bool CLocalNotificationJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CLocalNotificationJavaInterface::InterfaceID);
		}
		//-------------------------------------------------------------------------
		/// Schedule Notification
		//-------------------------------------------------------------------------
		void CLocalNotificationJavaInterface::ScheduleNotification(const Notification& insNotification)
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

			//Convert Trigger time from seconds to milliseconds as this is what will be used on the Android end;
			u64 uqwFireDate = insNotification.TriggerTime * 1000;

			jstring jstrAlertBody = JavaInterfaceUtils::CreateJStringFromSTDString(insNotification.sParams.ValueForKey("Body"));
			u32 udwID = insNotification.ID;
			u32 udwPriority = insNotification.ePriority;

			//Create 2 arrays from ParamDictionary as this is easier to pass to Java
			jobjectArray ajstrKey = pEnv->NewObjectArray(insNotification.sParams.size(), pEnv->FindClass("java/lang/String"), pEnv->NewStringUTF(""));
			jobjectArray ajstrValue = pEnv->NewObjectArray(insNotification.sParams.size(), pEnv->FindClass("java/lang/String"), pEnv->NewStringUTF(""));;

			u32 udwParamCount = 0;
			for(Core::StringToStringMap::const_iterator it = insNotification.sParams.begin(); it != insNotification.sParams.end(); ++it)
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
		void CLocalNotificationJavaInterface::CancelByID(u32 inID)
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("CancelByID"),inID);
		}
		//-------------------------------------------------------------------------
		/// Cancel All
		//-------------------------------------------------------------------------
		void CLocalNotificationJavaInterface::CancelAll()
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("CancelAll"));
		}
	}
}
