//
//  HttpRequest.cpp
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

#import <CSBackend/Platform/iOS/Networking/Http/HttpRequest.h>

#import <CSBackend/Platform/iOS/Networking/Http/HttpDelegate.h>
#include <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

#import <Foundation/Foundation.h>

namespace CSBackend
{
	namespace iOS
	{
        //------------------------------------------------------------------
        //------------------------------------------------------------------
		HttpRequest::HttpRequest(Type in_type, const std::string& in_url, const std::string& in_body, const ChilliSource::ParamDictionary& in_headers, u32 in_timeoutSecs, u32 in_maxBufferSize, const Delegate& in_delegate)
        : m_type(in_type), m_url(in_url), m_body(in_body), m_headers(in_headers), m_completionDelegate(in_delegate)
		{
            CS_ASSERT(m_completionDelegate, "Http request cannot have null delegate");
            
            m_complete = std::shared_ptr<bool>(new bool(false));
            m_isCancelled = std::shared_ptr<bool>(new bool(false));
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                @autoreleasepool
                {
                    NSString* urlString = [NSStringUtils newNSStringWithUTF8String:m_url];
                    NSURL* url = [NSURL URLWithString:urlString];
                    [urlString release];
                    
                    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:in_timeoutSecs];
                    
                    //apply header
                    NSMutableDictionary* header = [NSMutableDictionary dictionaryWithCapacity:m_headers.size()];
                    for (const auto& fieldPair : m_headers)
                    {
                        NSString* key = [NSStringUtils newNSStringWithUTF8String:fieldPair.first];
                        NSString* value = [NSStringUtils newNSStringWithUTF8String:fieldPair.second];
                        [header setObject:value forKey:key];
                        [value release];
                        [key release];
                    }
                    [request setAllHTTPHeaderFields:header];
                    
                    //apply body if a post request.
                    if (m_type == Type::k_post)
                    {
                        request.HTTPMethod = @"POST";
                        request.HTTPBody = [NSData dataWithBytes:m_body.c_str() length:m_body.length()];
                    }
                    
                    auto connectionEstablishedDelegate = [this](u64 in_expectedSize)
                    {
                        m_expectedSize = in_expectedSize;
                    };
                    
                    auto complete = m_complete;
                    auto cancelled = m_isCancelled;
                    
                    auto connectionFlushedDelegate = [=](ChilliSource::HttpResponse::Result in_result, u32 in_responseCode, const std::string& in_data)
                    {
                        ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
                        {
                            //Ensure that we only call the delegate when cancelled is false as (*this) may not be around
                            if(!*complete && !*cancelled)
                            {
                                m_downloadedBytes += in_data.length();
                                m_completionDelegate(this, ChilliSource::HttpResponse(in_result, in_responseCode, in_data));
                            }
                        });
                    };
                    
                    auto connectionCompleteDelegate = [=](ChilliSource::HttpResponse::Result in_result, u32 in_responseCode, const std::string& in_data)
                    {
                        ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
                        {
                            //Ensure that we only call the delegate when cancelled is false as (*this) may not be around
                            if(!*complete && !*cancelled)
                            {
                                *complete = true;
                                m_downloadedBytes += in_data.length();
                                
                                m_completionDelegate(this, ChilliSource::HttpResponse(in_result, in_responseCode, in_data));
                            }
                        });
                    };
                    
                    m_httpDelegate = [[HttpDelegate alloc] initWithConnectionDelegate:connectionEstablishedDelegate andFlushedDelegate:connectionFlushedDelegate andCompleteDelegate:connectionCompleteDelegate andMaxBufferSize:in_maxBufferSize];
                    m_connection = [[NSURLConnection connectionWithRequest:[request copy] delegate: m_httpDelegate] retain];
                }
            });
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
        const ChilliSource::ParamDictionary& HttpRequest::GetHeaders() const
        {
            return m_headers;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        u64 HttpRequest::GetExpectedSize() const
        {
            return m_expectedSize;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        u64 HttpRequest::GetDownloadedBytes() const
        {
            return m_downloadedBytes;
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
		void HttpRequest::Cancel()
		{
            CS_ASSERT(*m_complete == false, "Cannot cancel an already complete request.");
            
            *m_isCancelled = true;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                [m_connection cancel];
            });
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        HttpRequest::~HttpRequest()
        {
            NSURLConnection* connection = m_connection;
            HttpDelegate* delegate = m_httpDelegate;
            m_connection = nil;
            m_httpDelegate = nil;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                [connection release];
                [delegate release];
            });
        }
	}
}

#endif
