//
//  Keyboard.cpp
//  Chilli Source
//
//  Created by I Copland on 25/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Input/Keyboard/Keyboard.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#   include <ChilliSource/Backend/Platform/Android/Input/Keyboard/Keyboard.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#   include <ChilliSource/Backend/Platform/iOS/Input/Keyboard/Keyboard.h>
#endif

namespace ChilliSource
{
    namespace Input
    {
        CS_DEFINE_NAMEDTYPE(Keyboard);
        //-------------------------------------------------------
        //-------------------------------------------------------
        KeyboardUPtr Keyboard::Create()
        {
#if defined CS_TARGETPLATFORM_ANDROID
            return KeyboardUPtr(new Android::Keyboard());
#elif defined CS_TARGETPLATFORM_IOS
            return KeyboardUPtr(new iOS::Keyboard());
#else
            return nullptr;
#endif

        }
    }
}
