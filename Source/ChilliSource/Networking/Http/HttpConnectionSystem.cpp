/*
 *  HttpConnectionSystem.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 23/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Networking/Http/HttpConnectionSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Networking/Http/HttpConnectionSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpConnectionSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <ChilliSource/Backend/Platform/Windows/Networking/Http/HttpConnectionSystem.h>
#endif

namespace ChilliSource
{
	namespace Networking
    {
		CS_DEFINE_NAMEDTYPE(HttpConnectionSystem);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        HttpConnectionSystemUPtr HttpConnectionSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return HttpConnectionSystemUPtr(new iOS::HttpConnectionSystem());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return HttpConnectionSystemUPtr(new Android::HttpConnectionSystem());
#endif
#ifdef CS_TARGETPLATFORM_WINDOWS
            return HttpConnectionSystemUPtr(new Windows::HttpConnectionSystem());
#endif
            return nullptr;
        }
	}
}
