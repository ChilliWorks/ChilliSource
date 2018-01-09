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

#include <CSBackend/Platform/RPi/Networking/Http/HttpRequest.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
	namespace RPi
	{
		std::list<std::shared_ptr<std::mutex>> HttpRequest::s_destroyingMutexes;
		std::mutex HttpRequest::s_addingMutexesMutex;
		bool HttpRequest::s_isDestroying = false;

		/// Callback function used by curl to write data into our response buffer.
		/// Needs access to private method of HttpRequest so is a friend function
		///
		/// @param data
		///		Response data to write
		/// @param size
		///		Size of response data element
		/// @param num
		///		Number of response data elements
		/// @param request
		///		Request to write the data to
		///
		/// @return Size of data read
		///
		std::size_t CurlWriteResponseData(void* data, std::size_t size, std::size_t num, HttpRequest* request) noexcept
		{
			// If destroying then just abort
			if (HttpRequest::s_isDestroying == true || request->m_isRequestCancelled == true)
				return 0;

			std::size_t writeSize = size * num;
			request->WriteResponseData((const char*)data, writeSize);
			return writeSize;
		}

		namespace
		{
			/// Convert our internal header storage into curl header storage. The curl headers
			/// must be manually released
			///
			/// @param headers
			///		Headers to convert
			///
			/// @return Ownership of the converted headers
			///
			curl_slist* CreateHeaders(const ChilliSource::ParamDictionary& headers) noexcept
			{
				curl_slist* chunk = nullptr;

				for(const auto& header : headers)
				{
					std::string formatted = header.first + ": " + header.second;
					chunk = curl_slist_append(chunk, formatted.c_str());
				}

				return chunk;
			}
		}

		//------------------------------------------------------------------
		HttpRequest::HttpRequest(Type type, std::string url, std::string body, ChilliSource::ParamDictionary headers, u32 timeoutSecs, CURL* curl, u32 bufferFlushSize, Delegate delegate) noexcept
		: m_url(std::move(url)), m_type(type), m_headers(std::move(headers)), m_body(std::move(body)), m_bufferFlushSize(bufferFlushSize), m_completionDelegate(std::move(delegate)), m_flushesPending(0), m_curl(curl)
		{
			CS_ASSERT(m_completionDelegate, "Http request cannot have null delegate");

			auto destroyingMutex = std::make_shared<std::mutex>();
			std::unique_lock<std::mutex> lock(s_addingMutexesMutex);
			s_destroyingMutexes.push_back(destroyingMutex);
			lock.unlock();

			curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSecs);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteResponseData);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

			if(m_bufferFlushSize != 0)
			{
				curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, m_bufferFlushSize);
			}

			if(m_headers.size() > 0)
			{
				m_curlHeaders = CreateHeaders(m_headers);
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, m_curlHeaders);
			}

			if(type == Type::k_post)
			{
				curl_easy_setopt(curl, CURLOPT_POST, 1L);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, m_body.size());
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_body.c_str());
			}

			//TODO: This should probably be handled by a HTTP system specific thread, like the other platforms.
			m_taskScheduler = ChilliSource::Application::Get()->GetTaskScheduler();
			m_taskScheduler->ScheduleTask(ChilliSource::TaskType::k_large, [=](const ChilliSource::TaskContext&)
			{
				PerformRequest(destroyingMutex);
			});
		}

		//------------------------------------------------------------------
		void HttpRequest::Update(f32 timeSinceLastUpdate) noexcept
		{
			//Check if the data has finished streaming and invoke the completion delegate on the main thread
			if(m_isPollingComplete == true && m_flushesPending <= 0)
			{
				m_isRequestComplete = true;

				if (m_isRequestCancelled == false)
				{
					m_completionDelegate(this, ChilliSource::HttpResponse(m_requestResult, m_responseCode, m_responseData));
				}
			}
		}

		//------------------------------------------------------------------
		void HttpRequest::PerformRequest(std::shared_ptr<std::mutex> destroyingMutex) noexcept
		{
			std::unique_lock<std::mutex> lock(*destroyingMutex);

			if (s_isDestroying == true)
				return;

			lock.unlock();

			//Blocking call. Will call into the write callback which is responsible for reading the response data into the output buffer.
			CURLcode curlResult = curl_easy_perform(m_curl);

			//Fetch the response code
			long httpCode = 0;
			curl_easy_getinfo (m_curl, CURLINFO_RESPONSE_CODE, &httpCode);
			m_responseCode = (u32)httpCode;

			if(m_curlHeaders != nullptr)
			{
				curl_slist_free_all(m_curlHeaders);
				m_curlHeaders = nullptr;
			}

			lock.lock();

			if (s_isDestroying == true)
				return;

			switch(curlResult)
			{
				case CURLE_OK:
					m_requestResult = ChilliSource::HttpResponse::Result::k_completed;
					m_responseData = m_streamBuffer.str();
					m_streamBuffer.clear();
					m_streamBuffer.str("");
					break;
				case CURLE_OPERATION_TIMEDOUT:
					m_requestResult = ChilliSource::HttpResponse::Result::k_timeout;
					break;
				default:
					m_requestResult = ChilliSource::HttpResponse::Result::k_failed;
					CS_LOG_ERROR_FMT("Curl Error: %d\n", curlResult);
					break;
			}

			m_isPollingComplete = true;
		}

		//----------------------------------------------------------------------------------------
		void HttpRequest::WriteResponseData(const char* data, std::size_t dataSize) noexcept
		{
			//Once we have some response data we are able to fetch the content length from the headers
			if(m_expectedSize == 0)
			{
				double contentLength = 0;
				curl_easy_getinfo(m_curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &contentLength);
				m_expectedSize = (u32)contentLength;
			}

			m_streamBuffer.write(data, dataSize);
			m_totalBytesRead += dataSize;
			m_totalBytesReadThisBlock += dataSize;

			if (m_bufferFlushSize != 0 && m_totalBytesReadThisBlock >= m_bufferFlushSize)
			{
				m_responseData = m_streamBuffer.str();
				m_requestResult = ChilliSource::HttpResponse::Result::k_flushed;

				++m_flushesPending;
				m_taskScheduler->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
				{
					--m_flushesPending;
					m_completionDelegate(this, ChilliSource::HttpResponse(m_requestResult, m_responseCode, m_responseData));
				});

				m_streamBuffer.clear();
				m_streamBuffer.str("");
				m_totalBytesReadThisBlock = 0;
			}
		}

		//----------------------------------------------------------------------------------------
		void HttpRequest::Cancel() noexcept
		{
			m_isRequestCancelled = true;
		}

		//----------------------------------------------------------------------------------------
		void HttpRequest::Shutdown() noexcept
		{
			std::unique_lock<std::mutex> mutexLock(s_addingMutexesMutex);

			std::vector<std::unique_lock<std::mutex>> locks;
			locks.reserve(s_destroyingMutexes.size());

			for (auto& mutex : s_destroyingMutexes)
			{
				locks.push_back(std::unique_lock<std::mutex>(*mutex));
			}

			s_isDestroying = true;
		}
	}
}

#endif
