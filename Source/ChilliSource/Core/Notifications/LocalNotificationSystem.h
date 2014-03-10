//
//  CNotificationFactory.h
//  MoFlow
//
//  Created by Ian Copland on 12/01/2012.
//  Copyright 202 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_CORE_LOCALNOTIFICATIONSCHEDULER_H_
#define _MOFLOW_CORE_LOCALNOTIFICATIONSCHEDULER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>

namespace ChilliSource
{
    namespace Core
    {
        class LocalNotificationScheduler
        {
        public:
            
            //-------------------------------------------------------
            /// Create the platform dependent backend
            ///
            /// @author S Downie
            ///
            /// @return New backend instance
            //-------------------------------------------------------
            static LocalNotificationSchedulerUPtr Create();
            
            virtual ~LocalNotificationScheduler(){};
            //------------------------------------------------------------------------------
            /// Schedule Notification
            ///
            /// Once the time is reached the notification will be inserted into the queue.
            /// Upon reaching the head of the queue it will trigger.
            ///
            /// @param Notification
            //------------------------------------------------------------------------------
            virtual void ScheduleNotification(const NotificationSPtr& insNotification) = 0;
            //-------------------------------------------------------------------------
            /// Try Get Notifications Scheduled Within Time Period
            ///
            /// Checks if any notifications have been scheduled to trigger
            /// within the given window of the given time
            ///
            /// @param Time
            /// @param Timeframe
            /// @param Out: Notifications that meet criteria
            /// @return Whether any notifications exist within that time period
            //-------------------------------------------------------------------------
            virtual bool TryGetNotificationsScheduledWithinTimePeriod(TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, std::vector<NotificationSPtr>& outaNotifications) = 0;
            //-------------------------------------------------------------------------
            /// Cancel By ID
            ///
            /// Prevent any notifications with given ID type from firing
            ///
            /// @param ID type
            //-------------------------------------------------------------------------
            virtual void CancelByID(NotificationID inID) = 0;
            //-------------------------------------------------------------------------
            /// Cancel All
            ///
            /// Terminate all currently scheduled notifications
            //-------------------------------------------------------------------------
            virtual void CancelAll() = 0;
            
        protected:
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            LocalNotificationScheduler(){}
        };
    }
}

#endif
