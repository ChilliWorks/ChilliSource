//
//  TwitterPostSystem.h
//  Chilli Source
//
//  Created by Robert Henning on 06/09/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_SOCIAL_TWITTER_TWITTERPOSTSYSTEM_H_
#define _CHILLISOURCE_SOCIAL_TWITTER_TWITTERPOSTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Networking/Http/HttpConnectionSystem.h>
#include <ChilliSource/Core/Cryptographic/OAuthSystem.h>
#include <ChilliSource/Social/Twitter/TwitterAuthenticationActivity.h>

#include <functional>

namespace ChilliSource
{
	namespace Social
	{
        //-------------------------------------------------------------------
        /// System that allows the user to post tweets to Twitter. User
        /// must first authenticate with the Twitter app key and secret before
        /// posting is allowed. Posts are restricted to standard twitter
        /// character limits. NOTE: currently only one post is allowed at a time.
        ///
        /// @author R Henning
        //-------------------------------------------------------------------
		class TwitterPostSystem: public ChilliSource::Core::System
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(TwitterPostSystem);
            
            struct PostDesc
            {
                std::string m_text;
                std::string m_url;
                
                Core::StorageLocation m_localImageStorageLocation;
                std::string m_localImagePath;
            };

            enum class AuthenticationResult
			{
                k_success,
                k_failed
			};
			
			enum class PostResult
			{
                k_success,
                k_cancelled,
                k_failed,
                k_notAuthenticated
			};
			
            typedef std::function<void(const AuthenticationResult&)> AuthenticationResultDelegate;
			typedef std::function<void(const PostResult&)> PostResultDelegate;
			
            //------------------------------------------------
			/// Create a new instance of the Twitter Post
            /// system.
            ///
            /// @author I Copland
            ///
            /// @param The http connection system.
            /// @param The OAuth system.
            ///
            /// @return The new instance.
			//------------------------------------------------
            static TwitterPostSystemUPtr Create(Networking::HttpConnectionSystem* in_httpConnectionSystem, Core::OAuthSystem* in_oauthSystem);
            //------------------------------------------------------------------------
            /// Run the OAuth process and, if successful, leave the system in state
            /// ready to communicate with Twitter. Result will be returned through
            /// the delegate
            ///
            /// @author R Henning
            ///
            /// @param Key from Twitter application
            /// @param Secret from Twitter application
            /// @param Authentication delegate
            //------------------------------------------------------------------------
            virtual void Authenticate(const std::string& in_key, const std::string& in_secret, const AuthenticationResultDelegate& in_delegate) = 0;
            //------------------------------------------------------------------------
			/// @author S Downie
            ///
            /// @return Whether or not the use has authenticated successfully
			//------------------------------------------------------------------------
			inline bool IsAuthenticated() const
            {
                return m_isAuthenticated;
            }
			//------------------------------------------------------------------------
			/// @author I Copland
            ///
            /// @return Whether or not posting images using twitter is supported.
			//------------------------------------------------------------------------
			virtual bool IsImagePostSupported() const = 0;
			//------------------------------------------------------------------------
			/// Attempt to post a tweet to Twitter. Delegate will be called with
            /// failure or success result
            ///
            /// @author R Henning
			///
			/// @param Twitter post description (i.e. image, text etc)
			/// @param Completion delegate
			/// @param Post handle identifier
			//------------------------------------------------------------------------
			virtual void Post(const PostDesc& in_desc, const PostResultDelegate& in_resultCallback) = 0;
			//------------------------------------------------------------------------
			/// Destructor
            ///
            /// @author R Henning
			//------------------------------------------------------------------------
			virtual ~TwitterPostSystem(){};
        protected:
            //-----------------------------------------------
			/// Protected constructor to force the use of
            /// the factory method.
            ///
            /// @author I Copland
            ///
            /// @param The http connection system.
            /// @param The OAuth system.
			//-----------------------------------------------
            TwitterPostSystem(Networking::HttpConnectionSystem* in_httpConnectionSystem, Core::OAuthSystem* in_oauthSystem);
            //------------------------------------------------------------------------
			/// Trys to load saved token and secret keys
            ///
            /// @author R Henning
			//------------------------------------------------------------------------
            void LoadAuthenticationKeys();
			//------------------------------------------------------------------------
			/// Saves out current OAuth token key and secret key
            ///
            /// @author R Henning
			//------------------------------------------------------------------------
            void SaveAuthenticationKeys();
            //------------------------------------------------------------------------
			/// Attempt to post a tweet to Twitter using the CS implementation.
            /// Completion delegate will be called with resule
            ///
            /// @author R Henning
			///
			/// @param Twitter post description (i.e. image, text etc)
			//------------------------------------------------------------------------
            void PostUsingChilliSource(const PostDesc& in_desc);
            //------------------------------------------------------------------------
			/// Exchange our request token with an access token
			/// This step is called after the user has entered the PIN displayed
			/// during the initial authentication process. This stage uses are newly
			/// entered PIN to authenticate our request token which Twitter exchanges
			/// for an access token. We then save this access token and use it in
			/// future to talk to Twitter.
            ///
            /// @author R Henning
			//------------------------------------------------------------------------
            void RequestOAuthAccessToken();
            //------------------------------------------------------------------------
			/// Gets a request token and secret. This is used to authorise the user
			/// and get a PIN from Twitter
            ///
            /// @author R Henning
			///
            /// @return Success or failure
			//------------------------------------------------------------------------
            bool RequestOAuthToken();
        private:
			//------------------------------------------------------------------------
			/// Delegate method called when a status update has completed
            ///
            /// @author R Henning
			///
			/// @param HTTP request
			/// @param The result from the request
			//------------------------------------------------------------------------
            void OnStatusUpdateComplete(Networking::HttpRequest* in_request, Networking::HttpRequest::CompletionResult in_result);
			//------------------------------------------------------------------------
			/// Delegate method called when an OAuth token request has completed.
			/// This is used during the initial OAuth request before the user has
			/// verified the token. The response received here is a URL to a page
			/// on Twitter which displays a PIN which the user must enter into our
			/// app at a later time in order to us to get a usable OAuth token
            ///
            /// @author R Henning
			///
			/// @param HTTP request
			/// @param The result from the request
			//------------------------------------------------------------------------
            void OnRequestOAuthTokenComplete(Networking::HttpRequest* in_request, Networking::HttpRequest::CompletionResult in_result);
			//------------------------------------------------------------------------
			/// Delegate method called when an OAuth access token request has
			/// completed.
			/// If the request has succeeded then we will be saving our access tokens
			/// here for use from now on.
            ///
            /// @author R Henning
			///
			/// @param HTTP request
			/// @param The result from the request
			//------------------------------------------------------------------------
			virtual void OnRequestOAuthAccessTokenComplete(Networking::HttpRequest* in_request, Networking::HttpRequest::CompletionResult in_result);

        private:
            
			Networking::HttpConnectionSystem* m_httpConnectionSystem;
			
        protected:
            
            AuthenticationResultDelegate m_authDelegate;
            PostResultDelegate m_postDelegate;
            
            Core::OAuthSystem* m_oauthSystem;
            
            bool m_isAuthenticated;
            
            // There are the OAuth tokens that we use to authenicate ourselves
			// to Twitter. Once received these are saved to disk.
			std::string	m_savedOAuthTokenKey;
			std::string	m_savedOAuthTokenSecret;
            
            // View used during initial authorisation phase
			TwitterAuthenticationActivityUPtr m_authenticationView;
		};
	}
}

#endif
