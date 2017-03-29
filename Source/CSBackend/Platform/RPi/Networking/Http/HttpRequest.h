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

#ifndef _CSBACKEND_PLATFORM_RPI_HTTP_HTTPREQUEST_H_
#define _CSBACKEND_PLATFORM_RPI_HTTP_HTTPREQUEST_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequest.h>
#include <ChilliSource/Networking/Http/HttpResponse.h>

#include <curl/curl.h>

#include <atomic>
#include <list>
#include <mutex>
#include <sstream>

namespace CSBackend
{
	namespace RPi
	{
		/// Concerete implementation of the Raspberry Pi http request using curl library. Requests are
		/// threaded to avoid blocking the main thread
		///
		class HttpRequest final : public ChilliSource::HttpRequest
		{
		public:

			/// @return The type of the request (POST or GET)
			///
			Type GetType() const noexcept override { return m_type; }

			/// @return The original url to which the request was sent
			///
			const std::string& GetUrl() const noexcept override { return m_url; }

			/// @return The body of the POST request (GET request will return empty)
			///
			const std::string& GetBody() const noexcept override { return m_body; }

			/// @return The original headers of the request as keys/values
			///
			const ChilliSource::ParamDictionary& GetHeaders() const noexcept override { return m_headers; }

			/// Close the request.
			/// NOTE: The completion delegate is not invoked
			///
			void Cancel() noexcept override;

			/// @return the expected size in bytes of the response
			///
			u64 GetExpectedSize() const noexcept override { return m_expectedSize; }

			/// @return The size in bytes of the response that has already been downloaded
			///
			u64 GetDownloadedBytes() const noexcept override { return m_totalBytesRead; }

		private:
			friend class HttpRequestSystem;
			friend std::size_t CurlWriteResponseData(void*, std::size_t, std::size_t, HttpRequest*) noexcept;

			/// @param type
			///		POST or GET
			/// @param url
			///		Url of server to which to make the request
			/// @param body
			///		 POST only. The Http data to send to the server
			/// @param headers
			///		Http headers as key-values.
			/// @param timeoutSecs
			///		Request timeout in seconds
			/// @param curl
			///		Curl instance on which to set options and perform the request (passed in so it can be reused for other requests)
			/// @param bufferFlushSize
			///		Max size before the response buffer is flushed to the application
			/// @param delegate
			///		Called on success or fail
			///
			HttpRequest(Type type, std::string url, std::string body, ChilliSource::ParamDictionary headers, u32 timeoutSecs, CURL* curl, u32 bufferFlushSize, Delegate delegate) noexcept;

			/// Run on a background thread this function uses a pre-setup curl object to make an http request.
			/// It will store the response state and then terminate once finished. The completion event
			/// will be fired on the next update
			///
			/// @param destroyingMutex
			///		Mutex that manages the critical section of the HTTP system being shutdown
			///
			void PerformRequest(std::shared_ptr<std::mutex> destroyingMutex) noexcept;

			/// Called via curl with response data (partial or full). We then write
			/// to our response buffer and flush if we exceed the max buffer size
			///
			/// @param data
			///		Data to write
			/// @param dataSize
			///		Size of data to write in bytes
			///
			void WriteResponseData(const char* data, std::size_t dataSize) noexcept;

			/// Polls for the request thread finishing and invokes the completion delegate
			///
			/// @param timeSinceLastUpdate
			///		Time in seconds since last update
			///
			void Update(f32 timeSinceLastUpdate) noexcept;

			/// @return TRUE if the request is finished
			///
			bool HasCompleted() const noexcept { return m_isRequestComplete; }

			/// Inform the polling threads in a thread-safe manner that the system is shutting down
			///
			static void Shutdown() noexcept;

		private:

			static std::mutex s_addingMutexesMutex;
			static std::list<std::shared_ptr<std::mutex>> s_destroyingMutexes;
			static bool s_isDestroying;

			const Type m_type;
			const std::string m_url;
			const std::string m_body;
			const ChilliSource::ParamDictionary m_headers;
			const Delegate m_completionDelegate;
			const u32 m_bufferFlushSize;

			std::string m_responseData;
			u32 m_responseCode = 0;
			ChilliSource::HttpResponse::Result m_requestResult = ChilliSource::HttpResponse::Result::k_failed;

			u64 m_totalBytesRead = 0;
			u64 m_totalBytesReadThisBlock = 0;
			u64 m_expectedSize = 0;

			std::stringstream m_streamBuffer;
			CURL* m_curl;
			curl_slist* m_curlHeaders = nullptr;

			bool m_isPollingComplete = false;
			bool m_isRequestComplete = false;
			bool m_isRequestCancelled = false;

			std::atomic_int m_flushesPending;

			ChilliSource::TaskScheduler* m_taskScheduler;
		};
	}
}

#endif

#endif
