/*
 *  AndroidHttpConnectionSystem.h
 *  moFlow
 *
 *  Created by Ian Copland on 08/11/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_ANDROID_HTTPCONNECTIONSYSTEM_H_
#define _MOFLO_PLATFORM_ANDROID_HTTPCONNECTIONSYSTEM_H_

#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Networking/HTTP/HttpConnectionSystem.h>
#include <vector>

namespace ChilliSource
{
	namespace Android
	{	
		class HttpConnectionSystem : public Networking::HttpConnectionSystem, public Core::IUpdateable
		{
		public:
			
			//--------------------------------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interace ID
			/// @return Whether object if of argument type
			//--------------------------------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//--------------------------------------------------------------------------------------------------
			/// Make Request 
			///
			/// Causes the system to issue a request with the given details.
			/// @param A HttpRequestDetails struct with valid params per the documentation of HttpRequestDetails
			/// @param (Optional) A function to call when the request is completed. Note that the request can be completed by failure/cancellation as well as success.
			/// @return A pointer to the request. The system owns this pointer. Returns NULL if the request cannot be created.
			//--------------------------------------------------------------------------------------------------
            Networking::HttpRequest* MakeRequest(const Networking::HttpRequestDetails & insRequestDetails, Networking::HttpRequest::CompletionDelegate inOnComplete = Networking::HttpRequest::CompletionDelegate()) override;
			//--------------------------------------------------------------------------------------------------
			/// Cancel All Requests
			///
            /// Equivalent to calling the above on every incomplete request in progress.
            //--------------------------------------------------------------------------------------------------
            void CancelAllRequests() override;
            //--------------------------------------------------------------------------------------------------
            /// Check Reachability
            ///
            /// Checks if the device is internet ready and pings Google for a response.
            ///
            /// @return Success if URL is reachable
            //--------------------------------------------------------------------------------------------------
            bool CheckReachability() const override;
			//--------------------------------------------------------------------------------------------------
			/// Update
			///
			/// For all active requests in the system call update on them allowing them to check the status
			/// of thier requests
			///
			/// @param Time between frames
			//--------------------------------------------------------------------------------------------------
            void Update(f32 infDT);
			
		private:
			
			class HttpRequest : public Networking::HttpRequest
            {
			public:
				HttpRequest(const Networking::HttpRequestDetails & insDetails, const Networking::HttpRequest::CompletionDelegate & inCompletionDelegate);

				//------------------------------------------------------------------
				/// Update
				///
				/// Checks the stream to see if any data is available for reading
				/// and reads this into a buffer. Once all the data is read
				/// the request will call the complete delegate
				///
				/// @param Time since last frame
				//------------------------------------------------------------------
				void Update(f32 infDT);
				//----------------------------------------------------------------------------------------
				/// Cancel
				///
				/// Close the request and invoke the completion delegate with the cancel response
				//----------------------------------------------------------------------------------------
				void Cancel();
				//----------------------------------------------------------------------------------------
				/// Has Completed
				///
				/// @return Whether the request has completed - regardless of success or failure
				//----------------------------------------------------------------------------------------
				bool HasCompleted() const;
				//----------------------------------------------------------------------------------------
				/// Get Details
				///
				/// @return The original request details (i.e. whether it is post/get the body and header)
				//----------------------------------------------------------------------------------------
                const Networking::HttpRequestDetails & GetDetails() const;
				//----------------------------------------------------------------------------------------
				/// Get Completion Delegate
				///
				/// @return The delegate that will be invoked on request complete
				//----------------------------------------------------------------------------------------
                const Networking::HttpRequest::CompletionDelegate & GetCompletionDelegate() const;
				//----------------------------------------------------------------------------------------
				/// Get Response String
				///
				/// @return The contents of the response as a string. This could be binary data
				//----------------------------------------------------------------------------------------
                const std::string & GetResponseString() const;
				//----------------------------------------------------------------------------------------
				/// Get Response Code
				///
				/// @return HTTP response code (i.e. 200 = OK, 400 = Error)
				//----------------------------------------------------------------------------------------
                u32 GetResponseCode() const;
                //----------------------------------------------------------------------------------------
    			/// Get Bytes Read
    			///
    			/// @return Number of bytes read til now
    			//----------------------------------------------------------------------------------------
    			u32 GetBytesRead() const;
			private:
				
				//------------------------------------------------------------------
				/// Perform Request
				///
				/// Sends the request
				//------------------------------------------------------------------
				void PerformRequest();
				
			private:
				
				Networking::HttpRequest::CompletionDelegate mCompletionDelegate;
                Networking::HttpRequestDetails msDetails;
				
                //Shared resources between thread and main
				std::string mResponseData;
				u32 mudwResponseCode;
                CompletionResult meRequestResult;
				volatile bool mbCompleted;
                volatile bool mbThreadCompleted;
                
                bool mbRequestCompleted;
			};
			
			typedef std::vector<HttpRequest*> RequestVector;
			RequestVector mapRequests;
		};
	}

}
#endif
