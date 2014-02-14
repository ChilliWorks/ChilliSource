/*
 *  TwitterPostSystem.h
 *  moFlow
 *
 *  Created by Scott Downie on 06/09/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_SOCIAL_TWITTER_TWITTER_POST_SYSTEM_H_
#define _MO_FLO_SOCIAL_TWITTER_TWITTER_POST_SYSTEM_H_

#include <ChilliSource/Core/Main/FastDelegate.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Networking/HttpConnectionSystem.h>
#include <ChilliSource/Networking/OAuthSystem.h>
#include <ChilliSource/Social/Twitter/TwitterAuthenticationActivity.h>

namespace moFlo
{
	namespace Social
	{
		namespace TwitterOAuthAPIURLs
		{
			/* Twitter OAuth API URLs */
			const std::string TWITTER_OAUTH_REQUEST_TOKEN_URL = "http://api.twitter.com/oauth/request_token";
			const std::string TWITTER_OAUTH_AUTHORIZE_URL = "http://twitter.com/oauth/authorize?oauth_token=";
			const std::string TWITTER_OAUTH_ACCESS_TOKEN_URL = "http://twitter.com/oauth/access_token";
		}
		
		namespace TwitterOAuthAPIHeaders
		{
			const std::string TWITTER_OAUTH_REQUEST_HEADER_AUTHORIZATION = "Authorization";
		}
		
		namespace TwitterURL
		{
			// Status
			const std::string TWITTER_STATUS_UPDATE_URL = "http://api.twitter.com/1/statuses/update.xml";
			const std::string TWITTER_STATUS_SHOW_URL = "http://api.twitter.com/1/statuses/show";
			const std::string TWITTER_STATUS_DESTROY_URL = "http://api.twitter.com/1/statuses/destroy";
			
			// Others go here...
		}
		
		namespace TwitterDefault
		{			
			// Used to build Twitter URLs
			const std::string TWITTER_STATUS_STRING = "status=";
			
			// Others go here...
			
			// URL separators
			const std::string TWITTER_URL_SEP_AMP = "&";
			const std::string TWITTER_URL_SEP_QUESTION_MARK = "?";
			
			const u32 kudwCharacterLimit = 140;
		}
		
		namespace TwitterDataStore
		{
			enum DATA_STORE_KEY
			{
				DATA_STORE_KEY_OAUTH_TOKEN,
				DATA_STORE_KEY_OAUTH_SECRET,
				NUM_DATA_STORE_KEYS,
			};
			
			const std::string	kstrDataStoreKey[NUM_DATA_STORE_KEYS]=
			{
				"TwitterOAuthTokenKey",
				"TwitterOAuthSecretKey",
			};
		}
		
		namespace TwitterPIN
		{
			const u32		kudwTwitterPINLength = 7;
		}

		struct TwitterPostDesc 
		{
            UTF8String strText;
            UTF8String strUrl;
            
            moCore::STORAGE_LOCATION eLocalImageStorageLocation;
            std::string strLocalImagePath;
		};
		
		class ITwitterPostSystem: public moFlo::Core::ISystem
		{
		public:
			
			DECLARE_NAMED_INTERFACE(ITwitterPostSystem);
			
			enum PostResult
			{
				PR_SUCCESS,
				PR_CANCELLED,	
				PR_FAILED,
				PR_NOT_AUTHENTICATED,
			};
			
			typedef fastdelegate::FastDelegate1<const PostResult&> PostResultDelegate;
			
            //Create platform specific system
            static ITwitterPostSystem* CreateSystem(Networking::IHttpConnectionSystem* inpHttpConnectionSystem, Networking::COAuthSystem* inpOAuthSystem);
            
			ITwitterPostSystem(Networking::IHttpConnectionSystem* inpHttpConnectionSystem,
							   Networking::COAuthSystem* inpOAuthSystem);
			//------------------------------------------------------------------------
			/// Destructor
			//------------------------------------------------------------------------
			~ITwitterPostSystem();
			//------------------------------------------------------------------------
            /// Init
            ///
            /// @param instrCustomerKey - Customer Key for Twitter application
            /// @param instrCustomerSecret - Customer Secret for Twitter application
            /// @return Init success or failure
            //------------------------------------------------------------------------
            virtual bool Init(const std::string& instrCustomerKey, const std::string& instrCustomerSecret);
            //------------------------------------------------------------------------
            /// Run the OAuth process and, if successful, leave the system in state
            /// ready to communicate with Twitter
            ///
            /// @return Returns true if successful, otherwise false
            //------------------------------------------------------------------------
            virtual bool Authenticate();
			//------------------------------------------------------------------------
			/// Is Image Post Supported
            ///
            /// @return Whether or not posting images using twitter is supported.
			//------------------------------------------------------------------------
			virtual bool IsImagePostSupported() const = 0;
			//------------------------------------------------------------------------
			/// Try Post
			///
			/// Attempt to post a tweet to Twitter
			///
			/// @param Twitter post description (i.e. image, text etc)
			/// @param Completion delegate
			/// @param Post handle identifier
            ///
            /// @return If the tweet exceeds the character limit imposed by Twitter
			//------------------------------------------------------------------------
			virtual bool TryPost(const TwitterPostDesc & insDesc, const PostResultDelegate & inResultCallback) = 0;
			//------------------------------------------------------------------------
			/// Delegate called when the user confirms entry of the PIN
			///
			/// @param PIN entered by user
			//------------------------------------------------------------------------
			virtual void OnPINComplete(const moFlo::Social::ITwitterAuthenticationActivity::AuthenticationPINResult &inResult) = 0;
			//------------------------------------------------------------------------
			/// Delegate called with the authorisation view is dismissed.
			///
			/// @param Pointer to IActivity that has been dismissed
			//------------------------------------------------------------------------
			virtual void OnAuthorisationDismissed(moFlo::IActivity* inpActivity) = 0;

        protected:
			//------------------------------------------------------------------------
			/// Post Using moFlow
			///
			/// Attempt to post a tweet to Twitter using the moFlow implementation
			///
			/// @param Twitter post description (i.e. image, text etc)
			///
			/// @return If the tweet exceeds the character limit imposed by Twitter
			//------------------------------------------------------------------------
			virtual bool PostUsingMoFlow(const Social::TwitterPostDesc & insDesc);
			//------------------------------------------------------------------------
			/// Delegate method called when a status update has completed
			///
			/// @param HTTP request
			/// @param The result from the request
			//------------------------------------------------------------------------
			virtual void OnStatusUpdateComplete(Networking::HttpRequestPtr inpRequest, Networking::IHttpRequest::CompletionResult ineResult);
			//------------------------------------------------------------------------
			/// Gets a request token and secret. This is used to authorise the user
			/// and get a PIN from Twitter
			///
			/// @param - outstrAuthoriseURL - This is the URL given from Twitter that
			///								  the user should visit to get a PIN
			/// @return Returns true if successful, otherwise false
			//------------------------------------------------------------------------
			virtual bool RequestOAuthToken(std::string& outstrAuthoriseURL);
			//------------------------------------------------------------------------
			/// Delegate method called when an OAuth token request has completed.
			/// This is used during the initial OAuth request before the user has
			/// verified the token. The response received here is a URL to a page
			/// on Twitter which displays a PIN which the user must enter into our
			/// app at a later time in order to us to get a usable OAuth token
			///
			/// @param HTTP request
			/// @param The result from the request
			//------------------------------------------------------------------------
			virtual void OnRequestOAuthTokenComplete(Networking::HttpRequestPtr inpRequest, Networking::IHttpRequest::CompletionResult ineResult);
			//------------------------------------------------------------------------
			/// Exchange our request token with an access token
			/// This step is called after the user has entered the PIN displayed
			/// during the initial authentication process. This stage uses are newly
			/// entered PIN to authenticate our request token which Twitter exchanges
			/// for an access token. We then save this access token and use it in
			/// future to talk to Twitter.
			///
			/// @return true if all went well, false otherwise
			//------------------------------------------------------------------------
			virtual bool RequestOAuthAccessToken();
			//------------------------------------------------------------------------
			/// Delegate method called when an OAuth access token request has
			/// completed.
			/// If the request has succeeded then we will be saving our access tokens
			/// here for use from now on.
			///
			/// @param HTTP request
			/// @param The result from the request
			//------------------------------------------------------------------------
			virtual void OnRequestOAuthAccessTokenComplete(moFlo::Networking::HttpRequestPtr inpRequest, moFlo::Networking::IHttpRequest::CompletionResult ineResult);
			//------------------------------------------------------------------------
			/// Trys to load save token and secret keys
			//------------------------------------------------------------------------
			virtual void TryLoadAuthenticationKeys();
			//------------------------------------------------------------------------
			/// Saves out current OAuth token key and secret key
			//------------------------------------------------------------------------
			virtual void SaveOAuthTokenKeyAndSecretKey();

			Networking::IHttpConnectionSystem*					mpHttpConnectionSystem;
			Networking::COAuthSystem*							mpOAuthSystem;

			// Customer Key and Secret are used to generate OAuth tokens
			// You can find them in the Twitter application. These values
			// should not be saved to disk.
			std::string											mstrCustomerKey;
			std::string											mstrCustomerSecret;

			// There are the OAuth tokens that we use to authenicate ourselves
			// to Twitter. Once received these are saved to disk.
			std::string											mstrSavedOAuthTokenKey;
			std::string											mstrSavedOAuthTokenSecret;

			// View used during initial authorisation phase
			moFlo::Social::ITwitterAuthenticationActivity*		mpAuthenticationView;

			// Post description
			// This is used to store the Tweet to send if the
			// user first needs to go though authentication
			Social::TwitterPostDesc								msDesc;
            
            PostResultDelegate									mCompletionDelegate;
		};
	}
}

#endif
