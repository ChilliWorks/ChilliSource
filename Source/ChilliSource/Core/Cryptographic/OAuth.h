//
//  OAuth.h
//  Chilli Source
//  Created by Ian Copland on 08/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_CRYPTOGRAPHIC_OAUTH_H_
#define _CHILLISOURCE_CORE_CRYPTOGRAPHIC_OAUTH_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------
        /// A collection of functions for working with OAuth authentication.
        ///
        /// @author R Henning
        //------------------------------------------------------------------
        namespace OAuth
        {
            //-----------------------------------------------------------
            /// An enum describing the various request types.
            ///
            /// @author R Henning
            //-----------------------------------------------------------
            enum class RequestType
			{
			    k_httpInvalid,
			    k_httpGet,
			    k_httpPost,
			    k_httpDelete
			};
            //-----------------------------------------------------------
            /// Generates an OAuth authorisation header string.
            ///
            /// @author R Henning
            ///
            /// @param The http request type.
            /// @param The URL.
            /// @param The Http data.
            /// @param The consumer key.
            /// @param The consumer secret.
            /// @param [Optional] The oauth token.
            /// @param [Optional] The oauth token secret.
            /// @param [Optional] The oauth verifier.
            ///
            /// @return The generated header string. If this failed an
            /// empty string will be returned.
            //-----------------------------------------------------------
            std::string GenerateAuthorisationHeader(RequestType in_requestType, const std::string& in_url, const std::string& in_rawData, const std::string& in_consumerKey, const std::string& in_consumerSecret,
                                                    const std::string& in_oauthToken = "", const std::string& in_oauthTokenSecret = "", const std::string& in_oauthVerifier = "");
        }
    }
}

#endif
