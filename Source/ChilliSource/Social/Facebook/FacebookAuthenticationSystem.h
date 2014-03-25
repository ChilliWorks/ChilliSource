//
//  FacebookAuthenticationSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 01/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_SOCIAL_FACEBOOK_FACEBOOKAUTHENTICATIONSYSTEM_H_
#define _CHILLISOURCE_SOCIAL_FACEBOOK_FACEBOOKAUTHENTICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>

namespace ChilliSource
{
	namespace Social
	{
        //------------------------------------------------
        /// Base class for logging in to Facebook using
        /// the back end Facebook SDK. Also handles
        /// the granting of read and write permissions
        ///
        /// @author S Downie
        //------------------------------------------------
		class FacebookAuthenticationSystem : public Core::AppSystem
		{
		public:
            
            enum class AuthenticateResult
            {
                k_success,
                k_failed,
                k_permissionMismatch
            };
            
            struct AuthenticateResponse
            {
                AuthenticateResult m_result;
                std::string m_token;
            };
            
            //------------------------------------------------
            /// Delegate call when authentication completes
            /// either successfully or not.
            ///
            /// @author S Downie
            ///
            /// @param Response containing result and token
            /// if successful
            //------------------------------------------------
            typedef std::function<void(const AuthenticateResponse&)> AuthenticationCompleteDelegate;
            
			CS_DECLARE_NAMEDTYPE(FacebookAuthenticationSystem);
			
            //------------------------------------------------
            /// Creates a new instance of the facebook
            /// authentication system.
            ///
            /// @author I Copland
            ///
            /// @return The new instance of the system.
            //------------------------------------------------
            static FacebookAuthenticationSystemUPtr Create();
            //------------------------------------------------
            /// Log the user into Facebook with the given
            /// read permissions. If no permissions are specified
            /// then only basic read permissions will be granted
            /// on authentication success
            ///
            /// @author S Downie
            ///
            /// @param Read permissions (http://developers.facebook.com/docs/authentication/permissions/)
            /// @param Result delegate
            //------------------------------------------------
			virtual void Authenticate(const std::vector<std::string>& in_readPermissions, const AuthenticationCompleteDelegate& in_delegate) = 0;
			//------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the user has authenticated with FB
            //------------------------------------------------
			virtual bool IsSignedIn() const = 0;
            //------------------------------------------------
            /// Request that the user grant additional read
            /// permissions (http://developers.facebook.com/docs/authentication/permissions/)
            ///
            /// @author S Downie
            ///
            /// @param Read permissions
            /// @param Result delegate
            //------------------------------------------------
            virtual void AuthoriseReadPermissions(const std::vector<std::string>& in_readPermissions, const AuthenticationCompleteDelegate& in_delegate) = 0;
            //------------------------------------------------
            /// Request that the user grant write
            /// permissions (http://developers.facebook.com/docs/authentication/permissions/)
            ///
            /// @author S Downie
            ///
            /// @param Write permissions
            /// @param Result delegate
            //------------------------------------------------
            virtual void AuthoriseWritePermissions(const std::vector<std::string>& in_writePermissions, const AuthenticationCompleteDelegate& in_delegate) = 0;
            //------------------------------------------------
            /// @author S Downie
            ///
            /// @param Permission to check
            ///
            /// @return Whether the given permission has been
            /// granted by the user
            //------------------------------------------------
            virtual bool HasPermission(const std::string& in_permission) const = 0;
			//------------------------------------------------
            /// Log the current user out of Facebook
            ///
            /// @author S Downie
            //------------------------------------------------
			virtual void SignOut() = 0;
            //------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S McGaw
            //------------------------------------------------
            virtual ~FacebookAuthenticationSystem(){}
            
        protected:
            //------------------------------------------------
            /// Private constructor to force use of factory
            /// method
            ///
            /// @author I Copland
            //------------------------------------------------
            FacebookAuthenticationSystem(){};
		};
	}
}

#endif