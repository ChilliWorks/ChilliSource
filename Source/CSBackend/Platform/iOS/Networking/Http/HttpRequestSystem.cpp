//
//  HttpRequestSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#include <CSBackend/Platform/iOS/Networking/Http/HttpRequestSystem.h>

#include <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#include <CSBackend/Platform/iOS/Networking/Http/HttpRequest.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <Reachability/CSReachability.h>

#include <CFNetwork/CFNetwork.h>

namespace CSBackend
{
	namespace iOS
	{
        namespace
        {
            const u32 k_keepAliveTimeSecs = 120;
            
            //--------------------------------------------------------------------------------------------------
            /// Setup the read stream to use SSL level 3
            ///
            /// @author S Downie
            ///
            /// @param [Out] Read stream
            //--------------------------------------------------------------------------------------------------
            void ApplySSLSettings(CFReadStreamRef out_readStream)
            {
                CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
                CFDictionarySetValue(dict, kCFStreamSSLValidatesCertificateChain, kCFBooleanFalse);
                CFDictionarySetValue(dict, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelSSLv3);
                CFReadStreamSetProperty(out_readStream, kCFStreamPropertySSLSettings, dict);
                CFRelease(dict);
            }
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Read stream
            ///
            /// @return Whether the read stream still open or has it errored or closed
            //--------------------------------------------------------------------------------------------------
            bool IsStreamOpen(CFReadStreamRef in_readStream)
            {
                CFStreamStatus status = CFReadStreamGetStatus(in_readStream);
                return (status != kCFStreamStatusError &&
                        status != kCFStreamStatusNotOpen &&
                        status != kCFStreamStatusClosed);
            }
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Request description
            ///
            /// @return New CFNetwork request message initialised based on the description
            //--------------------------------------------------------------------------------------------------
            CFHTTPMessageRef CreateRequestMessage(const CSNetworking::HttpRequest::Desc& in_requestDesc, CFURLRef in_url)
            {
                CFHTTPMessageRef requestMessage = nullptr;
                
                switch(in_requestDesc.m_type)
                {
                    case CSNetworking::HttpRequest::Type::k_post:
                    {
                        //Set message body if we're posting
                        requestMessage = CFHTTPMessageCreateRequest(kCFAllocatorDefault, CFSTR("POST"), in_url, kCFHTTPVersion1_1);
                        CFDataRef body = CFDataCreate(kCFAllocatorDefault, (const UInt8*)in_requestDesc.m_body.c_str(), in_requestDesc.m_body.length());
                        CFHTTPMessageSetBody(requestMessage, body);
                        CFRelease(body);
                        break;
                    }
                    case CSNetworking::HttpRequest::Type::k_get:
                    {
                        requestMessage = CFHTTPMessageCreateRequest(kCFAllocatorDefault, CFSTR("GET"), in_url, kCFHTTPVersion1_1);
                        break;
                    }
                }
                
                return requestMessage;
            }
            //--------------------------------------------------------------------------------------------------
            /// Initliase the request headers
            ///
            /// @author S Downie
            ///
            /// @param Request description
            /// @param [Out] Request message
            //--------------------------------------------------------------------------------------------------
            void SetRequestHeaders(const CSNetworking::HttpRequest::Desc& in_requestDesc, CFHTTPMessageRef out_requestMessage)
            {
                if(!in_requestDesc.m_headers.empty())
                {
                    for(auto it = in_requestDesc.m_headers.begin(); it != in_requestDesc.m_headers.end(); ++it)
                    {
                        CFStringRef key = CFStringCreateWithCString(nullptr, it->first.c_str(), kCFStringEncodingASCII);
                        CFStringRef value = CFStringCreateWithCString(nullptr, it->second.c_str(), kCFStringEncodingASCII);
                        CFHTTPMessageSetHeaderFieldValue(out_requestMessage, key, value);
                        CFRelease(key);
                        CFRelease(value);
                    }
                }
            }
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Scheme (http, https, etc)
            /// @param URL object
            ///
            /// @return Unique ID based on the url properties
            //--------------------------------------------------------------------------------------------------
            HttpRequestSystem::ConnectionId GeneratePropId(const std::string& in_scheme, CFURLRef in_url)
            {
                s32 portNum = CFURLGetPortNumber(in_url);
                CFStringRef cfHostName = CFURLCopyHostName(in_url);
                std::string hostName = [NSStringUtils newUTF8StringWithNSString:(NSString*)cfHostName];
                CFRelease(cfHostName);
                
                HttpRequestSystem::ConnectionId propId = CSCore::HashCRC32::GenerateHashCode(in_scheme + hostName + CSCore::ToString(portNum));
                return propId;
            }
        }
        
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
            m_connectionTimeoutSecs = in_timeoutSecs;
        }
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		CSNetworking::HttpRequest* HttpRequestSystem::MakeRequest(const CSNetworking::HttpRequest::Desc& in_requestDesc, const CSNetworking::HttpRequest::Delegate& in_delegate)
        {
            CS_ASSERT(in_requestDesc.m_url.empty() == false, "Cannot make an http request to a blank url");
            
            //NOTE: The CFNetwork framework handles persistent connections under the hood but it must be
            //coaxed into multiple persistent connections across domains. CFNetwork will reuse connections to the same domain
            //that are already open. This means we must keep old connections around until new ones are established
            //before we can release them.
            
            CFStringRef urlString = CFStringCreateWithCString(kCFAllocatorDefault, in_requestDesc.m_url.c_str(), kCFStringEncodingASCII);
			CFURLRef url = CFURLCreateWithString(kCFAllocatorDefault, urlString, nullptr);
            CFRelease(urlString);
            
            CS_ASSERT(url != nullptr, "Invalid http request url: " + in_requestDesc.m_url);
            
            CFHTTPMessageRef requestMessage = CreateRequestMessage(in_requestDesc, url);
            SetRequestHeaders(in_requestDesc, requestMessage);
            
            //Create the read stream
            CFReadStreamRef readStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, requestMessage);
            //Use persistent connections..
            CFReadStreamSetProperty(readStream, kCFStreamPropertyHTTPAttemptPersistentConnection, kCFBooleanTrue);
            
			//...enable SSL if the URL is https
            //Grab the connection information that allows us to identify the domain and the scheme
            CFStringRef cfScheme = CFURLCopyScheme(url);
            CS_ASSERT(cfScheme != nullptr, "Invalid http request url scheme: " + in_requestDesc.m_url);
            std::string scheme = [NSStringUtils newUTF8StringWithNSString:(NSString*)cfScheme];
            CFRelease(cfScheme);
            if(scheme == "https")
            {
                //Hey an https request
                ApplySSLSettings(readStream);
            }
            
            //We use the scheme, host and port to identify connections by generating a hash that uniquely identifes
            //a connection.
            ConnectionId connectionId = GeneratePropId(scheme, url);
            
            HttpRequest* httpRequest = new HttpRequest(in_requestDesc, m_connectionTimeoutSecs, GetMaxBufferSize(), in_delegate);
            
            bool streamOpened = false;
            
            auto itExisting = m_unusedConnections.find(connectionId);
            if(itExisting != m_unusedConnections.end())
            {
                //This is actually a bit of a hack to force CFNetwork to reuse an open connection.
                //We have found an unused existing connection to re-use. We have to cheat the CFNetwork framework by opening
                //a "new" stream (which we have done above) and closing the "old" one after.
                
                ConnectionInfo& connectionInfo = itExisting->second;
                CFNumberRef cfStreamID = CFNumberCreate(kCFAllocatorDefault,  kCFNumberSInt32Type,  &connectionInfo.m_streamId);
                CFReadStreamSetProperty(readStream, CFSTR("PersistentConnectionID"), cfStreamID);
                CFRelease(cfStreamID);
                
                //Open the new stream
                streamOpened = CFReadStreamOpen(readStream);
                
                //Close the old connection
                CFReadStreamClose(connectionInfo.m_readStream);
                CFRelease(connectionInfo.m_readStream);
                connectionInfo.m_readStream = readStream;
                
                //Add to the active pool and remove from the recycle pool
                m_unusedConnections.erase(itExisting);
                m_activeConnections.insert(std::make_pair(httpRequest, connectionInfo));
            }
            else
            {
                //There is no unused connection with this Id create a new one
                m_totalNumConnectionsEstablished++;
                ConnectionId streamId = m_totalNumConnectionsEstablished;
                CFNumberRef cfStreamID = CFNumberCreate(kCFAllocatorDefault,  kCFNumberSInt32Type,  &streamId);
                CFReadStreamSetProperty(readStream, CFSTR("PersistentConnectionID"), cfStreamID);
                CFRelease(cfStreamID);
                
                //Open the new stream
                streamOpened = CFReadStreamOpen(readStream);
                
                ConnectionInfo connectionInfo;
                connectionInfo.m_connectionId = connectionId;
                connectionInfo.m_streamId = streamId;
                connectionInfo.m_readStream = readStream;
                connectionInfo.m_connectionOpenTime = CSCore::Application::Get()->GetSystemTime();
                m_activeConnections.insert(std::make_pair(httpRequest, connectionInfo));
            }
            
			CFRelease(requestMessage);
			CFRelease(url);
            
            if(streamOpened == true)
            {
                //Begin the request with the read stream
                httpRequest->Start(readStream);
                m_requests.push_back(httpRequest);
                return httpRequest;
            }
            else
            {
                //The stream could not be opened
                delete httpRequest;
                CFRelease(readStream);
                return nullptr;
            }
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
            CSReachability* reachability = [CSReachability reachabilityForInternetConnection];
            NetworkStatus status = [reachability currentReachabilityStatus];
            
            return (status != NotReachable);
        }
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::OnUpdate(f32 in_timeSinceLastUpdate)
		{
            //We should do this in two loops incase anyone tries to insert into the requests from the completion callback
			for(u32 i=0; i<m_requests.size(); ++i)
            {
                m_requests[i]->Update(in_timeSinceLastUpdate);
            }
            
            for(auto it = m_requests.begin(); it != m_requests.end(); /*No increment*/)
            {
                if((*it)->HasCompleted())
                {
                    //We have finished with this connection so we can add it back to the pool if there is not already one there
                    auto itConnection = m_activeConnections.find(*it);
                    const ConnectionInfo& connectionInfo = itConnection->second;
                    if(m_unusedConnections.find(connectionInfo.m_connectionId) == m_unusedConnections.end())
                    {
                        m_unusedConnections.insert(std::make_pair(connectionInfo.m_connectionId, connectionInfo));
                    }
                    else
                    {
                        CFReadStreamClose(connectionInfo.m_readStream);
                        CFRelease(connectionInfo.m_readStream);
                    }
                    m_activeConnections.erase(itConnection);
                    
                    //remove the completed request
                    CS_SAFEDELETE(*it);
                    it = m_requests.erase(it);
                }
                else
                {
                    ++it;
                }
			}
            
            //Check the connection pool for any unused connections that are no longer required because they have
            //exceeded their lifespan
            for(auto it = m_unusedConnections.begin(); it != m_unusedConnections.end(); /*No increment*/)
            {
                const ConnectionInfo& connectionInfo = it->second;
                
                if(CSCore::Application::Get()->GetSystemTime() > (connectionInfo.m_connectionOpenTime + k_keepAliveTimeSecs))
                {
                    //Close the old stream
                    CFReadStreamClose(connectionInfo.m_readStream);
                    CFRelease(connectionInfo.m_readStream);
                    it = m_unusedConnections.erase(it);
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
            
            for(auto it = m_unusedConnections.begin(); it != m_unusedConnections.end(); ++it)
            {
                CFReadStreamClose(it->second.m_readStream);
                CFRelease(it->second.m_readStream);
            }
            
            m_unusedConnections.clear();
            
            for(auto it = m_activeConnections.begin(); it != m_activeConnections.end(); ++it)
            {
                CFReadStreamClose(it->second.m_readStream);
                CFRelease(it->second.m_readStream);
            }
            
            m_activeConnections.clear();
            
            m_totalNumConnectionsEstablished = 0;
        }
	}
}

#endif