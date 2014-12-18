//
//  HttpRequestJavaInterface.cpp
//  Chilli Source
//  Created by Ian Copland on 09/11/2011.
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

#include <CSBackend/Platform/Android/Networking/Http/HttpRequestJavaInterface.h>

#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

namespace CSBackend
{
	namespace Android
	{
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void HttpRequestJavaInterface::SetupJavaInterface(JavaVM* in_javaVM)
		{
			mspJavaVM = in_javaVM;

			//Setup Java calls
			InitCallableStaticMethod("com/chilliworks/chillisource/networking/HttpRequestNativeInterface","HttpRequestWithHeaders", "(Ljava/lang/String;Z[Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;I[I[I[I)[B");
			InitCallableStaticMethod("com/chilliworks/chillisource/networking/HttpRequestNativeInterface","IsConnected", "()Z");
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		HttpRequestJavaInterface::RequestResultCode HttpRequestJavaInterface::MakeHttpRequest(const std::string& in_url, RequestType in_type, const CSCore::ParamDictionary& in_headers, const std::string& in_body, s32 in_timeout,
																		std::string& out_response, s32& out_responseCode)
		{
			MethodReference methodRef = GetStaticMethodReference("HttpRequestWithHeaders");
			CS_ASSERT(methodRef.mMethodID != 0 && methodRef.mClassID != 0, "Cannot find HttpRequestWithHeaders Java method");

			JNIEnv* env = GetJNIEnvironmentPtr();

			//Create an array out headers ParamDictionary as this is easier to pass to Java
			jstring emptyString = env->NewStringUTF("");
			jclass stringClass = env->FindClass("java/lang/String");
			jobjectArray keys = env->NewObjectArray(in_headers.size(), stringClass, emptyString);
			jobjectArray values = env->NewObjectArray(in_headers.size(), stringClass, emptyString);

			u32 udwParamCount = 0;
			for(auto it = in_headers.begin(); it != in_headers.end(); ++it)
			{
				jstring jstrFirst = CreateJStringFromSTDString(it->first);
				jstring jstrSecond = CreateJStringFromSTDString(it->second);
				env->SetObjectArrayElement(keys, udwParamCount, jstrFirst);
				env->SetObjectArrayElement(values, udwParamCount, jstrSecond);
				env->DeleteLocalRef(jstrFirst);
				env->DeleteLocalRef(jstrSecond);
				udwParamCount++;
			}

			//create the strings
			jstring url = CreateJStringFromSTDString(in_url);
			jstring body = CreateJStringFromSTDString(in_body);
			jintArray resultLengthBuffer = env->NewIntArray(1);
			jintArray responseCodeBuffer = env->NewIntArray(1);
			jintArray resultCodeBuffer = env->NewIntArray(1);
			bool isPost = in_type == RequestType::k_post;

			//send the request
			jbyteArray responseBuffer = static_cast<jbyteArray>(env->CallStaticObjectMethod(methodRef.mClassID, methodRef.mMethodID,
					url, isPost, keys, values, body, in_timeout,
					resultLengthBuffer, resultCodeBuffer, responseCodeBuffer));

			//get the result codes
			RequestResultCode result = (RequestResultCode)GetIntElementFromJArray(resultCodeBuffer, 0);
			out_responseCode = GetIntElementFromJArray(responseCodeBuffer, 0);

			//if successful, get the response
			if (result == RequestResultCode::k_success)
			{
				s32 length = GetIntElementFromJArray(resultLengthBuffer, 0);
				out_response = CreateSTDStringFromJByteArray(responseBuffer, length);
			}

			//delete all local references
			env->DeleteLocalRef(url);
			env->DeleteLocalRef(body);
			env->DeleteLocalRef(resultLengthBuffer);
			env->DeleteLocalRef(responseCodeBuffer);
			env->DeleteLocalRef(resultCodeBuffer);
			env->DeleteLocalRef(responseBuffer);
			env->DeleteLocalRef(keys);
			env->DeleteLocalRef(values);
			env->DeleteLocalRef(emptyString);
			env->DeleteLocalRef(stringClass);

			return result;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		bool HttpRequestJavaInterface::IsConnected()
		{
			MethodReference methodRef = GetStaticMethodReference("IsConnected");
			CS_ASSERT(methodRef.mMethodID != 0 && methodRef.mClassID != 0, "Cannot find Http: IsConnected Java method");
			JNIEnv* env = GetJNIEnvironmentPtr();
			return static_cast<bool>(env->CallStaticBooleanMethod(methodRef.mClassID, methodRef.mMethodID));
		}
	}
}


#endif
