/*
 *  FacebookAuthentication.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 01/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Social/Facebook/FacebookAuthentication.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookAuthentication.h>
#elif CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookAuthentication.h>
#endif

namespace ChilliSource
{
    namespace Social
    {
        CS_DEFINE_NAMEDTYPE(FacebookAuthenticationSystem);
        
        FacebookAuthenticationSystem* FacebookAuthenticationSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return new ChilliSource::iOS::FacebookAuthenticationSystem();
#elif CS_TARGETPLATFORM_ANDROID
            return new ChilliSource::Android::FacebookAuthenticationSystem();
#else
			return nullptr;
#endif
        }
    }
}

