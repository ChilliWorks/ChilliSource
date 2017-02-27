//
//  HttpRequestSystem.cpp
//  ChilliSource
//  Created by Scott Downie on 23/05/2011.
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

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Networking/Http/HttpRequestSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#import <Reachability/CSReachability.h>
#import <CFNetwork/CFNetwork.h>

namespace CSBackend
{
	namespace iOS
	{
        CS_DEFINE_NAMEDTYPE(HttpRequestSystem);
        
        //------------------------------------------------------------------
        //------------------------------------------------------------------
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
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        HttpRequest* HttpRequestSystem::MakeRequest(HttpRequest::Type in_type, const std::string& in_url, const std::string& in_body, const ChilliSource::ParamDictionary& in_headers, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
        {
            CS_RELEASE_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Http requests can currently only be made on the main thread");
            CS_ASSERT(in_delegate != nullptr, "Cannot make an http request with a null delegate");
            CS_ASSERT(in_url.empty() == false, "Cannot make an http request to a blank url");
            
            HttpRequestUPtr request(new HttpRequest(in_type, in_url, in_body, in_headers, in_timeoutSecs, GetMaxBufferSize(), [=](const ChilliSource::HttpRequest* in_request, const ChilliSource::HttpResponse& in_response)
            {
                //If flushed, we are not finished with the request yet
                if(in_response.GetResult() != ChilliSource::HttpResponse::Result::k_flushed)
                {
                    m_finishedRequests.push_back(in_request);
                }
                
                in_delegate(in_request, in_response);
            }));
            
            auto requestRaw = request.get();
            m_requests.push_back(std::move(request));
            
            return requestRaw;
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
		void HttpRequestSystem::CancelAllRequests()
        {
            CS_RELEASE_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Http requests can currently only be made on the main thread");
            
            for(auto& request : m_requests)
            {
				request->Cancel();
			}
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        void HttpRequestSystem::CheckReachability(const ReachabilityResultDelegate& in_reachabilityDelegate) const
        {
            CS_ASSERT(in_reachabilityDelegate, "The reachability delegate should not be null.");
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                CSReachability* reachability = [CSReachability reachabilityForInternetConnection];
                NetworkStatus status = [reachability currentReachabilityStatus];
                
                ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
                {
                    in_reachabilityDelegate(status != NotReachable);
                });
            });
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
		void HttpRequestSystem::OnUpdate(f32 in_timeSinceLastUpdate)
		{
            //remove any finished requests.
            for (auto& finishedRequest : m_finishedRequests)
            {
                for (auto requestIt = m_requests.begin(); requestIt != m_requests.end();)
                {
                    if (requestIt->get() == finishedRequest)
                    {
                        requestIt = m_requests.erase(requestIt);
                        break;
                    }
                    else
                    {
                        ++requestIt;
                    }
                }
            }
            
            m_finishedRequests.clear();
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        void HttpRequestSystem::OnDestroy()
        {
            CancelAllRequests();

            m_requests.clear();
            m_requests.shrink_to_fit();
        }
	}
}

#endif
