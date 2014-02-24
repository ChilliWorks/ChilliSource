//
//  CNotificationFactory.cpp
//  moFloTest
//
//  Created by Scott Downie on 01/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Notifications/NotificationScheduler.h>
#include <ChilliSource/Core/Notifications/LocalNotificationScheduler.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
    namespace Core
    {
        bool CNotificationScheduler::bSystemNotificationsEnabled = true;
        bool CNotificationScheduler::bAppNotificationsEnabled = true;
        bool CNotificationScheduler::bPushNotificationsEnabled = true;
        
        std::deque<Notification> CNotificationScheduler::NotificationQueue;
        DYNAMIC_ARRAY<Notification> CNotificationScheduler::TimedAppNotifications;
        
        ILocalNotificationScheduler* CNotificationScheduler::mspLocalNotificationScheduler = NULL;

        const f32 kfMinTimeBetweenNotifications = 2.0f;
        f32 gfTimeBetweenNotifications = 0.0f;

        //------------------------------------------------------------------------------
        /// Initialise
        ///
        /// Provides this with a local and remote notification scheduler
        ///
        /// @param local
        //------------------------------------------------------------------------------
        void CNotificationScheduler::Initialise(ILocalNotificationScheduler* inLocalNS)
        {
            mspLocalNotificationScheduler = inLocalNS;
        }
        //------------------------------------------------------------------------------
        /// Schedule Notification
        ///
        /// Immediately queue a notification. This will trigger when it reaches
        /// the front of the queue.
        ///
        /// @param Notification Type (Push, System or In-App)
        /// @param Notification 
        /// @param Priority (High priority will leap-frog lower priority in the queue
        //------------------------------------------------------------------------------
        void CNotificationScheduler::ScheduleNotification(NotificationType ineType, const Notification& insNotification, NotificationPriority inePriority, Notification::NotificationPresentedDelegate inpDelegate)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    if(bAppNotificationsEnabled)
                    {
                        switch(inePriority)
                        {
                            case NotificationPriority::k_standard:
                            {
                                Notification sNotification(insNotification);
                                sNotification.eType = ineType;
                                sNotification.ePriority = inePriority;
                                sNotification.TriggerTime = 0;
                                sNotification.bDismissed = false;
                                sNotification.bTriggered = false;
                                if(!inpDelegate.empty())
                                {
                                    sNotification.pNotificationPresentedEvent += inpDelegate;
                                }                            
                                NotificationQueue.push_back(sNotification);
                                break;
                            }
                            case NotificationPriority::k_high:
                            {
                                Notification sNotification(insNotification);
                                sNotification.eType = ineType;
                                sNotification.ePriority = inePriority;
                                sNotification.TriggerTime = 0;
                                sNotification.bDismissed = false;
                                sNotification.bTriggered = false;
                                if(!inpDelegate.empty())
                                {
                                    sNotification.pNotificationPresentedEvent += inpDelegate;
                                }
                                NotificationQueue.push_front(sNotification);
                                break;
                            }
                        }
                    }
                    break;
                case NotificationType::k_push:
                    ERROR_LOG("Push notifications can not be scheduled within the app");
                    break;
                case NotificationType::k_system:
                    ERROR_LOG("System notifications must be scheduled with a time");
                    break;
            }
        
        }
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
        void CNotificationScheduler::ScheduleNotificationForTime(NotificationType ineType, const Notification& insNotification, TimeIntervalSecs inTime, NotificationPriority inePriority)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    if(bAppNotificationsEnabled)
                    {
                        Notification sNotification(insNotification);
                        sNotification.eType = ineType;
                        sNotification.ePriority = inePriority;
                        sNotification.TriggerTime = inTime;
                        sNotification.bDismissed = false;
                        sNotification.bTriggered = false;
                        TimedAppNotifications.push_back(sNotification);
                    }
                    break;
                case NotificationType::k_push:
                    ERROR_LOG("Push notifications can not be scheduled within the app");
                    break;
                case NotificationType::k_system:
                    if(bSystemNotificationsEnabled)
                    {
                        Notification sNotification(insNotification);
                        sNotification.eType = ineType;
                        sNotification.ePriority = inePriority;
                        sNotification.TriggerTime = inTime;
                        sNotification.bDismissed = false;
                        sNotification.bTriggered = false;
                        mspLocalNotificationScheduler->ScheduleNotification(sNotification);
                    }
                    break;
            }
        }
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
        void CNotificationScheduler::ScheduleNotificationAfterTime(NotificationType ineType, const Notification& insNotification, TimeIntervalSecs inTime, NotificationPriority inePriority)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    if(bAppNotificationsEnabled)
                    {
                        Notification sNotification(insNotification);
                        sNotification.eType = ineType;
                        sNotification.ePriority = inePriority;
                        sNotification.TriggerTime = Core::CApplication::GetSystemTime() + inTime;
                        sNotification.bDismissed = false;
                        sNotification.bTriggered = false;
                        TimedAppNotifications.push_back(sNotification);
                        break;
                    }
                    break;
                case NotificationType::k_push:
                    ERROR_LOG("Push notifications can not be scheduled within the app");
                    break;
                case NotificationType::k_system:
                    if(bSystemNotificationsEnabled)
                    {
                        Notification sNotification(insNotification);
                        sNotification.eType = ineType;
                        sNotification.ePriority = inePriority;
                        sNotification.TriggerTime = Core::CApplication::GetSystemTime() + inTime;
                        sNotification.bDismissed = false;
                        sNotification.bTriggered = false;
                        mspLocalNotificationScheduler->ScheduleNotification(sNotification);
                    }
                    break;
            }
        }
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
        bool CNotificationScheduler::TryGetNotificationsScheduledWithinTimePeriod(NotificationType ineType, TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, DYNAMIC_ARRAY<Notification>& outaNotifications)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    WARNING_LOG("CNotificationScheduler::TryGetNotificationsScheduledWithinTimePeriod is not implemented for type NOTICE_APP");
                    break;
                case NotificationType::k_push:
                    FATAL_LOG("Push notifications can not be checked within the app");
                    break;
                case NotificationType::k_system:
                    return mspLocalNotificationScheduler->TryGetNotificationsScheduledWithinTimePeriod(inTime, inPeriod, outaNotifications);
            }
            
            return false;
        }
        //-------------------------------------------------------------------------
        /// Enable Notifications
        ///
        /// Switch the notifications on or off 
        ///
        /// @param Notification Type
        /// @param Enable/disable
        //-------------------------------------------------------------------------
        void CNotificationScheduler::EnableNotifications(NotificationType ineType, bool inbEnabled)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    bAppNotificationsEnabled = inbEnabled;
                    break;
                case NotificationType::k_push:
                    bPushNotificationsEnabled = inbEnabled;
                    break;
                case NotificationType::k_system:
                    bSystemNotificationsEnabled = inbEnabled;
                    break;
            }
            
            //Cancel any outstanding ones
            if(!inbEnabled)
            {
                CancelAll(ineType);
            }
        }
        //-------------------------------------------------------------------------
        /// Cancel By ID
        ///
        /// Prevent any notifications with given ID type from firing
        ///
        /// @param Notification Type
        /// @param ID type
        //-------------------------------------------------------------------------
        void CNotificationScheduler::CancelByID(NotificationType ineType, NotificationID inID)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    for(DYNAMIC_ARRAY<Notification>::iterator it = TimedAppNotifications.begin(); it != TimedAppNotifications.end(); /*No Increment*/)
                    {  
                        if(inID == it->ID)
                        {
                            it = TimedAppNotifications.erase(it);
                        }
                        else
                        {
                            ++it;
                        }
                    }
                    break;
                case NotificationType::k_push:
                    FATAL_LOG("Push notifications can not be cancelled within the app");
                    break;
                case NotificationType::k_system:
                    mspLocalNotificationScheduler->CancelByID(inID);
                    break;
            }
        }
        //-------------------------------------------------------------------------
        /// Cancel All 
        ///
        /// Terminate all currently scheduled notifications
        ///
        /// @param Notification Type
        //-------------------------------------------------------------------------
        void CNotificationScheduler::CancelAll(NotificationType ineType)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    TimedAppNotifications.clear();
                    break;
                case NotificationType::k_push:
                    FATAL_LOG("Push notifications can not be cancelled within the app");
                    break;
                case NotificationType::k_system:
                    mspLocalNotificationScheduler->CancelAll();
                    break;
            }
        }
        //-------------------------------------------------------------------------
        /// Update
        ///
        /// Called by the main loop and used to trigger time based notices
        ///
        /// @param Time between frames
        //-------------------------------------------------------------------------
        void CNotificationScheduler::Update(f32 infDt)
        {
            gfTimeBetweenNotifications += infDt;
            
            //Update the app notifications
            TimeIntervalSecs CurrentTime = Core::CApplication::GetSystemTime();
            
            for(DYNAMIC_ARRAY<Notification>::iterator it = TimedAppNotifications.begin(); it != TimedAppNotifications.end(); /*No Increment*/)
            {  
                if(CurrentTime >= it->TriggerTime)
                {
                    //This notification should be triggered
                    switch(it->ePriority)
                    {
                        case NotificationPriority::k_standard:
                            NotificationQueue.push_back((*it));
                            break;
                        case NotificationPriority::k_high:
                            NotificationQueue.push_front((*it));
                            break;
                    }
                    
                    it = TimedAppNotifications.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            
            //Check if the front of the queue has been dismissed and display 
            //the next notification
            if(!NotificationQueue.empty() && gfTimeBetweenNotifications >= kfMinTimeBetweenNotifications)
            {
                if(NotificationQueue.front().bDismissed)
                {
                    //Ditch the old one
                    NotificationQueue.pop_front();
                }
                
                if(!NotificationQueue.empty() && !NotificationQueue.front().bTriggered)
                {
                    //Trigger the next one
                    if(Core::CApplication::GetStateManagerPtr()->OnNotificationReceived(&NotificationQueue.front()))
                    {
                        NotificationQueue.front().bTriggered = true;
                    }

                    gfTimeBetweenNotifications = 0.0f;
                }
            }
        }
        //-------------------------------------------------------------------------
        /// On Notification Received
        ///
        /// Called by the platform systems when they fire notices.
        /// The scheduler will then queue the notification
        ///
        /// @param Notification
        //-------------------------------------------------------------------------
        void CNotificationScheduler::OnNotificationReceived(const Notification& insNotification)
        {
            if(IsTypeEnabled(insNotification.eType))
            {
                switch(insNotification.ePriority)
                {
                    case NotificationPriority::k_standard:
                    {
                        Notification notification = insNotification;
                        notification.bDismissed = false;
                        notification.bTriggered = false;
                        NotificationQueue.push_back(notification);
                        break;
                    }
                    case NotificationPriority::k_high:
                    {
                        Notification notification = insNotification;
                        notification.bDismissed = false;
                        notification.bTriggered = false;
                        NotificationQueue.push_front(notification);                    
                        break;
                    }
                }
            }
        }
        //-------------------------------------------------------------------------
        /// Is Type Enabled
        ///
        /// @param Notification type
        /// @return Is notification type enabled
        //-------------------------------------------------------------------------
        bool CNotificationScheduler::IsTypeEnabled(NotificationType ineType)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    return bAppNotificationsEnabled;
                    break;
                case NotificationType::k_push:
                    return bPushNotificationsEnabled;
                    break;
                case NotificationType::k_system:
                    return bSystemNotificationsEnabled;
                    break;
            }

            return false;
        }
    }
}
