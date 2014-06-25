//
//  JavaInterfaceUtils.cpp
//  Chilli Source
//  Created by Ian Copland on 09/08/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace CSBackend
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

#endif
