//
//  FacebookAuthenticationSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 01/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Social/Facebook/FacebookAuthenticationSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookAuthenticationSystem.h>
#elif CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookAuthenticationSystem.h>
#endif

namespace ChilliSource
{
    namespace Social
    {
        CS_DEFINE_NAMEDTYPE(FacebookAuthenticationSystem);
        
        //------------------------------------------------
        //------------------------------------------------
        FacebookAuthenticationSystemUPtr FacebookAuthenticationSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return FacebookAuthenticationSystemUPtr(new ChilliSource::iOS::FacebookAuthenticationSystem());
#elif CS_TARGETPLATFORM_ANDROID
            return FacebookAuthenticationSystemUPtr(new ChilliSource::Android::FacebookAuthenticationSystem());
#else
			return FacebookAuthenticationSystemUPtr();
#endif
        }
    }
}

