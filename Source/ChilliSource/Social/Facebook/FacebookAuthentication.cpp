/*
 *  FacebookAuthentication.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 01/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include "FacebookAuthentication.h"

#ifdef TARGET_OS_IPHONE
#include <moFlo/Platform/iOS/Social/Facebook/FacebookAuthentication.h>
#elif TARGET_ANDROID
#include <moFlo/Platform/Android/Social/Facebook/FacebookAuthentication.h>
#endif

namespace moFlo
{
    namespace Social
    {
        DEFINE_NAMED_INTERFACE(IFacebookAuthenticationSystem);
        
        IFacebookAuthenticationSystem* IFacebookAuthenticationSystem::CreateSystem()
        {
#ifdef TARGET_OS_IPHONE
            return new moFlo::iOSPlatform::CFacebookAuthenticationSystem();
#elif TARGET_ANDROID
            return new moFlo::AndroidPlatform::CFacebookAuthenticationSystem();
#endif
			return NULL;
        }
    }
}

