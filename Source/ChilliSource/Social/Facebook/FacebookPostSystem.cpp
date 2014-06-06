//
//  FacebookPostSystem.cpp
//  Chilli Source
//
//  Created by Stuart McGaw on 06/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Social/Facebook/FacebookPostSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Social/Facebook/FacebookPostSystem.h>
#elif CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Social/Facebook/FacebookPostSystem.h>
#endif

namespace ChilliSource
{
	namespace Social
	{
		CS_DEFINE_NAMEDTYPE(FacebookPostSystem);
        //---------------------------------------------------
        //---------------------------------------------------
        FacebookPostSystemUPtr FacebookPostSystem::Create(FacebookAuthenticationSystem* inpAuthSystem)
        {
#ifdef CS_TARGETPLATFORM_IOS
            return FacebookPostSystemUPtr(new iOS::FacebookPostSystem(inpAuthSystem));
#elif CS_TARGETPLATFORM_ANDROID
            return FacebookPostSystemUPtr(new Android::FacebookPostSystem(inpAuthSystem));
#else
			return FacebookPostSystemUPtr();
#endif
        }
	};
};