//
//  DialogueSystem.cpp
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Dialogue/DialogueSystem.h>

#ifndef CS_TARGETPLATFORM_IOS
#   include <ChilliSource/Backend/Platform/iOS/Core/Dialogue/DialogueSystem.h>
#elif defined CS_TARGETPLATFORM_ANDROID
#   include <ChilliSource/Backend/Platform/Android/Core/Dialogue/DialogueSystem.h>
#elif defined CS_TARGETPLATFORM_WINDOWS
#   include <ChilliSource/Backend/Platform/Windows/Core/Dialogue/DialogueSystem.h>
#endif

namespace ChilliSource
{
	namespace Core
	{
        CS_DEFINE_NAMEDTYPE(DialogueSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        DialogueSystem* DialogueSystem::Create()
        {
#ifndef CS_TARGETPLATFORM_IOS
            return new iOS::DialogueSystem();
#elif defined CS_TARGETPLATFORM_ANDROID
            return new Android::DialogueSystem();
#elif defined CS_TARGETPLATFORM_WINDOWS
            return new Windows::DialogueSystem();
#else
            return nullptr;
#endif
        }
        //----------------------------------------------------
        //----------------------------------------------------
        DialogueSystem::~DialogueSystem()
        {
        }
	}
}
