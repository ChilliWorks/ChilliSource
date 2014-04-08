//
//  OAuth.h
//  Chilli Source
//
//  Created by I Copland on 08/04/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
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
