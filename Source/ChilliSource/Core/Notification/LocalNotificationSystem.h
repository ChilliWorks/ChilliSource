//
//  LocalNotificationSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 12/01/2012.
//  Copyright 202 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_NOTIFICATIONS_LOCALNOTIFICATIONSYSTEM_H_
#define _CHILLISOURCE_CORE_NOTIFICATIONS_LOCALNOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/Notification/Notification.h>

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
            using RecievedDelegate = std::function<void(const NotificationSPtr&)>;
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
            /// @author I Copland
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
            /// @author I Copland
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
            /// within the given time peroid.
            ///
            /// @author I Copland
            ///
            /// @param [Out] The list of notifications.
            /// @param [Optional] The start time.
            /// @param [Optional] The end time.
            //--------------------------------------------------------
            virtual void GetScheduledNotifications(std::vector<NotificationSPtr>& out_notifications, TimeIntervalSecs in_time = 0, TimeIntervalSecs in_peroid = std::numeric_limits<TimeIntervalSecs>::max()) = 0;
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
            /// @author I Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being recieved.
            //---------------------------------------------------
            virtual IConnectableEvent<RecievedDelegate>& GetRecievedEvent() = 0;
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
