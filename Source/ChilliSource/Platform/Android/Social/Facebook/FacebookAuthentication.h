/**
 * CFacebookAuthentication.h
 * moFlow
 *
 * Created by Robert Henning on 01/05/2012
 * Copyright �2012 Tag Games Limited - All rights reserved
 */

#ifndef _MOFLO_PLATFORM_ANDROID_SOCIAL_FACEBOOKAUTHENTICATION_H_
#define _MOFLO_PLATFORM_ANDROID_SOCIAL_FACEBOOKAUTHENTICATION_H_

#include <moFlo/Social/Facebook/FacebookAuthentication.h>
#include <moFlo/Platform/Android/Social/Facebook/FacebookJavaInterface.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		class CFacebookAuthenticationSystem : public Social::IFacebookAuthenticationSystem
		{
		public:

			DECLARE_NAMED_INTERFACE(CFacebookAuthenticationSystem);

			CFacebookAuthenticationSystem();

			bool IsA(Core::InterfaceIDType inID) const;

			void Authenticate(const DYNAMIC_ARRAY<std::string>& inastrReadPermissions = DYNAMIC_ARRAY<std::string>(), const AuthenticationCompleteDelegate& inDelegate = NULL);

			bool IsSignedIn() const;
            std::string GetActiveToken() const;

            void AuthoriseWritePermissions(const DYNAMIC_ARRAY<std::string> & inaWritePerms, const AuthenticationCompleteDelegate& inDelegate);
            void AuthoriseReadPermissions(const DYNAMIC_ARRAY<std::string> & inaReadPerms, const AuthenticationCompleteDelegate& inDelegate);

            bool HasPermission(const std::string& instrPermission) const;

			void SignOut();

			void PublishInstall();

			void OnAuthenticationComplete(bool inbSuccess);
			void OnAuthoriseReadPermissionsComplete(bool inbSuccess);
			void OnAuthoriseWritePermissionsComplete(bool inbSuccess);

			const SHARED_PTR<CFacebookJavaInterface>& GetJavaInterface() const;

		private:

			SHARED_PTR<CFacebookJavaInterface> mpJavaInterface;

			AuthenticationCompleteDelegate mAuthDelegate;
			AuthenticationCompleteDelegate mAuthReadDelegate;
			AuthenticationCompleteDelegate mAuthWriteDelegate;
		};
	}
}

#endif
