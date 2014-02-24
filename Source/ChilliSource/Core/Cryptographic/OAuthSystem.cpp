/*
 *  OAuthSystem.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 01/03/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Cryptographic/HMAC_SHA1.h>
#include <ChilliSource/Core/Cryptographic/OAuthSystem.h>

namespace ChilliSource
{
	namespace Networking
	{
		DEFINE_NAMED_INTERFACE(COAuthSystem);
        
        static u32 udwNonceCounter = 0;

		bool COAuthSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == COAuthSystem::InterfaceID;
		}

		/*++
		* @method: COAuthSystem::oAuth
		*
		* @description: constructor
		*
		* @input: none
		*
		* @output: none
		*
		*--*/
		COAuthSystem::COAuthSystem()
		{
		}

		/*++
		* @method: COAuthSystem::~oAuth
		*
		* @description: destructor
		*
		* @input: none
		*
		* @output: none
		*
		*--*/
		COAuthSystem::~COAuthSystem()
		{
		}

		/*++
		* @method: COAuthSystem::GetConsumerKey
		*
		* @description: this method gives consumer key that is being used currently
		*
		* @input: none
		*
		* @output: consumer key
		*
		*--*/
		void COAuthSystem::GetConsumerKey(std::string& outstrConsumerKey)
		{
			outstrConsumerKey = mstrConsumerKey;
		}

		/*++
		* @method: COAuthSystem::SetConsumerKey
		*
		* @description: this method saves consumer key that should be used
		*
		* @input: consumer key
		*
		* @output: none
		*
		*--*/
		void COAuthSystem::SetConsumerKey(const std::string& instrConsumerKey)
		{
		    mstrConsumerKey.assign(CBaseEncoding::URLEncode(instrConsumerKey));
		}

		/*++
		* @method: COAuthSystem::GetConsumerSecret
		*
		* @description: this method gives consumer secret that is being used currently
		*
		* @input: none
		*
		* @output: consumer secret
		*
		*--*/
		void COAuthSystem::GetConsumerSecret(std::string& outstrConsumerSecret)
		{
			outstrConsumerSecret = mstrConsumerSecret;
		}

		/*++
		* @method: COAuthSystem::SetConsumerSecret
		*
		* @description: this method saves consumer secret that should be used
		*
		* @input: consumer secret
		*
		* @output: none
		*
		*--*/
		void COAuthSystem::SetConsumerSecret(const std::string& instrConsumerSecret)
		{
		    mstrConsumerSecret = CBaseEncoding::URLEncode(instrConsumerSecret);
		}

		/*++
		* @method: COAuthSystem::GetOAuthTokenKey
		*
		* @description: this method gives OAuth token (also called access token) that is being used currently
		*
		* @input: none
		*
		* @output: OAuth token
		*
		*--*/
		void COAuthSystem::GetOAuthTokenKey(std::string& outstrOAuthTokenKey)
		{
			outstrOAuthTokenKey = mstrOAuthTokenKey;
		}

		/*++
		* @method: COAuthSystem::SetOAuthTokenKey
		*
		* @description: this method saves OAuth token that should be used
		*
		* @input: OAuth token
		*
		* @output: none
		*
		*--*/
		void COAuthSystem::SetOAuthTokenKey(const std::string& instrOAuthTokenKey)
		{
		    mstrOAuthTokenKey = CBaseEncoding::URLEncode(instrOAuthTokenKey);
		}

		/*++
		* @method: COAuthSystem::GetOAuthTokenSecret
		*
		* @description: this method gives OAuth token secret that is being used currently
		*
		* @input: none
		*
		* @output: OAuth token secret
		*
		*--*/
		void COAuthSystem::GetOAuthTokenSecret(std::string& outstrOAuthTokenSecret)
		{
			outstrOAuthTokenSecret = mstrOAuthTokenSecret;
		}

		/*++
		* @method: COAuthSystem::SetOAuthTokenSecret
		*
		* @description: this method saves OAuth token that should be used
		*
		* @input: OAuth token secret
		*
		* @output: none
		*
		*--*/
		void COAuthSystem::SetOAuthTokenSecret(const std::string& instrOAuthTokenSecret)
		{
		    mstrOAuthTokenSecret = CBaseEncoding::URLEncode(instrOAuthTokenSecret);
		}

		/*++
		* @method: COAuthSystem::GetOAuthScreenName
		*
		* @description: this method gives authorized user's screenname
		*
		* @input: none
		*
		* @output: screen name
		*
		*--*/
		void COAuthSystem::GetOAuthScreenName(std::string& outstrOAuthScreenName)
		{
			outstrOAuthScreenName = mstrOAuthScreenName;
		}

		/*++
		* @method: COAuthSystem::SetOAuthScreenName
		*
		* @description: this method sets authorized user's screenname
		*
		* @input: screen name
		*
		* @output: none
		*
		*--*/
		void COAuthSystem::SetOAuthScreenName(const std::string& instrAuthScreenName)
		{
		    mstrOAuthScreenName = instrAuthScreenName;
		}

		/*++
		* @method: COAuthSystem::GetOAuthPin
		*
		* @description: this method gives OAuth verifier PIN
		*
		* @input: none
		*
		* @output: OAuth verifier PIN
		*
		*--*/
		void COAuthSystem::GetOAuthPin(std::string& outstrOAuthPin)
		{
			outstrOAuthPin = mstrOAuthPIN;
		}

		/*++
		* @method: COAuthSystem::SetOAuthPin
		*
		* @description: this method sets OAuth verifier PIN
		*
		* @input: OAuth verifier PIN
		*
		* @output: none
		*
		*--*/
		void COAuthSystem::SetOAuthPin(const std::string& instrAuthPin)
		{
		    mstrOAuthPIN = instrAuthPin;
		}

		/*++
		* @method: COAuthSystem::GenerateNonceTimeStamp
		*
		* @description: this method generates nonce and timestamp for OAuth header
		*
		* @input: none
		*
		* @output: @output: Nonce timestamp
		*
		* @remarks: internal method
		*
		*--*/
        std::string COAuthSystem::GenerateNonceTimeStamp()
		{
            return GenerateNonceTimeStamp((TimeIntervalSecs)time(NULL));
		}
		/*++
         * @method: COAuthSystem::GenerateNonceTimeStamp
         *
         * @description: this method generates nonce and timestamp for OAuth header
         *
         * @input: Timestampe
         *
         * @output: Nonce timestamp
         *
         * @remarks: internal method
         *
         *--*/
        std::string COAuthSystem::GenerateNonceTimeStamp(TimeIntervalSecs TimeStamp)
		{
            std::string strTime = Core::CStringConverter::ToString(TimeStamp);
            std::string strCounter = Core::CStringConverter::ToString(udwNonceCounter);
            
            udwNonceCounter++;
            return strTime + strCounter;
		}
        
		/*++
		* @method: COAuthSystem::BuildOAuthTokenKeyValuePairs
		*
		* @description: this method prepares key-value pairs required for OAuth header
		*               and signature generation.
		*
		* @input: inbIncludeOAuthVerifierPin - flag to indicate whether oauth_verifer key-value
		*                                   pair needs to be included. oauth_verifer is only
		*                                   used during exchanging request token with access token.
		*         instrRawData - url encoded data. this is used during signature generation.
		*         instrOAuthSignature - base64 and url encoded OAuth signature.
		*         inbGenerateTimestamp - If true, then generate new timestamp for nonce.
		*
		* @output: outmapKeyValueMap - map in which key-value pairs are populated
		*
		* @remarks: internal method
		*
		*--*/
		bool COAuthSystem::BuildOAuthTokenKeyValuePairs(const bool inbIncludeOAuthVerifierPin,
														const std::string& instrRawData,
														const std::string& instrOAuthSignature,
														OAuthKeyValuePairsMap& outmapKeyValueMap,
														const bool inbGenerateTimestamp)
		{
		    /* Generate nonce and timestamp if required */
		    if(inbGenerateTimestamp)
		    {
                TimeIntervalSecs Timestamp = (TimeIntervalSecs)time(NULL);
                mstrNOnce = GenerateNonceTimeStamp(Timestamp);
		        mstrTimeStamp = Core::CStringConverter::ToString(Timestamp);
		    }

		    /* Consumer key and its value */
		    outmapKeyValueMap[OAUTHLIB_CONSUMERKEY_KEY] = mstrConsumerKey;

		    /* Nonce key and its value */
		    outmapKeyValueMap[OAUTHLIB_NONCE_KEY] = mstrNOnce;

		    /* Signature if supplied */
		    if(instrOAuthSignature.length())
		    {
		    	outmapKeyValueMap[OAUTHLIB_SIGNATURE_KEY] = instrOAuthSignature;
		    }

		    /* Signature method, only HMAC-SHA1 as of now */
		    outmapKeyValueMap[OAUTHLIB_SIGNATUREMETHOD_KEY] = std::string("HMAC-SHA1");

		    /* Timestamp */
		    outmapKeyValueMap[OAUTHLIB_TIMESTAMP_KEY] = mstrTimeStamp;

		    /* Token */
		    if(mstrOAuthTokenKey.length())
		    {
		    	outmapKeyValueMap[OAUTHLIB_TOKEN_KEY] = mstrOAuthTokenKey;
		    }

		    /* Verifier */
		    if(inbIncludeOAuthVerifierPin && mstrOAuthPIN.length())
		    {
		    	outmapKeyValueMap[OAUTHLIB_VERIFIER_KEY] = mstrOAuthPIN;
		    }

		    /* Version */
		    outmapKeyValueMap[OAUTHLIB_VERSION_KEY] = std::string("1.0");

		    /* Data if it's present */
		    if(instrRawData.length())
		    {
		        /* Data should already be urlencoded once */
		        std::string strDummyStrKey;
		        std::string strDummyStrValue;
		        size_t dwPos = instrRawData.find_first_of("=");
		        if(std::string::npos != dwPos)
		        {
		            strDummyStrKey = instrRawData.substr(0, dwPos);
		            strDummyStrValue = instrRawData.substr(dwPos + 1);
		            outmapKeyValueMap[strDummyStrKey] = strDummyStrValue;
		        }
		    }

		    return (outmapKeyValueMap.size()) ? true : false;
		}

		/*++
		* @method: COAuthSystem::GetSignature
		*
		* @description: this method calculates HMAC-SHA1 signature of OAuth header
		*
		* @input: ineType - HTTP request type
		*         instrRawUrl - raw url of the HTTP request
		*         inmapRawKeyValuePairs - key-value pairs containing OAuth headers and HTTP data
		*
		* @output: outstrOAuthSignature - base64 and url encoded signature
		*
		* @remarks: internal method
		*
		*--*/
		bool COAuthSystem::GetSignature(const OAuthHttpRequestType ineType,
										const std::string& instrRawUrl,
										const OAuthKeyValuePairsMap& inmapRawKeyValuePairs,
										std::string& outstrOAuthSignature)
		{
		    std::string strRawParams;
		    std::string strParamsSeperator;
		    std::string strSigBase;

		    /* Initially empty signature */
		    outstrOAuthSignature.assign("");

		    /* Build a string using key-value pairs */
		    strParamsSeperator = "&";
		    GetStringFromOAuthKeyValuePairs(inmapRawKeyValuePairs, strRawParams, strParamsSeperator);

		    /* Start constructing base signature string. Refer http://dev.twitter.com/auth#intro */
		    switch(ineType)
		    {
                case OAuthHttpRequestType::k_httpGet:
		        {
		        	strSigBase.assign("GET&");
		        }
		        break;

		    case OAuthHttpRequestType::k_httpPost:
		        {
		        	strSigBase.assign("POST&");
		        }
		        break;

		    case OAuthHttpRequestType::k_httpDelete:
		        {
		        	strSigBase.assign("DELETE&");
		        }
		        break;

		    default:
		        {
		            return false;
		        }
		        break;
		    }
		    //DEBUG_LOG("\nCOAuthSystem::GetSignature() - URLEncoding URL...");
		    strSigBase.append(CBaseEncoding::URLEncode(instrRawUrl));
		    strSigBase.append("&");
		    //DEBUG_LOG("\nCOAuthSystem::GetSignature() - URLEncoding params...");
		    strSigBase.append(CBaseEncoding::URLEncode(strRawParams));

		    //DEBUG_LOG("\nCOAuthSystem::GetSignature() - Signature Raw URL is: \""+instrRawUrl+"\"");
		    //DEBUG_LOG("COAuthSystem::GetSignature() - Signature Raw Params is: \""+strRawParams+"\"");
		    //DEBUG_LOG("COAuthSystem::GetSignature() - Signature string is: \""+strSigBase+"\"");

		    /* Now, hash the signature base string using HMAC_SHA1 class */
		    ChilliSource::Core::CHMAC_SHA1 objHMACSHA1;
		    std::string strSecretSigningKey;
		    u8 bypDigest[OAUTHLIB_BUFFSIZE_LARGE];

		    memset(bypDigest, 0, OAUTHLIB_BUFFSIZE_LARGE);

		    /* Signing key is composed of consumer_secret&token_secret */
		    //DEBUG_LOG("COAuthSystem::GetSignature() - Signing with customer secret:\""+mstrConsumerSecret+"\"");
		    strSecretSigningKey.assign(mstrConsumerSecret);
		    strSecretSigningKey.append("&");
		    if(mstrOAuthTokenSecret.length())
		    {
		    	strSecretSigningKey.append(mstrOAuthTokenSecret);
		    }

		    objHMACSHA1.HMAC_SHA1((u8*)strSigBase.c_str(),
		    					  strSigBase.length(),
		                          (u8*)strSecretSigningKey.c_str(),
		                          strSecretSigningKey.length(),
		                          (u8*)bypDigest);

		    /* Do a base64 encode of signature - SHA 1 digest is 160 bits*/
		    std::string strBase64String = ChilliSource::CBaseEncoding::Base64Encode((s8*)bypDigest, 20);
		    //DEBUG_LOG("COAuthSystem::GetSignature() - Base64 of signature SHA1:\""+strBase64String+"\"");


		    //DEBUG_LOG("COAuthSystem::GetSignature() - Encoding Base64 signature...");
		    /* Do an url encode */
		    outstrOAuthSignature = CBaseEncoding::URLEncode(strBase64String);
		    //DEBUG_LOG("COAuthSystem::GetSignature() - Encoding result:\""+outstrOAuthSignature+"\"");

		    return (outstrOAuthSignature.length()) ? true : false;
		}

		/*++
		* @method: COAuthSystem::GetOAuthHeader
		*
		* @description: this method builds OAuth header that should be used in HTTP requests to twitter
		*
		* @input: ineType - HTTP request type
		*         instrRawUrl - raw url of the HTTP request
		*         instrRawData - HTTP data
		*         inbIncludeOAuthVerifierPin - flag to indicate whether or not oauth_verifier needs to included
		*                                   in OAuth header
		*
		* @output: outstrOAuthHttpHeader - OAuth header
		*
		*--*/
		bool COAuthSystem::GetOAuthHeader(const OAuthHttpRequestType ineType,
										  const std::string& instrRawUrl,
										  const std::string& instrRawData,
										  std::string& outstrOAuthHttpHeader,
										  const bool inbIncludeOAuthVerifierPin)
		{
		    OAuthKeyValuePairsMap mapRawKeyValuePairs;
		    std::string strRawParams;
		    std::string strOAuthSignature;
		    std::string strParamsSeperator;
		    std::string strPureUrl(instrRawUrl);

		    /* Clear header string initially */
		    outstrOAuthHttpHeader.assign("");
		    mapRawKeyValuePairs.clear();

		    /* If URL itself contains ?key=value, then extract and put them in map */
		    size_t dwPos = instrRawUrl.find_first_of("?");
		    if(std::string::npos != dwPos)
		    {
		        /* Get only URL */
		        strPureUrl = instrRawUrl.substr(0, dwPos);

		        /* Get only key=value data part */
		        std::string dataPart = instrRawUrl.substr(dwPos + 1);

		        /* This dataPart can contain many key value pairs: key1=value1&key2=value2&key3=value3 */
		        size_t dwSep = std::string::npos;
		        size_t dwPos2 = std::string::npos;
		        std::string strDataKeyVal;
		        std::string strDataKey;
		        std::string strDataVal;
		        while(std::string::npos != (dwSep = dataPart.find_first_of("&")))
		        {
		            /* Extract first key=value pair */
		            strDataKeyVal = dataPart.substr(0, dwSep);

		            /* Split them */
		            dwPos2 = strDataKeyVal.find_first_of("=");
		            if(std::string::npos != dwPos2)
		            {
		                strDataKey = strDataKeyVal.substr(0, dwPos2);
		                strDataVal = strDataKeyVal.substr(dwPos2 + 1);

		                /* Put this key=value pair in map */
		                mapRawKeyValuePairs[strDataKey] = CBaseEncoding::URLEncode(strDataVal);
		            }
		            dataPart = dataPart.substr(dwSep + 1);
		        }

		        /* For the last key=value */
		        strDataKeyVal = dataPart.substr(0, dwSep);

		        /* Split them */
		        dwPos2 = strDataKeyVal.find_first_of("=");
		        if(std::string::npos != dwPos2)
		        {
		            strDataKey = strDataKeyVal.substr(0, dwPos2);
		            strDataVal = strDataKeyVal.substr(dwPos2 + 1);

		            /* Put this key=value pair in map */
		            mapRawKeyValuePairs[strDataKey] = CBaseEncoding::URLEncode(strDataVal);
		        }
		    }

		    /* Build key-value pairs needed for OAuth request token, without signature */
		    BuildOAuthTokenKeyValuePairs(inbIncludeOAuthVerifierPin, instrRawData, std::string(""), mapRawKeyValuePairs, true);

		    /* Get url encoded base64 signature using request type, url and parameters */
		    GetSignature(ineType, strPureUrl, mapRawKeyValuePairs, strOAuthSignature);

		    /* Now, again build key-value pairs with signature this time */
		    BuildOAuthTokenKeyValuePairs(inbIncludeOAuthVerifierPin, std::string(""), strOAuthSignature, mapRawKeyValuePairs, false);

		    /* Get OAuth header in string format */
		    strParamsSeperator = ",";
		    GetStringFromOAuthKeyValuePairs(mapRawKeyValuePairs, strRawParams, strParamsSeperator);

		    /* Build authorization header */
		    outstrOAuthHttpHeader.assign(OAUTHLIB_AUTHHEADER_STRING);
		    outstrOAuthHttpHeader.append(strRawParams);

		    return (outstrOAuthHttpHeader.length()) ? true : false;
		}

		/*++
		* @method: COAuthSystem::GetStringFromOAuthKeyValuePairs
		*
		* @description: this method builds a sorted string from key-value pairs
		*
		* @input: inmapRawParamMap - key-value pairs map
		*         instrParamsSeperator - sepearator, either & or ,
		*
		* @output: outstrRawParams - sorted string of OAuth parameters
		*
		* @remarks: internal method
		*
		*--*/
		bool COAuthSystem::GetStringFromOAuthKeyValuePairs(const OAuthKeyValuePairsMap& inmapRawParamMap,
														   std::string& outstrRawParams,
														   const std::string& instrParamsSeperator)
		{
		    outstrRawParams.assign("");
		    if(inmapRawParamMap.size())
		    {
		        OAuthKeyValueList KeyValueList;
		        std::string strDummyString;

		        /* Push key-value pairs to a list of strings */
		        KeyValueList.clear();
		        OAuthKeyValuePairsMap::const_iterator itMap = inmapRawParamMap.begin();
		        for(; itMap != inmapRawParamMap.end(); itMap++)
		        {
		            strDummyString.assign(itMap->first);
		            strDummyString.append("=");
		            if(instrParamsSeperator == ",")
		            {
		                strDummyString.append("\"");
		            }
		            strDummyString.append(itMap->second);
		            if(instrParamsSeperator == ",")
		            {
		                strDummyString.append("\"");
		            }
		            KeyValueList.push_back(strDummyString);
		        }

		        /* Sort key-value pairs based on key name */
		        KeyValueList.sort();

		        /* Now, form a string */
		        strDummyString.assign("");
		        OAuthKeyValueList::iterator itKeyValue = KeyValueList.begin();
		        for(; itKeyValue != KeyValueList.end(); itKeyValue++)
		        {
		            if(strDummyString.length())
		            {
		                strDummyString.append(instrParamsSeperator);
		            }
		            strDummyString.append(itKeyValue->c_str());
		        }
		        outstrRawParams.assign(strDummyString);
		    }
		    return (outstrRawParams.length()) ? true : false;
		}

		/*++
		* @method: COAuthSystem::ExtractOAuthTokenKeySecret
		*
		* @description: this method extracts oauth token key and secret from
		*               twitter's HTTP response
		*
		* @input: requestTokenResponse - response from twitter
		*
		* @output: none
		*
		*--*/
		bool COAuthSystem::ExtractOAuthTokenKeySecret(const std::string& instrRequestTokenResponse)
		{
		    if(instrRequestTokenResponse.length())
		    {
		        size_t dwPos = std::string::npos;
		        std::string strDummy;

		        /* Get oauth_token key */
		        dwPos = instrRequestTokenResponse.find(OAUTHLIB_TOKEN_KEY);
		        if(std::string::npos != dwPos)
		        {
		            dwPos = dwPos + OAUTHLIB_TOKEN_KEY.length() + strlen("=");
		            strDummy = instrRequestTokenResponse.substr(dwPos);
		            dwPos = strDummy.find("&");
		            if(std::string::npos != dwPos)
		            {
		                mstrOAuthTokenKey = strDummy.substr(0, dwPos);
		            }
		        }

		        /* Get oauth_token_secret */
		        dwPos = instrRequestTokenResponse.find(OAUTHLIB_TOKENSECRET_KEY);
		        if(std::string::npos != dwPos)
		        {
		            dwPos = dwPos + OAUTHLIB_TOKENSECRET_KEY.length() + strlen("=");
		            strDummy = instrRequestTokenResponse.substr(dwPos);
		            dwPos = strDummy.find("&");
		            if(std::string::npos != dwPos)
		            {
		                mstrOAuthTokenSecret = strDummy.substr(0, dwPos);
		            }
		        }

		        /* Get screen_name */
		        dwPos = instrRequestTokenResponse.find(OAUTHLIB_SCREENNAME_KEY);
		        if(std::string::npos != dwPos)
		        {
		            dwPos = dwPos + OAUTHLIB_SCREENNAME_KEY.length() + strlen("=");
		            strDummy = instrRequestTokenResponse.substr(dwPos);
		            mstrOAuthScreenName = strDummy;
		        }
				
				//DEBUG_LOG("COAuthSystem::ExtractOAuthTokenKeySecret() - Key:\""+mstrOAuthTokenKey+"\", Secret:\""+mstrOAuthTokenSecret+"\", Name:\""+mstrOAuthScreenName+"\"");
		    }
		    return true;
		}
	}
}
