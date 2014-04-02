//
//  HttpRequestSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_NETWORKING_HTTP_HTTPREQUESTSYSTEM_H_
#define _CHILLISOURCE_NETWORKING_HTTP_HTTPREQUESTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Networking/Http/HttpRequest.h>

#include <functional>

namespace ChilliSource
{
	namespace Networking
    {
        //--------------------------------------------------------------------------------------------------
        /// Common response codes
        //--------------------------------------------------------------------------------------------------
        namespace HttpResponseCode
        {
            const u32 k_ok = 200;
            const u32 k_redirect = 301;
            const u32 k_movedTemporarily = 302;
            const u32 k_duplicate = 304;
            const u32 k_redirectTemporarily = 307;
            const u32 k_notFound = 404;
            const u32 k_CASFailure = 409;
            const u32 k_error = 500;
            const u32 k_busy = 503;
        }
        
        //--------------------------------------------------------------------------------------------------
        /// Base class for platform depended http request system. This system is responsible for making
        /// http requests to a url and managing the lifetime of the requests and the connections
        ///
        /// @author S Downie
        //--------------------------------------------------------------------------------------------------
		class HttpConnectionSystem : public Core::AppSystem
        {
		public:
            
			CS_DECLARE_NAMEDTYPE(HttpConnectionSystem);
            
            //--------------------------------------------------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //--------------------------------------------------------------------------------------------------
            static HttpConnectionSystemUPtr Create();
            //--------------------------------------------------------------------------------------------------
            /// Causes the system to issue a request with the given details.
            ///
            /// @author S Downie
            ///
            /// @param A descriptor detailing the request params
			/// @param A function to call when the request is completed. Note that the request can be completed with failure as well as success.
            ///
            /// @return A pointer to the request. The system owns this pointer. Returns nullptr if the request cannot be created.
            //--------------------------------------------------------------------------------------------------
			virtual HttpRequest* MakeRequest(const HttpRequest::Desc& in_requestDesc, const HttpRequest::Delegate& in_delegate) = 0;
			//--------------------------------------------------------------------------------------------------
            /// Equivalent to calling cancel on every incomplete request in progress.
            ///
            /// @author S Downie
            //--------------------------------------------------------------------------------------------------
			virtual void CancelAllRequests() = 0;
            //--------------------------------------------------------------------------------------------------
            /// Checks if the device is internet ready
            ///
            /// @author S Downie
            ///
            /// @return Success if net available
            //--------------------------------------------------------------------------------------------------
            virtual bool CheckReachability() const = 0;
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The number of bytes read before the buffer is flushed (0 is infinite)
            //--------------------------------------------------------------------------------------------------
            inline void SetMaxBufferSize(u32 in_sizeInBytes)
            {
                m_maxBufferSize = in_sizeInBytes;
            }
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The number of seconds that will elapse before a request is deemed to have timed out
            //--------------------------------------------------------------------------------------------------
            inline void SetTimeout(f32 in_timeoutSecs)
            {
                m_timeoutSecs = in_timeoutSecs;
            }
            
        protected:
            
            //--------------------------------------------------------------------------------------------------
            /// Private constructor to fore use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------------------------------------------------
            HttpConnectionSystem() = default;
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The number of bytes read before the buffer is flushed
            //--------------------------------------------------------------------------------------------------
            inline u32 GetMaxBufferSize() const
            {
                return m_maxBufferSize;
            }
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The number of seconds that will elapse before a request is deemed to have timed out
            //--------------------------------------------------------------------------------------------------
            inline f32 GetTimeout() const
            {
                return m_timeoutSecs;
            }
            
        private:
            
            u32 m_maxBufferSize = 0;
            f32 m_timeoutSecs = 15.0f;
		};
	}
}



#endif