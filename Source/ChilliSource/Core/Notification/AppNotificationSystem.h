//
//  AppNotificationSystem.h
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

#ifndef _CHILLISOURCE_CORE_NOTIFICATION_APPNOTIFICATIONSYSTEM_H_
#define _CHILLISOURCE_CORE_NOTIFICATION_APPNOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/Notification/Notification.h>

#include <functional>
#include <limits>
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
            using ReceivedDelegate = std::function<void(const NotificationCSPtr&)>;
            //--------------------------------------------------
            /// Creates a new instance of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //--------------------------------------------------
            static AppNotificationSystemUPtr Create();
            //--------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The interface ID to check against.
            /// @return Whether this implements the passed in
            /// interface id.
            //--------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------
            /// Enables and disables addition of app notifications.
            /// All existing notifications will be cancelled
            /// when this is disabled. This is enabled by default.
            ///
            /// @author Ian Copland
            ///
            /// @param Whether or not to enable the scheduling
            /// of app notifications.
            //---------------------------------------------------
            void SetEnabled(bool in_enabled);
            //---------------------------------------------------
            /// Once the time is reached the notification will
            /// be inserted into the queue. Upon reaching the head
            /// of the queue it will trigger.
            ///
            /// @author Ian Copland
            ///
            /// @param The notification id
            /// @param The notification params.
            /// @param Time in seconds at which it should trigger.
            /// @param [Optional] The notification priority. Defaults
            /// to standard priority.
            //---------------------------------------------------
            void ScheduleNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority = Notification::Priority::k_standard);
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
            //--------------------------------------------------------
            /// Prevent any notifications with given ID type from
            /// firing.
            ///
            /// @author Ian Copland
            ///
            /// @param ID type
            //--------------------------------------------------------
            void CancelByID(Notification::ID in_id);
            //--------------------------------------------------------
            /// Terminate all currently scheduled notifications.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            void CancelAll();
            //--------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being received.
            //---------------------------------------------------
            IConnectableEvent<ReceivedDelegate>& GetReceivedEvent();
            
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
            //--------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author Ian Copland
            //--------------------------------------------------
            AppNotificationSystem();
            //--------------------------------------------------
            /// Processes the notifications, triggering any that
            /// are queued.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time.
            //--------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            
            bool m_enabled;
            std::vector<NotificationContainer> m_notifications;
            Event<ReceivedDelegate> m_receivedEvent;
        };
    }
}

#endif
