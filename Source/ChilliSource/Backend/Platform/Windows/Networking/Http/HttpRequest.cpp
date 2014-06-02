//
//  HttpRequest.cpp
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2012 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <ChilliSource/Backend/Platform/Windows/Networking/Http/HttpRequest.h>

#include <ChilliSource/Backend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <Windows.h>
#include <winhttp.h>
#include <sstream>

#pragma comment(lib, "winhttp")

namespace ChilliSource
{
	namespace Windows
	{
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
			Core::ParamDictionary ParseHeaders(const WCHAR* in_headerBlob, DWORD in_headerSize)
			{
				//---SAMPLE

				//"HTTP/1.1 200 OK\r\n
				//Connection: keep - alive\r\n
				//Date : Thu, 03 Apr 2014 08 : 18 : 57 GMT\r\n
				//Content - Length: 24\r\n
				//Content - Type: application / json\r\n
				//Server : Apache\r\n
				//X - Powered - By: PHP / 5.3.10 - 1ubuntu3.9\r\n\r\n"

				Core::ParamDictionary headers;

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
			Core::ParamDictionary GetHeaders(HINTERNET in_requestHandle)
			{
				Core::ParamDictionary headers;

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
			m_responseCode(0), m_requestResult(Result::k_failed), m_totalBytesRead(0), m_totalBytesReadThisBlock(0)
		{
			CS_ASSERT(m_completionDelegate, "Http request cannot have null delegate");

			//Begin the read loop as a threaded task
			Core::Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(&HttpRequest::PollReadStream, this, in_requestHandle, in_connectionHandle));
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
		void HttpRequest::PollReadStream(HINTERNET in_requestHandle, HINTERNET in_connectionHandle)
		{
			if (WinHttpSendRequest(in_requestHandle, 0, WINHTTP_NO_REQUEST_DATA, (LPVOID)m_desc.m_body.data(), m_desc.m_body.length(), m_desc.m_body.length(), NULL) == false)
			{
				DWORD error = GetLastError();

				switch (error)
				{
					case ERROR_WINHTTP_TIMEOUT:
						m_requestResult = Result::k_timeout;
						break;
					default:
						m_requestResult = Result::k_failed;
						break;
				}

				WinHttpCloseHandle(in_requestHandle);
				WinHttpCloseHandle(in_connectionHandle);
				m_isPollingComplete = true;
				return;
			}

			if (WinHttpReceiveResponse(in_requestHandle, nullptr) == false)
			{
				DWORD error = GetLastError();

				switch (error)
				{
				case ERROR_WINHTTP_TIMEOUT:
					m_requestResult = Result::k_timeout;
					break;
				default:
					m_requestResult = Result::k_failed;
					break;
				}

				WinHttpCloseHandle(in_requestHandle);
				WinHttpCloseHandle(in_connectionHandle);
				m_isPollingComplete = true;
				return;
			}

			//Get the status from the server
			DWORD headerSize = sizeof(DWORD);
			WinHttpQueryHeaders(in_requestHandle, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, nullptr, &m_responseCode, &headerSize, nullptr);

			//Pull the headers and check for a redirect URL
			Core::ParamDictionary headers = GetHeaders(in_requestHandle);
			headers.TryGetValue("Location", m_desc.m_redirectionUrl);

			// Keep reading from the remote server until there's
			// nothing left to read
			DWORD bytesToBeRead = 0;
			DWORD bytesRead = 0;
			s8 readBuffer[k_readBufferSize];
			std::stringstream streamBuffer;

			do
			{
				if (WinHttpQueryDataAvailable(in_requestHandle, &bytesToBeRead) == false)
				{
					WinHttpCloseHandle(in_requestHandle);
					WinHttpCloseHandle(in_connectionHandle);
					m_requestResult = Result::k_failed;
					m_isPollingComplete = true;
					return;
				}

				if (!WinHttpReadData(in_requestHandle, readBuffer, k_readBufferSize, &bytesRead))
				{
					WinHttpCloseHandle(in_requestHandle);
					WinHttpCloseHandle(in_connectionHandle);
					m_requestResult = Result::k_failed;
					m_isPollingComplete = true;;
					return;
				}

				//We have read some data
				if (bytesRead > 0)
				{
					streamBuffer.write(readBuffer, bytesRead);
					m_totalBytesRead += bytesRead;
					m_totalBytesReadThisBlock += bytesRead;

					if (m_bufferFlushSize != 0 && m_totalBytesReadThisBlock >= m_bufferFlushSize)
					{
						m_responseData = streamBuffer.str();
						streamBuffer.clear();
						streamBuffer.str("");
						m_totalBytesReadThisBlock = 0;
						m_completionDelegate(this, Result::k_flushed);
					}
				}

			} while (bytesRead > 0 && m_shouldKillThread == false);	

			if (m_shouldKillThread == false)
			{
				m_responseData = streamBuffer.str();
				m_requestResult = Result::k_completed;
			}

			WinHttpCloseHandle(in_requestHandle);
			WinHttpCloseHandle(in_connectionHandle);
			m_isPollingComplete = true;
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
		const Networking::HttpRequest::Desc& HttpRequest::GetDescription() const
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
	}
}

#endif
