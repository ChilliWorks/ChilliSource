//
//  HttpRequest.cpp
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2012 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/Networking/Http/HttpRequest.h>

#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <Windows.h>
#include <winhttp.h>
#include <sstream>

#pragma comment(lib, "winhttp")

namespace CSBackend
{
	namespace Windows
	{
		std::list<std::shared_ptr<std::mutex>> HttpRequest::s_destroyingMutexes;
		std::mutex HttpRequest::s_addingMutexesMutex;
		bool HttpRequest::s_isDestroying = false;

		namespace
		{
			const u32 k_readBufferSize = 1024 * 50;

			//-------------------------------------------------------------------
			/// Split the WinHTTP blob header into key values
			///
			/// @author S Downie
			///
			/// @param Blob header
			/// @param Blob size
			///
			/// @return Key value dictionary
			//-------------------------------------------------------------------
			CSCore::ParamDictionary ParseHeaders(const WCHAR* in_headerBlob, DWORD in_headerSize)
			{
				//---SAMPLE

				//"HTTP/1.1 200 OK\r\n
				//Connection: keep - alive\r\n
				//Date : Thu, 03 Apr 2014 08 : 18 : 57 GMT\r\n
				//Content - Length: 24\r\n
				//Content - Type: application / json\r\n
				//Server : Apache\r\n
				//X - Powered - By: PHP / 5.3.10 - 1ubuntu3.9\r\n\r\n"

				CSCore::ParamDictionary headers;

				std::wstring key;
				std::wstring value;

				u32 headerIndex = 0;

				//Disregard the first line as it contains only the request status
				while (in_headerBlob[headerIndex] != '\r')
				{
					headerIndex++;
				}
				//Skip the '\r\n'
				headerIndex += 2;

				while (headerIndex < in_headerSize)
				{
					//Read the key
					while (in_headerBlob[headerIndex] != ':')
					{
						key += in_headerBlob[headerIndex];
						headerIndex++;
					}
					//Skip the ': '
					headerIndex += 2;

					//Read the value
					while (in_headerBlob[headerIndex] != '\r')
					{
						value += in_headerBlob[headerIndex];
						headerIndex++;
					}
					//Skip the '\r\n'
					headerIndex += 2;

					//Add to param dictionary
					headers.SetValue(WindowsStringUtils::UTF16ToUTF8(key), WindowsStringUtils::UTF16ToUTF8(value));
					key.clear();
					value.clear();

					//Check for the terminating '\r\n'
					if (in_headerBlob[headerIndex] == '\r' && in_headerBlob[headerIndex + 1] == '\n')
					{
						break;
					}
				}

				return headers;
			}
			//-------------------------------------------------------------------
			/// Get the Http header from the request
			///
			/// @author S Downie
			///
			/// @param Blob header
			///
			/// @return Key value dictionary
			//-------------------------------------------------------------------
			CSCore::ParamDictionary GetHeaders(HINTERNET in_requestHandle)
			{
				CSCore::ParamDictionary headers;

				DWORD headerSize;
				WinHttpQueryHeaders(in_requestHandle, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, nullptr, &headerSize, WINHTTP_NO_HEADER_INDEX);
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					DWORD bufferSize = headerSize / sizeof(WCHAR);
					WCHAR* headerBuffer = new WCHAR[bufferSize];

					if (WinHttpQueryHeaders(in_requestHandle, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, headerBuffer, &headerSize, WINHTTP_NO_HEADER_INDEX) == TRUE)
					{
						headers = ParseHeaders(headerBuffer, bufferSize);
					}

					delete[] headerBuffer;
				}

				return headers;
			}
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		HttpRequest::HttpRequest(const Desc& in_requestDesc, HINTERNET in_requestHandle, HINTERNET in_connectionHandle, u32 in_bufferFlushSize, const Delegate& in_delegate)
			: m_desc(in_requestDesc), m_bufferFlushSize(in_bufferFlushSize), m_completionDelegate(in_delegate), m_shouldKillThread(false), m_isPollingComplete(false), m_isRequestComplete(false),
			m_responseCode(0), m_requestResult(Result::k_failed), m_totalBytesRead(0)
		{
			CS_ASSERT(m_completionDelegate, "Http request cannot have null delegate");

			//Begin the read loop as a threaded task
			auto destroyingMutex = std::make_shared<std::mutex>();
			std::unique_lock<std::mutex> lock(s_addingMutexesMutex);
			s_destroyingMutexes.push_back(destroyingMutex);
			lock.unlock();
			CSCore::Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(&HttpRequest::PollReadStream, this, in_requestHandle, in_connectionHandle, destroyingMutex));
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void HttpRequest::Update(f32 infDT)
		{
			//Check if the data has finished streaming and invoke the completion delegate on the main thread
			if(m_isPollingComplete == true)
			{
				m_isRequestComplete = true;

				if (m_requestResult != Result::k_cancelled)
				{
					m_completionDelegate(this, m_requestResult);
				}

				m_completionDelegate = nullptr;
			}
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void HttpRequest::PollReadStream(HINTERNET in_requestHandle, HINTERNET in_connectionHandle, std::shared_ptr<std::mutex> in_destroyingMutex)
		{	
			std::unique_lock<std::mutex> lock(*in_destroyingMutex);

			if (s_isDestroying == true)
				return;

			u32 bufferFlushSize = m_bufferFlushSize;
			std::string body = m_desc.m_body;

			lock.unlock();
			BOOL sendRequestResult = WinHttpSendRequest(in_requestHandle, 0, WINHTTP_NO_REQUEST_DATA, (LPVOID)body.data(), body.length(), body.length(), NULL);
			lock.lock();

			if (s_isDestroying == true)
				return;
		
			if (sendRequestResult == FALSE)
			{
				DWORD error = GetLastError();
				Result result;
				switch (error)
				{
				case ERROR_WINHTTP_TIMEOUT:
					result = Result::k_timeout;
					break;
				default:
					result = Result::k_failed;
					break;
				}

				WinHttpCloseHandle(in_requestHandle);
				WinHttpCloseHandle(in_connectionHandle);
				m_isPollingComplete = true;
				m_requestResult = result;
				return;
			}

			lock.unlock();
			BOOL receiveResponseResult = WinHttpReceiveResponse(in_requestHandle, nullptr);
			lock.lock();

			if (s_isDestroying == true)
				return;

			if (receiveResponseResult == FALSE)
			{
				DWORD error = GetLastError();
				Result result;
				switch (error)
				{
				case ERROR_WINHTTP_TIMEOUT:
					result = Result::k_timeout;
					break;
				default:
					result = Result::k_failed;
					break;
				}

				WinHttpCloseHandle(in_requestHandle);
				WinHttpCloseHandle(in_connectionHandle);
				m_isPollingComplete = true;
				m_requestResult = result;
				return;
			}

			//Get the status from the server
			DWORD headerSize = sizeof(DWORD);
			u32 responseCode = 0;
			WinHttpQueryHeaders(in_requestHandle, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, nullptr, &responseCode, &headerSize, nullptr);

			//Pull the headers and check for a redirect URL
			std::string redirectUrl;
			CSCore::ParamDictionary headers = GetHeaders(in_requestHandle);
			headers.TryGetValue("Location", redirectUrl);

			// Keep reading from the remote server until there's
			// nothing left to read
			DWORD bytesToBeRead = 0;
			DWORD bytesRead = 0;
			Result result = Result::k_failed;
			u32 totalBytesRead = 0;
			u32 totalBytesReadThisBlock = 0;
			s8 readBuffer[k_readBufferSize];
			std::stringstream streamBuffer;

			do
			{
				lock.unlock();
				BOOL queryAvailableResult = WinHttpQueryDataAvailable(in_requestHandle, &bytesToBeRead);
				lock.lock();

				if (s_isDestroying == true)
					return;

				if (queryAvailableResult == FALSE)
				{
					WinHttpCloseHandle(in_requestHandle);
					WinHttpCloseHandle(in_connectionHandle);
					m_requestResult = Result::k_failed;
					m_isPollingComplete = true;
					return;
				}

				lock.unlock();
				BOOL readDataResult = WinHttpReadData(in_requestHandle, readBuffer, k_readBufferSize, &bytesRead);
				lock.lock();

				if (s_isDestroying == true)
					return;

				if (readDataResult == FALSE)
				{
					WinHttpCloseHandle(in_requestHandle);
					WinHttpCloseHandle(in_connectionHandle);
					m_requestResult = Result::k_failed;
					m_isPollingComplete = true;
					return;
				}

				//We have read some data
				if (bytesRead > 0)
				{
					streamBuffer.write(readBuffer, bytesRead);
					totalBytesRead += bytesRead;
					totalBytesReadThisBlock += bytesRead;

					if (bufferFlushSize != 0 && totalBytesReadThisBlock >= bufferFlushSize)
					{
						m_responseData = streamBuffer.str();
						m_completionDelegate(this, Result::k_flushed);
						streamBuffer.clear();
						streamBuffer.str("");
						totalBytesReadThisBlock = 0;
					}
				}

			} while (bytesRead > 0 && m_shouldKillThread == false);

			if (m_shouldKillThread == false)
			{
				result = Result::k_completed;
			}

			WinHttpCloseHandle(in_requestHandle);
			WinHttpCloseHandle(in_connectionHandle);
			m_isPollingComplete = true;
			m_requestResult = result;
			m_responseCode = responseCode;
			m_totalBytesRead = totalBytesRead;
			m_responseData = streamBuffer.str();
			m_desc.m_redirectionUrl = redirectUrl;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		void HttpRequest::Cancel()
		{
			m_shouldKillThread = true;
			m_requestResult = Result::k_cancelled;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		bool HttpRequest::HasCompleted() const
		{
			return m_isRequestComplete;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		const CSNetworking::HttpRequest::Desc& HttpRequest::GetDescription() const
		{
			return m_desc;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		const std::string & HttpRequest::GetResponse() const
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
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		void HttpRequest::Shutdown()
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
