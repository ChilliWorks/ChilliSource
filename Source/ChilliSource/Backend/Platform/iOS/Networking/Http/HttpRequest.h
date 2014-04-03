//
//  HttpRequest.h
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_HTTP_HTTPREQUEST_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_HTTP_HTTPREQUEST_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequest.h>

#include <CoreFoundation/CoreFoundation.h>

#include <vector>

namespace ChilliSource
{
	namespace iOS
	{
        //----------------------------------------------------------------------------------------
        /// Concrete implementation of iOS http request using the CFNetworking library. Requests
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
            /// @param Timeout in seconds
            /// @param Max buffer size before flush required
            /// @param Completion delegate
            //----------------------------------------------------------------------------------------
            HttpRequest(const Desc& in_requestDesc, u32 in_timeoutSecs, u32 in_bufferFlushSize, const Delegate& in_delegate);
            //----------------------------------------------------------------------------------------
            /// Begin the request
            ///
            /// @author S Downie
            ///
            /// @param Read stream to pull from
            //----------------------------------------------------------------------------------------
            void Start(CFReadStreamRef in_readStream);
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
            /// @param Time since last update in seconds
            //----------------------------------------------------------------------------------------
            void Update(f32 in_timeSinceLastUpdate);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the request has completed - regardless of success or failure
            //----------------------------------------------------------------------------------------
            bool HasCompleted() const;
            
        private:
            
            //----------------------------------------------------------------------------------------
            /// Reads data from the open stream when it becomes available
            /// and buffers it to the request response
            ///
            /// @author S Downie
            ///
            /// @param Read stream
            //----------------------------------------------------------------------------------------
            void PollReadStream(CFReadStreamRef inReadStreamRef);
            //----------------------------------------------------------------------------------------
            /// Blocking call that will poll the read stream until a connection has been
            /// established
            ///
            /// @author S Downie
            ///
            /// @param Read stream
            //----------------------------------------------------------------------------------------
            void BlockingPollForConnection(CFReadStreamRef in_readStream);
            //----------------------------------------------------------------------------------------
            /// Blocking call that will poll the read stream until all the data has been read or an
            /// error occurs
            ///
            /// @author S Downie
            ///
            /// @param Read stream
            /// @param Write stream to fill
            ///
            /// @return Success or failure
            //----------------------------------------------------------------------------------------
            bool BlockingReadData(CFReadStreamRef in_readStream, std::stringstream& out_streamBuffer);
            
        private:
            
            Delegate m_completionDelegate;
            Desc m_desc;
            
            std::string m_responseData;
            
            f32 m_connectingTime;
            const u32 m_timeoutSecs;
            const u32 m_bufferFlushSize;
            
            Result m_result;
            
            u32 m_totalBytesRead;
            u32 m_totalBytesReadThisBlock;
            u32 m_responseCode;
            
            bool m_isConnectionEstablished;
            bool m_isPollingCompleted;
            bool m_isRequestCompleted;
            bool m_shouldKillThread;
        };
	}
    
}
#endif