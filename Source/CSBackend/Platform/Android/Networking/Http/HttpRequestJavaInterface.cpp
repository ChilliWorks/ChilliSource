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

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <CSBackend/Platform/Android/Networking/Http/HttpRequest.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	//-----------------------------------------------------------------------
	/// Called when http request contents are flushed due to exceeding
	/// the buffer size
    ///
    /// @author S Downie
	///
	/// @param The jni environment.
	/// @param The java object calling the function.
	/// @param Partial data
	/// @param Length of partial data
	/// @param Response code
	/// @param Request Id
	//-----------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_networking_HttpRequestNativeInterface_OnBufferFlushed(JNIEnv* in_env, jobject in_this, jbyteArray in_data, jint in_dataLength, jint in_responseCode, jint in_requestId);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void Java_com_chilliworks_chillisource_networking_HttpRequestNativeInterface_OnBufferFlushed(JNIEnv* in_env, jobject in_this, jbyteArray in_data, jint in_dataLength, jint in_responseCode, jint in_requestId)
{
	std::string data = CSBackend::Android::JavaInterfaceUtils::CreateSTDStringFromJByteArray(in_data, in_dataLength);
	CSBackend::Android::HttpRequestJavaInterface::OnFlushed(data, (u32)in_responseCode, in_requestId);
}

namespace CSBackend
{
	namespace Android
	{
		std::unordered_map<s32, HttpRequest*> HttpRequestJavaInterface::s_requestMap;
		std::mutex HttpRequestJavaInterface::s_requestMutex;
		s32 HttpRequestJavaInterface::s_requestIdCounter = 0;

		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void HttpRequestJavaInterface::SetupJavaInterface(JavaVM* in_javaVM)
		{
			mspJavaVM = in_javaVM;

			//Setup Java calls
			InitCallableStaticMethod("com/chilliworks/chillisource/networking/HttpRequestNativeInterface","HttpRequestWithHeaders", "(Ljava/lang/String;Z[Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;III[I[I[I)[B");
			InitCallableStaticMethod("com/chilliworks/chillisource/networking/HttpRequestNativeInterface","IsConnected", "()Z");
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		HttpRequestJavaInterface::RequestResultCode HttpRequestJavaInterface::MakeHttpRequest(HttpRequest* in_request, const std::string& in_url, RequestType in_type, const CSCore::ParamDictionary& in_headers, const std::string& in_body,
																		s32 in_timeout, s32 in_maxBufferSize,
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

			//Add this to the pending requests
			std::unique_lock<std::mutex> lockInsert(s_requestMutex);
			auto requestId = s_requestIdCounter;
			s_requestIdCounter++;
			s_requestMap.emplace(requestId, in_request);
			lockInsert.unlock();

			//send the request
			jbyteArray responseBuffer = static_cast<jbyteArray>(env->CallStaticObjectMethod(methodRef.mClassID, methodRef.mMethodID,
					url, isPost, keys, values, body, in_timeout, in_maxBufferSize, requestId,
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

			//Remove this from the pending requests
			std::unique_lock<std::mutex> lockErase(s_requestMutex);
			s_requestMap.erase(requestId);
			lockErase.unlock();

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
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void HttpRequestJavaInterface::OnFlushed(const std::string& in_data, u32 in_responseCode, s32 in_requestId)
		{
			std::unique_lock<std::mutex> lock(s_requestMutex);
			auto it = s_requestMap.find(in_requestId);
			if(it != s_requestMap.end())
			{
				CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&CSBackend::Android::HttpRequest::OnFlushed, it->second, in_data, in_responseCode));
			}
		}
	}
}


#endif
