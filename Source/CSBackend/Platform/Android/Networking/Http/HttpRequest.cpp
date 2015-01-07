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

#include <CSBackend/Platform/Android/Networking/Http/HttpRequest.h>

#include <CSBackend/Platform/Android/Networking/Http/HttpRequestJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

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

			//Begin the read loop as a threaded task
			CSCore::Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(&HttpRequest::PerformRequest, this));
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void HttpRequest::Update()
		{
			//Check if the data has finished streaming and invoke the completion delegate on the main thread
			if(m_isPollingComplete == true)
			{
                m_isRequestComplete = true;

				if(m_isRequestCancelled == false)
				{
					m_completionDelegate(this, CSNetworking::HttpResponse(m_requestResult, m_responseCode, m_responseData));
				}
			}
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void HttpRequest::PerformRequest()
		{
			HttpRequestJavaInterface::RequestType type = HttpRequestJavaInterface::RequestType::k_get;
			if (m_type == CSNetworking::HttpRequest::Type::k_post)
			{
				type = HttpRequestJavaInterface::RequestType::k_post;
			}

			//This is a blocking call
			HttpRequestJavaInterface::RequestResultCode requestResultCode = HttpRequestJavaInterface::MakeHttpRequest(this, m_url, type, m_headers, m_body, (s32)m_timeoutSecs, (s32)m_maxBufferSize, m_responseData, (s32&)m_responseCode);

			if (m_shouldKillThread == false)
			{
				switch (requestResultCode)
				{
				case HttpRequestJavaInterface::RequestResultCode::k_success:
					m_requestResult = CSNetworking::HttpResponse::Result::k_completed;
					break;
				case HttpRequestJavaInterface::RequestResultCode::k_couldNotConnect:
				case HttpRequestJavaInterface::RequestResultCode::k_couldNotMakeRequest:
					m_requestResult = CSNetworking::HttpResponse::Result::k_failed;
					break;
				case HttpRequestJavaInterface::RequestResultCode::k_timeout:
					m_requestResult = CSNetworking::HttpResponse::Result::k_timeout;
					break;
				default:
					break;
				}
			}

			m_isPollingComplete = true;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void HttpRequest::OnFlushed(const std::string& in_data, u32 in_responseCode)
		{
			m_completionDelegate(this, CSNetworking::HttpResponse(CSNetworking::HttpResponse::Result::k_flushed, in_responseCode, in_data));
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
	}
}

#endif
