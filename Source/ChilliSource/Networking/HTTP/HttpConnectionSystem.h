/*
 *  HttpConnectionSystem.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 23/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_NETWORKING_HTTPCONNECTIONSYSTEM_H_
#define _MOFLOW_NETWORKING_HTTPCONNECTIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/System/System.h>

#include <functional>

namespace ChilliSource
{
	namespace Networking
    {
		//HTTP Response codes
		const u32 kHTTPResponseOK = 200;
		const u32 kHTTPRedirect = 301;
        const u32 kHTTPMovedTemporarily = 302;
        const u32 kHTTPDuplicate = 304;
        const u32 kHTTPRedirectTemporarily = 307;
        const u32 kHTTPNotFound = 404;
        const u32 kHTTPCASFailure = 409;
        const u32 kHTTPError = 500;
        const u32 kHTTPBusy = 503;
        const f32 kfDefaultHTTPTimeout = 15.0f;
		
		struct HttpRequestDetails
        {
			enum class Type
            {
                k_get,
                k_post
            };
            
			std::string strURL;
			Core::ParamDictionary sHeaders;
			std::string	strBody;
			Type eType;
            
			std::string strRedirectionURL;
		};
		
		class HttpRequest;
        
		class HttpRequest
        {
		public:
            virtual ~HttpRequest(){}
			enum class CompletionResult
			{
                k_completed,    //The request completed and returned a result
                k_failed,       //The request failed
                k_cancelled,    //The request was cancelled
                k_timeout,      //The request timed out
                k_flushed       //The request buffer is full and the buffer needs to be flushed
			};
            
			typedef std::function<void(HttpRequest*, CompletionResult)> CompletionDelegate;
			//----------------------------------------------------------------------------------------
			/// Get Details
			///
			/// @return The original request details (i.e. whether it is post/get the body and header)
			//----------------------------------------------------------------------------------------
			virtual const HttpRequestDetails & GetDetails() const = 0;
			//----------------------------------------------------------------------------------------
			/// Get Completion Delegate
			///
			/// @return The delegate that will be invoked on request complete
			//----------------------------------------------------------------------------------------
			virtual const CompletionDelegate & GetCompletionDelegate() const = 0;
			//----------------------------------------------------------------------------------------
			/// Get Response String
			///
			/// @return The contents of the response as a string. This could be binary data
			//----------------------------------------------------------------------------------------
			virtual const std::string & GetResponseString() const = 0;
			//----------------------------------------------------------------------------------------
			/// Get Response Code
			///
			/// @return HTTP response code (i.e. 200 = OK, 400 = Error)
			//----------------------------------------------------------------------------------------
			virtual u32 GetResponseCode() const = 0;
            //----------------------------------------------------------------------------------------
			/// Get Bytes Read
			///
			/// @return Number of bytes read til now
			//----------------------------------------------------------------------------------------
			virtual u32 GetBytesRead() const = 0;
            //----------------------------------------------------------------------------------------
            /// Cancel
            ///
            /// Cancel the request but do not invoke the completion delegate
            //----------------------------------------------------------------------------------------
            virtual void Cancel() = 0;
		};
		
		
		class HttpConnectionSystem : public ChilliSource::Core::System
        {
		public:
			DECLARE_NAMED_INTERFACE(HttpConnectionSystem);
            
            //--------------------------------------------------------------------------------------------------
			/// Make Request
			///
            /// Causes the system to issue a request with the given details.
            /// @param A HttpRequestDetails struct with valid params per the documentation of HttpRequestDetails
			/// @param (Optional) A function to call when the request is completed. Note that the request can be completed by failure/cancellation as well as success.
            /// @return A pointer to the request. The system owns this pointer. Returns NULL if the request cannot be created.
            //--------------------------------------------------------------------------------------------------
			virtual HttpRequest* MakeRequest(const HttpRequestDetails & insRequestDetails, HttpRequest::CompletionDelegate inOnComplete = HttpRequest::CompletionDelegate()) = 0;
			//--------------------------------------------------------------------------------------------------
			/// Cancel All Requests
			///
            /// Equivalent to calling the above on every incomplete request in progress.
            //--------------------------------------------------------------------------------------------------
			virtual void CancelAllRequests() = 0;
            //--------------------------------------------------------------------------------------------------
            /// Check Reachability
            ///
            /// Checks if the device is internet ready and pings Google for a response.
            ///
            /// @return Success if URL is reachable
            //--------------------------------------------------------------------------------------------------
            virtual bool CheckReachability() const = 0;
            //----------------------------------------------------------------------------------------
            /// Handle Redirection
            ///
            /// Handles redirecting to the given url on receiving a 302
            //----------------------------------------------------------------------------------------
            HttpRequest* HandleRedirection(const HttpRequest* inpRequest);
            //--------------------------------------------------------------------------------------------------
            /// Set Max Buffer Size
            ///
            /// @param The number of bytes read before the buffer is flushed
            //--------------------------------------------------------------------------------------------------
            static void SetMaxBufferSize(u32 inudwSize);
            
        protected:
            
            static u32 mudwMaxBufferSize;
		};
	}
}



#endif