//
//  HttpRequestSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 23/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/Networking/Http/HttpRequestSystem.h>

#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <Windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp")

namespace CSBackend
{
	namespace Windows
	{
		namespace
		{
			//--------------------------------------------------------------------------------------------------
			/// Setup the request to use SSL level 3
			///
			/// @author S Downie
			///
			/// @param Request handle
			///
			/// @return Success or failure
			//--------------------------------------------------------------------------------------------------
			bool ApplySSLSettings(HINTERNET in_requestHandle)
			{
				DWORD requestOptions = SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
				if (!WinHttpSetOption(in_requestHandle, WINHTTP_OPTION_SECURITY_FLAGS, &requestOptions, sizeof(DWORD)))
				{
					CS_LOG_ERROR("Failed to set HTTP SSL security flag options");
					return false;
				}

				return true;
			}
			//--------------------------------------------------------------------------------------------------
			/// Convert param dictionary to header format required by WinHTTP
			///
			/// @author S Downie
			///
			/// @param Header dictionary
			///
			/// @return Header blob
			//--------------------------------------------------------------------------------------------------
			std::wstring ConvertHeaders(const ChilliSource::ParamDictionary& in_headers)
			{
				//The headers must be passed as a single string with each header except the last
				//terminated by a carraige return/line feed
				std::string headerBlob;
				for (auto it = in_headers.begin(); it != in_headers.end(); ++it)
				{
					headerBlob += it->first;
					headerBlob += ": ";
					headerBlob += it->second;
					headerBlob += '\r';
					headerBlob += '\n';
				}

				CS_ASSERT(headerBlob.length() > 2, "Http: Cannot convert empty headers");

				std::wstring headerBlobWide(headerBlob.length() - 2, L' ');
				std::copy(headerBlob.begin(), headerBlob.end()-2, headerBlobWide.begin());
				return headerBlobWide;
			}
		}

		CS_DEFINE_NAMEDTYPE(HttpRequestSystem);

		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::OnInit()
		{
			m_sessionHandle = ::WinHttpOpen(L"CSWinHttpClient", WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
			CS_ASSERT(m_sessionHandle != nullptr, "Failed to open WinHTTP session");
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		bool HttpRequestSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ChilliSource::HttpRequestSystem::InterfaceID || in_interfaceId == HttpRequestSystem::InterfaceID;
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakeGetRequest(const std::string& in_url, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
		{
			return MakeRequest(HttpRequest::Type::k_get, in_url, "", ChilliSource::ParamDictionary(), in_delegate, in_timeoutSecs);
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakeGetRequest(const std::string& in_url, const ChilliSource::ParamDictionary& in_headers, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
		{
			return MakeRequest(HttpRequest::Type::k_get, in_url, "", in_headers, in_delegate, in_timeoutSecs);
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakePostRequest(const std::string& in_url, const std::string& in_body, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
		{
			return MakeRequest(HttpRequest::Type::k_post, in_url, in_body, ChilliSource::ParamDictionary(), in_delegate, in_timeoutSecs);
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakePostRequest(const std::string& in_url, const std::string& in_body, const ChilliSource::ParamDictionary& in_headers, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
		{
			return MakeRequest(HttpRequest::Type::k_post, in_url, in_body, in_headers, in_delegate, in_timeoutSecs);
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		HttpRequest* HttpRequestSystem::MakeRequest(HttpRequest::Type in_type, const std::string& in_url, const std::string& in_body, const ChilliSource::ParamDictionary& in_headers, const HttpRequest::Delegate& in_delegate, u32 in_timeoutSecs)
		{
			CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Http requests can currently only be made on the main thread");
			CS_ASSERT(in_delegate != nullptr, "Cannot make an http request with a null delegate");
			CS_ASSERT(in_url.empty() == false, "Cannot make an http request to a blank url");

			URL_COMPONENTS urlComps;

			//Initialize the URL_COMPONENTS structure.
			ZeroMemory(&urlComps, sizeof(URL_COMPONENTS));
			urlComps.dwStructSize = sizeof(URL_COMPONENTS);

			//Set required component lengths to non-zero so that they are cracked.
			urlComps.dwSchemeLength    = (DWORD)-1;
			urlComps.dwHostNameLength  = (DWORD)-1;
			urlComps.dwUrlPathLength   = (DWORD)-1;
			urlComps.dwExtraInfoLength = (DWORD)-1;

			//Change the URL to wide string
			std::wstring url(WindowsStringUtils::UTF8ToUTF16(in_url));

			//Crack the URL.
			if (!WinHttpCrackUrl(url.c_str(), (DWORD)url.length(), 0, &urlComps))
			{
				CS_LOG_FATAL("Cannot crack URL: " + in_url);
				return nullptr;
			}

			//Weirdly the cracked URL struct only gives you the ability to crack your own URL
			std::wstring hostName = urlComps.lpszHostName;
			hostName = hostName.substr(0, urlComps.dwHostNameLength);
			HINTERNET connectionHandle = ::WinHttpConnect(m_sessionHandle, hostName.c_str(), INTERNET_DEFAULT_PORT, 0);

			if (!connectionHandle)
			{
				CS_LOG_ERROR("Failed to connect to server: " + in_url);
				return nullptr;
			}

			//Set up the request based on whether it is POST or GET and whether it is SSL
			LPCWSTR type = (in_type == ChilliSource::HttpRequest::Type::k_get ? L"GET" : L"POST");
			HINTERNET requestHandle = 0;

			std::wstring urlPath = urlComps.lpszUrlPath;
			urlPath = urlPath.substr(0, urlComps.dwUrlPathLength);

			if (urlComps.nScheme == INTERNET_SCHEME_HTTPS)
			{
				requestHandle = ::WinHttpOpenRequest(connectionHandle, type, urlPath.c_str(), L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
				if (requestHandle == nullptr || ApplySSLSettings(requestHandle) == false)
				{
					CS_LOG_ERROR("Failed to open request: " + in_url);
					WinHttpCloseHandle(connectionHandle);
					return nullptr;
				}
			}
			else
			{
				requestHandle = ::WinHttpOpenRequest(connectionHandle, type, urlPath.c_str(), L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
				if (requestHandle == nullptr)
				{
					CS_LOG_ERROR("Failed to open request: " + in_url);
					WinHttpCloseHandle(connectionHandle);
					return nullptr;
				}
			}

			if (in_headers.empty() == false)
			{
				std::wstring headerBlob = ConvertHeaders(in_headers);

				if (WinHttpAddRequestHeaders(requestHandle, headerBlob.c_str(), DWORD(headerBlob.length()), WINHTTP_ADDREQ_FLAG_ADD) == false)
				{
					CS_LOG_ERROR("Failed to add http headers: " + in_url);
					WinHttpCloseHandle(requestHandle);
					WinHttpCloseHandle(connectionHandle);
					return nullptr;
				}
			}

			HttpRequest* httpRequest = new HttpRequest(in_type, in_url, in_body, in_headers, in_timeoutSecs, requestHandle, connectionHandle, GetMaxBufferSize(), in_delegate);
			m_requests.push_back(httpRequest);
			return httpRequest;
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::CancelAllRequests()
		{
			CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Http requests can currently only be made on the main thread");

			for(u32 nRequest=0; nRequest<m_requests.size(); ++nRequest)
			{
				m_requests[nRequest]->Cancel();
			}
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		bool HttpRequestSystem::CheckReachability() const
		{
			return true;
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		void HttpRequestSystem::OnUpdate(f32 in_timeSinceLastUpdate)
		{
			//We should do this in two loops incase anyone tries to insert into the requests from the completion callback
			for (u32 i=0; i<m_requests.size(); ++i)
			{
				m_requests[i]->Update(in_timeSinceLastUpdate);
			}

			for (auto it = m_requests.begin(); it != m_requests.end(); /*No increment*/)
			{
				if((*it)->HasCompleted())
				{
					//...and remove the completed request
					CS_SAFEDELETE(*it);
					it = m_requests.erase(it);
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

			HttpRequest::Shutdown();

			for (auto it = m_requests.begin(); it != m_requests.end(); ++it)
			{
				CS_SAFEDELETE(*it);
			}

			m_requests.clear();
			m_requests.shrink_to_fit();

			WinHttpCloseHandle(m_sessionHandle);
		}
	}
}

#endif
