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

#include <moFlo/Core/System.h>

namespace moFlo
{
	namespace Social
	{
		/**
		 * See http://developers.facebook.com/docs/authentication/permissions/ for definitions of what these provide
         **/
		
		class IFacebookAuthenticationSystem : public Core::ISystem
		{
		public:
            
            enum AuthenticateResult
            {
                AR_SUCCESS,
                AR_FAILED,
                AR_PERMISSION_MISMATCH,
            };
            
            struct AuthenticateResponse
            {
                AuthenticateResult eResult;
                std::string strToken;
            };
            
            typedef fastdelegate::FastDelegate1<const AuthenticateResponse&> AuthenticationCompleteDelegate;
            
			DECLARE_NAMED_INTERFACE(IFacebookAuthenticationSystem);
			
            virtual ~IFacebookAuthenticationSystem(){}
			
			virtual void Authenticate(const DYNAMIC_ARRAY<std::string>& inastrReadPermissions = DYNAMIC_ARRAY<std::string>(), const AuthenticationCompleteDelegate& inDelegate = NULL) = 0;
			
			virtual bool IsSignedIn() const = 0;
			virtual std::string GetActiveToken() const = 0;
            
            virtual void AuthoriseReadPermissions(const DYNAMIC_ARRAY<std::string> & inaReadPerms, const AuthenticationCompleteDelegate& inDelegate) = 0;
            virtual void AuthoriseWritePermissions(const DYNAMIC_ARRAY<std::string> & inaWritePerms, const AuthenticationCompleteDelegate& inDelegate) = 0;
            
            virtual bool HasPermission(const std::string& instrPermission) const = 0;
			
			virtual void SignOut() = 0;
            
            virtual void PublishInstall() = 0;
            
            static IFacebookAuthenticationSystem* CreateSystem();
		};
	}
}

#endif