//
//  HttpRequest.cpp
//  Chilli Source
//  Created by Ian Copland on 08/11/2011.
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

#include <CSBackend/Platform/Android/Main/JNI/Networking/Http/HttpRequest.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClass.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClassDef.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/NativeObjectMediator.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	//-----------------------------------------------------------------------
	/// Called when http request contents are flushed due to exceeding
	/// the buffer size
    ///
    /// @author HMcLaughlin
	///
    /// @param in_env - The jni environment.
    /// @param in_this - The java object calling the function.
    /// @param in_data - Partial Data
    /// @param in_dataLength - Length of partial data
    /// @param in_responseCode - Response code
    /// @param in_objectPointer - NativePointer Java object
	//-----------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_networking_HttpRequest_OnBufferFlushed(JNIEnv* in_env, jobject in_this, jbyteArray in_data, jint in_dataLength, jint in_responseCode, jobject in_objectPointer);
	//-----------------------------------------------------------------------
    /// Called when the connection has finished
    ///
    /// @author HMcLaughlin
    ///
    /// @param in_env - The jni environment.
    /// @param in_this - The java object calling the function.
    /// @param in_resultCode - Result code
    /// @param in_data - Data
    /// @param in_dataLength - Length of partial data
    /// @param in_responseCode - Response code
    /// @param in_objectPointer - NativePointer Java object
    //-----------------------------------------------------------------------
    void Java_com_chilliworks_chillisource_networking_HttpRequest_OnComplete(JNIEnv* in_env, jobject in_this, jint in_resultCode, jbyteArray in_data, jint in_dataLength, jint in_responseCode, jobject in_objectPointer);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void Java_com_chilliworks_chillisource_networking_HttpRequest_OnBufferFlushed(JNIEnv* in_env, jobject in_this, jbyteArray in_data, jint in_dataLength, jint in_responseCode, jobject in_objectPointer)
{
	std::string data = CSBackend::Android::JavaUtils::CreateSTDStringFromJByteArray(in_data, in_dataLength);
	CSBackend::Android::JavaClassSPtr javaNativePointer = CSBackend::Android::JavaClassSPtr(new CSBackend::Android::JavaClass(in_objectPointer, CSBackend::Android::NativeObjectMediator::GetNativePointerClassDef()));

	CSBackend::Android::HttpRequest* httpRequest = CSBackend::Android::NativeObjectMediator::JavaToNativePointer<CSBackend::Android::HttpRequest>(javaNativePointer);
	CS_ASSERT(httpRequest, "Java pointer conversion Error!");
	if(httpRequest)
	{
		std::string data = CSBackend::Android::JavaUtils::CreateSTDStringFromJByteArray(in_data, in_dataLength);
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&CSBackend::Android::HttpRequest::OnFlushed, httpRequest, data, (u32)in_responseCode));
	}
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void Java_com_chilliworks_chillisource_networking_HttpRequest_OnComplete(JNIEnv* in_env, jobject in_this, jint in_resultCode, jbyteArray in_data, jint in_dataLength, jint in_responseCode, jobject in_objectPointer)
{
	std::string data = CSBackend::Android::JavaUtils::CreateSTDStringFromJByteArray(in_data, in_dataLength);

	CSBackend::Android::JavaClassSPtr javaNativePointer = CSBackend::Android::JavaClassSPtr(new CSBackend::Android::JavaClass(in_objectPointer, CSBackend::Android::NativeObjectMediator::GetNativePointerClassDef()));

	CSBackend::Android::HttpRequest* httpRequest = CSBackend::Android::NativeObjectMediator::JavaToNativePointer<CSBackend::Android::HttpRequest>(javaNativePointer);
	CS_ASSERT(httpRequest, "Java pointer conversion Error!");
	if(httpRequest)
	{
		std::string data = CSBackend::Android::JavaUtils::CreateSTDStringFromJByteArray(in_data, in_dataLength);
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&CSBackend::Android::HttpRequest::OnComplete, httpRequest, (u32)in_resultCode, data, (u32)in_responseCode));
	}
}

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		HttpRequest::HttpRequest(Type in_type, const std::string& in_url, const std::string& in_body, const CSCore::ParamDictionary& in_headers, u32 in_timeoutSecs, u32 in_maxBufferSize, const Delegate& in_delegate)
	    : m_type(in_type), m_url(in_url), m_body(in_body), m_headers(in_headers), m_completionDelegate(in_delegate), m_timeoutSecs(in_timeoutSecs), m_maxBufferSize(in_maxBufferSize)
		{
			CS_ASSERT(m_completionDelegate, "Http request cannot have null delegate");

			//Create a java NativePointer for this instance
			JavaClassSPtr nativePointerClass = NativeObjectMediator::NativeToJavaPointer(this);

			//Create a java HttpRequest class definition
			JavaClassDef javaRequestClassDef("com/chilliworks/chillisource/networking/HttpRequest", "(Lcom/chilliworks/chillisource/core/NativePointer;)V");
			javaRequestClassDef.AddMethod("makeHttpRequestWithHeaders", "(Ljava/lang/String;Z[Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;II)V");
			javaRequestClassDef.AddMethod("getCurrentSize", "()J");
			javaRequestClassDef.AddMethod("getExpectedTotalSize", "()J");

			//Create a java HttpRequest instance
			m_javaHttpRequest = JavaClassSPtr(new JavaClass(javaRequestClassDef, nativePointerClass->GetJavaObject()));

			//Immediately make the request
			PerformRequest();
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void HttpRequest::PerformRequest()
		{
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

            //Create an array out headers ParamDictionary as this is easier to pass to Java
            jstring emptyString = env->NewStringUTF("");
            jclass stringClass = env->FindClass("java/lang/String");
            jobjectArray keys = env->NewObjectArray(m_headers.size(), stringClass, emptyString);
            jobjectArray values = env->NewObjectArray(m_headers.size(), stringClass, emptyString);

            u32 paramCount = 0;
            for(auto it = m_headers.begin(); it != m_headers.end(); ++it)
            {
            	jstring jstrFirst = JavaUtils::CreateJStringFromSTDString(it->first);
            	jstring jstrSecond = JavaUtils::CreateJStringFromSTDString(it->second);
            	env->SetObjectArrayElement(keys, paramCount, jstrFirst);
            	env->SetObjectArrayElement(values, paramCount, jstrSecond);
            	env->DeleteLocalRef(jstrFirst);
            	env->DeleteLocalRef(jstrSecond);
            	paramCount++;
            }

            //create the strings
            jstring url = JavaUtils::CreateJStringFromSTDString(m_url);
            jstring body = JavaUtils::CreateJStringFromSTDString(m_body);

            bool isPost = m_type == CSNetworking::HttpRequest::Type::k_post;

			//Finally call the java function
			m_javaHttpRequest->CallVoidMethod("makeHttpRequestWithHeaders", url, isPost, keys, values, body, m_timeoutSecs, m_maxBufferSize);

            //delete all local references
            env->DeleteLocalRef(url);
            env->DeleteLocalRef(body);
            env->DeleteLocalRef(keys);
            env->DeleteLocalRef(values);
            env->DeleteLocalRef(emptyString);
            env->DeleteLocalRef(stringClass);
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void HttpRequest::OnFlushed(const std::string& in_data, u32 in_responseCode)
		{
			m_completionDelegate(this, CSNetworking::HttpResponse(CSNetworking::HttpResponse::Result::k_flushed, in_responseCode, in_data));
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void HttpRequest::OnComplete(u32 in_resultCode, const std::string& in_data, u32 in_responseCode)
		{
			if(m_isRequestCancelled == false && m_completionDelegate)
			{
				m_completionDelegate(this, CSNetworking::HttpResponse((CSNetworking::HttpResponse::Result)in_resultCode, in_responseCode, in_data));
			}
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		void HttpRequest::Cancel()
		{
            m_shouldKillThread = true;
            m_isRequestCancelled = true;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		bool HttpRequest::HasCompleted() const
		{
			return m_isRequestComplete;
		}
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        HttpRequest::Type HttpRequest::GetType() const
        {
            return m_type;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const std::string& HttpRequest::GetUrl() const
        {
            return m_url;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const std::string& HttpRequest::GetBody() const
        {
            return m_body;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const CSCore::ParamDictionary& HttpRequest::GetHeaders() const
        {
            return m_headers;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        u64 HttpRequest::GetExpectedTotalSize() const
        {
			return m_javaHttpRequest->CallLongMethod("getExpectedTotalSize");
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        u64 HttpRequest::GetCurrentSize() const
        {
			return m_javaHttpRequest->CallLongMethod("getCurrentSize");
        }
	}
}

#endif
