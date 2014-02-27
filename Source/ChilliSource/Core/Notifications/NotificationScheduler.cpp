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
        bool NotificationScheduler::bSystemNotificationsEnabled = true;
        bool NotificationScheduler::bAppNotificationsEnabled = true;
        bool NotificationScheduler::bPushNotificationsEnabled = true;
        
        std::deque<NotificationSPtr> NotificationScheduler::NotificationQueue;
        std::vector<NotificationSPtr> NotificationScheduler::TimedAppNotifications;
        
        LocalNotificationScheduler* NotificationScheduler::mspLocalNotificationScheduler = nullptr;

        const f32 kfMinTimeBetweenNotifications = 2.0f;
        f32 gfTimeBetweenNotifications = 0.0f;

        //------------------------------------------------------------------------------
        /// Initialise
        ///
        /// Provides this with a local and remote notification scheduler
        ///
        /// @param local
        //------------------------------------------------------------------------------
        void NotificationScheduler::Initialise(LocalNotificationScheduler* inLocalNS)
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
        /// @param Params
        /// @param ID
        /// @param Priority (High priority will leap-frog lower priority in the queue
        /// @return Connection to presented event
        //------------------------------------------------------------------------------
        ConnectionUPtr NotificationScheduler::ScheduleNotification(NotificationType ineType, const Core::ParamDictionary& insParams, NotificationID inID, NotificationPriority inePriority, const Notification::NotificationPresentedDelegate& in_delegate)
        {
            ConnectionUPtr presentedConnection;
            
            switch(ineType)
            {
                case NotificationType::k_app:
                    if(bAppNotificationsEnabled)
                    {
                        NotificationSPtr notification(std::make_shared<Notification>());
                        notification->eType = ineType;
                        notification->ePriority = inePriority;
                        notification->TriggerTime = 0;
                        notification->bDismissed = false;
                        notification->bTriggered = false;
                        notification->ID = inID;
                        notification->sParams = insParams;
                        
                        if(in_delegate != nullptr)
                        {
                            presentedConnection = notification->m_notificationPresentedEvent.OpenConnection(in_delegate);
                        }
                        
                        switch(inePriority)
                        {
                            case NotificationPriority::k_standard:
                            {
                                NotificationQueue.push_back(notification);
                                break;
                            }
                            case NotificationPriority::k_high:
                            {
                                NotificationQueue.push_front(notification);
                                break;
                            }
                        }
                    }
                    break;
                case NotificationType::k_push:
                    CS_ERROR_LOG("Push notifications can not be scheduled within the app");
                    break;
                case NotificationType::k_system:
                    CS_ERROR_LOG("System notifications must be scheduled with a time");
                    break;
            }
        
            return presentedConnection;
        }
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
        ConnectionUPtr NotificationScheduler::ScheduleNotificationForTime(NotificationType ineType, const Core::ParamDictionary& insParams, NotificationID inID, TimeIntervalSecs inTime, NotificationPriority inePriority, const Notification::NotificationPresentedDelegate& in_delegate)
        {
            ConnectionUPtr presentedConnection;
            
            switch(ineType)
            {
                case NotificationType::k_app:
                    if(bAppNotificationsEnabled)
                    {
                        NotificationSPtr notification(std::make_shared<Notification>());
                        notification->eType = ineType;
                        notification->ePriority = inePriority;
                        notification->TriggerTime = inTime;
                        notification->bDismissed = false;
                        notification->bTriggered = false;
                        notification->ID = inID;
                        notification->sParams = insParams;
                        TimedAppNotifications.push_back(notification);
                        
                        if(in_delegate != nullptr)
                        {
                            presentedConnection = notification->m_notificationPresentedEvent.OpenConnection(in_delegate);
                        }
                    }
                    break;
                case NotificationType::k_push:
                    CS_ERROR_LOG("Push notifications can not be scheduled within the app");
                    break;
                case NotificationType::k_system:
                    if(bSystemNotificationsEnabled)
                    {
                        NotificationSPtr notification(std::make_shared<Notification>());
                        notification->eType = ineType;
                        notification->ePriority = inePriority;
                        notification->TriggerTime = inTime;
                        notification->bDismissed = false;
                        notification->bTriggered = false;
                        notification->ID = inID;
                        notification->sParams = insParams;
                        mspLocalNotificationScheduler->ScheduleNotification(notification);
                    }
                    break;
            }
            
            return presentedConnection;
        }
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
        ConnectionUPtr NotificationScheduler::ScheduleNotificationAfterTime(NotificationType ineType, const Core::ParamDictionary& insParams, NotificationID inID, TimeIntervalSecs inTime, NotificationPriority inePriority, const Notification::NotificationPresentedDelegate& in_delegate)
        {
            ConnectionUPtr presentedConnection;
            
            switch(ineType)
            {
                case NotificationType::k_app:
                    if(bAppNotificationsEnabled)
                    {
                        NotificationSPtr notification(std::make_shared<Notification>());
                        notification->eType = ineType;
                        notification->ePriority = inePriority;
                        notification->TriggerTime = Core::Application::GetSystemTime() + inTime;
                        notification->bDismissed = false;
                        notification->bTriggered = false;
                        notification->ID = inID;
                        notification->sParams = insParams;
                        TimedAppNotifications.push_back(notification);
                        
                        if(in_delegate != nullptr)
                        {
                            presentedConnection = notification->m_notificationPresentedEvent.OpenConnection(in_delegate);
                        }
                        break;
                    }
                    break;
                case NotificationType::k_push:
                    CS_ERROR_LOG("Push notifications can not be scheduled within the app");
                    break;
                case NotificationType::k_system:
                    if(bSystemNotificationsEnabled)
                    {
                        NotificationSPtr notification(std::make_shared<Notification>());
                        notification->eType = ineType;
                        notification->ePriority = inePriority;
                        notification->TriggerTime = Core::Application::GetSystemTime() + inTime;
                        notification->bDismissed = false;
                        notification->bTriggered = false;
                        notification->ID = inID;
                        notification->sParams = insParams;
                        mspLocalNotificationScheduler->ScheduleNotification(notification);
                    }
                    break;
            }
            
            return presentedConnection;
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
        bool NotificationScheduler::TryGetNotificationsScheduledWithinTimePeriod(NotificationType ineType, TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, std::vector<NotificationSPtr>& outaNotifications)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    CS_WARNING_LOG("NotificationScheduler::TryGetNotificationsScheduledWithinTimePeriod is not implemented for type NOTICE_APP");
                    break;
                case NotificationType::k_push:
                    CS_FATAL_LOG("Push notifications can not be checked within the app");
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
        void NotificationScheduler::EnableNotifications(NotificationType ineType, bool inbEnabled)
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
        void NotificationScheduler::CancelByID(NotificationType ineType, NotificationID inID)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    for(std::vector<NotificationSPtr>::iterator it = TimedAppNotifications.begin(); it != TimedAppNotifications.end(); /*No Increment*/)
                    {  
                        if(inID == (*it)->ID)
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
                    CS_FATAL_LOG("Push notifications can not be cancelled within the app");
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
        void NotificationScheduler::CancelAll(NotificationType ineType)
        {
            switch(ineType)
            {
                case NotificationType::k_app:
                    TimedAppNotifications.clear();
                    break;
                case NotificationType::k_push:
                    CS_FATAL_LOG("Push notifications can not be cancelled within the app");
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
        void NotificationScheduler::Update(f32 infDt)
        {
            gfTimeBetweenNotifications += infDt;
            
            //Update the app notifications
            TimeIntervalSecs CurrentTime = Core::Application::GetSystemTime();
            
            for(std::vector<NotificationSPtr>::iterator it = TimedAppNotifications.begin(); it != TimedAppNotifications.end(); /*No Increment*/)
            {  
                if(CurrentTime >= (*it)->TriggerTime)
                {
                    //This notification should be triggered
                    switch((*it)->ePriority)
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
                if(NotificationQueue.front()->bDismissed)
                {
                    //Ditch the old one
                    NotificationQueue.pop_front();
                }
                
                if(!NotificationQueue.empty() && !NotificationQueue.front()->bTriggered)
                {
                    //Trigger the next one
                    if(Core::Application::GetStateManagerPtr()->OnNotificationReceived(NotificationQueue.front().get()))
                    {
                        NotificationQueue.front()->bTriggered = true;
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
        void NotificationScheduler::OnNotificationReceived(const NotificationSPtr& insNotification)
        {
            if(IsTypeEnabled(insNotification->eType))
            {
                switch(insNotification->ePriority)
                {
                    case NotificationPriority::k_standard:
                    {
                        insNotification->bDismissed = false;
                        insNotification->bTriggered = false;
                        NotificationQueue.push_back(insNotification);
                        break;
                    }
                    case NotificationPriority::k_high:
                    {
                        insNotification->bDismissed = false;
                        insNotification->bTriggered = false;
                        NotificationQueue.push_front(insNotification);
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
        bool NotificationScheduler::IsTypeEnabled(NotificationType ineType)
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
