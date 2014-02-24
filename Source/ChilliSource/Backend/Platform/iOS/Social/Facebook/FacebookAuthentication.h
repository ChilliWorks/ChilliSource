/*
 *  FacebookAuthentication.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 02/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_IOS_SOCIAL_FACEBOOKAUTHENTICATION_H_
#define _MOFLO_PLATFORM_IOS_SOCIAL_FACEBOOKAUTHENTICATION_H_

#include <ChilliSource/Social/Facebook/FacebookAuthentication.h>

#include <FacebookSDK/FacebookSDK.h>

namespace ChilliSource
{
	namespace iOS
	{
		class CFacebookAuthenticationSystem : public Social::IFacebookAuthenticationSystem
		{
		public:
			//Define an interface for this platform specific version as the
			//app delegate needs to be able to query for this system specifically. (Avoiding system casts).
			DECLARE_NAMED_INTERFACE(CFacebookAuthenticationSystem);
            
            ~CFacebookAuthenticationSystem();
			
            bool IsA(Core::InterfaceIDType inID) const;
			
            void Authenticate( const DYNAMIC_ARRAY<std::string>& inastrReadPermissions = DYNAMIC_ARRAY<std::string>(), const IFacebookAuthenticationSystem::AuthenticationCompleteDelegate& inDelegate = NULL);
			
            bool IsSignedIn() const;
            std::string GetActiveToken() const;
            
            void AuthoriseReadPermissions(const DYNAMIC_ARRAY<std::string> & inaReadPerms, const IFacebookAuthenticationSystem::AuthenticationCompleteDelegate& inDelegate);
            void AuthoriseWritePermissions(const DYNAMIC_ARRAY<std::string> & inaWritePerms, const IFacebookAuthenticationSystem::AuthenticationCompleteDelegate& inDelegate);
            
            bool HasPermission(const std::string& instrPermission) const;
			
            void SignOut();
            
            void PublishInstall();
			
		private:
            void CreateNewSession();
            bool TryResumeExisitingSession();
            bool OpenSession(bool bShowLogin);
            
            void OnSessionStateChanged(FBSession* inpSession, FBSessionState ineState);
            
            DYNAMIC_ARRAY<std::string> mastrPermissions;
            
            IFacebookAuthenticationSystem::AuthenticationCompleteDelegate mAuthenticateDelegate;
            IFacebookAuthenticationSystem::AuthenticationCompleteDelegate mAuthoriseReadDelegate;
            IFacebookAuthenticationSystem::AuthenticationCompleteDelegate mAuthoriseWriteDelegate;
            
            DYNAMIC_ARRAY<std::string> maRequestedWritePermissions;
		};
	}
}

#endif
