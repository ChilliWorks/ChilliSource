//
//  LocalNotificationSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 12/01/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Notifications/LocalNotificationSystem.h>

#include <ChilliSource/Core/Base/Application.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Core/Notification/LocalNotificationSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Core/Notification/LocalNotificationSystem.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        CS_DEFINE_NAMEDTYPE(LocalNotificationSystem);
        //-------------------------------------------------------
        //-------------------------------------------------------
        LocalNotificationSystemUPtr LocalNotificationSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return LocalNotificationSystemUPtr(new iOS::LocalNotificationSystem());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return LocalNotificationSystemUPtr(new Android::LocalNotificationSystem());
#endif
            return nullptr;
        }
    }
}

