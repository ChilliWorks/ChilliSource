//
//  HttpRequest.h
//  Chilli Source
//  Created by Scott Downie on 23/05/2011.
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
        /// via the HttpRequestSystem to a given URL. Requests can be of type POST or GET. The
        /// request object can be used to cancel the request
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
            /// Delegate called when the request completes (either with success of failure)
            ///
            /// @param Original request
            /// @param Request response
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
			typedef std::function<void(const HttpRequest*, const HttpResponse&)> Delegate;
			//----------------------------------------------------------------------------------------
			/// Constructor
			///
			/// @author S Downie
			//----------------------------------------------------------------------------------------
			HttpRequest() = default;
			//----------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The type of the request (POST or GET)
			//----------------------------------------------------------------------------------------
			virtual Type GetType() const = 0;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The original url to which the request was sent
            //----------------------------------------------------------------------------------------
            virtual const std::string& GetUrl() const = 0;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The body of the POST request (GET request will return empty)
            //----------------------------------------------------------------------------------------
            virtual const std::string& GetBody() const = 0;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The original headers of the request as keys/values
            //----------------------------------------------------------------------------------------
            virtual const Core::ParamDictionary& GetHeaders() const = 0;
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
