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

#include <CSBackend/Platform/Android/Networking/Http/HttpRequestSystem.h>

#include <CSBackend/Platform/Android/Networking/Http/HttpRequest.h>
#include <CSBackend/Platform/Android/Networking/Http/HttpRequestJavaInterface.h>

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(HttpRequestSystem);

		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		bool HttpRequestSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == CSNetworking::HttpRequestSystem::InterfaceID || in_interfaceId == HttpRequestSystem::InterfaceID;
		}
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        void HttpRequestSystem::SetConnectionTimeout(u32 in_timeoutSecs)
        {
        	HttpRequestJavaInterface::SetConnectionTimeout(in_timeoutSecs);
        }
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		CSNetworking::HttpRequest* HttpRequestSystem::MakeRequest(const CSNetworking::HttpRequest::Desc& in_requestDesc, const CSNetworking::HttpRequest::Delegate& in_delegate)
        {
			CS_ASSERT(in_requestDesc.m_url.empty() == false, "Cannot make an http request to a blank url");

            //The ownership of the request is with the request itself
			HttpRequest* request = new HttpRequest(in_requestDesc, GetMaxBufferSize(), in_delegate);
            
			m_requests.push_back(request);
			return request;
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::CancelAllRequests()
        {
			for(u32 nRequest=0; nRequest<m_requests.size(); ++nRequest)
            {
				m_requests[nRequest]->Cancel();
			}
		}
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        bool HttpRequestSystem::CheckReachability() const
        {
            return HttpRequestJavaInterface::IsConnected();
        }
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::OnUpdate(f32 in_timeSinceLastUpdate)
		{
            //We should do this in two loops incase anyone tries to insert into the requests from the completion callback
			for(u32 i=0; i<m_requests.size(); ++i)
            {
                m_requests[i]->Update();
            }

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
        }
	}
}

#endif
