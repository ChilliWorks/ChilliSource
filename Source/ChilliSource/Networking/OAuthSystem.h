/*
 *  OAuthSystem.h
 *  moFlow
 *
 *  Created by Robert Henning on 01/03/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_NETWORKING_OAUTHSYSTEM_H_
#define _MOFLOW_NETWORKING_OAUTHSYSTEM_H_

#include <ChilliSource/Core/BaseEncoding.h>
#include <ChilliSource/Core/System.h>

#include <ChilliSource/Networking/ForwardDeclarations.h>

#include "time.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>

namespace moFlo
{
	namespace Networking
	{
		const u32 OAUTHLIB_BUFFSIZE										= 1024;
		const u32 OAUTHLIB_BUFFSIZE_LARGE 								= 1024;
		const std::string OAUTHLIB_CONSUMERKEY_KEY 						= "oauth_consumer_key";
		const std::string OAUTHLIB_CALLBACK_KEY							= "oauth_callback";
		const std::string OAUTHLIB_VERSION_KEY							= "oauth_version";
		const std::string OAUTHLIB_SIGNATUREMETHOD_KEY					= "oauth_signature_method";
		const std::string OAUTHLIB_SIGNATURE_KEY						= "oauth_signature";
		const std::string OAUTHLIB_TIMESTAMP_KEY						= "oauth_timestamp";
		const std::string OAUTHLIB_NONCE_KEY							= "oauth_nonce";
		const std::string OAUTHLIB_TOKEN_KEY							= "oauth_token";
		const std::string OAUTHLIB_TOKENSECRET_KEY						= "oauth_token_secret";
		const std::string OAUTHLIB_VERIFIER_KEY							= "oauth_verifier";
		const std::string OAUTHLIB_SCREENNAME_KEY						= "screen_name";
		const std::string OAUTHLIB_AUTHENTICITY_TOKEN_KEY				= "authenticity_token";
		const std::string OAUTHLIB_SESSIONUSERNAME_KEY					= "session[username_or_email]";
		const std::string OAUTHLIB_SESSIONPASSWORD_KEY					= "session[password]";
		const std::string OAUTHLIB_AUTHENTICITY_TOKEN_TWITTER_RESP_KEY	= "authenticity_token\" type=\"hidden\" value=\"";
		const std::string OAUTHLIB_TOKEN_TWITTER_RESP_KEY				= "oauth_token\" type=\"hidden\" value=\"";
		const std::string OAUTHLIB_PIN_TWITTER_RESP_KEY					= "code-desc\"><code>";
		const std::string OAUTHLIB_TOKEN_END_TAG_TWITTER_RESP			= "\" />";
		const std::string OAUTHLIB_PIN_END_TAG_TWITTER_RESP				= "</code>";
		const std::string OAUTHLIB_AUTHHEADER_STRING					= "OAuth ";

		class COAuthSystem : public Core::ISystem
		{
		public:

			DECLARE_NAMED_INTERFACE(COAuthSystem);

			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;

			typedef enum OAuthHttpRequestType
			{
			    eOAuthHttpInvalid = 0,
			    eOAuthHttpGet,
			    eOAuthHttpPost,
			    eOAuthHttpDelete
			} eOAuthHttpRequestType;

			typedef std::list<std::string> OAuthKeyValueList;
			typedef std::map<std::string, std::string> OAuthKeyValuePairsMap;

			COAuthSystem();
			~COAuthSystem();
            
            static std::string GenerateNonceTimeStamp();

		    /* OAuth public methods used by twitCurl */
		    void GetConsumerKey(std::string& outstrConsumerKey);
		    void SetConsumerKey(const std::string& instrConsumerKey);

		    void GetConsumerSecret(std::string& outstrConsumerSecret);
		    void SetConsumerSecret(const std::string& instrConsumerSecret);

		    void GetOAuthTokenKey(std::string& outstrOAuthTokenKey);
		    void SetOAuthTokenKey(const std::string& instrOAuthTokenKey);

		    void GetOAuthTokenSecret(std::string& outstrOAuthTokenSecret);
		    void SetOAuthTokenSecret(const std::string& instrOAuthTokenSecret);

		    void GetOAuthScreenName(std::string& outstrOAuthScreenName);
		    void SetOAuthScreenName(const std::string& instrAuthScreenName);

		    void GetOAuthPin(std::string& outstrOAuthPin);
		    void SetOAuthPin(const std::string& instrAuthPin);

		    bool GetOAuthHeader(const eOAuthHttpRequestType ineType,
		                        const std::string& instrRawUrl,
		                        const std::string& instrRawData,
		                        std::string& outstrOAuthHttpHeader,
		                        const bool inbIncludeOAuthVerifierPin = false);

		    bool ExtractOAuthTokenKeySecret(const std::string& instrRequestTokenResponse);
		private:

		    /* OAuth data */
		    std::string mstrConsumerKey;
		    std::string mstrConsumerSecret;
		    std::string mstrOAuthTokenKey;
		    std::string mstrOAuthTokenSecret;
		    std::string mstrOAuthPIN;
		    std::string mstrNOnce;
		    std::string mstrTimeStamp;
		    std::string mstrOAuthScreenName;

		    /* OAuth twitter related utility methods */
		    bool BuildOAuthTokenKeyValuePairs(const bool inbIncludeOAuthVerifierPin,
		                                      const std::string& instrRawData,
		                                      const std::string& instrOAuthSignature,
		                                      OAuthKeyValuePairsMap& outmapKeyValueMap,
		                                      const bool inbGenerateTimestamp);

		    bool GetStringFromOAuthKeyValuePairs(const OAuthKeyValuePairsMap& inmapRawParamMap,
		                                         std::string& outstrRawParams,
		                                         const std::string& instrParamsSeperator);

		    bool GetSignature(const eOAuthHttpRequestType ineType,
		                      const std::string& instrRawUrl,
		                      const OAuthKeyValuePairsMap& inmapRawKeyValuePairs,
		                      std::string& outstrOAuthSignature);
            
            static std::string GenerateNonceTimeStamp(TimeIntervalSecs TimeStamp);
		};
	}
}

#endif

