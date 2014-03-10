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
        //---------------------------------------------------
        //---------------------------------------------------
        void LocalNotificationSystem::ScheduleNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority)
        {
            NotificationSPtr notification = std::make_shared<Notification>(in_id, in_params, in_priority);
            ScheduleNotification(notification, in_time);
        }
        //---------------------------------------------------
        //----------------------------------------------------
        void LocalNotificationSystem::ScheduleNotificationAfterTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority)
        {
            NotificationSPtr notification = std::make_shared<Notification>(in_id, in_params, in_priority);
            ScheduleNotification(notification, Application::Get()->GetSystemTime() + in_time);
        }
    }
}

