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

#ifdef TARGET_OS_IPHONE
#include <ChilliSource/Backend/Platform/iOS/Social/Twitter/TwitterPostSystem.h>
#elif TARGET_ANDROID
#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterPostSystem.h>
#endif

namespace ChilliSource
{
	namespace Social
	{
		DEFINE_NAMED_INTERFACE(TwitterPostSystem);

        TwitterPostSystem* TwitterPostSystem::CreateSystem(Networking::HttpConnectionSystem* inpHttpConnectionSystem, Core::OAuthSystem* inpOAuthSystem)
        {
#ifdef TARGET_OS_IPHONE
            return new ChilliSource::iOS::CTwitterPostSystem(static_cast<iOS::CHttpConnectionSystem*>(inpHttpConnectionSystem), inpOAuthSystem);
#elif TARGET_ANDROID
            return new ChilliSource::Android::CTwitterPostSystem(static_cast<Android::CHttpConnectionSystem*>(inpHttpConnectionSystem), inpOAuthSystem);
#endif
			return nullptr;
        }
        
		//------------------------------------------------------------------------
		/// Constructor
		//------------------------------------------------------------------------
		TwitterPostSystem::TwitterPostSystem(Networking::HttpConnectionSystem* inpHttpConnectionSystem,
											   Core::OAuthSystem* inpOAuthSystem) : mstrCustomerKey(""),
																						   mstrCustomerSecret(""),
																						   mpAuthenticationView(nullptr)
		{
				mpHttpConnectionSystem = inpHttpConnectionSystem;
				mpOAuthSystem = inpOAuthSystem;
				CS_ASSERT(mpHttpConnectionSystem != nullptr && mpOAuthSystem != nullptr, "Twitter post system requires the http request system and oauth system.");
		}
		//------------------------------------------------------------------------
		/// Destructor
		//------------------------------------------------------------------------
		TwitterPostSystem::~TwitterPostSystem()
		{
		}
		//------------------------------------------------------------------------
        /// Init
        //------------------------------------------------------------------------
        bool TwitterPostSystem::Init(const std::string& instrCustomerKey, const std::string& instrCustomerSecret)
        {
        	bool bResult = false;
			
			msDesc.strUrl.clear();
			msDesc.strText.clear();
			
        	if(!instrCustomerKey.empty() && !instrCustomerSecret.empty())
        	{
        		mstrCustomerKey = instrCustomerKey;
        		mstrCustomerSecret = instrCustomerSecret;
        		if(mpOAuthSystem)
        		{
        			mpOAuthSystem->SetConsumerKey(mstrCustomerKey);
        			mpOAuthSystem->SetConsumerSecret(mstrCustomerSecret);
        			bResult = true;
        		}
				else
					CS_ERROR_LOG("CTwitterPostSystem::Init() - ERROR: Twitter does not have an OAuth system!");
        	}
        	else
        		CS_ERROR_LOG("CTwitterPostSystem::Init() - ERROR: Twitter customer key and/or customer secret string(s) empty!");
			
        	return bResult;
        }
        //------------------------------------------------------------------------
        /// Run the OAuth process and, if successful, leave the system in state
        /// ready to communicate with Twitter
        //------------------------------------------------------------------------
        bool TwitterPostSystem::Authenticate()
        {
        	CS_WARNING_LOG("TwitterPostSystem::Authenticate() - This platform does not have an Authenticate() method\n\tTwitter may not function correctly or at all!");

        	return true;
        }
        //------------------------------------------------------------------------
		/// Post Using moFlow
		//------------------------------------------------------------------------
		bool TwitterPostSystem::PostUsingMoFlow(const Social::TwitterPostDesc & insDesc)
		{
			bool bResult = false;

			// But frist we need to check our post is within character count limits
			if(insDesc.strText.size() <= 0 || insDesc.strText.size() > Social::TwitterDefault::kudwCharacterLimit)
			{
				if(mCompletionDelegate)
				{
					mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_failed);
				}
				bResult = false;
			}
			else
			if(mpHttpConnectionSystem->CheckReachability())
			{
				// Construct our Tweet request URL
				std::string strStatus = Social::TwitterDefault::TWITTER_STATUS_STRING + Core::CBaseEncoding::URLEncode(insDesc.strText.ToASCII());
				std::string strStatusURL = Social::TwitterURL::TWITTER_STATUS_UPDATE_URL;
				std::string strOAuthHeader;

				if(mpOAuthSystem->GetOAuthHeader(Core::OAuthSystem::OAuthHttpRequestType::k_httpPost,
												 strStatusURL,
												 strStatus, strOAuthHeader))
				{
					Networking::HttpRequestDetails sHttpRequest;
					sHttpRequest.strURL = Social::TwitterURL::TWITTER_STATUS_UPDATE_URL;
					sHttpRequest.strBody = strStatus;
					sHttpRequest.eType = Networking::HttpRequestDetails::Type::k_post;
					sHttpRequest.sHeaders.SetValueForKey("Content-Type", "application/x-www-form-urlencoded");
					sHttpRequest.sHeaders.SetValueForKey(Social::TwitterOAuthAPIHeaders::TWITTER_OAUTH_REQUEST_HEADER_AUTHORIZATION, strOAuthHeader);

					mpHttpConnectionSystem->MakeRequest(sHttpRequest, Core::MakeDelegate(this, &TwitterPostSystem::OnStatusUpdateComplete));
					bResult = true;
				}
			}

			return bResult;
		}
		//------------------------------------------------------------------------
		/// Delegate method called when a status update has completed
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnStatusUpdateComplete(Networking::HttpRequest* inpRequest, Networking::HttpRequest::CompletionResult ineResult)
		{
			if(ineResult == ChilliSource::Networking::HttpRequest::CompletionResult::k_completed)
			{
				if(mCompletionDelegate)
				{
					mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_success);
				}
			}
			else if(ineResult == ChilliSource::Networking::HttpRequest::CompletionResult::k_failed)
			{
				if(mCompletionDelegate)
				{
					mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_failed);
				}
			}
			else if(ineResult == ChilliSource::Networking::HttpRequest::CompletionResult::k_cancelled)
			{
				if(mCompletionDelegate)
				{
					mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_cancelled);
				}
			}
			else if(ineResult == ChilliSource::Networking::HttpRequest::CompletionResult::k_timeout)
			{
				if(mCompletionDelegate)
				{
					mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_failed);
				}
			}
			else if(ineResult == ChilliSource::Networking::HttpRequest::CompletionResult::k_flushed)
			{
				if(mCompletionDelegate)
				{
					mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_failed);
				}
			}
			else
			{
				if(mCompletionDelegate)
				{
					mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_failed);
				}
			}
		}
		//------------------------------------------------------------------------
		/// Gets a request token and secret. This is used to authorise the user
		/// and get a PIN from Twitter
		//------------------------------------------------------------------------
		bool TwitterPostSystem::RequestOAuthToken(std::string& outstrAuthoriseURL)
		{
			bool bResult = false;

			outstrAuthoriseURL.clear();

			if(mpHttpConnectionSystem->CheckReachability())
			{
				// Construct our OAuth request URL - 'oob' means out-of-band and tells Twitter we
				// are on a mobile device.
				std::string strURL = Social::TwitterOAuthAPIURLs::TWITTER_OAUTH_REQUEST_TOKEN_URL + Social::TwitterDefault::TWITTER_URL_SEP_QUESTION_MARK + Core::OAUTHLIB_CALLBACK_KEY + "=oob";
				std::string strOAuthHeader;

				if(mpOAuthSystem->GetOAuthHeader(Core::OAuthSystem::OAuthHttpRequestType::k_httpGet,
												 strURL,
												 std::string(""), strOAuthHeader))
				{
					Networking::HttpRequestDetails sHttpRequest;
					sHttpRequest.strURL = Social::TwitterOAuthAPIURLs::TWITTER_OAUTH_REQUEST_TOKEN_URL;
					sHttpRequest.eType = Networking::HttpRequestDetails::Type::k_get;
					sHttpRequest.sHeaders.SetValueForKey("Content-Type", "application/x-www-form-urlencoded");
					sHttpRequest.sHeaders.SetValueForKey(Social::TwitterOAuthAPIHeaders::TWITTER_OAUTH_REQUEST_HEADER_AUTHORIZATION, strOAuthHeader);

					mpHttpConnectionSystem->MakeRequest(sHttpRequest, Core::MakeDelegate(this, &TwitterPostSystem::OnRequestOAuthTokenComplete));
					bResult = true;
				}
			}

			return bResult;
		}
		//------------------------------------------------------------------------
		/// Delegate called with RequestOAuthToken() completes.
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnRequestOAuthTokenComplete(ChilliSource::Networking::HttpRequest* inpRequest, ChilliSource::Networking::HttpRequest::CompletionResult ineResult)
		{
			if(ineResult == ChilliSource::Networking::HttpRequest::CompletionResult::k_completed)
			{
				// Tell OAuth system to save access token and secret from web response
				mpOAuthSystem->ExtractOAuthTokenKeySecret(inpRequest->GetResponseString());

				// Get access token and secret from OAuth object
				std::string strOAuthTokenKey;
				mpOAuthSystem->GetOAuthTokenKey(strOAuthTokenKey);

				// Build authorise URL so that user can visit in browser and get PIN
				std::string outstrAuthoriseURL;
				outstrAuthoriseURL.assign(Social::TwitterOAuthAPIURLs::TWITTER_OAUTH_AUTHORIZE_URL);
				outstrAuthoriseURL.append(strOAuthTokenKey.c_str());

				// OK, let's tell the authentication view our URL
				// From here the user will be able to login to Twitter and enter the PIN
				if(mpAuthenticationView)
					mpAuthenticationView->PresentURL(outstrAuthoriseURL);
			}
		}
		//------------------------------------------------------------------------
		/// Exchange our request token with an access token
		//------------------------------------------------------------------------
		bool TwitterPostSystem::RequestOAuthAccessToken()
		{
			bool bResult = false;

			if(mpHttpConnectionSystem->CheckReachability())
			{
				std::string strOAuthHeader;

				if(mpOAuthSystem->GetOAuthHeader(Core::OAuthSystem::OAuthHttpRequestType::k_httpGet,
												 Social::TwitterOAuthAPIURLs::TWITTER_OAUTH_ACCESS_TOKEN_URL,
												 std::string(""), strOAuthHeader, true))
				{
					Networking::HttpRequestDetails sHttpRequest;
					sHttpRequest.strURL = Social::TwitterOAuthAPIURLs::TWITTER_OAUTH_ACCESS_TOKEN_URL;
					sHttpRequest.eType = Networking::HttpRequestDetails::Type::k_get;
					sHttpRequest.sHeaders.SetValueForKey("Content-Type", "application/x-www-form-urlencoded");
					sHttpRequest.sHeaders.SetValueForKey(Social::TwitterOAuthAPIHeaders::TWITTER_OAUTH_REQUEST_HEADER_AUTHORIZATION, strOAuthHeader);

					mpHttpConnectionSystem->MakeRequest(sHttpRequest, Core::MakeDelegate(this, &TwitterPostSystem::OnRequestOAuthAccessTokenComplete));
					bResult = true;
				}
			}

			return bResult;
		}
		//------------------------------------------------------------------------
		/// Delegate called with RequestOAuthToken() completes.
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnRequestOAuthAccessTokenComplete(Networking::HttpRequest* inpRequest, Networking::HttpRequest::CompletionResult ineResult)
		{
			if(ineResult == ChilliSource::Networking::HttpRequest::CompletionResult::k_completed)
			{
				CS_DEBUG_LOG("CTwitterPostSystem::OnRequestOAuthAccessTokenComplete() - Got response:\n"+inpRequest->GetResponseString());
				// Tell OAuth system to save access token and secret from web response
				mpOAuthSystem->ExtractOAuthTokenKeySecret(inpRequest->GetResponseString());
				SaveOAuthTokenKeyAndSecretKey();
				// Now that we've finish with all that shenanigans, let us Tweet...
				PostUsingMoFlow(msDesc);
			}
		}
		//------------------------------------------------------------------------
		/// Trys to load save token and secret keys
		//------------------------------------------------------------------------
		void TwitterPostSystem::TryLoadAuthenticationKeys()
		{
			std::string strTokenKey;
			std::string strSecretKey;

			ChilliSource::Core::CLocalDataStore& pLocalData = ChilliSource::Core::CLocalDataStore::GetSingleton();

			if(pLocalData.TryGetValue(Social::TwitterDataStore::kstrDataStoreKey[(u32)Social::TwitterDataStore::DataStoreKey::k_OAuthToken], strTokenKey))
			{
				mstrSavedOAuthTokenKey.assign(strTokenKey);
			}
			else
			{
				mstrSavedOAuthTokenKey.clear();
			}

			if(pLocalData.TryGetValue(Social::TwitterDataStore::kstrDataStoreKey[(u32)Social::TwitterDataStore::DataStoreKey::k_OAuthSecret], strSecretKey))
			{
				mstrSavedOAuthTokenSecret.assign(strSecretKey);
			}
			else
			{
				mstrSavedOAuthTokenSecret.clear();
			}

			if(mstrSavedOAuthTokenKey.empty() || mstrSavedOAuthTokenSecret.empty())
			{
				// At least one has failed to load
				mstrSavedOAuthTokenKey.clear();
				mstrSavedOAuthTokenSecret.clear();
			}
		}
		//------------------------------------------------------------------------
		/// Saves out current OAuth token key and secret key
		//------------------------------------------------------------------------
		void TwitterPostSystem::SaveOAuthTokenKeyAndSecretKey()
		{
			ChilliSource::Core::CLocalDataStore& pLocalData = ChilliSource::Core::CLocalDataStore::GetSingleton();

			// Save our OAuth Token and Secret
			mpOAuthSystem->GetOAuthTokenKey(mstrSavedOAuthTokenKey);
			mpOAuthSystem->GetOAuthTokenSecret(mstrSavedOAuthTokenSecret);

			pLocalData.SetValueForKey(Social::TwitterDataStore::kstrDataStoreKey[(u32)Social::TwitterDataStore::DataStoreKey::k_OAuthToken], mstrSavedOAuthTokenKey);
			pLocalData.SetValueForKey(Social::TwitterDataStore::kstrDataStoreKey[(u32)Social::TwitterDataStore::DataStoreKey::k_OAuthSecret], mstrSavedOAuthTokenSecret);
			pLocalData.Synchronise();
		}
	}
}

