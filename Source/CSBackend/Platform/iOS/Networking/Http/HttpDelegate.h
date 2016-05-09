//
//  HttpDelegate.h
//  Chilli Source
//  Created by Ian Copland on 11/12/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/Networking/Http/HttpRequestSystem.h>

#import <Foundation/Foundation.h>

#include <functional>

//--------------------------------------------------------------------------------------------------
/// Called when the connection is established.
///
/// @author HMcLaughlin
///
/// @param Expected size of the request
//--------------------------------------------------------------------------------------------------
typedef std::function<void(u64 in_expectedSize)> ConnectionEstablishedDelegate;
//--------------------------------------------------------------------------------------------------
/// Called when the max buffer size is exceeded
///
/// @author HMcLaughlin
///
/// @param in_result - The result.
/// @param in_responseCode - The response code.
/// @param in_data - The data in string form.
//--------------------------------------------------------------------------------------------------
typedef std::function<void(ChilliSource::HttpResponse::Result in_result, u32 in_responseCode, const std::string& in_data)> FlushedDelegate;
//--------------------------------------------------------------------------------------------------
/// Called once the request has complete
///
/// @author HMcLaughlin
///
/// @param in_result - The result.
/// @param in_responseCode - The response code.
/// @param in_data - The data in string form.
//--------------------------------------------------------------------------------------------------
typedef std::function<void(ChilliSource::HttpResponse::Result in_result, u32 in_responseCode, const std::string& in_data)> CompleteDelegate;


//--------------------------------------------------------------------------------------------------
/// A delegate for callbacks from a NSURLConnection. This passes any events on to
/// the Http Request System.
///
/// @author Ian Copland
//--------------------------------------------------------------------------------------------------
@interface HttpDelegate : NSObject <NSURLConnectionDataDelegate>
{
    u32 m_responseCode;
    u32 m_maxBufferSize;
    
    ConnectionEstablishedDelegate m_connectionEstablishedDelegate;
    FlushedDelegate m_flushedDelegate;
    CompleteDelegate m_completeDelegate;
    
    NSMutableData* m_data;
}

//--------------------------------------------------------------------------------------------------
/// Constructor.
///
/// @author HMcLaughlin
///
/// @param in_connectionEstablishedDelegate - Delegate to call when connection is established.
/// @param in_flushedDelegate - Delegate to call when connection is flushed.
/// @param in_completeDelegate - Delegate to call when connection is completed.
/// @param in_bufferSize - The buffer size of the request in bytes after which we flush
//--------------------------------------------------------------------------------------------------
- (id) initWithConnectionDelegate:(const ConnectionEstablishedDelegate&)in_connectionEstablishedDelegate andFlushedDelegate:(const FlushedDelegate&)in_flushedDelegate andCompleteDelegate:(const FlushedDelegate&)in_completeDelegate andMaxBufferSize:(u32) in_bufferSize;
//--------------------------------------------------------------------------------------------------
/// Cleans up the delegate.
///
/// @author Ian Copland
//--------------------------------------------------------------------------------------------------
-(void) dealloc;

@end

#endif