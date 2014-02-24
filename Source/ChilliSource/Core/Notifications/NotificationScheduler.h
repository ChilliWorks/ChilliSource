//
//  CNotificationFactory.h
//  moFloTest
//
//  Created by Scott Downie on 01/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_NOTIFICATION_SCHEDULER_H_
#define _MO_FLO_CORE_NOTIFICATION_SCHEDULER_H_

#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Core/Notifications/RemoteNotificationSystem.h>
#include <queue>

namespace ChilliSource
{
	class ILocalNotificationScheduler;
	
    enum class NotificationType
    {
        k_system,
        k_app,
        k_push
    };
    
    enum class NotificationPriority
    {
        k_standard,
        k_high
    };
    
    typedef u32 NotificationID;
    
    struct Notification
    {
        typedef fastdelegate::FastDelegate0<> NotificationDismissedDelegate;
        typedef fastdelegate::FastDelegate1<const void*> NotificationPresentedDelegate;
        
        Core::ParamDictionary sParams;
        NotificationID ID;
        
        NotificationPriority ePriority;
        NotificationType eType;
        TimeIntervalSecs TriggerTime;
        
        void Dismiss()
        {
            bDismissed = true;
			
			pNotificationDismissedEvent.Invoke();
			pNotificationDismissedEvent.RemoveAllListeners();
        }
              
        bool bDismissed;
        bool bTriggered;
        
        CEvent0<NotificationDismissedDelegate> pNotificationDismissedEvent;
        CEvent1<NotificationPresentedDelegate> pNotificationPresentedEvent;
    };
    
    class CNotificationScheduler
    {
    public:
        //------------------------------------------------------------------------------
        /// Initialise
        ///
        /// Provides this with a local and remote notification scheduler
        ///
        /// @param local
        //------------------------------------------------------------------------------
    	static void Initialise(ILocalNotificationScheduler* inLocalNS);
        //------------------------------------------------------------------------------
        /// Schedule Notification
        ///
        /// Immediately queue a notification. This will trigger when it reaches
        /// the front of the queue.
        ///
        /// @param Notification Type (Push, System or In-App)
        /// @param Notification 
        /// @param Priority (High priority will leap-frog lower priority in the queue
        /// @param Delegate An optional callback delegate to allow application to track notifications
        //------------------------------------------------------------------------------
        static void ScheduleNotification(NotificationType ineType, const Notification& insNotification, NotificationPriority inePriority = NotificationPriority::k_standard, Notification::NotificationPresentedDelegate inpDelegate = NULL);
        //------------------------------------------------------------------------------
        /// Schedule Notification For Time
        ///
        /// Once the time is reached the notification will be inserted into the queue.
        /// Upon reaching the head of the queue it will trigger.
        ///
        /// @param Notification Type (Push, System or In-App)
        /// @param Notification 
        /// @param Time in seconds at which it should trigger
        /// @param Priority (High priority will leap-frog lower priority in the queue
        //------------------------------------------------------------------------------
        static void ScheduleNotificationForTime(NotificationType ineType, const Notification& insNotification, TimeIntervalSecs inTime, NotificationPriority inePriority = NotificationPriority::k_standard);
        //------------------------------------------------------------------------------
        /// Schedule Notification After Time
        ///
        /// After the given number of second the notification will be inserted 
        /// into the queue. Upon reaching the head of the queue it will trigger.
        ///
        /// @param Notification Type (Push, System or In-App)
        /// @param Notification 
        /// @param Time in seconds at which it should trigger
        /// @param Priority (High priority will leap-frog lower priority in the queue
        //------------------------------------------------------------------------------
        static void ScheduleNotificationAfterTime(NotificationType ineType, const Notification& insNotification, TimeIntervalSecs inTime, NotificationPriority inePriority = NotificationPriority::k_standard);
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
        static bool TryGetNotificationsScheduledWithinTimePeriod(NotificationType ineType, TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, DYNAMIC_ARRAY<Notification>& outaNotifications);
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
        static void OnNotificationReceived(const Notification& insNotification);
        //-------------------------------------------------------------------------
        /// Update
        ///
        /// Called by the main loop and used to trigger time based notices
        ///
        /// @param Time between frames
        //-------------------------------------------------------------------------
        static void Update(f32 infDt);
        
    private:
        //-------------------------------------------------------------------------
        /// Is Type Enabled
        ///
        /// @param Notification type
        /// @return Is notification type enabled
        //-------------------------------------------------------------------------
        static bool IsTypeEnabled(NotificationType ineType);
        
    private:
        
		static bool bSystemNotificationsEnabled;
        static bool bAppNotificationsEnabled;
        static bool bPushNotificationsEnabled;
        
        static std::deque<Notification> NotificationQueue;
        static DYNAMIC_ARRAY<Notification> TimedAppNotifications;

        static ILocalNotificationScheduler* mspLocalNotificationScheduler;
    };
}

#endif
