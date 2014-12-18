//
//  HttpDelegate.mm
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

#import <CSBackend/Platform/iOS/Networking/Http/HttpDelegate.h>

#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#import <CSBackend/Platform/iOS/Networking/Http/HttpRequest.h>

@implementation HttpDelegate

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
- (id) initWithRequest:(CSBackend::iOS::HttpRequest*)in_request andMaxBufferSize:(u32) in_bufferSize
{
    if (self = [super init])
    {
        m_data = nil;
        m_request = in_request;
        m_responseCode = 0;
        m_maxBufferSize = in_bufferSize;
        CS_ASSERT(m_request != nullptr, "Request must be supplied to Http Delegate.");
        return self;
    }
    
    return nil;
}
//-----------------------------------------------------------------------------
/// Called when a connection is established.
///
/// @author Ian Copland
///
/// @param The connection.
/// @param The response.
//-----------------------------------------------------------------------------
- (void) connection:(NSURLConnection*)in_connection didReceiveResponse:(NSURLResponse *)in_response
{
    //if we get a redirect we may want to clear the data.
    if (m_data != nil)
    {
        [m_data release];
        m_data = nil;
    }
    
    m_data = [[NSMutableData alloc] init];
    
    m_responseCode = ((NSHTTPURLResponse*)in_response).statusCode;
}
//-----------------------------------------------------------------------------
/// Called when the next block of data is received. This should be appended to
/// the retained data.
///
/// @author Ian Copland
///
/// @param The connection.
/// @param The data.
//-----------------------------------------------------------------------------
- (void) connection:(NSURLConnection*)in_connection didReceiveData:(NSData *)in_data
{
    CS_ASSERT(m_data != nil, "Cannot receive data before connection is estabilished!");
    
    if(m_maxBufferSize > 0)
    {
        NSUInteger currentSize = [m_data length];
        NSUInteger appendSize = [in_data length];
        if(currentSize + appendSize >= m_maxBufferSize)
        {
            std::string data(reinterpret_cast<const s8*>([m_data bytes]), (s32)[m_data length]);
            m_request->OnFlushed(CSNetworking::HttpResponse::Result::k_flushed, m_responseCode, data);
            [m_data setLength:0];
        }
        
    }
    
    [m_data appendData:in_data];
}
//-----------------------------------------------------------------------------
/// Called when the connection has finished sending data.
///
/// @author Ian Copland
///
/// @param The connection.
//-----------------------------------------------------------------------------
- (void) connectionDidFinishLoading:(NSURLConnection*)in_connection
{
    CS_ASSERT(m_data != nil, "Cannot finish before connection is estabilished!");
    
    std::string data(reinterpret_cast<const s8*>([m_data bytes]), (s32)[m_data length]);
    [m_data release];
    m_data = nil;
    
    m_request->OnComplete(CSNetworking::HttpResponse::Result::k_completed, m_responseCode, data);
}
//-----------------------------------------------------------------------------
/// Called if a connection fails.
///
/// @author Ian Copland
///
/// @param The connection.
/// @param The error.
//-----------------------------------------------------------------------------
- (void) connection:(NSURLConnection*)in_connection didFailWithError:(NSError*)in_error
{
    std::string errorMessage = [NSStringUtils newUTF8StringWithNSString:[in_error localizedDescription]];
    CS_LOG_VERBOSE("HTTP Request error: " + errorMessage);
    
    m_request->OnComplete(CSNetworking::HttpResponse::Result::k_failed, m_responseCode, "");
}
//-----------------------------------------------------------------------------
/// Called to check how a response should be cached. We don't want to cache
/// so we return nil.
///
/// @author Ian Copland
///
/// @param The connection.
/// @param The cached response.
//-----------------------------------------------------------------------------
- (NSCachedURLResponse*) connection:(NSURLConnection*)in_connection willCacheResponse:(NSCachedURLResponse*)in_cachedResponse
{
    return nil;
}
//-----------------------------------------------------------------------------
/// Called when authenticating a HTTPS connection. This will always allow even
/// untrusted connection, so be wary. This will need to be improved in the
/// future.
///
/// @author Ian Copland
///
/// @param The connection.
/// @param The challenge.
//-----------------------------------------------------------------------------
-(void)connection:(NSURLConnection *)in_connection willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge *)in_challenge
{
    //TODO: Make this safe!
    [in_challenge.sender useCredential:[NSURLCredential credentialForTrust:in_challenge.protectionSpace.serverTrust] forAuthenticationChallenge:in_challenge];
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
-(void) dealloc
{
    m_request = nullptr;
    
    if (m_data != nil)
    {
        [m_data release];
    }
    
    [super dealloc];
}

@end

#endif