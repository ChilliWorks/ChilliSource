//
//  HttpRequest.h
//  ChilliSource
//  Created by Ian Copland on 08/11/2011.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUEST_H_
#define _CSBACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUEST_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequest.h>
#include <ChilliSource/Networking/Http/HttpResponse.h>

namespace CSBackend
{
	namespace Android
	{	
		//----------------------------------------------------------------------------------------
		/// Concrete implementation of Android http request using the Java HttpConnection library. Requests
		/// are performed on a background thread.
		///
		/// @author Ian Copland
		//----------------------------------------------------------------------------------------
		class HttpRequest final : public ChilliSource::HttpRequest
		{
		public:
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The type of the request (POST or GET)
            //----------------------------------------------------------------------------------------
            Type GetType() const override;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The original url to which the request was sent
            //----------------------------------------------------------------------------------------
            const std::string& GetUrl() const override;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The body of the POST request (GET request will return empty)
            //----------------------------------------------------------------------------------------
            const std::string& GetBody() const override;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The original headers of the request as keys/values
            //----------------------------------------------------------------------------------------
            const ChilliSource::ParamDictionary& GetHeaders() const override;
            //----------------------------------------------------------------------------------------
            /// @author HMcLaughlin
            ///
            /// @return The expected total size of the request
            //----------------------------------------------------------------------------------------
            u64 GetExpectedSize() const override;
            //----------------------------------------------------------------------------------------
            /// @author HMcLaughlin
            ///
            /// @return The current transferred size of the request
            //----------------------------------------------------------------------------------------
            u64 GetDownloadedBytes() const override;
            //----------------------------------------------------------------------------------------
            /// Close the request. Note: The completion delegate is not invoked
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------------------------------
            void Cancel() override;
			//--------------------------------------------------------------------------------------
			/// Called by Java when the request contents exceed the max buffer size and are flushed.
			/// This is called on the main thread.
			///
			/// @author S Downie
			///
			/// @param in_data - Partial data
			/// @param in_responseCode - Response code
			//--------------------------------------------------------------------------------------
			void OnFlushed(const std::string& in_data, u32 in_responseCode);
			//--------------------------------------------------------------------------------------
			/// Called by Java when the request completes.
			/// This is called on the main thread.
			///
			/// @author HMcLaughlin
			///
			/// @param in_resultCode - Result code
			/// @param in_data - Data
			/// @param in_responseCode - Response code for request
			//--------------------------------------------------------------------------------------
		    void OnComplete(u32 in_resultCode, const std::string& in_data, u32 in_responseCode);

		private:

            friend class HttpRequestSystem;

            //------------------------------------------------------------------
            /// Constructor. Can only be created via HttpRequestSystem
            ///
            /// @author S Downie
            ///
            /// @param Type (POST or GET)
            /// @param Url to send request to
            /// @param POST body
            /// @param Headers
            /// @param Timeout in seconds
            /// @param Max buffer size in bytes
            /// @param Completion delegate
            //------------------------------------------------------------------
            HttpRequest(Type in_type, const std::string& in_url, const std::string& in_body, const ChilliSource::ParamDictionary& in_headers, u32 in_timeoutSecs, u32 in_maxBufferSize, const Delegate& in_delegate);
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether the request has completed - regardless of success or failure
            //----------------------------------------------------------------------------------------
            bool HasCompleted() const;
			//------------------------------------------------------------------
			/// Perform Request
			///
			/// Sends the request
			//------------------------------------------------------------------
			void PerformRequest();

		private:
			
            const Type m_type;
            const std::string m_url;
            const std::string m_body;
            const ChilliSource::ParamDictionary m_headers;
			const Delegate m_completionDelegate;
			const u32 m_timeoutSecs;
			const u32 m_maxBufferSize;
			
			bool m_shouldKillThread = false;
			bool m_isPollingComplete = false;
			bool m_isRequestComplete = false;
			bool m_isRequestCancelled = false;

            JavaClassSPtr m_javaHttpRequest;

			std::string m_responseData;
			ChilliSource::HttpResponse::Result m_requestResult = ChilliSource::HttpResponse::Result::k_failed;
			u32 m_responseCode = 0;
		};
	}
}

#endif

#endif
