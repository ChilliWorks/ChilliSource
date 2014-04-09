//
//  HttpRequest.cpp
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Networking/Http/HttpRequest.h>

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

#include <CFNetwork/CFNetwork.h>

#include <sstream>
#include <thread>

namespace ChilliSource
{
	namespace iOS
	{
        namespace
        {
            const u32 k_readBufferSize = 1024 * 50;
            const u32 k_readThreadSleepInMS = 100;
        }
        
        //------------------------------------------------------------------
        //------------------------------------------------------------------
		HttpRequest::HttpRequest(const Desc& in_requestDesc, u32 in_timeoutSecs, u32 in_bufferFlushSize, const Delegate& in_delegate)
		: m_desc(in_requestDesc), m_timeoutSecs(in_timeoutSecs), m_bufferFlushSize(in_bufferFlushSize), m_completionDelegate(in_delegate), m_isPollingCompleted(false),
        m_isRequestCompleted(false), m_shouldKillThread(false), m_isConnectionEstablished(false),
        m_connectingTime(0.0f), m_totalBytesRead(0), m_totalBytesReadThisBlock(0), m_responseCode(0), m_result(Result::k_failed)
		{
            CS_ASSERT(m_completionDelegate, "Http request cannot have null delegate");
		}
        //------------------------------------------------------------------
		//------------------------------------------------------------------
        void HttpRequest::Start(CFReadStreamRef in_readStream)
        {
            //Begin the read loop as a threaded task
			Core::TaskScheduler::ScheduleTask(Core::Task<CFReadStreamRef>(this, &HttpRequest::PollReadStream, in_readStream));
        }
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void HttpRequest::Update(f32 in_timeSinceLastUpdate)
		{
			//Check if the data has finished streaming and invoke the completion delegate on the main thread
			if(m_isPollingCompleted == true)
			{
                m_isRequestCompleted = true;
                
                if(m_result != Result::k_cancelled)
                {
                    m_completionDelegate(this, m_result);
                }
                
                m_completionDelegate = nullptr;
			}
            //Track the time the request has been active so we can manually timeout. Make sure we limit the time diff
            if(m_isConnectionEstablished == false)
            {
                m_connectingTime += std::min(in_timeSinceLastUpdate, 0.5f);
                
                if(m_connectingTime > m_timeoutSecs)
                {
                    //Attempting to connect has timed out
                    m_connectingTime = 0.0f;
                    m_isRequestCompleted = true;
                    m_shouldKillThread = true;
                    m_isConnectionEstablished = true;
                    
                    m_completionDelegate(this, Result::k_timeout);
                    m_completionDelegate = nullptr;
                }
            }
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void HttpRequest::PollReadStream(CFReadStreamRef in_readStream)
		{
            //Poll the stream for data
            CFStreamStatus status = CFReadStreamGetStatus(in_readStream);
            
			switch(status)
			{
				case kCFStreamStatusOpen:
				{
                    if(m_shouldKillThread == false)
                    {
                        BlockingPollForConnection(in_readStream);
                    }
                    
                    bool readSuccess = false;
                    std::stringstream streamBuffer;
                    if(m_shouldKillThread == false)
                    {
                        readSuccess = BlockingReadData(in_readStream, streamBuffer);
                    }
                    
                    if(m_shouldKillThread == false)
                    {
                        if(readSuccess == true)
                        {
                            //Great success. The data is finished
                            //The stream is empty we should now have all the data
                            //Copy the buffer into the response
                            m_responseData = streamBuffer.str();
                            m_result = Result::k_completed;
                        }
                        else
                        {
                            //Error
                            m_result = Result::k_failed;
                        }
                    }
                    
					break;
				}
				case kCFStreamStatusError:
				case kCFStreamStatusNotOpen:
                default:
                {
                    m_result = Result::k_failed;
					break;
                }
			}
            
            m_isPollingCompleted = true;
		}
        //----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
        void HttpRequest::BlockingPollForConnection(CFReadStreamRef in_readStream)
        {
            while(m_isConnectionEstablished == false && m_shouldKillThread == false)
            {
                if(CFReadStreamHasBytesAvailable(in_readStream))
                {
                    CFHTTPMessageRef response = (CFHTTPMessageRef)CFReadStreamCopyProperty(in_readStream, kCFStreamPropertyHTTPResponseHeader);
                    if(response != nullptr)
                    {
                        //We have a response from the server we can stop the timeout timer
                        m_isConnectionEstablished = true;
                        
                        //Grab the http response (i.e. 400 etc)
                        m_responseCode = CFHTTPMessageGetResponseStatusCode(response);
                        
                        //Check if we have a redirect URL
                        CFStringRef redirectUrlString = CFHTTPMessageCopyHeaderFieldValue(response, CFSTR("Location"));
                        
                        if(redirectUrlString != nullptr)
                        {
                            m_desc.m_redirectionUrl = [NSStringUtils newStringWithNSString:(NSString*)redirectUrlString];
                            CFRelease(redirectUrlString);
                        }
                        
                        CFRelease(response);
                    }
                }
                
                //Sleep the thread if we are still waiting to connect
                if(m_isConnectionEstablished == false)
                {
                    std::chrono::milliseconds sleepDuration(k_readThreadSleepInMS);
                    std::this_thread::sleep_for(sleepDuration);
                }
            }
        }
        //----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
        bool HttpRequest::BlockingReadData(CFReadStreamRef in_readStream, std::stringstream& out_streamBuffer)
        {
            s32 bytesRead = 0;
            u8 readBuffer[k_readBufferSize];
            
            do
            {
                bytesRead = CFReadStreamRead(in_readStream, readBuffer, k_readBufferSize);
                if(bytesRead > 0)
                {
                    //We only want to pipe the actual data to the buffer and not
                    //any trailing garbage that may be resident
                    out_streamBuffer.write((s8*)readBuffer, bytesRead);
                    m_totalBytesRead += bytesRead;
                    m_totalBytesReadThisBlock += bytesRead;
                    
                    //If the number of bytes read exceeds the buffer size then we have to flush
                    if(m_bufferFlushSize != 0 && m_totalBytesReadThisBlock >= m_bufferFlushSize)
                    {
                        m_responseData = out_streamBuffer.str();
                        out_streamBuffer.clear();
                        out_streamBuffer.str("");
                        m_totalBytesReadThisBlock = 0;
                        m_completionDelegate(this, Result::k_flushed);
                    }
                }
            } while(bytesRead > 0 && m_shouldKillThread == false);
            
            return bytesRead == 0;
        }
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		void HttpRequest::Cancel()
		{
            m_shouldKillThread = true;
            m_result = Result::k_cancelled;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		bool HttpRequest::HasCompleted() const
		{
			return m_isRequestCompleted;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		const HttpRequest::Desc& HttpRequest::GetDescription() const
		{
			return m_desc;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		const std::string& HttpRequest::GetResponse() const
		{
			return m_responseData;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		u32 HttpRequest::GetResponseCode() const
		{
			return m_responseCode;
		}
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        u32 HttpRequest::GetBytesRead() const
        {
            return m_totalBytesRead;
        }
	}
}