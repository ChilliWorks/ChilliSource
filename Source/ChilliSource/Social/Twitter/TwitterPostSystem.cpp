/*
 *  TwitterPostSystem.cpp
 *  moFlow
 *
 *  Created by Scott Downie on 06/09/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Cryptographic/OAuth.h>
#include <ChilliSource/Core/File/LocalDataStore.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>
#include <ChilliSource/Social/Twitter/TwitterPostSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Social/Twitter/TwitterPostSystem.h>
#elif CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterPostSystem.h>
#endif

namespace ChilliSource
{
	namespace Social
	{
        namespace
		{
            const std::string k_OAuthTokenStoreKey = "TwitterOAuthTokenKey";
            const std::string k_OAuthSecretStoreKey = "TwitterOAuthSecretKey";
            
			const std::string k_oauthTokenRequestURL = "http://api.twitter.com/oauth/request_token";
			const std::string k_oauthAuthoriseURL = "http://twitter.com/oauth/authorize?oauth_token=";
			const std::string k_oauthTokenAccessURL = "http://twitter.com/oauth/access_token";

			const std::string k_oauthAuthHeaderKey = "Authorization";
            
            const std::string k_oauthCallbackKey = "oauth_callback";
            const std::string k_oauthTokenKey = "oauth_token";
            const std::string k_oauthTokenSecretKey = "oauth_token_secret";
            
			// Status
			const std::string k_statusUpdateURL = "http://api.twitter.com/1/statuses/update.xml";
			const std::string k_statusShowURL = "http://api.twitter.com/1/statuses/show";
			const std::string k_statusDestroyURL = "http://api.twitter.com/1/statuses/destroy";

			// Used to build Twitter URLs
			const std::string k_statusKey = "status=";
			
			const u32 k_characterLimit = 140;
            
            //------------------------------------------------------------------------
			/// this method extracts oauth token key and secret from twitter's HTTP
            /// response.
            ///
            /// @author R Henning
			///
			/// @param Response from twitter
            /// @param [Out] The oauth token key.
            /// @param [Out] The oauth token secret.
			//------------------------------------------------------------------------
            void ExtractOAuthTokenKeySecret(const std::string& in_requestTokenResponse, std::string& out_oauthTokenKey, std::string& out_oauthTokenSecret)
            {
                if(in_requestTokenResponse.length())
                {
                    size_t position = std::string::npos;
                    std::string dummy;
                    
                    //Get oauth_token key
                    position = in_requestTokenResponse.find(k_oauthTokenKey);
                    if(std::string::npos != position)
                    {
                        position += k_oauthTokenKey.length() + strlen("=");
                        dummy = in_requestTokenResponse.substr(position);
                        position = dummy.find("&");
                        if(std::string::npos != position)
                        {
                            out_oauthTokenKey = dummy.substr(0, position);
                        }
                    }
                    
                    //Get oauth_token_secret
                    position = in_requestTokenResponse.find(k_oauthTokenSecretKey);
                    if(std::string::npos != position)
                    {
                        position += k_oauthTokenSecretKey.length() + strlen("=");
                        dummy = in_requestTokenResponse.substr(position);
                        position = dummy.find("&");
                        if(std::string::npos != position)
                        {
                            out_oauthTokenSecret = dummy.substr(0, position);
                        }
                    }
                }
            }
		}
        
		CS_DEFINE_NAMEDTYPE(TwitterPostSystem);

        //------------------------------------------------
        //------------------------------------------------
        TwitterPostSystemUPtr TwitterPostSystem::Create(Networking::HttpRequestSystem* in_httpRequestSystem)
        {
#ifdef CS_TARGETPLATFORM_IOS
            return TwitterPostSystemUPtr(new ChilliSource::iOS::TwitterPostSystem(in_httpRequestSystem));
#elif CS_TARGETPLATFORM_ANDROID
            return TwitterPostSystemUPtr(new ChilliSource::Android::TwitterPostSystem(in_httpRequestSystem));
#endif
			return TwitterPostSystemUPtr();
        }
		//---------------------------------------------------
		//---------------------------------------------------
		TwitterPostSystem::TwitterPostSystem(Networking::HttpRequestSystem* in_httpRequestSystem)
        : m_isAuthenticated(false)
		{
            m_httpRequestSystem = in_httpRequestSystem;
            CS_ASSERT(m_httpRequestSystem != nullptr, "Twitter post system requires the http request system.");
		}
        //------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::PostUsingChilliSource(const PostDesc& in_desc)
		{
            if(in_desc.m_text.size() > 0 && in_desc.m_text.size() <= k_characterLimit && m_httpRequestSystem->CheckReachability())
			{
				// Construct our Tweet request URL
				std::string status = k_statusKey + Core::BaseEncoding::URLEncode(in_desc.m_text);
				std::string oauthHeader = Core::OAuth::GenerateOAuthAuthorisationHeader(Core::OAuth::RequestType::k_httpPost, k_statusUpdateURL, status, "", "", m_savedOAuthTokenKey, m_savedOAuthTokenSecret);

				if(oauthHeader.empty() == false)
				{
					Networking::HttpRequest::Desc request;
					request.m_url = k_statusUpdateURL;
					request.m_body = status;
					request.m_type = Networking::HttpRequest::Type::k_post;
					request.m_headers.SetValueForKey("Content-Type", "application/x-www-form-urlencoded");
					request.m_headers.SetValueForKey(k_oauthAuthHeaderKey, oauthHeader);

					m_httpRequestSystem->MakeRequest(request, Core::MakeDelegate(this, &TwitterPostSystem::OnStatusUpdateComplete));
                    return;
				}
			}

            if(m_postDelegate)
            {
                m_postDelegate(PostResult::k_failed);
                m_postDelegate = nullptr;
            }
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnStatusUpdateComplete(Networking::HttpRequest* in_request, Networking::HttpRequest::Result in_result)
		{
            if(m_postDelegate == nullptr)
            {
                return;
            }
            
            switch(in_result)
            {
                case Networking::HttpRequest::Result::k_completed:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_success);
                    break;
                case Networking::HttpRequest::Result::k_failed:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_failed);
                    break;
                case Networking::HttpRequest::Result::k_cancelled:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_cancelled);
                    break;
                case Networking::HttpRequest::Result::k_timeout:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_failed);
                    break;
                case Networking::HttpRequest::Result::k_flushed:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_failed);
                    break;
            }
            
            m_postDelegate = nullptr;
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		bool TwitterPostSystem::RequestOAuthToken()
		{
			bool bResult = false;

			if(m_httpRequestSystem->CheckReachability())
			{
				// Construct our OAuth request URL - 'oob' means out-of-band and tells Twitter we
				// are on a mobile device.
				std::string strURL = k_oauthTokenRequestURL + "?" + k_oauthCallbackKey + "=oob";
				
                std::string strOAuthHeader = Core::OAuth::GenerateOAuthAuthorisationHeader(Core::OAuth::RequestType::k_httpGet, strURL, "", "", "", m_savedOAuthTokenKey, m_savedOAuthTokenSecret);
                if(strOAuthHeader.empty() == false)
				{
					Networking::HttpRequest::Desc httpRequest;
					httpRequest.m_url = k_oauthTokenRequestURL;
					httpRequest.m_type = Networking::HttpRequest::Type::k_get;
					httpRequest.m_headers.SetValueForKey("Content-Type", "application/x-www-form-urlencoded");
					httpRequest.m_headers.SetValueForKey(k_oauthAuthHeaderKey, strOAuthHeader);

					m_httpRequestSystem->MakeRequest(httpRequest, Core::MakeDelegate(this, &TwitterPostSystem::OnRequestOAuthTokenComplete));
					bResult = true;
				}
			}

			return bResult;
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnRequestOAuthTokenComplete(ChilliSource::Networking::HttpRequest* in_request, Networking::HttpRequest::Result in_result)
		{
			if(in_result == Networking::HttpRequest::Result::k_completed)
			{
				//Get the key and secret
				ExtractOAuthTokenKeySecret(in_request->GetResponse(), m_savedOAuthTokenKey, m_savedOAuthTokenSecret);
                
				// Build authorise URL so that user can visit in browser and get PIN
				std::string authoriseURL;
				authoriseURL.assign(k_oauthAuthoriseURL);
				authoriseURL.append(m_savedOAuthTokenKey);

				// OK, let's tell the authentication view our URL
				// From here the user will be able to login to Twitter and enter the PIN
				if(m_authenticationView)
                {
					m_authenticationView->PresentURL(authoriseURL);
                }
			}
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::RequestOAuthAccessToken(const std::string& in_pin)
		{
			m_isAuthenticated = false;

			if(m_httpRequestSystem->CheckReachability())
			{
				std::string strOAuthHeader = Core::OAuth::GenerateOAuthAuthorisationHeader(Core::OAuth::RequestType::k_httpGet, k_oauthTokenAccessURL, "", "", "", m_savedOAuthTokenKey, m_savedOAuthTokenSecret, in_pin);
				if(strOAuthHeader.empty() == false)
				{
					Networking::HttpRequest::Desc request;
					request.m_url = k_oauthTokenAccessURL;
					request.m_type = Networking::HttpRequest::Type::k_get;
					request.m_headers.SetValueForKey("Content-Type", "application/x-www-form-urlencoded");
					request.m_headers.SetValueForKey(k_oauthAuthHeaderKey, strOAuthHeader);

					m_httpRequestSystem->MakeRequest(request, Core::MakeDelegate(this, &TwitterPostSystem::OnRequestOAuthAccessTokenComplete));
                    return;
				}
			}
            
            if(m_authDelegate)
            {
                m_authDelegate(AuthenticationResult::k_failed);
                m_authDelegate = nullptr;
            }
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnRequestOAuthAccessTokenComplete(Networking::HttpRequest* in_request, Networking::HttpRequest::Result in_result)
		{
			if(in_result == Networking::HttpRequest::Result::k_completed)
			{
				//Get the key and secret
				ExtractOAuthTokenKeySecret(in_request->GetResponse(), m_savedOAuthTokenKey, m_savedOAuthTokenSecret);
                
				SaveAuthenticationKeys();
                
				m_isAuthenticated = true;
                
                if(m_authDelegate)
                {
                    m_authDelegate(AuthenticationResult::k_success);
                    m_authDelegate = nullptr;
                }
			}
            else
            {
                m_isAuthenticated = false;
                
                if(m_authDelegate)
                {
                    m_authDelegate(AuthenticationResult::k_failed);
                    m_authDelegate = nullptr;
                }
            }
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::LoadAuthenticationKeys()
		{
			ChilliSource::Core::LocalDataStore& pLocalData = ChilliSource::Core::LocalDataStore::GetSingleton();
            
			pLocalData.TryGetValue(k_OAuthTokenStoreKey, m_savedOAuthTokenKey);
			pLocalData.TryGetValue(k_OAuthSecretStoreKey, m_savedOAuthTokenSecret);

            //If either of the tokens have failed to load then discard the other
            //they only work as a pair
			if(m_savedOAuthTokenKey.empty() || m_savedOAuthTokenSecret.empty())
			{
				m_savedOAuthTokenKey.clear();
				m_savedOAuthTokenSecret.clear();
			}
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::SaveAuthenticationKeys()
		{
			ChilliSource::Core::LocalDataStore& pLocalData = ChilliSource::Core::LocalDataStore::GetSingleton();

			pLocalData.SetValueForKey(k_OAuthTokenStoreKey, m_savedOAuthTokenKey);
			pLocalData.SetValueForKey(k_OAuthSecretStoreKey, m_savedOAuthTokenSecret);
			pLocalData.Synchronise();
		}
	}
}

