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
#include <ChilliSource/Core/File/LocalDataStore.h>
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

			// Status
			const std::string k_statusUpdateURL = "http://api.twitter.com/1/statuses/update.xml";
			const std::string k_statusShowURL = "http://api.twitter.com/1/statuses/show";
			const std::string k_statusDestroyURL = "http://api.twitter.com/1/statuses/destroy";

			// Used to build Twitter URLs
			const std::string k_statusKey = "status=";
			
			const u32 k_characterLimit = 140;
		}
        
		CS_DEFINE_NAMEDTYPE(TwitterPostSystem);

        //------------------------------------------------
        //------------------------------------------------
        TwitterPostSystemUPtr TwitterPostSystem::Create(Networking::HttpConnectionSystem* inpHttpConnectionSystem, Core::OAuthSystem* inpOAuthSystem)
        {
#ifdef CS_TARGETPLATFORM_IOS
            return TwitterPostSystemUPtr(new ChilliSource::iOS::TwitterPostSystem(inpHttpConnectionSystem, inpOAuthSystem));
#elif CS_TARGETPLATFORM_ANDROID
            return TwitterPostSystemUPtr(new ChilliSource::Android::TwitterPostSystem(inpHttpConnectionSystem, inpOAuthSystem));
#endif
			return TwitterPostSystemUPtr();
        }
		//---------------------------------------------------
		//---------------------------------------------------
		TwitterPostSystem::TwitterPostSystem(Networking::HttpConnectionSystem* in_httpConnectionSystem, Core::OAuthSystem* in_oauthSystem)
        : m_isAuthenticated(false)
		{
            m_httpConnectionSystem = in_httpConnectionSystem;
            m_oauthSystem = in_oauthSystem;
            CS_ASSERT(m_httpConnectionSystem != nullptr && m_oauthSystem != nullptr, "Twitter post system requires the http request system and oauth system.");
		}
        //------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::PostUsingChilliSource(const PostDesc& in_desc)
		{
            if(in_desc.m_text.size() > 0 && in_desc.m_text.size() <= k_characterLimit && m_httpConnectionSystem->CheckReachability())
			{
				// Construct our Tweet request URL
				std::string status = k_statusKey + Core::BaseEncoding::URLEncode(in_desc.m_text);
				std::string OAuthHeader;

				if(m_oauthSystem->GetOAuthHeader(Core::OAuthSystem::OAuthHttpRequestType::k_httpPost, k_statusUpdateURL, status, OAuthHeader))
				{
					Networking::HttpRequestDetails request;
					request.strURL = k_statusUpdateURL;
					request.strBody = status;
					request.eType = Networking::HttpRequestDetails::Type::k_post;
					request.sHeaders.SetValueForKey("Content-Type", "application/x-www-form-urlencoded");
					request.sHeaders.SetValueForKey(k_oauthAuthHeaderKey, OAuthHeader);

					m_httpConnectionSystem->MakeRequest(request, Core::MakeDelegate(this, &TwitterPostSystem::OnStatusUpdateComplete));
                    return;
				}
			}

            if(m_postDelegate)
            {
                m_postDelegate(PostResult::k_failed);
            }
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnStatusUpdateComplete(Networking::HttpRequest* in_request, Networking::HttpRequest::CompletionResult in_result)
		{
            if(m_postDelegate == nullptr)
            {
                return;
            }
            
            switch(in_result)
            {
                case Networking::HttpRequest::CompletionResult::k_completed:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_success);
                    break;
                case Networking::HttpRequest::CompletionResult::k_failed:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_failed);
                    break;
                case Networking::HttpRequest::CompletionResult::k_cancelled:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_cancelled);
                    break;
                case Networking::HttpRequest::CompletionResult::k_timeout:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_failed);
                    break;
                case Networking::HttpRequest::CompletionResult::k_flushed:
                    m_postDelegate(Social::TwitterPostSystem::PostResult::k_failed);
                    break;
            }
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		bool TwitterPostSystem::RequestOAuthToken()
		{
			bool bResult = false;

			if(m_httpConnectionSystem->CheckReachability())
			{
				// Construct our OAuth request URL - 'oob' means out-of-band and tells Twitter we
				// are on a mobile device.
				std::string strURL = k_oauthTokenRequestURL + "?" + Core::OAUTHLIB_CALLBACK_KEY + "=oob";
				std::string strOAuthHeader;

				if(m_oauthSystem->GetOAuthHeader(Core::OAuthSystem::OAuthHttpRequestType::k_httpGet,
												 strURL,
												 "", strOAuthHeader))
				{
					Networking::HttpRequestDetails sHttpRequest;
					sHttpRequest.strURL = k_oauthTokenRequestURL;
					sHttpRequest.eType = Networking::HttpRequestDetails::Type::k_get;
					sHttpRequest.sHeaders.SetValueForKey("Content-Type", "application/x-www-form-urlencoded");
					sHttpRequest.sHeaders.SetValueForKey(k_oauthAuthHeaderKey, strOAuthHeader);

					m_httpConnectionSystem->MakeRequest(sHttpRequest, Core::MakeDelegate(this, &TwitterPostSystem::OnRequestOAuthTokenComplete));
					bResult = true;
				}
			}

			return bResult;
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnRequestOAuthTokenComplete(ChilliSource::Networking::HttpRequest* in_request, ChilliSource::Networking::HttpRequest::CompletionResult in_result)
		{
			if(in_result == ChilliSource::Networking::HttpRequest::CompletionResult::k_completed)
			{
                //TODO: The OAuthSystem should know nothing of twitter and should not be
                //extracting the key and secret for us. Once we learn to extract the key and secret ourselves there is no need to retrieve it from the
                //OAuthSystem
                
				// Tell OAuth system to save access token and secret from web response
				m_oauthSystem->ExtractOAuthTokenKeySecret(in_request->GetResponseString());
                m_oauthSystem->GetOAuthTokenKey(m_savedOAuthTokenKey);
                m_oauthSystem->GetOAuthTokenSecret(m_savedOAuthTokenSecret);
                
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
		void TwitterPostSystem::RequestOAuthAccessToken()
		{
			m_isAuthenticated = false;

			if(m_httpConnectionSystem->CheckReachability())
			{
				std::string strOAuthHeader;

				if(m_oauthSystem->GetOAuthHeader(Core::OAuthSystem::OAuthHttpRequestType::k_httpGet, k_oauthTokenAccessURL, "", strOAuthHeader, true))
				{
					Networking::HttpRequestDetails sHttpRequest;
					sHttpRequest.strURL = k_oauthTokenAccessURL;
					sHttpRequest.eType = Networking::HttpRequestDetails::Type::k_get;
					sHttpRequest.sHeaders.SetValueForKey("Content-Type", "application/x-www-form-urlencoded");
					sHttpRequest.sHeaders.SetValueForKey(k_oauthAuthHeaderKey, strOAuthHeader);

					m_httpConnectionSystem->MakeRequest(sHttpRequest, Core::MakeDelegate(this, &TwitterPostSystem::OnRequestOAuthAccessTokenComplete));
                    return;
				}
			}
            
            if(m_authDelegate)
            {
                m_authDelegate(AuthenticationResult::k_failed);
            }
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnRequestOAuthAccessTokenComplete(Networking::HttpRequest* in_request, Networking::HttpRequest::CompletionResult in_result)
		{
			if(in_result == ChilliSource::Networking::HttpRequest::CompletionResult::k_completed)
			{
                //TODO: The OAuthSystem should know nothing of twitter and should not be
                //extracting the key and secret for us. Once we learn to extract the key and secret ourselves there is no need to retrieve it from the
                //OAuthSystem

				// Tell OAuth system to save access token and secret from web response
				m_oauthSystem->ExtractOAuthTokenKeySecret(in_request->GetResponseString());
                m_oauthSystem->GetOAuthTokenKey(m_savedOAuthTokenKey);
                m_oauthSystem->GetOAuthTokenSecret(m_savedOAuthTokenSecret);
                
				SaveAuthenticationKeys();
                
				m_isAuthenticated = true;
                
                if(m_authDelegate)
                {
                    m_authDelegate(AuthenticationResult::k_success);
                }
			}
            else
            {
                m_isAuthenticated = false;
                
                if(m_authDelegate)
                {
                    m_authDelegate(AuthenticationResult::k_failed);
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

