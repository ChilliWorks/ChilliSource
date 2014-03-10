//
//  InputSystem.cpp
//  ChilliSource
//
//  Created by Stuart McGaw on 22/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Input/Base/InputSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Input/Base/InputSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Input/Base/InputSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <ChilliSource/Backend/Platform/Windows/Input/Base/InputSystem.h>
#endif

namespace ChilliSource
{
	namespace Input
	{
        CS_DEFINE_NAMEDTYPE(InputSystem);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        InputSystemUPtr InputSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return InputSystemUPtr(new iOS::InputSystem());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return InputSystemUPtr(new Android::InputSystem());
#endif
#ifdef CS_TARGETPLATFORM_WINDOWS
            return InputSystemUPtr(new Windows::InputSystem());
#endif
            return nullptr;
        }
	}
}

