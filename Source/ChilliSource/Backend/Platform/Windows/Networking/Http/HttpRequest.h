//
//  HttpRequest.h
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2012 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_HTTP_HTTPREQUEST_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_HTTP_HTTPREQUEST_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequest.h>

#include <list>
#include <mutex>

namespace ChilliSource
{
	namespace Windows
	{
		//Windows typedefs so we don't have to include windows.h in the header
		typedef void* HINTERNET;

		//----------------------------------------------------------------------------------------
		/// Concrete implementation of Windows http request using the WinHTTP library. Requests
		/// are performed on a background thread.
		///
		/// @author S Downie
		//----------------------------------------------------------------------------------------
		class HttpRequest final : public Networking::HttpRequest
		{
		public:
			//----------------------------------------------------------------------------------------
			/// Constructor
			///
			/// @author S Downie
			///
			/// @param Request description
			/// @param WinHTTP request handle
			/// @param WinHTTP connection handle
			/// @param Max buffer size before flush required
			/// @param Completion delegate
			//----------------------------------------------------------------------------------------
			HttpRequest(const Desc& in_requestDesc, HINTERNET in_requestHandle, HINTERNET in_connectionHandle, u32 in_bufferFlushSize, const Delegate& in_delegate);
			//----------------------------------------------------------------------------------------
			/// Close the request. Note: The completion delegate is not invoked
			///
			/// @author S Downie
			//----------------------------------------------------------------------------------------
			void Cancel() override;
			//----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The original request details (i.e. whether it is post/get the body and header)
			//----------------------------------------------------------------------------------------
			const Desc& GetDescription() const override;
			//----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The contents of the response as a string. This could be binary data
			//----------------------------------------------------------------------------------------
			const std::string& GetResponse() const override;
			//----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return HTTP response code (i.e. 200 = OK, 400 = Error)
			//----------------------------------------------------------------------------------------
			u32 GetResponseCode() const override;
			//----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Number of bytes read til now
			//----------------------------------------------------------------------------------------
			u32 GetBytesRead() const override;
			//----------------------------------------------------------------------------------------
			/// Checks the stream to see if any data is available for reading
			/// and reads this into a buffer. Once all the data is read
			/// the request will call the complete delegate
			///
			/// @author S Downie
			///
			/// @param Time in seconds since last update
			//----------------------------------------------------------------------------------------
			void Update(f32 in_timeSinceLastUpdate);
			//----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the request has completed - regardless of success or failure
			//----------------------------------------------------------------------------------------
			bool HasCompleted() const;
			//----------------------------------------------------------------------------------------
			/// Inform the polling threads in a thread-safe manner that the system is shutting down
			///
			/// @author S Downie
			//----------------------------------------------------------------------------------------
			static void Shutdown();

		private:
			//----------------------------------------------------------------------------------------
			/// Reads data from the open stream when it becomes available
			/// buffers the data flags on complete
			///
			/// @author S Downie
			///
			/// @param Request handle
			/// @param Connection handle
			/// @param Mutex that manages the critical section of the HTTP system being shutdown
			//----------------------------------------------------------------------------------------
			void PollReadStream(HINTERNET inRequestHandle, HINTERNET inConnectionHandle, std::shared_ptr<std::mutex> in_destroyingMutex);

		private:

			static std::mutex s_addingMutexesMutex;
			static std::list<std::shared_ptr<std::mutex>> s_destroyingMutexes;
			static bool s_isDestroying;

			Delegate m_completionDelegate;
			Desc m_desc;

			std::string m_responseData;
			u32 m_responseCode;

			Result m_requestResult;

			u32 m_totalBytesRead;
			u32 m_bufferFlushSize;

			bool m_shouldKillThread;
			bool m_isPollingComplete;
			bool m_isRequestComplete;
		};
	}
}

#endif

#endif
