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
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

extern "C"
{
	void Java_com_chillisource_core_LocalNotificationNativeInterface_nativeOnNotificationReceived(JNIEnv* in_environment, jobject in_this, s32 in_id, jobjectArray in_keys, jobjectArray in_values, s32 in_priority);
}

void Java_com_chillisource_core_LocalNotificationNativeInterface_nativeOnNotificationReceived(JNIEnv* in_environment, jobject in_this, s32 in_id, jobjectArray in_keys, jobjectArray in_values, s32 in_priority)
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
			localNotificationSystem->OnNotificationReceived((CSCore::Notification::ID)in_id, params, (CSCore::Notification::Priority)in_priority);
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
			CreateNativeInterface("com/chillisource/core/LocalNotificationNativeInterface");
			CreateMethodReference("scheduleNotificationForTime", "(I[Ljava/lang/String;[Ljava/lang/String;JI)V");
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
