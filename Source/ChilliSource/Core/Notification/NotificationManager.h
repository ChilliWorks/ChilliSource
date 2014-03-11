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
#include <ChilliSource/Core/Notification/Notification.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <limits>
#include <queue>

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
        class NotificationManager final : public AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(NotificationManager);
            //-----------------------------------------------------
            /// Typedefs
            //-----------------------------------------------------
            using RecievedDelegate = std::function<void(const NotificationSPtr&)>;
            using DismissedDelegate = std::function<void(const NotificationSPtr&)>;
            //-----------------------------------------------------
            /// Creates a new instance of the system.
            ///
            /// @author I Copland
            ///
            /// @return The new instance.
            //-----------------------------------------------------
            static NotificationManagerUPtr Create();
            //-----------------------------------------------------
            /// @author I Copland
            ///
            /// @return Whether this implements the passed in
            /// interface id.
            //-----------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceID) const override;
            //--------------------------------------------------
            /// Schedules an App Notification which should fire
            /// as soon as it can. An App Notification is
            /// specfic to this process and will be lost if
            /// the application is restarted.
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
            /// Schedules an App Notification which should fire
            /// at the given time. An App Notification is specific
            /// to this process and will be lost if the application
            /// is restarted.
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
            /// Schedules an App Notification should shoud fire
            /// after the given time. An App Notification is
            /// specific to this process and will be lost if
            /// the application is restarted.
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
            //---------------------------------------------------
            /// Schedules a Local Notifcation which should fire
            /// at the given time. A Local Notification uses
            /// the system specfic notification alarms and can
            /// be received even if it was scheduled during a
            /// previous instance of the application.
            ///
            /// @author I Copland
            ///
            /// @param The notification id
            /// @param The notification params.
            /// @param Time in seconds at which it should trigger.
            /// @param [Optional] The notification priority. Defaults
            /// to standard priority.
            //---------------------------------------------------
            void ScheduleLocalNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority = Notification::Priority::k_standard);
            //---------------------------------------------------
            /// Schedules a Local Notifcation which should fire
            /// after the given time. A Local Notification uses
            /// the system specfic notification alarms and can
            /// be received even if it was scheduled during a
            /// previous instance of the application.
            ///
            /// @author I Copland
            ///
            /// @param The notification id
            /// @param The notification params.
            /// @param Time in seconds at which it should trigger.
            /// @param [Optional] The notification priority. Defaults
            /// to standard priority.
            //----------------------------------------------------
            void ScheduleLocalNotificationAfterTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority = Notification::Priority::k_standard);
            //--------------------------------------------------------
            /// Builds a list of all notifications currently scheduled
            /// within the given time peroid.
            ///
            /// @author I Copland
            ///
            /// @param [Out] The list of notifications.
            /// @param [Optional] The start time.
            /// @param [Optional] The end time.
            //--------------------------------------------------------
            void GetScheduledNotifications(std::vector<NotificationSPtr>& out_notifications, TimeIntervalSecs in_time = 0, TimeIntervalSecs in_peroid = std::numeric_limits<TimeIntervalSecs>::max());
            //---------------------------------------------------
            /// Prevent any notifications with given ID type from
            /// firing.
            ///
            /// @author I Copland
            ///
            /// @param The ID of the notification to cancel.
            //---------------------------------------------------
            void CancelByID(Notification::ID in_id);
            //---------------------------------------------------
            /// Terminate all currently scheduled notifications.
            ///
            /// @author I Copland
            //---------------------------------------------------
            void CancelAll();
            //---------------------------------------------------
            /// Dismisses the given event. This must be called when
            /// an even is received to allow the next event though.
            ///
            /// @author
            //----------------------------------------------------
            void Dismiss(const NotificationSPtr& in_notification);
            //---------------------------------------------------
            /// @author I Copland
            ///
            /// @return An event which will be fired whenever a
            /// notification is received.
            //----------------------------------------------------
            IConnectableEvent<RecievedDelegate>& GetRecievedEvent();
            //---------------------------------------------------
            /// @author I Copland
            ///
            /// @return An event which will be fired whenever a
            /// notification is dismissed.
            //----------------------------------------------------
            IConnectableEvent<DismissedDelegate>& GetDismissedEvent();
        private:
            //--------------------------------------------------
            /// A container for information about a single
            /// notification, including the time it is scheduled
            /// for.
            //--------------------------------------------------
            struct NotificationContainer
            {
                NotificationSPtr m_notification;
                bool m_dismissed;
                bool m_triggered;
            };
            using NotificationContainerSPtr = std::shared_ptr<NotificationContainer>;
            //--------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //--------------------------------------------------
            NotificationManager();
            //------------------------------------------------
            /// Gets handles to the different notification
            /// systems if they exist.
            ///
            /// @author I Copland
            //------------------------------------------------
            void OnInit() override;
            //------------------------------------------------
            /// Called when a notification is received from
            /// any of the notification systems.
            ///
            /// @author I Copland
            //------------------------------------------------
            void OnNotificationRecieved(const NotificationSPtr& in_notification);
            //------------------------------------------------
            /// Updates the notification queue and fires any
            /// notifications that are ready.
            ///
            /// @author I Copland
            ///
            /// @param The delta time.
            //------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //------------------------------------------------
            /// Cleans up the handles to the different
            /// notification systems.
            ///
            /// @author I Copland
            //------------------------------------------------
            void OnDestroy() override;
            
            AppNotificationSystem* m_appNotificationSystem;
            LocalNotificationSystem* m_localNotificationSystem;
            RemoteNotificationSystem* m_remoteNotificationSystem;
            std::deque<NotificationContainerSPtr> m_notificationQueue;
            Event<RecievedDelegate> m_recievedEvent;
            Event<DismissedDelegate> m_dismissedEvent;
            ConnectionUPtr m_appRecievedConnection;
            ConnectionUPtr m_localRecievedConnection;
            ConnectionUPtr m_remoteRecievedConnection;
            f32 m_timeSinceNotification;
        };
    }
}

#endif
