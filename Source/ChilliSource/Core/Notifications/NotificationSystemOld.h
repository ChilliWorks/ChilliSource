//
//  CNotificationFactory.h
//  moFloTest
//
//  Created by Scott Downie on 01/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_NOTIFICATION_SCHEDULER_H_
#define _MO_FLO_CORE_NOTIFICATION_SCHEDULER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Notifications/RemoteNotificationSystem.h>

#include <functional>
#include <queue>

namespace ChilliSource
{
    namespace Core
    {
        //-------------------------------------------------------------------
        /// A base class for any systems that schedule notifications.
        //-------------------------------------------------------------------
        class NotificationScheduler : public AppSystem
        {
        public:
            //------------------------------------------------------------------------------
            /// Schedule Notification
            ///
            /// Immediately queue a notification. This will trigger when it reaches
            /// the front of the queue.
            ///
            /// @param Notification Type (Push, System or In-App)
            /// @param Params
            /// @param ID
            /// @param Priority (High priority will leap-frog lower priority in the queue
            /// @param Delegate An optional callback delegate to allow application to track notifications
            /// @return Connection to presented event
            //------------------------------------------------------------------------------
            static ConnectionUPtr ScheduleNotification(NotificationType ineType, const Core::ParamDictionary& insParams, NotificationID inID, NotificationPriority inePriority = NotificationPriority::k_standard, const Notification::NotificationPresentedDelegate& in_delegate = nullptr);
            //------------------------------------------------------------------------------
            /// Schedule Notification For Time
            ///
            /// Once the time is reached the notification will be inserted into the queue.
            /// Upon reaching the head of the queue it will trigger.
            ///
            /// @param Notification Type (Push, System or In-App)
            /// @param Params
            /// @param ID
            /// @param Time in seconds at which it should trigger
            /// @param Priority (High priority will leap-frog lower priority in the queue
            /// @return Connection to presented event
            //------------------------------------------------------------------------------
            static ConnectionUPtr ScheduleNotificationForTime(NotificationType ineType, const Core::ParamDictionary& insParams, NotificationID inID, TimeIntervalSecs inTime, NotificationPriority inePriority = NotificationPriority::k_standard, const Notification::NotificationPresentedDelegate& in_delegate = nullptr);
            //------------------------------------------------------------------------------
            /// Schedule Notification After Time
            ///
            /// After the given number of second the notification will be inserted 
            /// into the queue. Upon reaching the head of the queue it will trigger.
            ///
            /// @param Notification Type (Push, System or In-App)
            /// @param Params
            /// @param ID
            /// @param Time in seconds at which it should trigger
            /// @param Priority (High priority will leap-frog lower priority in the queue
            /// @return Connection to presented event
            //------------------------------------------------------------------------------
            static ConnectionUPtr ScheduleNotificationAfterTime(NotificationType ineType, const Core::ParamDictionary& insParams, NotificationID inID, TimeIntervalSecs inTime, NotificationPriority inePriority = NotificationPriority::k_standard, const Notification::NotificationPresentedDelegate& in_delegate = nullptr);
            //-------------------------------------------------------------------------
            /// Try Get Notifications Scheduled Within Time Period
            ///
            /// Checks if any notifications have been scheduled to trigger
            /// within the given window of the given time
            ///
            /// @param Notification type
            /// @param Time
            /// @param Timeframe
            /// @param Out: Notifications that meet criteria
            /// @return Whether any notifications exist within that time period
            //-------------------------------------------------------------------------
            static bool TryGetNotificationsScheduledWithinTimePeriod(NotificationType ineType, TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, std::vector<NotificationSPtr>& outaNotifications);
            //-------------------------------------------------------------------------
            /// Enable Notifications
            ///
            /// Switch the notifications on or off 
            ///
            /// @param Notification type
            /// @param Enable/disable
            //-------------------------------------------------------------------------
            static void EnableNotifications(NotificationType ineType, bool inbEnabled);
            //-------------------------------------------------------------------------
            /// Cancel By ID
            ///
            /// Prevent any notifications with given ID type from firing
            ///
            /// @param Notification Type
            /// @param ID type
            //-------------------------------------------------------------------------
            static void CancelByID(NotificationType ineType, NotificationID inID);
            //-------------------------------------------------------------------------
            /// Cancel All 
            ///
            /// Terminate all currently scheduled notifications
            ///
            /// @param Notification Type
            //-------------------------------------------------------------------------
            static void CancelAll(NotificationType ineType);
            //-------------------------------------------------------------------------
            /// On Notification Received
            ///
            /// Called by the platform systems when they fire notices.
            /// The scheduler will then queue the notification
            ///
            /// @param Notification
            //-------------------------------------------------------------------------
            static void OnNotificationReceived(const NotificationSPtr& insNotification);
            //-------------------------------------------------------------------------
            /// Update
            ///
            /// Called by the main loop and used to trigger time based notices
            ///
            /// @param Time between frames
            //-------------------------------------------------------------------------
            static void Update(f32 infDt);
            
        };
    }
}

#endif
