/*
 *  FacebookPostSystem.h
 *  moFlo
 *
 *  Created by Stuart McGaw on 06/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_IOS_SOCIAL_FACEBOOK_FACEBOOKPOSTSYSTEM_H_
#define _MOFLO_PLATFORM_IOS_SOCIAL_FACEBOOK_FACEBOOKPOSTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Social/Facebook/FacebookAuthentication.h>
#include <ChilliSource/Social/Facebook/FacebookPostSystem.h>

namespace ChilliSource
{
	namespace iOS
	{
		class FacebookPostSystem : public Social::FacebookPostSystem
		{
		public:
			
			
            bool IsA(Core::InterfaceIDType inID) const override;
			
            void TryPost(const Social::FacebookPostDesc& insDesc, const PostResultDelegate& insResultCallback) override;
            
            void TrySendRequest(const Social::FacebookPostDesc& insDesc, const PostResultDelegate& insResultCallback, std::vector<std::string>& inastrRecommendedFriends) override;
			
		private:
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
            bool TryPostNative(const Social::FacebookPostDesc& insDesc);
            void PostWebBased(const Social::FacebookPostDesc& insDesc);
            void OnPublishPermissionAuthorised(const Social::FacebookAuthenticationSystem::AuthenticateResponse& insResponse);
			
            friend Social::FacebookPostSystemUPtr Social::FacebookPostSystem::Create(Social::FacebookAuthenticationSystem* in_authSystem);
            
            Social::FacebookAuthenticationSystem* mpAuthSystem;
            Social::FacebookPostDesc msPostDesc;
		};
	}
}

#endif
