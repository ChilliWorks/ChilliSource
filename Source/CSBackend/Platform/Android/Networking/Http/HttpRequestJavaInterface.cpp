//
//  HttpRequestJavaInterface.cpp
//  Chilli Source
//
//  Created by Ian Copland on 09/11/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpRequestJavaInterface.h>

#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void HttpRequestJavaInterface::SetupJavaInterface(JavaVM* in_javaVM)
		{
			mspJavaVM = in_javaVM;

			//Setup Java calls
			InitCallableStaticMethod("com/chillisource/networking/HttpRequestNativeInterface","HttpRequestWithHeaders", "(Ljava/lang/String;Z[Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;[I[Ljava/lang/String;[I[I)[B");
			InitCallableStaticMethod("com/chillisource/networking/HttpRequestNativeInterface","IsConnected", "()Z");
			InitCallableStaticMethod("com/chillisource/networking/HttpRequestNativeInterface","setConnectionTimeout", "(I)V");
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void HttpRequestJavaInterface::SetConnectionTimeout(u32 in_timeoutSecs)
		{
			MethodReference methodRef = GetStaticMethodReference("setConnectionTimeout");
			CS_ASSERT(methodRef.mMethodID != 0 && methodRef.mClassID != 0, "Cannot find Http: setConnectionTimeout Java method");
			JNIEnv* env = GetJNIEnvironmentPtr();
			env->CallStaticVoidMethod(methodRef.mClassID, methodRef.mMethodID, (s32)in_timeoutSecs);
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		HttpRequestJavaInterface::RequestResultCode HttpRequestJavaInterface::MakeHttpRequest(const std::string& in_url, RequestType in_type, const Core::ParamDictionary& in_headers, const std::string& in_body,
																		std::string& out_response, std::string& out_redirectUrl, s32& out_responseCode)
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
			jobjectArray redirectBuffer = env->NewObjectArray(1, stringClass, emptyString);
			jintArray resultLengthBuffer = env->NewIntArray(1);
			jintArray responseCodeBuffer = env->NewIntArray(1);
			jintArray resultCodeBuffer = env->NewIntArray(1);
			bool isPost = in_type == RequestType::k_post;

			//send the request
			jbyteArray responseBuffer = static_cast<jbyteArray>(env->CallStaticObjectMethod(methodRef.mClassID, methodRef.mMethodID,
					url, isPost, keys, values, body,
					resultLengthBuffer, redirectBuffer, resultCodeBuffer, responseCodeBuffer));

			//get the result codes
			RequestResultCode result = (RequestResultCode)GetIntElementFromJArray(resultCodeBuffer, 0);
			out_responseCode = GetIntElementFromJArray(responseCodeBuffer, 0);

			//if successful, get the response
			if (result == RequestResultCode::k_success)
			{
				s32 length = GetIntElementFromJArray(resultLengthBuffer, 0);
				out_response = CreateSTDStringFromJByteArray(responseBuffer, length);

				jstring redirectUrl = (jstring)env->GetObjectArrayElement(redirectBuffer, 0);
				out_redirectUrl = CreateSTDStringFromJString(redirectUrl);
			}

			//delete all local references
			env->DeleteLocalRef(url);
			env->DeleteLocalRef(body);
			env->DeleteLocalRef(redirectBuffer);
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