//
//  PlatformSystem.cpp
//  ChilliSource
//
//  Created by Ian Copland on 24/02/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Base/PlatformSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#   include <ChilliSource/Backend/Platform/iOS/Core/Base/PlatformSystem.h>
#elif defined CS_TARGETPLATFORM_ANDROID
#   include <ChilliSource/Backend/Platform/Android/PlatformSystem.h>
#elif defined TARGET_WINDOWS
#   include <ChilliSource/Backend/Platform/Windows/WindowsPlatformSystem.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        //-----------------------------------------
        //-----------------------------------------
        PlatformSystem* PlatformSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return new iOS::CPlatformSystem();
#elif defined CS_TARGETPLATFORM_ANDROID
            return new Android::CPlatformSystem();
#elif defined TARGET_WINDOWS
            return new Windows::CWindowsPlatformSystem();
#else
            return nullptr;
#endif
        }
    }
}
