//
//  FacebookAuthenticationSystem.h
//  Chilli Source
//  Created by Scott Downie on 01/06/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_SOCIAL_FACEBOOK_FACEBOOKAUTHENTICATIONSYSTEM_H_
#define _CHILLISOURCE_SOCIAL_FACEBOOK_FACEBOOKAUTHENTICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Delegate/ConnectableDelegate.h>
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
            
            //-------------------------------------------------
            /// @author S McGaw
            ///
            /// Hold the response from any authentication
            /// request
            //-------------------------------------------------
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
            using AuthenticationCompleteDelegate = Core::ConnectableDelegate<void(const AuthenticateResponse&)>;
            
			CS_DECLARE_NAMEDTYPE(FacebookAuthenticationSystem);
			
            //------------------------------------------------
            /// Creates a new instance of the facebook
            /// authentication system.
            ///
            /// @author Ian Copland
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
            /// @param Connection to result delegate
            //------------------------------------------------
			virtual void Authenticate(const std::vector<std::string>& in_readPermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection) = 0;
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
            /// @param Connection to result delegate
            //------------------------------------------------
            virtual void AuthoriseReadPermissions(const std::vector<std::string>& in_readPermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection) = 0;
            //------------------------------------------------
            /// Request that the user grant write
            /// permissions (http://developers.facebook.com/docs/authentication/permissions/)
            ///
            /// @author S Downie
            ///
            /// @param Write permissions
            /// @param Connection to result delegate
            //------------------------------------------------
            virtual void AuthoriseWritePermissions(const std::vector<std::string>& in_writePermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection) = 0;
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
		};
	}
}

#endif