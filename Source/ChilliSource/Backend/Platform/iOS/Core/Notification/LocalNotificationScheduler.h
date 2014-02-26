//
//  CNotificationFactory.h
//  moFloTest
//
//  Created by Scott Downie on 01/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_PLATFORM_IOS_LOCAL_NOTIFICATION_SCHEDULER_H_
#define _MO_FLO_PLATFORM_IOS_LOCAL_NOTIFICATION_SCHEDULER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>
#include <ChilliSource/Core/Notifications/LocalNotificationScheduler.h>

@class UIApplication;
@class UILocalNotification;
@class NSDictionary;

namespace ChilliSource
{
    namespace iOS
    {
        class CLocalNotificationScheduler : public Core::ILocalNotificationScheduler
        {
        public:
            CLocalNotificationScheduler(){};
			virtual ~CLocalNotificationScheduler(){};
            //------------------------------------------------------------------------------
            /// Schedule Notification 
            ///
            /// Once the time is reached the notification will be inserted into the queue.
            /// Upon reaching the head of the queue it will trigger.
            ///
            /// @param Notification 
            //------------------------------------------------------------------------------
            void ScheduleNotification(const Core::Notification& insNotification);
            //-------------------------------------------------------------------------
            /// Cancel By ID
            ///
            /// Prevent any notifications with given ID type from firing
            ///
            /// @param ID type
            //-------------------------------------------------------------------------
            void CancelByID(Core::NotificationID inID);
            //-------------------------------------------------------------------------
            /// Cancel All 
            ///
            /// Terminate all currently scheduled notifications
            //-------------------------------------------------------------------------
            void CancelAll();
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
            bool TryGetNotificationsScheduledWithinTimePeriod(TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, std::vector<Core::Notification>& outaNotifications);
            //-------------------------------------------------------------------------
            /// Application Did Finish Launching With Options
            ///
            /// Called by the app delegate when the app launches. Used to pull
            /// local notification data from the options
            ///
            /// @param UIApplication
            /// @param Launching options dictionary
            /// @return Whether the app was launched with local notifications
            //-------------------------------------------------------------------------
            static bool ApplicationDidFinishLaunchingWithOptions(UIApplication* inpApplication, NSDictionary* inpOptions);
			//-------------------------------------------------------------------------
            /// Application Did Become Active
            ///
            /// Called by the app delegate when the app becomes active. Used to clear
			/// the icon badge number
            ///
            /// @param UIApplication
            //-------------------------------------------------------------------------
            static void ApplicationDidBecomeActive(UIApplication* inpApplication);
            //-------------------------------------------------------------------------
            /// Application Did Receive Local Notification
            ///
            /// Called by the app delegate when it receives a local notification
            ///
            /// @param UILocalNotification
            //-------------------------------------------------------------------------
            static void ApplicationDidReceiveLocalNotification(UIApplication* inpApplication, UILocalNotification* inpNotification);
            
        private:
            
            //-------------------------------------------------------------------------
            /// Convert UILocalNotification to Notification
            ///
            /// @param Apple UILocalNotification
            /// @param Out: moFlow notification
            //-------------------------------------------------------------------------
            static void ConvertUILocalNotificationToNotification(UILocalNotification* inpUILocal, Core::Notification& outsNotification);
        };
    }
}

#endif