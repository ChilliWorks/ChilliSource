//
//  NotificationManager.h
//  Chilli Source
//  Created by Ian Copland on 10/03/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_CORE_NOTIFICATION_NOTIFICATIONMANAGER_H_
#define _CHILLISOURCE_CORE_NOTIFICATION_NOTIFICATIONMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Notification/Notification.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>
#include <limits>
#include <queue>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------
        /// Manages the various different notification systems,
        /// providing a single interface though which different
        /// notification types can be created and received. Notifications
        /// received though this are queued so that only one notification
        /// can be received at a time.
        //------------------------------------------------------------
        class NotificationManager final : public AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(NotificationManager);
            //-----------------------------------------------------
            /// Typedefs
            //-----------------------------------------------------
            using ReceivedDelegate = std::function<void(NotificationManager*, const NotificationCSPtr&)>;
            using DismissedDelegate = std::function<void(const NotificationCSPtr&)>;
            //-----------------------------------------------------
            /// Creates a new instance of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //-----------------------------------------------------
            static NotificationManagerUPtr Create();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The interface Id to check against.
            /// @return Whether this implements the passed in
            /// interface id.
            //-----------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------
            /// Schedules an App Notification which should fire
            /// as soon as it can. An App Notification is
            /// specfic to this process and will be lost if
            /// the application is restarted.
            ///
            /// @author Ian Copland
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
            /// @author Ian Copland
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
            /// @author Ian Copland
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
            /// @author Ian Copland
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
            /// @author Ian Copland
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
            /// within the given time period.
            ///
            /// @author Ian Copland
            ///
            /// @param [Out] The list of notifications.
            /// @param [Optional] The start time.
            /// @param [Optional] The end time.
            //--------------------------------------------------------
            void GetScheduledNotifications(std::vector<NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time = 0, TimeIntervalSecs in_period = std::numeric_limits<TimeIntervalSecs>::max()) const;
            //---------------------------------------------------
            /// Prevent any notifications with given ID type from
            /// firing.
            ///
            /// @author Ian Copland
            ///
            /// @param The ID of the notification to cancel.
            //---------------------------------------------------
            void CancelByID(Notification::ID in_id);
            //---------------------------------------------------
            /// Terminate all currently scheduled notifications.
            ///
            /// @author Ian Copland
            //---------------------------------------------------
            void CancelAll();
            //---------------------------------------------------
            /// Dismisses the given event. This must be called when
            /// an even is received to allow the next event though.
            ///
            /// @author
            //----------------------------------------------------
            void Dismiss(const NotificationCSPtr& in_notification);
            //---------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event which will be fired whenever a
            /// notification is received.
            //----------------------------------------------------
            IConnectableEvent<ReceivedDelegate>& GetReceivedEvent();
            //---------------------------------------------------
            /// @author Ian Copland
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
            //--------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author Ian Copland
            //--------------------------------------------------
            NotificationManager();
            //------------------------------------------------
            /// Gets handles to the different notification
            /// systems if they exist.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            void OnInit() override;
            //------------------------------------------------
            /// Called when a notification is received from
            /// any of the notification systems.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            void OnNotificationReceived(const NotificationCSPtr& in_notification);
            //------------------------------------------------
            /// Updates the notification queue and fires any
            /// notifications that are ready.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time.
            //------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //------------------------------------------------
            /// Cleans up the handles to the different
            /// notification systems.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            void OnDestroy() override;
            
            AppNotificationSystem* m_appNotificationSystem;
            LocalNotificationSystem* m_localNotificationSystem;
            RemoteNotificationSystem* m_remoteNotificationSystem;
            std::deque<NotificationContainer> m_notificationQueue;
            Event<ReceivedDelegate> m_receivedEvent;
            Event<DismissedDelegate> m_dismissedEvent;
            EventConnectionUPtr m_appReceivedConnection;
            EventConnectionUPtr m_localReceivedConnection;
            EventConnectionUPtr m_remoteReceivedConnection;
            f32 m_timeSinceNotification;
        };
    }
}

#endif
