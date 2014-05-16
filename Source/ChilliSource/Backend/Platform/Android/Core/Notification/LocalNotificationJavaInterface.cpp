//
//  LocalNotificationJavaInterface.h
//  Chilli Source
//
//  Created by Steven Hendrie on 28/09/2012.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Core/Notification/LocalNotificationJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Backend/Platform/Android/Core/Notification/LocalNotificationSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <jni.h>

extern "C"
{
	void Java_com_chillisource_core_LocalNotificationNativeInterface_nativeOnNotificationReceived(JNIEnv* in_environment, jobject in_this, s32 in_id, jobjectArray in_keys, jobjectArray in_values, s32 in_priority);
}

void Java_com_chillisource_core_LocalNotificationNativeInterface_nativeOnNotificationReceived(JNIEnv* in_environment, jobject in_this, s32 in_id, jobjectArray in_keys, jobjectArray in_values, s32 in_priority)
{
	ChilliSource::Core::ParamDictionary params;

	u32 numParams = in_environment->GetArrayLength(in_keys);
	for(u32 keyIndex = 0; keyIndex < numParams; ++keyIndex)
	{
		jstring keyJava = (jstring)in_environment->GetObjectArrayElement(in_keys, keyIndex);
		jstring valueJava =  (jstring)in_environment->GetObjectArrayElement(in_values, keyIndex);

		std::string key = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(keyJava);
		std::string value = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(valueJava);

		params.SetValue(key, value);

		in_environment->DeleteLocalRef(keyJava);
		in_environment->DeleteLocalRef(valueJava);
	}

	ChilliSource::Android::LocalNotificationSystem* localNotificationSystem = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::Android::LocalNotificationSystem>();
	if (localNotificationSystem != nullptr)
	{
		localNotificationSystem->OnNotificationReceived((ChilliSource::Core::Notification::ID)in_id, params, (ChilliSource::Core::Notification::Priority)in_priority);
	}
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(LocalNotificationJavaInterface);
		//--------------------------------------------------------
		//--------------------------------------------------------
		LocalNotificationJavaInterface::LocalNotificationJavaInterface()
		{
			CreateNativeInterface("com/chillisource/core/LocalNotificationNativeInterface");
			CreateMethodReference("scheduleNotificationForTime", "(I[Ljava/lang/String;[Ljava/lang/String;JI)V");
			CreateMethodReference("cancelByID","(I)V");
			CreateMethodReference("cancelAll","()V");
		}
		//--------------------------------------------------------
		//--------------------------------------------------------
		bool LocalNotificationJavaInterface::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == LocalNotificationJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------
		//--------------------------------------------------------
		void LocalNotificationJavaInterface::ScheduleNotificationForTime(Core::Notification::ID in_id, const Core::ParamDictionary& in_params, TimeIntervalSecs in_time, Core::Notification::Priority in_priority)
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
