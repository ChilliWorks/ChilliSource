/*
 *  CurlHttpConnectionSystem.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 23/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Platform/iOS/HttpConnectionSystem.h>
#include <moFlo/Platform/iOS/MoFloReachability.h>

#include <moFlo/Core/TaskScheduler.h>
#include <moFlo/Core/Thread.h>
#include <moFlo/Core/Math/MathUtils.h>
#include <moFlo/Core/Application.h>
#include <moFlo/Core/StringUtils.h>
#include <moFlo/Core/HashCRC32.h>

#include <sstream>


#ifdef DEBUG
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

namespace moFlo
{
	using namespace Networking;
	
	namespace iOSPlatform
	{
		const u32 kudwBufferSize = 1024 * 50;
        const u32 kudwKeepAliveTimeInSeconds = 120;
        const u32 kudwReadThreadSleepInMS = 100;
        
        u32 CHttpConnectionSystem::udwStaticNumConnectionsEstablished = 0;
        
		//--------------------------------------------------------------------------------------------------
		/// Is A
		///
		/// @param Interace ID
		/// @return Whether object if of argument type
		//--------------------------------------------------------------------------------------------------
		bool CHttpConnectionSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IHttpConnectionSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
		}
		//--------------------------------------------------------------------------------------------------
		/// Make Request
		///
		/// Causes the system to issue a request with the given details.
		/// @param A HttpRequestDetails struct with valid params per the documentation of HttpRequestDetails
		/// @param (Optional) A function to call when the request is completed. Note that the request can be completed by failure/cancellation as well as success.
		/// @return A pointer to the request. The system owns this pointer. Returns NULL if the request cannot be created.
		//--------------------------------------------------------------------------------------------------
		HttpRequestPtr CHttpConnectionSystem::MakeRequest(const HttpRequestDetails & insRequestDetails, IHttpRequest::CompletionDelegate inOnComplete)
        {
            //NOTE: The CFNetwork framework handles persistent connections under the hood but it must be
            //coaxed into multiple persistent connections across domains. CFNetwork will reuse connections to the same domain
            //that are already open. This means we must keep old connections around until new ones are established
            //before we can release them.
            
            //Create the URL object
            CFStringRef pstrURL = CFStringCreateWithCString(kCFAllocatorDefault, insRequestDetails.strURL.c_str(), kCFStringEncodingASCII);
			CFURLRef pURL = CFURLCreateWithString(kCFAllocatorDefault, pstrURL, NULL);
            CFRelease(pstrURL);
            
            //Create the request
            CFHTTPMessageRef pRequest = NULL;
            
			switch(insRequestDetails.eType)
			{
				case HttpRequestDetails::POST:
                {
                    //Set message body if we're posting
					pRequest = CFHTTPMessageCreateRequest(kCFAllocatorDefault, CFSTR("POST"), pURL, kCFHTTPVersion1_1);
                    CFDataRef pBodyRef = CFDataCreate(kCFAllocatorDefault, (const UInt8*)insRequestDetails.strBody.c_str(), insRequestDetails.strBody.length());
                    CFHTTPMessageSetBody(pRequest, pBodyRef);
                    CFRelease(pBodyRef);
					break;
                }
				case HttpRequestDetails::GET:
                {
                    pRequest = CFHTTPMessageCreateRequest(kCFAllocatorDefault, CFSTR("GET"), pURL, kCFHTTPVersion1_1);
                    break;
                }
			}
			
			// Set request headers
			if(!insRequestDetails.sHeaders.empty())
			{
				// Set headers
				for(Core::StringToStringMap::const_iterator it = insRequestDetails.sHeaders.begin(); it != insRequestDetails.sHeaders.end(); ++it)
				{
					std::string strKey = it->first;
					std::string strValue = it->second;
					CFStringRef cfsKey = CFStringCreateWithCString(NULL, strKey.c_str(), kCFStringEncodingASCII);
					CFStringRef cfsValue = CFStringCreateWithCString(NULL, strValue.c_str(), kCFStringEncodingASCII);
					CFHTTPMessageSetHeaderFieldValue(pRequest, cfsKey, cfsValue);
					CFRelease(cfsKey);
					CFRelease(cfsValue);
				}
			}
            
            //Grab the connection information that allows us to identify the domain and the scheme
            s32 dwPortNum = CFURLGetPortNumber(pURL);
            CFStringRef pstrHostName = CFURLCopyHostName(pURL);
            CFStringRef pstrScheme = CFURLCopyScheme(pURL);
            std::string strHostName = Core::CStringUtils::NSStringToString((NSString*)pstrHostName);
            std::string strScheme = Core::CStringUtils::NSStringToString((NSString*)pstrScheme);
            CFRelease(pstrHostName);
            CFRelease(pstrScheme);
            
            //Create the read stream...
            CFReadStreamRef ReadStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, pRequest);
            
            //...use persistent connections..
            CFReadStreamSetProperty(ReadStream, kCFStreamPropertyHTTPAttemptPersistentConnection, kCFBooleanTrue);
            
			//...enable SSL if the URL is https
            if(strScheme == "https")
            {
                //Hey an https request
                ApplySSLSettings(ReadStream);
            }
            
            //Tell CFNetwork to re-use the connection with this ID or create one with this ID if none exist.
            //This is actually a bit of a hack to force CFNetwork to open a new connection
            //We use the scheme, host and port to identify connections by generating a hash that uniquely identifes
            //a connection. Then because CFNumber does not allow unsigned ints we must create an incremental counter
            //to act as a UDID!
            ConnectionID PropID = CHashCRC32::GenerateHashCode(strScheme + strHostName + STRING_CAST(dwPortNum));
            ConnectionID StreamID = 0;
            bool bConnectionAlreadyExists = false;
            
            //Check the connection pool for any open connections to this domain using the unique connection ID
            for(ConnectionPool::iterator itExisting = mPersistentConnectionPool.begin(); itExisting != mPersistentConnectionPool.end(); ++itExisting)
            {
                if(itExisting->PropertyID == PropID)
                {
                    //We have found an unused existing connection to re-use. We have to cheat the CFNetwork framework by opening
                    //a "new" stream (which we have done above) and closing the "old" one then remove it from the cache
                    //Streams are only added to the cache once they have been completed but not yet closed
                    StreamID = itExisting->StreamID;
                    CFReadStreamClose(itExisting->ReadStream);
                    CFRelease(itExisting->ReadStream);
                    mPersistentConnectionPool.erase(itExisting);
                    bConnectionAlreadyExists = true;
                    break;
                }
            }
            
            if(!bConnectionAlreadyExists)
            {
                udwStaticNumConnectionsEstablished++;
                StreamID = udwStaticNumConnectionsEstablished;
            }
            
            CFNumberRef pStreamID = CFNumberCreate(kCFAllocatorDefault,  kCFNumberSInt32Type,  &StreamID);
            CFReadStreamSetProperty(ReadStream, CFSTR("PersistentConnectionID"), pStreamID);
            
			//Release request and its ilk.
            CFRelease(pStreamID);
			CFRelease(pRequest);
			CFRelease(pURL);
            
            //We always need to open a connection but we can rely on CFNetwork to re-use open sockets
            //We must make sure we release the "old" connection on re-use
            if(CFReadStreamOpen(ReadStream))
            {
                //Cache the connection info so we can identify streams
                ConnectionInfo sInfo;
                sInfo.PropertyID = PropID;
                sInfo.StreamID = StreamID;
                sInfo.ReadStream = ReadStream;
                sInfo.ConnectionOpenTime = Core::CApplication::GetSystemTime();
                
#ifdef DEBUG
                //LogConnectionAddress(ReadStream);
#endif
                
                //Begin the request with the read stream
                CHttpRequest* pHttpRequest = new CHttpRequest(insRequestDetails, sInfo, inOnComplete);
                mapRequests.push_back(pHttpRequest);
                return pHttpRequest;
            }
            else
            {
                //The stream could not be opened
                CFRelease(ReadStream);
                return NULL;
            }
		}
        //--------------------------------------------------------------------------------------------------
        /// Log Connection Address
        ///
        /// Log the unique TCP connection established by the read stream. This is for debug purposes only
        ///
        /// @param Read stream
        //--------------------------------------------------------------------------------------------------
        void CHttpConnectionSystem::LogConnectionAddress(CFReadStreamRef inReadStreamRef) const
        {
#ifdef DEBUG
            CFDataRef       sockObj;
            s32             sock;
            BOOL            success;
            struct sockaddr_storage localAddr;
            struct sockaddr_storage remoteAddr;
            socklen_t       localAddrLen;
            socklen_t       remoteAddrLen;
            s8            localAddrStr[NI_MAXHOST];
            s8            localPortStr[NI_MAXSERV];
            s8            remoteAddrStr[NI_MAXHOST];
            s8            remotePortStr[NI_MAXSERV];
            
            sockObj = (CFDataRef) CFReadStreamCopyProperty(inReadStreamRef, kCFStreamPropertySocketNativeHandle);
            if (sockObj != NULL)
            {
                assert(CFGetTypeID(sockObj) == CFDataGetTypeID());
                assert(CFDataGetLength(sockObj) == sizeof(s32));
                
                sock = * (const s32 *) CFDataGetBytePtr(sockObj);
                assert(sock >= 0);
                
                localAddrLen = sizeof(localAddr);
                success = getsockname(sock, (struct sockaddr *) &localAddr, &localAddrLen) >= 0;
                assert(success);
                
                remoteAddrLen = sizeof(remoteAddr);
                success = getpeername(sock, (struct sockaddr *) &remoteAddr, &remoteAddrLen) >= 0;
                assert(success);
                
                success = getnameinfo((const struct sockaddr *) &localAddr,  localAddrLen,  localAddrStr,  sizeof(localAddrStr),  localPortStr,  sizeof(localPortStr),  NI_NUMERICHOST | NI_NUMERICSERV) == 0;
                assert(success);
                
                success = getnameinfo((const struct sockaddr *) &remoteAddr, remoteAddrLen, remoteAddrStr, sizeof(remoteAddrStr), remotePortStr, sizeof(remotePortStr), NI_NUMERICHOST | NI_NUMERICSERV) == 0;
                assert(success);
                
                CFRelease(sockObj);
                
                NSLog(@"Address %s:%s -> %s:%s", localAddrStr, localPortStr, remoteAddrStr, remotePortStr);
            }
#endif
        }
        //--------------------------------------------------------------------------------------------------
        /// Apply SSL Settings
        ///
        /// Setup the read stream to use SSL level 3
        ///
        /// @param Read stream
        //--------------------------------------------------------------------------------------------------
        void CHttpConnectionSystem::ApplySSLSettings(CFReadStreamRef inReadStreamRef) const
        {
            CFMutableDictionaryRef pDict = CFDictionaryCreateMutable(kCFAllocatorDefault, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
            CFDictionarySetValue(pDict, kCFStreamSSLValidatesCertificateChain, kCFBooleanFalse);
            CFDictionarySetValue(pDict, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelSSLv3);
            CFReadStreamSetProperty(inReadStreamRef, kCFStreamPropertySSLSettings, pDict);
            CFRelease(pDict);
        }
        //--------------------------------------------------------------------------------------------------
        /// Is Stream Open
        ///
        /// Is the read stream still open or has it errored or closed
        ///
        /// @param Read stream
        /// @return Whether it is open
        //--------------------------------------------------------------------------------------------------
        bool CHttpConnectionSystem::IsStreamOpen(CFReadStreamRef inReadStreamRef) const
        {
            CFStreamStatus CFStatus = CFReadStreamGetStatus(inReadStreamRef);
            return (CFStatus != kCFStreamStatusError &&
                    CFStatus != kCFStreamStatusNotOpen &&
                    CFStatus != kCFStreamStatusClosed);
        }
		//--------------------------------------------------------------------------------------------------
		/// Cancel All Request
		///
		/// Equivalent to calling the above on every incomplete request in progress.
		//--------------------------------------------------------------------------------------------------
		void CHttpConnectionSystem::CancelAllRequests()
        {
			for(u32 nRequest = 0; nRequest < mapRequests.size(); nRequest++)
            {
				mapRequests[nRequest]->Cancel();
			}
		}
        //--------------------------------------------------------------------------------------------------
        /// Check Reachability
        ///
        /// Checks if the device is internet ready and pings google for a response.
        ///
        /// @return Success if URL is reachable
        //--------------------------------------------------------------------------------------------------
        bool CHttpConnectionSystem::CheckReachability() const
        {
            MoFloReachability* pReachability = [MoFloReachability reachabilityForInternetConnection];
            NetworkStatus NetStatus = [pReachability currentReachabilityStatus];
            
            return (NetStatus != NotReachable);
        }
		//--------------------------------------------------------------------------------------------------
		/// Update
		///
		/// For all active requests in the system call update on them allowing them to check the status
		/// of thier requests
		///
		/// @param Time between frames
		//--------------------------------------------------------------------------------------------------
		void CHttpConnectionSystem::Update(f32 infDT)
		{
            RequestVector RequestCopy = mapRequests;
            
            //We should do this in two loops incase anyone tries to insert from the callback hence the copy above
			for(RequestVector::iterator it = RequestCopy.begin(); it != RequestCopy.end(); ++it)
            {
                (*it)->Update(infDT);
            }
            
            for(RequestVector::iterator it = mapRequests.begin(); it != mapRequests.end(); )
            {
                if((*it)->HasCompleted())
                {
                    const ConnectionInfo& sInfoRef = (*it)->GetConnectionInfo();
                    
                    //Add the stream to the recycle pool if there isn't one in there
                    //already for this connection
                    bool bConnectionAlreadyExists = false;
                    
                    for(ConnectionPool::iterator itExisting = mPersistentConnectionPool.begin(); itExisting != mPersistentConnectionPool.end(); ++itExisting)
                    {
                        if(itExisting->PropertyID == sInfoRef.PropertyID)
                        {
                            //This connection already exists in the recycle pool
                            //so we don't need to cache it and we can release it
                            CFReadStreamClose(sInfoRef.ReadStream);
                            CFRelease(sInfoRef.ReadStream);
                            bConnectionAlreadyExists = true;
                            break;
                        }
                    }
                    
                    if(!bConnectionAlreadyExists)
                    {
                        //Connection is recycled
                        mPersistentConnectionPool.push_back(sInfoRef);
                    }
                    
                    //...and remove the completed request
                    SAFE_DELETE(*it);
                    it = mapRequests.erase(it);
                }
                else
                {
                    ++it;
                }
			}
            
            //Check the connection pool for any open connections that are no longer required because they have
            //exceeded their lifespan
            for(ConnectionPool::iterator it = mPersistentConnectionPool.begin(); it != mPersistentConnectionPool.end(); /*No increment*/)
            {
                if(Core::CApplication::GetSystemTime() > (it->ConnectionOpenTime + kudwKeepAliveTimeInSeconds))
                {
                    //Close the old stream
                    CFReadStreamClose(it->ReadStream);
                    CFRelease(it->ReadStream);
                    it = mPersistentConnectionPool.erase(it);
                }
                else
                {
                    ++it;
                }
            }
		}
		//=====================================================================================================
		/// Http Request
		///
		/// Constructor
		///
		/// @param Request details
		/// @param Completion delegate
		//=====================================================================================================
		CHttpConnectionSystem::CHttpRequest::CHttpRequest(const Networking::HttpRequestDetails & insDetails, const CHttpConnectionSystem::ConnectionInfo& insConnectionInfo, const Networking::IHttpRequest::CompletionDelegate & inCompletionDelegate)
		: msDetails(insDetails), mbCompleted(false), mCompletionDelegate(inCompletionDelegate), mfActiveTime(0.0f), mbReceivedResponse(false), mbThreadCompleted(false),
        mudwResponseCode(0), mudwBytesRead(0), mbRequestCompleted(false), mConnectionInfo(insConnectionInfo), mudwBytesReadThisBlock(0)
		{
			//Begin the read loop
			//Run this as a threaded task
			CTaskScheduler::ScheduleTask(Task1<CFReadStreamRef>(this, &CHttpConnectionSystem::CHttpRequest::PollReadStream, mConnectionInfo.ReadStream));
		}
		//------------------------------------------------------------------
		/// Update
		///
		/// Checks the stream to see if any data is available for reading
		/// and reads this into a buffer. Once all the data is read
		/// the request will call the complete delegate
		///
		/// @param Time since last frame
		//------------------------------------------------------------------
		void CHttpConnectionSystem::CHttpRequest::Update(f32 infDT)
		{
			//Check if the data has finished streaming and invoke the completion delegate on the main thread
			if(mbCompleted)
			{
                mfActiveTime = 0.0f;
                mbRequestCompleted = true;
                
                if(mCompletionDelegate)
                {
                    if(meRequestResult != IHttpRequest::CANCELLED)
                    {
                        mCompletionDelegate(this, meRequestResult);
                    }
                    
                    mCompletionDelegate = NULL;
                }
			}
            //Track the time the request has been active so we can manually timeout
            else if(!mbCompleted && !mbReceivedResponse && ((mfActiveTime += (Core::CMathUtils::Min(infDT, 0.5f))) > kfDefaultHTTPTimeout))
            {
                DEBUG_LOG("HTTP Connection timed out on request: " + msDetails.strURL);
				//Flag to stop the polling thread which should
				//exit gracefully
				mfActiveTime = 0.0f;
				mbCompleted = true;
                mbReceivedResponse = true;
                mbRequestCompleted = true;
                
                if(mCompletionDelegate)
                {
                    mCompletionDelegate(this, IHttpRequest::TIMEOUT);
                    mCompletionDelegate = NULL;
                }
            }
		}
		//------------------------------------------------------------------
		/// Poll Read Stream
		///
		/// Reads data from the open stream when it becomes available
		/// buffers the data flags on complete
		//------------------------------------------------------------------
		void CHttpConnectionSystem::CHttpRequest::PollReadStream(CFReadStreamRef inReadStreamRef)
		{
            //Poll the stream for data
            CFStreamStatus CFStatus = CFReadStreamGetStatus(inReadStreamRef);
            
			switch(CFStatus)
			{
				case kCFStreamStatusOpen:
				{
                    std::stringstream strBuffer;
                    
					while(!mbReceivedResponse)
					{
						//Check of data is available and begin to stream the bytes
						if(CFReadStreamHasBytesAvailable(inReadStreamRef))
						{
							CFHTTPMessageRef CFResponse = (CFHTTPMessageRef)CFReadStreamCopyProperty(inReadStreamRef, kCFStreamPropertyHTTPResponseHeader);
							if(CFResponse)
							{
								//We have a response from the server we can stop the timeout timer
								mbReceivedResponse = true;
								
								//Grab the http response (i.e. 400 etc)
								u32 udwResponseCode = CFHTTPMessageGetResponseStatusCode(CFResponse);
								
								//Buffer into which the data is read each time data is available from the read stream
								u8 abyDataBuffer[kudwBufferSize];
                                
                                if(udwResponseCode == Networking::kHTTPMovedTemporarily || udwResponseCode == Networking::kHTTPRedirectTemporarily)
                                {
                                    CFStringRef CFUrl = CFHTTPMessageCopyHeaderFieldValue(CFResponse, CFSTR("Location"));
                                    
                                    std::string strCFUrl = moCore::CStringUtils::NSStringToString((NSString*)CFUrl);
                                    
                                    msDetails.strRedirectionURL = strCFUrl;
                                    
                                    CFRelease(CFUrl);
                                }
								
								while(!mbCompleted)
								{
									s32 dwBytesRead = CFReadStreamRead(inReadStreamRef, abyDataBuffer, kudwBufferSize);
									if(dwBytesRead > 0)
									{
										//We only want to pipe the actual data to the buffer and not
										//any trailing garbage that may be resident
										strBuffer.write((s8*)abyDataBuffer, dwBytesRead);
                                        mudwBytesRead += dwBytesRead;
                                        mudwBytesReadThisBlock += dwBytesRead;
                                        
                                        //If the number of bytes read exceeds the buffer size then we have to flush
                                        if(mudwMaxBufferSize != 0 && mudwBytesReadThisBlock >= mudwMaxBufferSize)
                                        {
                                            mResponseData = strBuffer.str();
                                            strBuffer.clear();
                                            strBuffer.str("");
                                            mudwBytesReadThisBlock = 0;
                                            mCompletionDelegate(this, IHttpRequest::FLUSHED);
                                        }
									}
									else if(dwBytesRead < 0)
									{
										//error condition, log and diagnose
                                        meRequestResult = IHttpRequest::FAILED;
										mbCompleted = true;
									}
									else if(dwBytesRead == 0)
									{
										//Great success. The data is finished
										//The stream is empty we should now have all the data
										//Copy the buffer into the response
										mResponseData = strBuffer.str();
										mudwResponseCode = udwResponseCode;
                                        meRequestResult = IHttpRequest::COMPLETED;
										mbCompleted = true;
									}
								}
								
								CFRelease(CFResponse);
							}
						}
                        
                        if(!mbReceivedResponse)
                        {
                            CThread::Sleep(kudwReadThreadSleepInMS);
                        }
					}
					break;
				}
				case kCFStreamStatusError:
				case kCFStreamStatusNotOpen:
                default:
                {
                    meRequestResult = IHttpRequest::FAILED;
                    mbCompleted = true;
					break;
                }
			}
            
            mbThreadCompleted = true;
		}
		//----------------------------------------------------------------------------------------
		/// Cancel
		///
		/// Close the request and invoke the completion delegate with the cancel response
		//----------------------------------------------------------------------------------------
		void CHttpConnectionSystem::CHttpRequest::Cancel()
		{
            mbCompleted = true;
            mbReceivedResponse = true;
            meRequestResult = IHttpRequest::CANCELLED;
		}
		//----------------------------------------------------------------------------------------
		/// Has Completed
		///
		/// @return Whether the request has completed - regardless of success or failure
		//----------------------------------------------------------------------------------------
		bool CHttpConnectionSystem::CHttpRequest::HasCompleted() const
		{
			return mbRequestCompleted && mbThreadCompleted;
		}
		//----------------------------------------------------------------------------------------
		/// Get Details
		///
		/// @return The original request details (i.e. whether it is post/get the body and header)
		//----------------------------------------------------------------------------------------
		const Networking::HttpRequestDetails & CHttpConnectionSystem::CHttpRequest::GetDetails() const
		{
			return msDetails;
		}
		//----------------------------------------------------------------------------------------
		/// Get Completion Delegate
		///
		/// @return The delegate that will be invoked on request complete
		//----------------------------------------------------------------------------------------
		const Networking::IHttpRequest::CompletionDelegate & CHttpConnectionSystem::CHttpRequest::GetCompletionDelegate() const
		{
			return mCompletionDelegate;
		}
		//----------------------------------------------------------------------------------------
		/// Get Response String
		///
		/// @return The contents of the response as a string. This could be binary data
		//----------------------------------------------------------------------------------------
		const std::string & CHttpConnectionSystem::CHttpRequest::GetResponseString() const
		{
			return mResponseData;
		}
		//----------------------------------------------------------------------------------------
		/// Get Response Code
		///
		/// @return HTTP response code (i.e. 200 = OK, 400 = Error)
		//----------------------------------------------------------------------------------------
		u32 CHttpConnectionSystem::CHttpRequest::GetResponseCode() const
		{
			return mudwResponseCode;
		}
        //----------------------------------------------------------------------------------------
        /// Get Bytes Read
        ///
        /// @return Number of bytes read til now
        //----------------------------------------------------------------------------------------
        u32 CHttpConnectionSystem::CHttpRequest::GetBytesRead() const
        {
            return mudwBytesRead;
        }
        //----------------------------------------------------------------------------------------
        /// Get Connection Info
        ///
        /// @return Connection Info
        //----------------------------------------------------------------------------------------
        const CHttpConnectionSystem::ConnectionInfo& CHttpConnectionSystem::CHttpRequest::GetConnectionInfo()
        {
            return mConnectionInfo;
        }
	}
}