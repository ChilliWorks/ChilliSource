//
//  AppNotificationSystem.h
//  MoFlowSkeleton
//
//  Created by Tag Games on 10/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_NOTIFICATIONS_APPNOTIFICATIONSYSTEM_H_
#define _CHILLISOURCE_CORE_NOTIFICATIONS_APPNOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/Notifications/Notification.h>

#include <queue>
#include <vector>

namespace ChilliSource
{
    namespace Core
    {
        //---------------------------------------------------------
        /// Schedules notifications within the application. These
        /// will be lost if the application is re-started.
        //---------------------------------------------------------
        class AppNotificationSystem final : public AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(AppNotificationSystem);
            //--------------------------------------------------
            /// Delegates
            //--------------------------------------------------
            using RecievedDelegate = std::function<void(const NotificationSPtr&)>;
            //--------------------------------------------------
            /// Creates a new instance of the system.
            ///
            /// @author I Copland
            ///
            /// @return The new instance.
            //--------------------------------------------------
            static AppNotificationSystemUPtr Create();
            //--------------------------------------------------
            /// Is A
            ///
            /// @return Whether this implements the passed in
            /// interface id.
            //--------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceID) const override;
            //--------------------------------------------------
            /// Enables and disables addition of app notifications.
            /// All existing notifications will be cancelled
            /// when this is disabled. This is enabled by default.
            ///
            /// @author I Copland
            ///
            /// @param Whether or not to enable the scheduling
            /// of app notifications.
            //---------------------------------------------------
            void SetEnabled(bool in_enabled);
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
            void ScheduleNotification(Notification::ID in_id, const ParamDictionary& in_params, Notification::Priority in_priority = Notification::Priority::k_standard);
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
            void ScheduleNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority = Notification::Priority::k_standard);
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
            void ScheduleNotificationAfterTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority = Notification::Priority::k_standard);
            //--------------------------------------------------
            /// @author I Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being recieved.
            //---------------------------------------------------
            IConnectableEvent<RecievedDelegate>& GetRecievedEvent();
            
        private:
            //--------------------------------------------------
            /// A container for information about a single
            /// notification, including the time it is scheduled
            /// for.
            //--------------------------------------------------
            struct NotificationContainer
            {
                NotificationSPtr m_notification;
                TimeIntervalSecs m_triggerTime;
            };
            using NotificationContainerSPtr = std::shared_ptr<NotificationContainer>;
            //--------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //--------------------------------------------------
            AppNotificationSystem();
            //--------------------------------------------------
            /// Processes the notifications, triggering any that
            /// are queued.
            ///
            /// @author I Copland
            ///
            /// @param The delta time.
            //--------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //--------------------------------------------------
            /// Cancels all existing notificatations.
            ///
            /// @author I Copland
            //--------------------------------------------------
            void CancelAll();
            
            bool m_enabled;
            std::vector<NotificationContainerSPtr> m_timedAppNotifications;
            Event<RecievedDelegate> m_recievedEvent;
        };
    }
}

#endif
