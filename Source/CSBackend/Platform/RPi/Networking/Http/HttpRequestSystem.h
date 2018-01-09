//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_PLATFORM_RPI_HTTP_HTTPREQUESTSYSTEM_H_
#define _CSBACKEND_PLATFORM_RPI_HTTP_HTTPREQUESTSYSTEM_H_

#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <CSBackend/Platform/RPi/Networking/Http/HttpRequest.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

#include <vector>

namespace CSBackend
{
	namespace RPi
	{
		/// Raspberry Pi implementation of the Http connection system. Responsible for making http GET and POST
		/// requests to remote servers. Uses libCurl under the hood
		///
		class HttpRequestSystem final : public ChilliSource::HttpRequestSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(HttpRequestSystem);

			///
			bool IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept override;

			/// Issues an Http GET request to the given URL.
			///
			/// @param url
			///		URL address of the server to make the request to.
			/// @param delegate
			///		Delegate that is called when the request completes. Completion can be failure as well as success
			/// @param timeoutSecs
			///		Timeout in seconds after which the request is abandonded.
			///
			/// @return Handle to the http request - this is owned by the system.
			///
			HttpRequest* MakeGetRequest(const std::string& url, const HttpRequest::Delegate& delegate, u32 timeoutSecs = k_defaultTimeoutSecs) noexcept override;

			/// Issues an Http GET request with the given headers to the given URL.
			///
			/// @param url
			///		URL address of the server to make the request to.
			/// @param headers
			///		Http headers as key-values
			/// @param delegate
			///		Delegate that is called when the request completes. Completion can be failure as well as success
			/// @param timeoutSecs
			///		Timeout in seconds after which the request is abandonded.
			///
			/// @return Handle to the http request - this is owned by the system.
			///
			HttpRequest* MakeGetRequest(const std::string& url, const ChilliSource::ParamDictionary& headers, const HttpRequest::Delegate& delegate, u32 timeoutSecs = k_defaultTimeoutSecs) noexcept override;

			/// Issues an Http POST request with the given body to the given URL.
			///
			/// @param url
			///		URL address of the server to make the request to.
			/// @param body
			///		Http POST body (i.e. data that is sent to the server).
			/// @param delegate
			///		Delegate that is called when the request completes. Completion can be failure as well as success
			/// @param timeoutSecs
			///		Timeout in seconds after which the request is abandonded.
			///
			/// @return Handle to the http request - this is owned by the system.
			///
			HttpRequest* MakePostRequest(const std::string& url, const std::string& body, const HttpRequest::Delegate& delegate, u32 timeoutSecs = k_defaultTimeoutSecs) noexcept override;

			/// Issues an Http POST request with the given body and headers to the given URL.
			///
			/// @param url
			///		URL address of the server to make the request to.
			/// @param body
			///		Http POST body (i.e. data that is sent to the server).
			/// @param headers
			///		Http headers as key-values
			/// @param delegate
			///		Delegate that is called when the request completes. Completion can be failure as well as success
			/// @param timeoutSecs
			///		Timeout in seconds after which the request is abandonded.
			///
			/// @return Handle to the http request - this is owned by the system.
			///
			HttpRequest* MakePostRequest(const std::string& url, const std::string& body, const ChilliSource::ParamDictionary& headers, const HttpRequest::Delegate& delegate, u32 timeoutSecs = k_defaultTimeoutSecs) noexcept override;

			/// Equivalent to calling cancel on every incomplete request in progress.
			///
			void CancelAllRequests() noexcept override;

			/// Checks if the device is internet ready. Should be used only as a heuristic for informing the user and
			/// does ot guarantee the success of an individual request
			///
			/// @param delegate
			///		Called when reachability has been determined
			///
			void CheckReachability(const ReachabilityResultDelegate& delegate) const noexcept override;

		private:
			friend ChilliSource::HttpRequestSystemUPtr ChilliSource::HttpRequestSystem::Create();

			///
			HttpRequestSystem() = default;

			/// Concrete method to which all MakeRequest overloads feed.
			///
			/// @param type
			///		POST or GET
			/// @param url
			///		Url of server to which to make the request
			/// @param body
			///		 POST only. The Http data to send to the server
			/// @param headers
			///		Http headers as key-values.
			/// @param delegate
			///		Called on success or fail
			/// @param timeoutSecs
			///		Request timeout in seconds
			///
			/// @return Request. Owned by the system.
			///
			HttpRequest* MakeRequest(HttpRequest::Type type, const std::string& url, const std::string& body, const ChilliSource::ParamDictionary& headers, const HttpRequest::Delegate& delegate, u32 timeoutSecs) noexcept;

			/// Initialise libCurl
			///
			void OnInit() noexcept override;

			/// Check for finished requests and invoke the delegates on the main thread
			///
			/// @param timeSinceLastUpdate
			///		Time since last update in seconds
			///
			void OnUpdate(f32 timeSinceLastUpdate) noexcept override;

			/// Cleanup curl
			///
			void OnDestroy() noexcept override;

		private:

			std::vector<HttpRequest*> m_requests;
		};
	}
}

#endif

#endif
