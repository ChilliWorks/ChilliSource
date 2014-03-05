/*
 *  JavaInterfaceUtils.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace ChilliSource
{
	namespace Android
	{
		namespace JavaInterfaceUtils
		{
			//--------------------------------------------------------------------------------------
			/// Create JString From STDString
			//--------------------------------------------------------------------------------------
			jstring CreateJStringFromSTDString(const std::string& instrString)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				if (pEnv != nullptr)
				{
					jstring jstrNewString = pEnv->NewStringUTF(instrString.c_str());
					return jstrNewString;
				}
				return nullptr;
			}
			//--------------------------------------------------------------------------------------
			/// Create JString From UTF8String
			//--------------------------------------------------------------------------------------
			jstring CreateJStringFromUTF8String(const Core::UTF8String& instrString)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				if (pEnv != nullptr)
				{
					jstring jstrNewString = pEnv->NewStringUTF(instrString.ToASCII().c_str());
					return jstrNewString;
				}
				return nullptr;
			}
			//--------------------------------------------------------------------------------------
			/// Create STDString From JString
			//--------------------------------------------------------------------------------------
			std::string CreateSTDStringFromJString(jstring injstrString)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				if (pEnv != nullptr && injstrString != nullptr)
				{
					const char * cString = pEnv->GetStringUTFChars(injstrString, JNI_FALSE);
					if (cString != nullptr)
					{
						std::string stdString = std::string(cString);
						pEnv->ReleaseStringUTFChars(injstrString, cString);
						return stdString;
					}
				}
				return "";
			}
			//--------------------------------------------------------------------------------------
			/// Create STDString From JByteArray
			//--------------------------------------------------------------------------------------
			std::string CreateSTDStringFromJByteArray(jbyteArray injabyArray, int indwLength)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				if (pEnv != nullptr && injabyArray != nullptr && indwLength >= 0)
				{
					jbyte* pBytes = pEnv->GetByteArrayElements(injabyArray, JNI_FALSE);
					if (pBytes != nullptr)
					{
						std::string strOutput((const char*)pBytes, indwLength);
						pEnv->ReleaseByteArrayElements(injabyArray, pBytes, 0);
						return strOutput;
					}
				}
				return "";
			}
			//--------------------------------------------------------------------------------------
			/// Create UTF8String From JString
			//--------------------------------------------------------------------------------------
			Core::UTF8String CreateUTF8StringFromJString(jstring injstrString)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				if (pEnv != nullptr && injstrString != nullptr)
				{
					const char * cString = pEnv->GetStringUTFChars(injstrString, JNI_FALSE);
					if (cString != nullptr)
					{
						Core::UTF8String utf8String = Core::UTF8String(cString);
						pEnv->ReleaseStringUTFChars(injstrString, cString);
						return utf8String;
					}
				}
				return "";
			}
			//--------------------------------------------------------------------------------------
			/// Get Int Element From JArray
			//--------------------------------------------------------------------------------------
			s32 GetIntElementFromJArray(jintArray injadwArray, u32 inudwIndex)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				if (pEnv != nullptr && injadwArray != nullptr)
				{
					jint* pIntegers = pEnv->GetIntArrayElements(injadwArray, JNI_FALSE);
					if (pIntegers != nullptr)
					{
						s32 dwOutput = pIntegers[inudwIndex];
						pEnv->ReleaseIntArrayElements(injadwArray, pIntegers, 0);
						return dwOutput;
					}
				}
				return -1;
			}
		}
	}
}
