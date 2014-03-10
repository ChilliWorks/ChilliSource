//
//  NotificationManager.h
//  Chilli Source
//
//  Created by I Copland on 10/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_NOTIFICATIONS_NOTIFICATIONMANAGER_H_
#define _CHILLISOURCE_CORE_NOTIFICATIONS_NOTIFICATIONMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Notifications/Notification.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------
        /// Manages the various different notification systems,
        /// providing a single interface though which different
        /// notification types can be created and recieved. Notifications
        /// recieved though this are queued so that only one notification
        /// can be recieved at a time.
        //------------------------------------------------------------
        class NotificationManager : public AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(NotificationManager);
            //-----------------------------------------------------
            /// Creates a new instance of the system.
            ///
            /// @author I Copland
            ///
            /// @return The new instance.
            //-----------------------------------------------------
            static AppNotificationSystemUPtr Create();
            //-----------------------------------------------------
            /// Is A
            ///
            /// @return Whether this implements the passed in
            /// interface id.
            //-----------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceID) const override;
            //--------------------------------------------------
            /// Immediately queue a notification. This will
            /// trigger when it reaches the front of the queue.
            ///
            /// @author I Copland
            ///
            /// @param The notification id
            /// @param The notification params.
            /// @param [Optional] The notification priority. Defaults
            /// to standard priority.
            //---------------------------------------------------
            void ScheduleAppNotification(Notification::ID in_id, const ParamDictionary& in_params, Notification::Priority in_priority = Notification::Priority::k_standard);
            //---------------------------------------------------
            /// Once the time is reached the notification will
            /// be inserted into the queue. Upon reaching the head
            /// of the queue it will trigger.
            ///
            /// @author I Copland
            ///
            /// @param The notification id
            /// @param The notification params.
            /// @param Time in seconds at which it should trigger.
            /// @param [Optional] The notification priority. Defaults
            /// to standard priority.
            //---------------------------------------------------
            void ScheduleAppNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority = Notification::Priority::k_standard);
            //---------------------------------------------------
            /// After the given number of second the notification
            /// will be inserted into the queue. Upon reaching
            /// the head of the queue it will trigger.
            ///
            /// @author I Copland
            ///
            /// @param The notification id
            /// @param The notification params.
            /// @param Time in seconds at which it should trigger.
            /// @param [Optional] The notification priority. Defaults
            /// to standard priority.
            //----------------------------------------------------
            void ScheduleAppNotificationAfterTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority = Notification::Priority::k_standard);
        };
    }
}

#endif
