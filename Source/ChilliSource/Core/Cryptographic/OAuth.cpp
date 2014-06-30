//
//  OAuth.cpp
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

#include <ChilliSource/Core/Cryptographic/OAuth.h>

#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <SHA1/HMAC_SHA1.h>

#include <algorithm>
#include <ctime>
#include <unordered_map>
#include <vector>

namespace ChilliSource
{
    namespace Core
    {
        namespace OAuth
        {
            namespace
            {
                const std::string k_consumerKeyKey = "oauth_consumer_key";
                const std::string k_nonceKey = "oauth_nonce";
                const std::string k_signatureKey = "oauth_signature";
                const std::string k_signatureMethodKey = "oauth_signature_method";
                const std::string k_timestampKey = "oauth_timestamp";
                const std::string k_versionKey = "oauth_version";
                const std::string k_tokenKey = "oauth_token";
                const std::string k_verifierKey = "oauth_verifier";
                const std::string k_authHeader = "OAuth ";
                const std::string k_oauthVersion = "1.0";
                
                //-----------------------------------------------------------
                /// A map for containing key value pair parameters.
                ///
                /// @author R Henning
                //-----------------------------------------------------------
                using ParamsMap = std::unordered_map<std::string, std::string>;
                //-----------------------------------------------------------
                /// Generates nonce and timestamp for OAuth header.
                ///
                /// @author R Henning
                ///
                /// @param The timestamp
                ///
                /// @return The nounce timestamp.
                //-----------------------------------------------------------
                std::string GenerateNonce(TimeIntervalSecs in_timestamp)
                {
                    static u32 s_nounceCounter = 0;
                    
                    std::string time = Core::ToString(in_timestamp);
                    std::string counter = Core::ToString(s_nounceCounter);
                    
                    s_nounceCounter++;
                    return time + counter;
                }
                //-----------------------------------------------------------
                /// This method prepares key-value pairs required for OAuth
                /// header and signature generation.
                ///
                /// @author R Henning
                ///
                /// @param url encoded data. this is used during signature
                /// generation.
                /// @param base64 and url encoded OAuth signature.
                /// @param The timestamp.
                /// @param The nonce.
                /// @param The consumer key.
                /// @param The oauth token.
                /// @param The OAuth verifier;
                /// @param [Out] The params.
                //-----------------------------------------------------------
                void BuildOAuthTokenKeyValuePairs(const std::string& in_rawData, const std::string& in_signature, const std::string& in_timestamp, const std::string& in_nonce,
                                                  const std::string& in_consumerKey, const std::string& in_oauthToken, const std::string& in_oauthVerifier, ParamsMap& out_params)
                {
                    out_params[k_consumerKeyKey] = in_consumerKey;
                    out_params[k_nonceKey] = in_nonce;
                    
                    if(in_signature.length() > 0)
                    {
                        out_params[k_signatureKey] = in_signature;
                    }
                    
                    //only HMAC-SHA1 supported for now
                    out_params[k_signatureMethodKey] = "HMAC-SHA1";
                    out_params[k_timestampKey] = in_timestamp;
                    
                    if(in_oauthToken.length() > 0)
                    {
                        out_params[k_tokenKey] = in_oauthToken;
                    }
                    
                    if (in_oauthVerifier.length() > 0)
                    {
                        out_params[k_verifierKey] = in_oauthVerifier;
                    }
                    
                    out_params[k_versionKey] = k_oauthVersion;
                    
                    if(in_rawData.length())
                    {
                        //Data should already be urlencoded once
                        std::string dummyStrKey;
                        std::string dummyStrValue;
                        size_t position = in_rawData.find_first_of("=");
                        if(std::string::npos != position)
                        {
                            dummyStrKey = in_rawData.substr(0, position);
                            dummyStrValue = in_rawData.substr(position + 1);
                            out_params[dummyStrKey] = dummyStrValue;
                        }
                    }
                }
                //-----------------------------------------------------------
                /// Builds a sorted string from key-value pairs
                ///
                /// @author R Henning
                ///
                /// @param key-value pairs map
                /// @param sepearator, either & or ,
                ///
                /// @return sorted string of OAuth parameters
                //-----------------------------------------------------------
                std::string GetStringFromOAuthKeyValuePairs(const ParamsMap& in_paramsMap, const std::string& in_paramsSeperator)
                {
                    std::string output;
                    
                    if(in_paramsMap.size())
                    {
                        std::vector<std::string> keyValueList;
                        keyValueList.reserve(in_paramsMap.size());
                        std::string dummyString;
                        
                        //Push key-value pairs to a list of strings
                        for (const auto& param : in_paramsMap)
                        {
                            dummyString = param.first + "=";
                            if(in_paramsSeperator == ",")
                            {
                                dummyString += "\"";
                            }
                            dummyString += param.second;
                            if(in_paramsSeperator == ",")
                            {
                                dummyString += "\"";
                            }
                            
                            keyValueList.push_back(dummyString);
                        }
                        
                        //OAuth parameters need to be in order, so sort them
                        std::sort(keyValueList.begin(), keyValueList.end());
                        
                        // Now, form a string
                        for(const auto& param : keyValueList)
                        {
                            if(output.length() > 0)
                            {
                                output.append(in_paramsSeperator);
                            }
                            output.append(param.c_str());
                        }
                    }
                    
                    return output;
                }
                //-----------------------------------------------------------
                /// Calculates HMAC-SHA1 signature of OAuth header
                ///
                /// @author R Henning
                ///
                /// @param HTTP request type.
                /// @param raw url of the HTTP request.
                /// @param The consumer secret.
                /// @param The oauth token secret.
                /// @param key-value pairs containing OAuth headers and HTTP data
                ///
                /// @return base64 and url encoded signature.
                //-----------------------------------------------------------
                std::string GetSignature(RequestType in_type, const std::string& in_rawUrl, const std::string& in_consumerSecret, const std::string& in_oauthTokenSecret, const ParamsMap& in_paramsMap)
                {
                    //Build a string using key-value pairs
                    std::string rawParams = GetStringFromOAuthKeyValuePairs(in_paramsMap, "&");
                    
                    //Start constructing base signature string. Refer http://dev.twitter.com/auth#intro
                    std::string sigBase;
                    switch(in_type)
                    {
                    case RequestType::k_httpGet:
                        sigBase = "GET&";
                        break;
                    case RequestType::k_httpPost:
                        sigBase = "POST&";
                        break;
                    case RequestType::k_httpDelete:
                        sigBase = "DELETE&";
                        break;
                    default:
                        return "";
                    }
                    
                    sigBase += StringUtils::URLEncode(in_rawUrl) + "&" + StringUtils::URLEncode(rawParams);
                    
                    //Signing key is composed of consumer_secret&token_secret
                    std::string secretSigningKey = in_consumerSecret + "&" + in_oauthTokenSecret;
                    
                    //Now, hash the signature base string using HMAC_SHA1 class
                    const u32 k_bufferSize = 1024;
                    u8 digest[k_bufferSize];
                    memset(digest, 0, k_bufferSize);
                    
                    CHMAC_SHA1 objHMACSHA1;
                    objHMACSHA1.HMAC_SHA1((u8*)sigBase.c_str(), sigBase.length(), (u8*)secretSigningKey.c_str(), secretSigningKey.length(), (u8*)digest);
                    
                    //Do a base64 encode of signature - SHA 1 digest is 160 bits
                    std::string base64String = BaseEncoding::Base64Encode((s8*)digest, 20);
                    
                    //Do an url encode
                    return StringUtils::URLEncode(base64String);
                }
            }
            //-----------------------------------------------------------
            //-----------------------------------------------------------
            std::string GenerateAuthorisationHeader(RequestType in_requestType, const std::string& in_url, const std::string& in_rawData, const std::string& in_consumerKey, const std::string& in_consumerSecret,
                                                    const std::string& in_oauthToken, const std::string& in_oauthTokenSecret, const std::string& in_oauthVerifier)
            {
                std::string pureUrl = in_url;

                //If URL itself contains "?key=value", then extract and put them in map
                size_t position = pureUrl.find_first_of("?");
                std::unordered_map<std::string, std::string> urlParams;
                if(std::string::npos != position)
                {
                    //Get only URL
                    pureUrl = in_url.substr(0, position);
                    
                    //Get only key=value data part
                    std::string dataPart = in_url.substr(position + 1);
                    
                    //This dataPart can contain many key value pairs: key1=value1&key2=value2&key3=value3
                    size_t sep = std::string::npos;
                    size_t position2 = std::string::npos;
                    std::string dataKeyVal;
                    std::string dataKey;
                    std::string dataVal;
                    while(std::string::npos != (sep = dataPart.find_first_of("&")))
                    {
                        //Extract first key=value pair
                        dataKeyVal = dataPart.substr(0, sep);
                        
                        //Split them
                        position2 = dataKeyVal.find_first_of("=");
                        if(std::string::npos != position2)
                        {
                            dataKey = dataKeyVal.substr(0, position2);
                            dataVal = dataKeyVal.substr(position2 + 1);
                            
                            //Put this key=value pair in map
                            urlParams[dataKey] = StringUtils::URLEncode(dataVal);
                        }
                        dataPart = dataPart.substr(sep + 1);
                    }
                    
                    //For the last key=value
                    dataKeyVal = dataPart.substr(0, sep);
                    
                    //Split them
                    position2 = dataKeyVal.find_first_of("=");
                    if(std::string::npos != position2)
                    {
                        dataKey = dataKeyVal.substr(0, position2);
                        dataVal = dataKeyVal.substr(position2 + 1);
                        
                        //Put this key=value pair in map
                        urlParams[dataKey] = StringUtils::URLEncode(dataVal);
                    }
                }
                
                //Generate nonce and timestamp if required
                TimeIntervalSecs timestamp = (TimeIntervalSecs)time(0);
                std::string nonce = GenerateNonce(timestamp);
                std::string timestampString = Core::ToString(timestamp);
                
                //Build key-value pairs needed for OAuth request token, without signature
                BuildOAuthTokenKeyValuePairs(in_rawData, "", timestampString, nonce, in_consumerKey, in_oauthToken, in_oauthVerifier, urlParams);
                
                //Get url encoded base64 signature using request type, url and parameters
                std::string oauthSignature = GetSignature(in_requestType, pureUrl, in_consumerSecret, in_oauthTokenSecret, urlParams);
                
                //Now, again build key-value pairs with signature this time
                BuildOAuthTokenKeyValuePairs("", oauthSignature, timestampString, nonce, in_consumerKey, in_oauthToken, in_oauthVerifier, urlParams);
                
                //Get OAuth header in string format
                std::string strRawParams = GetStringFromOAuthKeyValuePairs(urlParams, ",");
                
                //Build authorization header
                return k_authHeader + strRawParams;
            }
        }
    }
}
