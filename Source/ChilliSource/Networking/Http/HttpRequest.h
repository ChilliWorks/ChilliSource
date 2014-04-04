//
//  HttpRequest.h
//  Chilli Source
//
//  Created by Scott Downie on 23/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_NETWORKING_HTTP_HTTPREQUEST_H_
#define _CHILLISOURCE_NETWORKING_HTTP_HTTPREQUEST_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

#include <functional>

namespace ChilliSource
{
	namespace Networking
    {
        //----------------------------------------------------------------------------------------
        /// Interface class to a platform dependent http request. A request can be issued
        /// via the HttpRequestSystem. Requests can be of type post or get and will store
        /// received data from the http connection. Requests are performed on a background thread.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
		class HttpRequest
        {
		public:
            
            CS_DECLARE_NOCOPY(HttpRequest);
            
            //----------------------------------------------------------------------------------------
            /// Type of the Http request (Post or Get)
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            enum class Type
            {
                k_get,
                k_post
            };
            //----------------------------------------------------------------------------------------
            /// Result of the http request
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            enum class Result
			{
                k_completed,    //The request completed and returned a result
                k_failed,       //The request failed
                k_cancelled,    //The request was cancelled
                k_timeout,      //The request timed out
                k_flushed       //The request buffer is full and the buffer needs to be flushed
			};
            //----------------------------------------------------------------------------------------
            /// Holds the description of the request
            /// including the type, url and headers
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            struct Desc
            {
                Core::ParamDictionary m_headers;
                std::string m_url;
                std::string	m_body;
                std::string m_redirectionUrl;
                Type m_type;
            };
            //----------------------------------------------------------------------------------------
            /// Delegate called when the request completes (either with success of failure)
            ///
            /// @param Request
            /// @param Result
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
			typedef std::function<void(HttpRequest*, Result)> Delegate;
			//----------------------------------------------------------------------------------------
			/// Constructor
			///
			/// @author S Downie
			//----------------------------------------------------------------------------------------
			HttpRequest() = default;
			//----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The original request description (i.e. whether it is post/get the body and header)
			//----------------------------------------------------------------------------------------
			virtual const Desc& GetDescription() const = 0;
			//----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The contents of the response as a string. (This could be binary data)
			//----------------------------------------------------------------------------------------
			virtual const std::string& GetResponse() const = 0;
			//----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return HTTP response code (i.e. 200 = OK, 400 = Error).
			//----------------------------------------------------------------------------------------
			virtual u32 GetResponseCode() const = 0;
            //----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Number of bytes read til now
			//----------------------------------------------------------------------------------------
			virtual u32 GetBytesRead() const = 0;
            //----------------------------------------------------------------------------------------
            /// Cancel the request. Does not invoke the completion delegate
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            virtual void Cancel() = 0;
            //----------------------------------------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            virtual ~HttpRequest(){}
		};
	}
}



#endif
