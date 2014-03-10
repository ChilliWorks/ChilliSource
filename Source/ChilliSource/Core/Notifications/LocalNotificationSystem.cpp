//
//  CNotificationFactory.h
//  MoFlow
//
//  Created by Ian Copland on 12/01/2012.
//  Copyright 202 Tag Games. All rights reserved.
//

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Notifications/LocalNotificationScheduler.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Core/Notification/LocalNotificationScheduler.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Core/Notification/LocalNotificationScheduler.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        //-------------------------------------------------------
        //-------------------------------------------------------
        LocalNotificationSchedulerUPtr LocalNotificationScheduler::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return LocalNotificationSchedulerUPtr(new iOS::LocalNotificationScheduler());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return LocalNotificationSchedulerUPtr(new Android::LocalNotificationScheduler());
#endif
            return nullptr;
        }
    }
}

