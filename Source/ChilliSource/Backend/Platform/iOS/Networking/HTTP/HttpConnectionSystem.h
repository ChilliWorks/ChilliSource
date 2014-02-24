/*
 *  iOSHttpConnectionSystem.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 23/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_IOS_HTTPCONNECTIONSYSTEM_H_
#define _MOFLO_PLATFORM_IOS_HTTPCONNECTIONSYSTEM_H_

#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Networking/Http/HttpConnectionSystem.h>
#include <vector>
#import <Foundation/Foundation.h>

namespace ChilliSource
{
	namespace iOS
	{
		class CHttpConnectionSystem : public Networking::IHttpConnectionSystem, public Core::IUpdateable
		{
		public:
			
			//--------------------------------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interace ID
			/// @return Whether object if of argument type
			//--------------------------------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//--------------------------------------------------------------------------------------------------
			/// Make Request
			///
			/// Causes the system to issue a request with the given details.
			/// @param A HttpRequestDetails struct with valid params per the documentation of HttpRequestDetails
			/// @param (Optional) A function to call when the request is completed. Note that the request can be completed by failure/cancellation as well as success.
			/// @return A pointer to the request. The system owns this pointer. Returns NULL if the request cannot be created.
			//--------------------------------------------------------------------------------------------------
            Networking::HttpRequestPtr MakeRequest(const Networking::HttpRequestDetails & insRequestDetails, Networking::IHttpRequest::CompletionDelegate inOnComplete = Networking::IHttpRequest::CompletionDelegate());
			//--------------------------------------------------------------------------------------------------
			/// Cancel All Requests
			///
            /// Equivalent to calling the above on every incomplete request in progress.
            //--------------------------------------------------------------------------------------------------
            void CancelAllRequests();
            //--------------------------------------------------------------------------------------------------
            /// Check Reachability
            ///
            /// Checks if the device is internet ready and pings Google for a response.
            ///
            /// @return Success if URL is reachable
            //--------------------------------------------------------------------------------------------------
            bool CheckReachability() const;
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
            
            //--------------------------------------------------------------------------------------------------
            /// Apply SSL Settings
            ///
            /// Setup the read stream to use SSL level 3
            ///
            /// @param Read stream
            //--------------------------------------------------------------------------------------------------
            void ApplySSLSettings(CFReadStreamRef inReadStreamRef) const;
            //--------------------------------------------------------------------------------------------------
            /// Is Stream Open
            ///
            /// Is the read stream still open or has it errored or closed
            ///
            /// @param Read stream
            /// @return Whether it is open
            //--------------------------------------------------------------------------------------------------
            bool IsStreamOpen(CFReadStreamRef inReadStreamRef) const;
            //--------------------------------------------------------------------------------------------------
            /// Log Connection Address
            ///
            /// Log the unique TCP connection established by the read stream. This is for debug purposes only
            ///
            /// @param Read stream
            //--------------------------------------------------------------------------------------------------
            void LogConnectionAddress(CFReadStreamRef inReadStreamRef) const;
            
        private:
            
            typedef u32 ConnectionID;
            struct ConnectionInfo
            {
                TimeIntervalSecs ConnectionOpenTime;
                ConnectionID PropertyID;
                ConnectionID StreamID;
                CFReadStreamRef ReadStream;
            };
            
        private:
			
			class CHttpRequest : public Networking::IHttpRequest
            {
			public:
				CHttpRequest(const Networking::HttpRequestDetails & insDetails, const CHttpConnectionSystem::ConnectionInfo& insConnectionInfo, const Networking::IHttpRequest::CompletionDelegate & inCompletionDelegate);
                
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
                const Networking::IHttpRequest::CompletionDelegate & GetCompletionDelegate() const;
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
                //----------------------------------------------------------------------------------------
                /// Get Connection Info
                ///
                /// @return Connection Info
                //----------------------------------------------------------------------------------------
                const ConnectionInfo& GetConnectionInfo();
				
			private:
				
				//----------------------------------------------------------------------------------------
				/// Poll Read Stream
				///
				/// Reads data from the open stream when it becomes available
				/// buffers the data flags on complete
				//----------------------------------------------------------------------------------------
				void PollReadStream(CFReadStreamRef inReadStreamRef);
				
			private:
				
				Networking::IHttpRequest::CompletionDelegate mCompletionDelegate;
                Networking::HttpRequestDetails msDetails;
                CHttpConnectionSystem::ConnectionInfo mConnectionInfo;
                
                f32 mfActiveTime;
                
                //Shared resources between thread and main
				std::string mResponseData;
				u32 mudwResponseCode;
                
                CompletionResult meRequestResult;
                
                volatile u32 mudwBytesRead;
                volatile u32 mudwBytesReadThisBlock;
				volatile bool mbCompleted;
				volatile bool mbReceivedResponse;
                volatile bool mbThreadCompleted;
                
                bool mbRequestCompleted;
			};
			
            typedef DYNAMIC_ARRAY<ConnectionInfo> ConnectionPool;
            ConnectionPool mPersistentConnectionPool;
            
			typedef DYNAMIC_ARRAY<CHttpRequest*> RequestVector;
			RequestVector mapRequests;
            
            static u32 udwStaticNumConnectionsEstablished;
		};
	}
    
}
#endif