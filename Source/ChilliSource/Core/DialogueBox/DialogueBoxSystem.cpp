//
//  DialogueBoxSystem.cpp
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/DialogueBox/DialogueBoxSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Core/DialogueBox/DialogueBoxSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Core/DialogueBox/DialogueBoxSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Core/DialogueBox/DialogueBoxSystem.h>
#endif

namespace ChilliSource
{
	namespace Core
	{
        CS_DEFINE_NAMEDTYPE(DialogueBoxSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        DialogueBoxSystemUPtr DialogueBoxSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return DialogueBoxSystemUPtr(new CSBackend::iOS::DialogueBoxSystem());
#elif defined CS_TARGETPLATFORM_ANDROID
            return DialogueBoxSystemUPtr(new CSBackend::Android::DialogueBoxSystem());
#elif defined CS_TARGETPLATFORM_WINDOWS
            return DialogueBoxSystemUPtr(new CSBackend::Windows::DialogueBoxSystem());
#else
            return nullptr;
#endif
        }
        //----------------------------------------------------
        //----------------------------------------------------
        DialogueBoxSystem::~DialogueBoxSystem()
        {
        }
	}
}
