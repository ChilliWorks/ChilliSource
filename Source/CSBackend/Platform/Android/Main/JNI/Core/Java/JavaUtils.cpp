//
//  JavaUtils.cpp
//  ChilliSource
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

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaVirtualMachine.h>

namespace CSBackend
{
	namespace Android
	{
		namespace JavaUtils
		{
			//------------------------------------------------------------------------------
			//------------------------------------------------------------------------------
			jstring CreateJStringFromSTDString(const std::string& in_string)
			{
				auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
				jstring output = environment->NewStringUTF(in_string.c_str());
				return output;
			}
			//------------------------------------------------------------------------------
			//------------------------------------------------------------------------------
			std::string CreateSTDStringFromJString(jstring in_jString)
			{
			    if (in_jString == nullptr)
			    {
			        return "";
			    }

				auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
                const char * cString = environment->GetStringUTFChars(in_jString, JNI_FALSE);
                CS_ASSERT(cString != nullptr, "String returned from GetStringUTFChars() cannot be null.");

                std::string output = std::string(cString);
                environment->ReleaseStringUTFChars(in_jString, cString);

                return output;
			}
			//------------------------------------------------------------------------------
			//------------------------------------------------------------------------------
			std::string CreateSTDStringFromJByteArray(jbyteArray in_array, u32 in_length)
			{
			    if (in_array == nullptr || in_length == 0)
			    {
			        return "";
			    }

				auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();

                jbyte* bytes = environment->GetByteArrayElements(in_array, JNI_FALSE);
                CS_ASSERT(bytes != nullptr, "Bytes returned by GetByteArrayElements() cannot be null.");

                std::string strOutput((const char*)bytes, in_length);
                environment->ReleaseByteArrayElements(in_array, bytes, 0);

                return strOutput;
			}
			//------------------------------------------------------------------------------
			//------------------------------------------------------------------------------
			s32 GetIntElementFromJArray(jintArray in_array, u32 in_index)
			{
			    CS_ASSERT(in_array != nullptr, "Cannot get int element from null array.");

			    auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();

                jint* integers = environment->GetIntArrayElements(in_array, JNI_FALSE);
                CS_ASSERT(integers != nullptr, "Integers returned by GetIntArrayElements() cannot be null.");

                s32 output = integers[in_index];
                environment->ReleaseIntArrayElements(in_array, integers, 0);

                return output;
			}
			//------------------------------------------------------------------------------
			//------------------------------------------------------------------------------
			void CheckJavaExceptions(const std::string& in_errorMessage)
			{
				auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();

				jthrowable exception = environment->ExceptionOccurred();
				if (exception != nullptr)
				{
					environment->ExceptionDescribe();
					environment->ExceptionClear();

					CS_LOG_FATAL(in_errorMessage);
				}
			}
			//------------------------------------------------------------------------------
			//------------------------------------------------------------------------------
			void DeleteLocalRef(jobject in_localReference)
			{
				auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();

				environment->DeleteLocalRef(in_localReference);
			}
		}
	}
}

#endif
