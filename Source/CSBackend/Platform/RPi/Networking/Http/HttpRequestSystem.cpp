//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#include <CSBackend/Platform/RPi/Networking/Http/HttpRequestSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <curl/curl.h>

namespace CSBackend
{
	namespace RPi
	{
		CS_DEFINE_NAMEDTYPE(HttpRequestSystem);

		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::OnInit() noexcept
		{
			curl_global_init(CURL_GLOBAL_SSL);
		}

		//--------------------------------------------------------------------------------------------------
		bool HttpRequestSystem::IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept
		{
			return interfaceId == ChilliSource::HttpRequestSystem::InterfaceID || interfaceId == HttpRequestSystem::InterfaceID;
		}

		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakeGetRequest(const std::string& url, const HttpRequest::Delegate& delegate, u32 timeoutSecs) noexcept
		{
			return MakeRequest(HttpRequest::Type::k_get, url, "", ChilliSource::ParamDictionary(), delegate, timeoutSecs);
		}

		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakeGetRequest(const std::string& url, const ChilliSource::ParamDictionary& headers, const HttpRequest::Delegate& delegate, u32 timeoutSecs) noexcept
		{
			return MakeRequest(HttpRequest::Type::k_get, url, "", headers, delegate, timeoutSecs);
		}

		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakePostRequest(const std::string& url, const std::string& body, const HttpRequest::Delegate& delegate, u32 timeoutSecs) noexcept
		{
			return MakeRequest(HttpRequest::Type::k_post, url, body, ChilliSource::ParamDictionary(), delegate, timeoutSecs);
		}

		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakePostRequest(const std::string& url, const std::string& body, const ChilliSource::ParamDictionary& headers, const HttpRequest::Delegate& delegate, u32 timeoutSecs) noexcept
		{
			return MakeRequest(HttpRequest::Type::k_post, url, body, headers, delegate, timeoutSecs);
		}

		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakeRequest(HttpRequest::Type type, const std::string& url, const std::string& body, const ChilliSource::ParamDictionary& headers, const HttpRequest::Delegate& delegate, u32 timeoutSecs) noexcept
		{
			CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Http requests can currently only be made on the main thread");
			CS_ASSERT(delegate != nullptr, "Cannot make an http request with a null delegate");
			CS_ASSERT(url.empty() == false, "Cannot make an http request to a blank url");

			//TODO: Persistent connections. Don't close the connection but instead check if the next request is to the same server
			//and reuse it. If not then close the connection and open a new one
			auto curl = curl_easy_init();
			HttpRequest* httpRequest = new HttpRequest(type, url, body, headers, timeoutSecs, curl, GetMaxBufferSize(), delegate);
			m_requests.push_back(httpRequest);
			return httpRequest;
		}

		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::CancelAllRequests() noexcept
		{
			CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Http requests can currently only be made on the main thread");

			for(auto request : m_requests)
			{
				request->Cancel();
			}
		}

		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::CheckReachability(const ReachabilityResultDelegate& delegate) const noexcept
		{
			//TODO: RPi: Add reachability check if possible
            CS_ASSERT(delegate, "The reachability delegate should not be null.");
			CS_LOG_WARNING("CheckReachability: Not implemented on Raspberry Pi");
			delegate(true);
		}

		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::OnUpdate(f32 timeSinceLastUpdate) noexcept
		{
			//We should do this in two loops incase anyone tries to insert into the requests from the completion callback
			for (u32 i=0; i<m_requests.size(); ++i)
			{
				m_requests[i]->Update(timeSinceLastUpdate);
			}

			for (auto it = m_requests.begin(); it != m_requests.end(); /*No increment*/)
			{
				if((*it)->HasCompleted())
				{
					//...and remove the completed request
					curl_easy_cleanup((*it)->m_curl);
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
		void HttpRequestSystem::OnDestroy() noexcept
		{
			CancelAllRequests();

			HttpRequest::Shutdown();

			for (auto it = m_requests.begin(); it != m_requests.end(); ++it)
			{
				curl_easy_cleanup((*it)->m_curl);
				CS_SAFEDELETE(*it);
			}

			m_requests.clear();
			m_requests.shrink_to_fit();

			curl_global_cleanup();
		}
	}
}

#endif
