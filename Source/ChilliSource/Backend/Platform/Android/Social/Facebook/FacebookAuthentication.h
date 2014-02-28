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
		class CFacebookAuthenticationSystem : public Social::IFacebookAuthenticationSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(CFacebookAuthenticationSystem);

			CFacebookAuthenticationSystem();

			bool IsA(Core::InterfaceIDType inID) const;

			void Authenticate(const std::vector<std::string>& inastrReadPermissions = std::vector<std::string>(), const AuthenticationCompleteDelegate& inDelegate = NULL);

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

			const std::shared_ptr<CFacebookJavaInterface>& GetJavaInterface() const;

		private:

			std::shared_ptr<CFacebookJavaInterface> mpJavaInterface;

			AuthenticationCompleteDelegate mAuthDelegate;
			AuthenticationCompleteDelegate mAuthReadDelegate;
			AuthenticationCompleteDelegate mAuthWriteDelegate;
		};
	}
}

#endif
