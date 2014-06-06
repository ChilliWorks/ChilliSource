/*
 *  HttpRequestSystem.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 23/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Networking/Http/HttpRequestSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Networking/Http/HttpRequestSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Networking/Http/HttpRequestSystem.h>
#endif

namespace ChilliSource
{
	namespace Networking
    {
		CS_DEFINE_NAMEDTYPE(HttpRequestSystem);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        HttpRequestSystemUPtr HttpRequestSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return HttpRequestSystemUPtr(new iOS::HttpRequestSystem());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return HttpRequestSystemUPtr(new Android::HttpRequestSystem());
#endif
#ifdef CS_TARGETPLATFORM_WINDOWS
            return HttpRequestSystemUPtr(new Windows::HttpRequestSystem());
#endif
            return nullptr;
        }
	}
}
