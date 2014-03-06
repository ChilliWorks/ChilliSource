/**
 * CFacebookPostSystem.h
 * moFlow
 *
 * Created by Robert Henning on 03/05/2012
 * Copyright �2012 Tag Games Limited - All rights reserved
 */

#ifndef _MOFLO_PLATFORM_ANDROID_SOCIAL_FACEBOOK_FACEBOOKPOSTSYSTEM_H_
#define _MOFLO_PLATFORM_ANDROID_SOCIAL_FACEBOOK_FACEBOOKPOSTSYSTEM_H_

#include <ChilliSource/Social/Facebook/FacebookPostSystem.h>
#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookAuthentication.h>
#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		class FacebookPostSystem : public Social::FacebookPostSystem
		{
		public:

			bool IsA(Core::InterfaceIDType inID) const override;

			void TryPost(const Social::FacebookPostDesc& insDesc, const Social::FacebookPostSystem::PostResultDelegate& insResultCallback) override;
			void TrySendRequest(const Social::FacebookPostDesc& insDesc, const Social::FacebookPostSystem::PostResultDelegate& insResultCallback, std::vector<std::string>& inastrRecommendedFriends) override;

			void OnPostToFeedComplete(bool inbSuccess);
			void OnPostRequestComplete(bool inbSuccess);

		private:
			friend Social::FacebookPostSystemUPtr Social::FacebookPostSystem::Create(Social::FacebookAuthenticationSystem* in_authSystem);
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            ///
            /// @param The facebook authentication system.
            //----------------------------------------------------
            FacebookPostSystem(Social::FacebookAuthenticationSystem* inpAuthSystem);

			void Post(const Social::FacebookPostDesc& insDesc);
			void PostRequest(const Social::FacebookPostDesc& insDesc);
			void OnPublishPermissionAuthorised(const Social::FacebookAuthenticationSystem::AuthenticateResponse& insResponse);

		private:

			FacebookJavaInterfaceSPtr mpJavaInterface;

			Social::FacebookAuthenticationSystem* mpAuthSystem;
			Social::FacebookPostDesc msPostDesc;
		};
	}
}

#endif
