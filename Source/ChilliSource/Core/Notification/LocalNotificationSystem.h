//
//  LocalNotificationSystem.h
//  Chilli Source
//  Created by Ian Copland on 12/01/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_
#define _CHILLISOURCE_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/Notification/Notification.h>

#include <functional>
#include <limits>

namespace ChilliSource
{
    namespace Core
    {
        //-------------------------------------------------------
        /// Schedules notifications using the platform specific
        /// notification alaram. Notifications scheduled using
        /// this can still be received even if the Application
        /// is restarted.
        //-------------------------------------------------------
        class LocalNotificationSystem : public AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(LocalNotificationSystem);
            //--------------------------------------------------
            /// Delegates
            //--------------------------------------------------
            using ReceivedDelegate = std::function<void(const NotificationCSPtr&)>;
            //-------------------------------------------------------
            /// Create the platform dependent backend
            ///
            /// @author S Downie
            ///
            /// @return New backend instance
            //-------------------------------------------------------
            static LocalNotificationSystemUPtr Create();
            //--------------------------------------------------
            /// Enables and disables addition of local notifications.
            /// All existing notifications will be cancelled
            /// when this is disabled. This is enabled by default.
            ///
            /// @author Ian Copland
            ///
            /// @param Whether or not to enable the scheduling
            /// of app notifications.
            //---------------------------------------------------
            virtual void SetEnabled(bool in_enabled) = 0;
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
            virtual void ScheduleNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority = Notification::Priority::k_standard) = 0;
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
            virtual void GetScheduledNotifications(std::vector<NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time = 0, TimeIntervalSecs in_period = std::numeric_limits<TimeIntervalSecs>::max()) const = 0;
            //--------------------------------------------------------
            /// Cancel By ID
            ///
            /// Prevent any notifications with given ID type from firing
            ///
            /// @param ID type
            //--------------------------------------------------------
            virtual void CancelByID(Notification::ID inID) = 0;
            //--------------------------------------------------------
            /// Cancel All
            ///
            /// Terminate all currently scheduled notifications
            //--------------------------------------------------------
            virtual void CancelAll() = 0;
            //--------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being received.
            //---------------------------------------------------
            virtual IConnectableEvent<ReceivedDelegate>& GetReceivedEvent() = 0;
            //--------------------------------------------------------
            /// Destructor
            //--------------------------------------------------------
            virtual ~LocalNotificationSystem(){};
        protected:
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            LocalNotificationSystem(){}
        };
    }
}

#endif
