//
//  DeviceJavaInterface.cpp
//  Chilli Source
//  Created by Scott Downie on 24/09/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <CSBackend/Platform/Android/Core/Base/DeviceJavaInterface.h>

#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(DeviceJavaInterface);

		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		DeviceJavaInterface::DeviceJavaInterface()
		: m_OSVersionCode(-1), m_numberOfCores(-1)
		{
			CreateNativeInterface("com/chilliworks/chillisource/core/DeviceNativeInterface");
			CreateMethodReference("getDefaultLocaleCode", "()Ljava/lang/String;");
			CreateMethodReference("getDeviceModel", "()Ljava/lang/String;");
			CreateMethodReference("getDeviceManufacturer", "()Ljava/lang/String;");
			CreateMethodReference("getDeviceModelType", "()Ljava/lang/String;");
			CreateMethodReference("getOSVersion", "()I");
			CreateMethodReference("getNumberOfCores", "()I");
			CreateMethodReference("getUniqueId", "()Ljava/lang/String;");
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		bool DeviceJavaInterface::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == DeviceJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string DeviceJavaInterface::GetDefaultLocaleCode()
		{
			if (m_localeCode == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrLocalCode = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getDefaultLocaleCode")));
				m_localeCode = JavaInterfaceUtils::CreateSTDStringFromJString(jstrLocalCode);
				env->DeleteLocalRef(jstrLocalCode);
			}
			return m_localeCode;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string DeviceJavaInterface::GetDeviceModel()
		{
			if (m_deviceModel == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceModel = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getDeviceModel")));
				m_deviceModel = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceModel);
				env->DeleteLocalRef(jstrDeviceModel);
			}
			return m_deviceModel;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string DeviceJavaInterface::GetDeviceModelType()
		{
			if (m_deviceModelType == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceModelType = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getDeviceModelType")));
				m_deviceModelType = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceModelType);
				env->DeleteLocalRef(jstrDeviceModelType);
			}
			return m_deviceModelType;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string DeviceJavaInterface::GetDeviceManufacturer()
		{
			if (m_deviceManufacturer == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceManufacturer = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getDeviceManufacturer")));
				m_deviceManufacturer = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceManufacturer);
				env->DeleteLocalRef(jstrDeviceManufacturer);
			}
			return m_deviceManufacturer;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		s32 DeviceJavaInterface::GetOSVersionCode()
		{
			if (m_OSVersionCode == -1)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				m_OSVersionCode = env->CallIntMethod(GetJavaObject(), GetMethodID("getOSVersion"));
			}
			return m_OSVersionCode;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		s32 DeviceJavaInterface::GetNumberOfCores()
		{
			if (m_numberOfCores == -1)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				m_numberOfCores = env->CallIntMethod(GetJavaObject(), GetMethodID("getNumberOfCores"));
			}
			return m_numberOfCores;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string DeviceJavaInterface::GetUniqueId()
		{
			std::string output = "";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring adId = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getUniqueId")));
			output = JavaInterfaceUtils::CreateSTDStringFromJString(adId);
			env->DeleteLocalRef(adId);
			return output;
		}
	}
}

#endif
