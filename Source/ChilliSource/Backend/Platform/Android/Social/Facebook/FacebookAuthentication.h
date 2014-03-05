/**
 * CFacebookAuthentication.h
 * moFlow
 *
 * Created by Robert Henning on 01/05/2012
 * Copyright �2012 Tag Games Limited - All rights reserved
 */

#ifndef _MOFLO_PLATFORM_ANDROID_SOCIAL_FACEBOOKAUTHENTICATION_H_
#define _MOFLO_PLATFORM_ANDROID_SOCIAL_FACEBOOKAUTHENTICATION_H_

#include <ChilliSource/Social/Facebook/FacebookAuthentication.h>
#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		class FacebookAuthenticationSystem : public Social::FacebookAuthenticationSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(FacebookAuthenticationSystem);

			FacebookAuthenticationSystem();

			bool IsA(Core::InterfaceIDType inID) const;

			void Authenticate(const std::vector<std::string>& inastrReadPermissions = std::vector<std::string>(), const AuthenticationCompleteDelegate& inDelegate = nullptr);

			bool IsSignedIn() const;
            std::string GetActiveToken() const;

            void AuthoriseWritePermissions(const std::vector<std::string> & inaWritePerms, const AuthenticationCompleteDelegate& inDelegate);
            void AuthoriseReadPermissions(const std::vector<std::string> & inaReadPerms, const AuthenticationCompleteDelegate& inDelegate);

            bool HasPermission(const std::string& instrPermission) const;

			void SignOut();

			void PublishInstall();

			void OnAuthenticationComplete(bool inbSuccess);
			void OnAuthoriseReadPermissionsComplete(bool inbSuccess);
			void OnAuthoriseWritePermissionsComplete(bool inbSuccess);

			const FacebookJavaInterfaceSPtr& GetJavaInterface() const;

		private:

			FacebookJavaInterfaceSPtr mpJavaInterface;

			AuthenticationCompleteDelegate mAuthDelegate;
			AuthenticationCompleteDelegate mAuthReadDelegate;
			AuthenticationCompleteDelegate mAuthWriteDelegate;
		};
	}
}

#endif
