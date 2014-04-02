//
//  HttpRequestSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 08/11/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUESTSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUESTSYSTEM_H_

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

#include <vector>

namespace ChilliSource
{
	namespace Android
	{	
		//--------------------------------------------------------------------------------------------------
		/// Android implementation of the http connection system. Reponsible for making http requests to remote
		/// urls and managing the lifetime of the requests and the connections. Uses the Java
		/// HttpUrlConnection library
		///
		/// @author I Copland
		//--------------------------------------------------------------------------------------------------
		class HttpRequestSystem final : public Networking::HttpRequestSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(HttpRequestSystem);

			//--------------------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Interface ID
            ///
			/// @return Whether object if of argument type
			//--------------------------------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The number of seconds that will elapse before a request is deemed to have timed out
            /// on connection
            //--------------------------------------------------------------------------------------------------
            void SetConnectionTimeout(u32 in_timeoutSecs) override;
            //--------------------------------------------------------------------------------------------------
            /// Causes the system to issue a request with the given details.
            ///
            /// @author I Copland
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
            /// @author I Copland
            //--------------------------------------------------------------------------------------------------
            void CancelAllRequests() override;
            //--------------------------------------------------------------------------------------------------
            /// Checks if the device is internet ready
            ///
            /// @author I Copland
            ///
            /// @return Success if net available
            //--------------------------------------------------------------------------------------------------
            bool CheckReachability() const override;
			//--------------------------------------------------------------------------------------------------
            /// Poll the connection on active requests
            ///
            /// @author I Copland
			///
			/// @param Time since last update in seconds
			//--------------------------------------------------------------------------------------------------
            void OnUpdate(f32 in_timeSinceLastUpdate) override;
            //--------------------------------------------------------------------------------------------------
            /// Called when the system is destroyed. Cancels all pending requests
            ///
            /// @author I Copland
			//--------------------------------------------------------------------------------------------------
            void OnDestroy() override;

		private:
            friend Networking::HttpRequestSystemUPtr Networking::HttpRequestSystem::Create();
            //--------------------------------------------------------------------------------------------------
            /// Private constructor to fore use of factory method
            ///
            /// @author I Copland
            //--------------------------------------------------------------------------------------------------
            HttpRequestSystem() = default;

		private:

			std::vector<HttpRequest*> m_requests;
		};
	}

}
#endif
