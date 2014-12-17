//
//  HttpRequest.cpp
//  Chilli Source
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

#include <sstream>
#include <thread>

namespace CSBackend
{
	namespace iOS
	{
        //------------------------------------------------------------------
        //------------------------------------------------------------------
		HttpRequest::HttpRequest(const Desc& in_requestDesc, u32 in_timeoutSecs, const Delegate& in_delegate)
            : m_desc(in_requestDesc), m_completionDelegate(in_delegate)
		{
            CS_ASSERT(m_completionDelegate, "Http request cannot have null delegate");
            
            @autoreleasepool
            {
                NSString* urlString = [NSStringUtils newNSStringWithUTF8String:in_requestDesc.m_url];
                NSURL* url = [NSURL URLWithString:urlString];
                [urlString release];
                
                NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:in_timeoutSecs];
                
                //apply header
                NSMutableDictionary* header = [NSMutableDictionary dictionaryWithCapacity:in_requestDesc.m_headers.size()];
                for (const auto& fieldPair : in_requestDesc.m_headers)
                {
                    NSString* key = [NSStringUtils newNSStringWithUTF8String:fieldPair.first];
                    NSString* value = [NSStringUtils newNSStringWithUTF8String:fieldPair.second];
                    [header setObject:value forKey:key];
                    [value release];
                    [key release];
                }
                [request setAllHTTPHeaderFields:header];
                
                //apply body if a post request.
                if (in_requestDesc.m_type == Type::k_post)
                {
                    request.HTTPMethod = @"POST";
                    request.HTTPBody =[NSData dataWithBytes:in_requestDesc.m_body.c_str() length:in_requestDesc.m_body.length()];
                }
                
                m_httpDelegate = [[HttpDelegate alloc] initWithRequest:this];
                m_connection = [[NSURLConnection connectionWithRequest:[request copy] delegate: m_httpDelegate] retain];
            }
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        const HttpRequest::Desc& HttpRequest::GetDescription() const
        {
            return m_desc;
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        const std::string& HttpRequest::GetResponse() const
        {
            CS_ASSERT(m_complete == true, "Cannot get response from incomplete request.");
            
            return m_responseData;
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        u32 HttpRequest::GetResponseCode() const
        {
            CS_ASSERT(m_complete == true, "Cannot get response code from incomplete request.");
            
            return m_responseCode;
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
		void HttpRequest::Cancel()
		{
            CS_ASSERT(m_complete == false, "Cannot cancel an already complete request.");
            
            m_complete = true;
            m_responseCode = 0;
            m_responseData = "";
            
            [m_connection cancel];
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        void HttpRequest::OnComplete(CSNetworking::HttpRequest::Result in_result, u32 in_responseCode, const std::string& in_data)
        {
            CS_ASSERT(m_complete == false, "Cannot complete an already completed request.");
            
            m_complete = true;
            m_responseCode = in_responseCode;
            m_responseData = in_data;
            
            m_completionDelegate(this, in_result);
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        HttpRequest::~HttpRequest()
        {
            [m_connection release];
            [m_httpDelegate release];
            m_connection = nil;
            m_httpDelegate = nil;
        }
	}
}

#endif