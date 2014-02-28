/*
 *  FacebookAuthentication.h
 *  moFlo
 *
 *  Created by Stuart McGaw on 01/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_SOCIAL_FACEBOOK_FACEBOOKAUTHENTICATION_H_
#define _MOFLO_SOCIAL_FACEBOOK_FACEBOOKAUTHENTICATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/System.h>

#include <functional>

namespace ChilliSource
{
	namespace Social
	{
		/**
		 * See http://developers.facebook.com/docs/authentication/permissions/ for definitions of what these provide
         **/
		
		class FacebookAuthenticationSystem : public Core::System
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
                AuthenticateResult eResult;
                std::string strToken;
            };
            
            typedef std::function<void(const AuthenticateResponse&)> AuthenticationCompleteDelegate;
            
			CS_DECLARE_NAMEDTYPE(FacebookAuthenticationSystem);
			
            virtual ~FacebookAuthenticationSystem(){}
			
			virtual void Authenticate(const std::vector<std::string>& inastrReadPermissions = std::vector<std::string>(), const AuthenticationCompleteDelegate& inDelegate = nullptr) = 0;
			
			virtual bool IsSignedIn() const = 0;
			virtual std::string GetActiveToken() const = 0;
            
            virtual void AuthoriseReadPermissions(const std::vector<std::string> & inaReadPerms, const AuthenticationCompleteDelegate& inDelegate) = 0;
            virtual void AuthoriseWritePermissions(const std::vector<std::string> & inaWritePerms, const AuthenticationCompleteDelegate& inDelegate) = 0;
            
            virtual bool HasPermission(const std::string& instrPermission) const = 0;
			
			virtual void SignOut() = 0;
            
            virtual void PublishInstall() = 0;
            
            static FacebookAuthenticationSystem* CreateSystem();
		};
	}
}

#endif