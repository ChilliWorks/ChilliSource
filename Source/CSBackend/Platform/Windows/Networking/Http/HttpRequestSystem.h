//
//  HttpRequestSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_HTTP_HTTPREQUESTSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_HTTP_HTTPREQUESTSYSTEM_H_

#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

namespace ChilliSource
{
	namespace Windows
	{	
		//Windows typedefs so we don't have to include windows.h in the header
		typedef void* HINTERNET;

		//--------------------------------------------------------------------------------------------------
		/// Windows implementation of the http connection system. Reponsible for making http requests to remote
		/// urls and managing the lifetime of the requests and the connections. Uses the WinHTTP library
		///
		/// @author S Downie
		//--------------------------------------------------------------------------------------------------
		class HttpRequestSystem final : public Networking::HttpRequestSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(HttpRequestSystem);

			//--------------------------------------------------------------------------------------------------
			/// @author S Downie
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

		private:
			friend Networking::HttpRequestSystemUPtr Networking::HttpRequestSystem::Create();
			//-------------------------------------------------------
			/// Private constructor to fore use of factory method
			///
			/// @author S Downie
			//-------------------------------------------------------
			HttpRequestSystem() = default;
			//--------------------------------------------------------------------------------------------------
			/// Called when the system is created. Creates the WinHTTP session
			///
			/// @author S Downie
			//--------------------------------------------------------------------------------------------------
			void OnInit() override;
			//--------------------------------------------------------------------------------------------------
			/// Poll the connection on active requests
			///
			/// @author S Downie
			///
			/// @param Time since last update in seconds
			//--------------------------------------------------------------------------------------------------
			void OnUpdate(f32 in_timeSinceLastUpdate) override;
			//--------------------------------------------------------------------------------------------------
			/// Called when the system is destroyed. Cancels all pending requests and destroys the session
			///
			/// @author S Downie
			//--------------------------------------------------------------------------------------------------
			void OnDestroy() override;

		private:

			std::vector<HttpRequest*> m_requests;

			HINTERNET m_sessionHandle;
		};
	}
}

#endif

#endif

