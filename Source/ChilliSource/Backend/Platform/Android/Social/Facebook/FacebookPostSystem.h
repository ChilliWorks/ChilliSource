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
	using namespace ChilliSource::Social;

	namespace Android
	{
		class CFacebookPostSystem : public Social::IFacebookPostSystem
		{
		public:
			CFacebookPostSystem(Social::IFacebookAuthenticationSystem* inpAuthSystem);

			bool IsA(Core::InterfaceIDType inID) const;

			void TryPost(const Social::FacebookPostDesc& insDesc, const IFacebookPostSystem::PostResultDelegate& insResultCallback);
			void TrySendRequest(const Social::FacebookPostDesc& insDesc, const PostResultDelegate& insResultCallback, std::vector<std::string>& inastrRecommendedFriends);

			void OnPostToFeedComplete(bool inbSuccess);
			void OnPostRequestComplete(bool inbSuccess);

		private:

			void Post(const Social::FacebookPostDesc& insDesc);
			void PostRequest(const Social::FacebookPostDesc& insDesc);
			void OnPublishPermissionAuthorised(const Social::IFacebookAuthenticationSystem::AuthenticateResponse& insResponse);

		private:

			SHARED_PTR<CFacebookJavaInterface> mpJavaInterface;

			Social::IFacebookAuthenticationSystem* mpAuthSystem;
			Social::FacebookPostDesc msPostDesc;
		};
	}
}

#endif
