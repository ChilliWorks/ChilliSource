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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Social/Facebook/FacebookAuthentication.h>

#include <FacebookSDK/FacebookSDK.h>

namespace ChilliSource
{
	namespace iOS
	{
		class FacebookAuthenticationSystem : public Social::FacebookAuthenticationSystem
		{
		public:
			//Define an interface for this platform specific version as the
			//app delegate needs to be able to query for this system specifically. (Avoiding system casts).
			CS_DECLARE_NAMEDTYPE(FacebookAuthenticationSystem);
            
            ~FacebookAuthenticationSystem();
			
            bool IsA(Core::InterfaceIDType inID) const override;
			
            void Authenticate( const std::vector<std::string>& inastrReadPermissions = std::vector<std::string>(), const FacebookAuthenticationSystem::AuthenticationCompleteDelegate& inDelegate = nullptr) override;
			
            bool IsSignedIn() const override;
            std::string GetActiveToken() const override;
            
            void AuthoriseReadPermissions(const std::vector<std::string> & inaReadPerms, const FacebookAuthenticationSystem::AuthenticationCompleteDelegate& inDelegate) override;
            void AuthoriseWritePermissions(const std::vector<std::string> & inaWritePerms, const FacebookAuthenticationSystem::AuthenticationCompleteDelegate& inDelegate) override;
            
            bool HasPermission(const std::string& instrPermission) const override;
			
            void SignOut() override;
            
            void PublishInstall() override;
			
		private:
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //----------------------------------------------------
            FacebookAuthenticationSystem(){};
            
            void CreateNewSession();
            bool TryResumeExisitingSession();
            bool OpenSession(bool bShowLogin);
            
            void OnSessionStateChanged(FBSession* inpSession, FBSessionState ineState);
            
            friend Social::FacebookAuthenticationSystemUPtr Social::FacebookAuthenticationSystem::Create();
            
            std::vector<std::string> mastrPermissions;
            
            FacebookAuthenticationSystem::AuthenticationCompleteDelegate mAuthenticateDelegate;
            FacebookAuthenticationSystem::AuthenticationCompleteDelegate mAuthoriseReadDelegate;
            FacebookAuthenticationSystem::AuthenticationCompleteDelegate mAuthoriseWriteDelegate;
            
            std::vector<std::string> maRequestedWritePermissions;
		};
	}
}

#endif
