//
//  HttpRequestSystem.cpp
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

#include <CSBackend/Platform/Android/Main/JNI/Networking/Http/HttpRequestSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(HttpRequestSystem);

		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		bool HttpRequestSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ChilliSource::HttpRequestSystem::InterfaceID || in_interfaceId == HttpRequestSystem::InterfaceID;
		}
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        HttpRequest* HttpRequestSystem::MakeGetRequest(const std::string& in_url, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
        {
            return MakeRequest(HttpRequest::Type::k_get, in_url, "", ChilliSource::ParamDictionary(), in_delegate, in_timeoutSecs);
        }
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        HttpRequest* HttpRequestSystem::MakeGetRequest(const std::string& in_url, const ChilliSource::ParamDictionary& in_headers, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
        {
            return MakeRequest(HttpRequest::Type::k_get, in_url, "", in_headers, in_delegate, in_timeoutSecs);
        }
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        HttpRequest* HttpRequestSystem::MakePostRequest(const std::string& in_url, const std::string& in_body, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
        {
            return MakeRequest(HttpRequest::Type::k_post, in_url, in_body, ChilliSource::ParamDictionary(), in_delegate, in_timeoutSecs);
        }
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        HttpRequest* HttpRequestSystem::MakePostRequest(const std::string& in_url, const std::string& in_body, const ChilliSource::ParamDictionary& in_headers, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
        {
            return MakeRequest(HttpRequest::Type::k_post, in_url, in_body, in_headers, in_delegate, in_timeoutSecs);
        }
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
        HttpRequest* HttpRequestSystem::MakeRequest(HttpRequest::Type in_type, const std::string& in_url, const std::string& in_body, const ChilliSource::ParamDictionary& in_headers, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
        {
        	CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Http requests can currently only be made on the main thread");
        	CS_ASSERT(in_delegate != nullptr, "Cannot make an http request with a null delegate");
			CS_ASSERT(in_url.empty() == false, "Cannot make an http request to a blank url");

			HttpRequest* request = new HttpRequest(in_type, in_url, in_body, in_headers, in_timeoutSecs, GetMaxBufferSize(), in_delegate);
            
			m_requests.push_back(request);
			return request;
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::CancelAllRequests()
        {
			CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Http requests can currently only be made on the main thread");

			for(u32 nRequest=0; nRequest<m_requests.size(); ++nRequest)
            {
				m_requests[nRequest]->Cancel();
			}
		}
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        bool HttpRequestSystem::CheckReachability() const
        {
            return m_javaSystem->CallBoolMethod("isConnected");
        }
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
        void HttpRequestSystem::OnInit()
        {
            JavaClassDef classDef("com/chilliworks/chillisource/networking/HttpRequestSystem");
            classDef.AddMethod("isConnected", "()Z");

            m_javaSystem = JavaSystemUPtr(new JavaSystem(classDef));
        }
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::OnUpdate(f32 in_timeSinceLastUpdate)
		{
            for(auto it = m_requests.begin(); it != m_requests.end(); /*No increment*/)
            {
                if((*it)->HasCompleted())
                {
                    CS_SAFEDELETE(*it);
                    it = m_requests.erase(it);
                }
                else
                {
                    ++it;
                }
			}
		}
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        void HttpRequestSystem::OnDestroy()
        {
            CancelAllRequests();

            for(auto it = m_requests.begin(); it != m_requests.end(); ++it)
            {
                CS_SAFEDELETE(*it);
            }

            m_requests.clear();
            m_requests.shrink_to_fit();

            m_javaSystem.reset();
        }
	}
}

#endif
