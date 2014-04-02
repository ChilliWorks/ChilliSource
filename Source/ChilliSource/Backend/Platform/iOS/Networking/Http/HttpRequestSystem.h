//
//  HttpRequestSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_HTTP_HTTPREQUESTSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_HTTP_HTTPREQUESTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

#include <unordered_map>
#include <vector>

#include <CoreFoundation/CoreFoundation.h>

namespace ChilliSource
{
	namespace iOS
	{
        //--------------------------------------------------------------------------------------------------
        /// iOS implementation of the http connection system. Reponsible for making http requests to remote
        /// urls and managing the lifetime of the requests and the connections. Uses the CFNetworking library
        ///
        /// @author S Downie
        //--------------------------------------------------------------------------------------------------
		class HttpRequestSystem final : public Networking::HttpRequestSystem
		{
		public:
            
            typedef u32 ConnectionId;
			
			//--------------------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface ID
            ///
			/// @return Whether object if of argument type
			//--------------------------------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------------------------------------------------------
            /// Causes the system to issue a request with the given details.
            ///
            /// @author S Downie
            ///
            /// @param A descriptor detailing the request params
			/// @param A function to call when the request is completed. Note that the request can be completed with failure as well as success.
            ///
            /// @return A pointer to the request. The system owns this pointer. Returns nullptr if the request cannot be created.
            //--------------------------------------------------------------------------------------------------
            Networking::HttpRequest* MakeRequest(const Networking::HttpRequest::Desc& in_requestDesc, const Networking::HttpRequest::Delegate& in_delegate) override;
			//--------------------------------------------------------------------------------------------------
            /// Equivalent to calling cancel on every incomplete request in progress.
            ///
            /// @author S Downie
            //--------------------------------------------------------------------------------------------------
            void CancelAllRequests() override;
            //--------------------------------------------------------------------------------------------------
            /// Checks if the device is internet ready
            ///
            /// @author S Downie
            ///
            /// @return Success if net available
            //--------------------------------------------------------------------------------------------------
            bool CheckReachability() const override;
			//--------------------------------------------------------------------------------------------------
            /// Poll the connection on active requests
            ///
            /// @author S Downie
			///
			/// @param Time since last update in seconds
			//--------------------------------------------------------------------------------------------------
            void OnUpdate(f32 in_timeSinceLastUpdate) override;
            //--------------------------------------------------------------------------------------------------
            /// Called when the system is destroyed. Cancels all pending requests
            ///
            /// @author S Downie
			//--------------------------------------------------------------------------------------------------
            void OnDestroy() override;
            
        private:
            
            friend Networking::HttpRequestSystemUPtr Networking::HttpRequestSystem::Create();
            //--------------------------------------------------------------------------------------------------
            /// Private constructor to fore use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------------------------------------------------
            HttpRequestSystem() = default;
            
        private:
            
            struct ConnectionInfo
            {
                TimeIntervalSecs m_connectionOpenTime;
                ConnectionId m_streamId;
                ConnectionId m_connectionId;
                CFReadStreamRef m_readStream;
            };
            
			std::vector<HttpRequest*> m_requests;
            
            std::unordered_map<ConnectionId, ConnectionInfo> m_unusedConnections;
            std::unordered_map<HttpRequest*, ConnectionInfo> m_activeConnections;
            
            u32 m_totalNumConnectionsEstablished = 0;
		};
	}
    
}
#endif