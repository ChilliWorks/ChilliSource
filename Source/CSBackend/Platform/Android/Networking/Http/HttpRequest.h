//
//  HttpRequest.h
//  Chilli Source
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
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequest.h>

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
		class HttpRequest final : public CSNetworking::HttpRequest
		{
		public:
			//----------------------------------------------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
            /// @param Request description
            /// @param Max buffer size before flush required
            /// @param Completion delegate
			//----------------------------------------------------------------------------------------
			HttpRequest(const Desc& in_requestDesc, u32 in_bufferFlushSize, const Delegate& in_delegate);
            //----------------------------------------------------------------------------------------
            /// Close the request. Note: The completion delegate is not invoked
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------------------------------
            void Cancel() override;
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The original request details (i.e. whether it is post/get the body and header)
            //----------------------------------------------------------------------------------------
            const Desc& GetDescription() const override;
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The contents of the response as a string. This could be binary data
            //----------------------------------------------------------------------------------------
            const std::string& GetResponse() const override;
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return HTTP response code (i.e. 200 = OK, 400 = Error)
            //----------------------------------------------------------------------------------------
            u32 GetResponseCode() const override;
            //----------------------------------------------------------------------------------------
            /// Checks the stream to see if any data is available for reading
            /// and reads this into a buffer. Once all the data is read
            /// the request will call the complete delegate
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------------------------------
            void Update();
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether the request has completed - regardless of success or failure
            //----------------------------------------------------------------------------------------
            bool HasCompleted() const;

		private:

			//------------------------------------------------------------------
			/// Perform Request
			///
			/// Sends the request
			//------------------------------------------------------------------
			void PerformRequest();

		private:
			
			Delegate m_completionDelegate;
			Desc m_desc;
			
			std::string m_responseData;
			u32 m_responseCode;
			Result m_requestResult;
			
			bool m_shouldKillThread;
			bool m_isPollingComplete;
			bool m_isRequestComplete;
		};
	}
}

#endif

#endif
