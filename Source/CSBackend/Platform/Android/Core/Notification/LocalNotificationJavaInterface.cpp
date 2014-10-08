//
//  LocalNotificationJavaInterface.cpp
//  Chilli Source
//  Created by Steven Hendrie on 13/12/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <CSBackend/Platform/Android/Core/Notification/LocalNotificationJavaInterface.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <CSBackend/Platform/Android/Core/Notification/LocalNotificationSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Notification/Notification.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

extern "C"
{
	void Java_com_chilliworks_chillisource_core_LocalNotificationNativeInterface_nativeOnNotificationReceived(JNIEnv* in_environment, jobject in_this, s32 in_id, jobjectArray in_keys, jobjectArray in_values, s32 in_priority);
}

void Java_com_chilliworks_chillisource_core_LocalNotificationNativeInterface_nativeOnNotificationReceived(JNIEnv* in_environment, jobject in_this, s32 in_id, jobjectArray in_keys, jobjectArray in_values, s32 in_priority)
{
	CSCore::ParamDictionary params;

	u32 numParams = in_environment->GetArrayLength(in_keys);
	for(u32 keyIndex = 0; keyIndex < numParams; ++keyIndex)
	{
		jstring keyJava = (jstring)in_environment->GetObjectArrayElement(in_keys, keyIndex);
		jstring valueJava =  (jstring)in_environment->GetObjectArrayElement(in_values, keyIndex);

		std::string key = CSBackend::Android::JavaInterfaceUtils::CreateSTDStringFromJString(keyJava);
		std::string value = CSBackend::Android::JavaInterfaceUtils::CreateSTDStringFromJString(valueJava);

		params.SetValue(key, value);

		in_environment->DeleteLocalRef(keyJava);
		in_environment->DeleteLocalRef(valueJava);
	}

	auto delegate = [=]()
	{
		CSBackend::Android::LocalNotificationSystem* localNotificationSystem = CSCore::Application::Get()->GetSystem<CSBackend::Android::LocalNotificationSystem>();
		if (localNotificationSystem != nullptr)
		{
			localNotificationSystem->OnNotificationReceived(static_cast<CSCore::Notification::ID>(in_id), params, static_cast<CSCore::Notification::Priority>(in_priority));
		}
	};

	CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(delegate);
}

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(LocalNotificationJavaInterface);
		//--------------------------------------------------------
		//--------------------------------------------------------
		LocalNotificationJavaInterface::LocalNotificationJavaInterface()
		{
			CreateNativeInterface("com/chilliworks/chillisource/core/LocalNotificationNativeInterface");
			CreateMethodReference("scheduleNotificationForTime", "(I[Ljava/lang/String;[Ljava/lang/String;JI)V");
			CreateMethodReference("getNotifications", "()[Lcom/chilliworks/chillisource/core/LocalNotification;");
			CreateMethodReference("cancelByID","(I)V");
			CreateMethodReference("cancelAll","()V");
		}
		//--------------------------------------------------------
		//--------------------------------------------------------
		bool LocalNotificationJavaInterface::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == LocalNotificationJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------
		//--------------------------------------------------------
		void LocalNotificationJavaInterface::ScheduleNotificationForTime(CSCore::Notification::ID in_id, const CSCore::ParamDictionary& in_params, TimeIntervalSecs in_time, CSCore::Notification::Priority in_priority)
		{
			JNIEnv* environment = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

			//Convert Trigger time from seconds to milliseconds as this is what will be used on the Android end;
			u64 triggerTime = in_time * 1000;

			u32 id = (u32)in_id;
			u32 priority = (u32)in_priority;

			//Create 2 arrays from ParamDictionary as this is easier to pass to Java
			jclass stringClass = environment->FindClass("java/lang/String");
			jstring emptyString = environment->NewStringUTF("");
			jobjectArray keys = environment->NewObjectArray(in_params.size(), stringClass, emptyString);
			jobjectArray values = environment->NewObjectArray(in_params.size(), stringClass, emptyString);

			u32 paramCount = 0;
			for(auto it = in_params.begin(); it != in_params.end(); ++it)
			{
				jstring key = JavaInterfaceUtils::CreateJStringFromSTDString(it->first);
				jstring value = JavaInterfaceUtils::CreateJStringFromSTDString(it->second);
				environment->SetObjectArrayElement(keys, paramCount, key);
				environment->SetObjectArrayElement(values, paramCount, value);
				environment->DeleteLocalRef(key);
				environment->DeleteLocalRef(value);
				paramCount++;
			}

			environment->CallVoidMethod(GetJavaObject(), GetMethodID("scheduleNotificationForTime"), id, keys, values, triggerTime, priority);

			//delete local references
			environment->DeleteLocalRef(stringClass);
			environment->DeleteLocalRef(emptyString);
			environment->DeleteLocalRef(keys);
			environment->DeleteLocalRef(values);
		}
		//--------------------------------------------------------
		//--------------------------------------------------------
		void LocalNotificationJavaInterface::GetScheduledNotifications(std::vector<CSCore::NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time, TimeIntervalSecs in_period) const
		{
			JNIEnv* environment = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jobjectArray notificationsJNI = static_cast<jobjectArray>(environment->CallObjectMethod(GetJavaObject(), GetMethodID("getNotifications")));

			s32 length = environment->GetArrayLength(notificationsJNI);
			for (s32 notificationIndex = 0; notificationIndex < length; ++notificationIndex)
			{
				jobject notificationJNI = environment->GetObjectArrayElement(notificationsJNI, notificationIndex);
				jclass notificationClassJNI = environment->GetObjectClass(notificationJNI);

				//Time
				jmethodID timeMethodId = environment->GetMethodID(notificationClassJNI, "getTime", "()J");
				TimeIntervalSecs time = static_cast<TimeIntervalSecs>(environment->CallLongMethod(notificationJNI, timeMethodId) / 1000);

				if (time > in_time && time < in_time + in_period)
				{
					CSCore::NotificationSPtr notification(new CSCore::Notification());

					//Notification Id
					jmethodID idMethodId = environment->GetMethodID(notificationClassJNI, "getNotificationId", "()I");
					notification->m_id = static_cast<CSCore::Notification::ID>(environment->CallIntMethod(notificationJNI, idMethodId));

					//Priority
					jmethodID priorityMethodId = environment->GetMethodID(notificationClassJNI, "getPriority", "()I");
					notification->m_priority = static_cast<CSCore::Notification::Priority>(environment->CallIntMethod(notificationJNI, priorityMethodId));

					//params length
					jmethodID paramsLengthMethodId = environment->GetMethodID(notificationClassJNI, "getNumParams", "()I");
					s32 paramsLength = environment->CallIntMethod(notificationJNI, paramsLengthMethodId);

					//params
					jclass stringClassJNI = environment->FindClass("java/lang/String");
					jobjectArray keysJNI = environment->NewObjectArray(paramsLength, stringClassJNI, nullptr);
					jobjectArray valuesJNI = environment->NewObjectArray(paramsLength, stringClassJNI, nullptr);
					environment->DeleteLocalRef(stringClassJNI);

					jmethodID paramsMethodId = environment->GetMethodID(notificationClassJNI, "getParams", "([Ljava/lang/String;[Ljava/lang/String;)V");
					environment->CallVoidMethod(notificationJNI, paramsMethodId, keysJNI, valuesJNI);

					//convert the params to param dictionary
					for (s32 paramIndex = 0; paramIndex < paramsLength; ++paramIndex)
					{
						jstring keyJNI = static_cast<jstring>(environment->GetObjectArrayElement(keysJNI, paramIndex));
						jstring valueJNI = static_cast<jstring>(environment->GetObjectArrayElement(valuesJNI, paramIndex));

						std::string key = JavaInterfaceUtils::CreateSTDStringFromJString(keyJNI);
						std::string value = JavaInterfaceUtils::CreateSTDStringFromJString(valueJNI);

						notification->m_params.SetValue(key, value);

						environment->DeleteLocalRef(keyJNI);
						environment->DeleteLocalRef(valueJNI);
					}

					environment->DeleteLocalRef(keysJNI);
					environment->DeleteLocalRef(valuesJNI);

					out_notifications.push_back(notification);
				}

				environment->DeleteLocalRef(notificationClassJNI);
				environment->DeleteLocalRef(notificationJNI);
			}

			environment->DeleteLocalRef(notificationsJNI);
		}
		//--------------------------------------------------------
		//--------------------------------------------------------
		void LocalNotificationJavaInterface::CancelByID(u32 in_id)
		{
			JNIEnv* environment = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return environment->CallVoidMethod(GetJavaObject(), GetMethodID("cancelByID"), in_id);
		}
		//--------------------------------------------------------
		//--------------------------------------------------------
		void LocalNotificationJavaInterface::CancelAll()
		{
			JNIEnv* environment = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return environment->CallVoidMethod(GetJavaObject(), GetMethodID("cancelAll"));
		}
	}
}

#endif
