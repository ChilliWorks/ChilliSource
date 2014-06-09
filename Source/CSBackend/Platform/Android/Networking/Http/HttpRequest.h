//
//  HttpRequest.h
//  Chilli Source
//
//  Created by Ian Copland on 08/11/2011.
//  Copyright 2011 Tag Games. All rights reserved.
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
		/// @author I Copland
		//----------------------------------------------------------------------------------------
		class HttpRequest final : public CSNetworking::HttpRequest
		{
		public:
			//----------------------------------------------------------------------------------------
			/// Constructor
			///
			/// @author I Copland
			///
            /// @param Request description
            /// @param Max buffer size before flush required
            /// @param Completion delegate
			//----------------------------------------------------------------------------------------
			HttpRequest(const Desc& in_requestDesc, u32 in_bufferFlushSize, const Delegate& in_delegate);
            //----------------------------------------------------------------------------------------
            /// Close the request. Note: The completion delegate is not invoked
            ///
            /// @author I Copland
            //----------------------------------------------------------------------------------------
            void Cancel() override;
            //----------------------------------------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The original request details (i.e. whether it is post/get the body and header)
            //----------------------------------------------------------------------------------------
            const Desc& GetDescription() const override;
            //----------------------------------------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The contents of the response as a string. This could be binary data
            //----------------------------------------------------------------------------------------
            const std::string& GetResponse() const override;
            //----------------------------------------------------------------------------------------
            /// @author I Copland
            ///
            /// @return HTTP response code (i.e. 200 = OK, 400 = Error)
            //----------------------------------------------------------------------------------------
            u32 GetResponseCode() const override;
            //----------------------------------------------------------------------------------------
            /// @author I Copland
            ///
            /// @return Number of bytes read til now
            //----------------------------------------------------------------------------------------
            u32 GetBytesRead() const override;
            //----------------------------------------------------------------------------------------
            /// Checks the stream to see if any data is available for reading
            /// and reads this into a buffer. Once all the data is read
            /// the request will call the complete delegate
            ///
            /// @author I Copland
            //----------------------------------------------------------------------------------------
            void Update();
            //----------------------------------------------------------------------------------------
            /// @author I Copland
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