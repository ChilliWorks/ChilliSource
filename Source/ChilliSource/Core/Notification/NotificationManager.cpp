//
//  NotificationSystem.cpp
//  Chilli Source
//
//  Created by I Copland on 10/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Notification/NotificationManager.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Notification/AppNotificationSystem.h>
#include <ChilliSource/Core/Notification/LocalNotificationSystem.h>
#include <ChilliSource/Core/Notification/RemoteNotificationSystem.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            const f32 k_minTimeBetweenNotifications = 2.0f;
        }
        
        CS_DEFINE_NAMEDTYPE(NotificationManager);
        //-----------------------------------------------------
        //-----------------------------------------------------
        NotificationManagerUPtr NotificationManager::Create()
        {
            return NotificationManagerUPtr(new NotificationManager());
        }
        //--------------------------------------------------
        //--------------------------------------------------
        NotificationManager::NotificationManager()
            : m_appNotificationSystem(nullptr), m_localNotificationSystem(nullptr), m_remoteNotificationSystem(nullptr), m_timeSinceNotification(0.0f)
        {
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        bool NotificationManager::IsA(Core::InterfaceIDType in_interfaceID) const
        {
            return (NotificationManager::InterfaceID == in_interfaceID);
        }
        //--------------------------------------------------
        //---------------------------------------------------
        void NotificationManager::ScheduleAppNotification(Notification::ID in_id, const ParamDictionary& in_params, Notification::Priority in_priority)
        {
            ScheduleAppNotificationForTime(in_id, in_params, 0, in_priority);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void NotificationManager::ScheduleAppNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority)
        {
            if (m_appNotificationSystem != nullptr)
            {
                m_appNotificationSystem->ScheduleNotificationForTime(in_id, in_params, in_time, in_priority);
            }
        }
        //---------------------------------------------------
        //----------------------------------------------------
        void NotificationManager::ScheduleAppNotificationAfterTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority)
        {
            ScheduleAppNotificationForTime(in_id, in_params, Application::Get()->GetSystemTime() + in_time, in_priority);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void NotificationManager::ScheduleLocalNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority)
        {
            if (m_localNotificationSystem != nullptr)
            {
                m_localNotificationSystem->ScheduleNotificationForTime(in_id, in_params, in_time, in_priority);
            }
        }
        //---------------------------------------------------
        //----------------------------------------------------
        void NotificationManager::ScheduleLocalNotificationAfterTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority)
        {
            ScheduleLocalNotificationForTime(in_id, in_params, Application::Get()->GetSystemTime() + in_time, in_priority);
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void NotificationManager::GetScheduledNotifications(std::vector<NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time, TimeIntervalSecs in_peroid) const
        {
            if (m_appNotificationSystem != nullptr)
            {
                m_appNotificationSystem->GetScheduledNotifications(out_notifications, in_time, in_peroid);
            }
            
            if (m_localNotificationSystem != nullptr)
            {
                m_localNotificationSystem->GetScheduledNotifications(out_notifications, in_time, in_peroid);
            }
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void NotificationManager::CancelByID(Notification::ID in_id)
        {
            if (m_appNotificationSystem != nullptr)
            {
                m_appNotificationSystem->CancelByID(in_id);
            }
            
            if (m_localNotificationSystem != nullptr)
            {
                m_localNotificationSystem->CancelByID(in_id);
            }
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void NotificationManager::CancelAll()
        {
            if (m_appNotificationSystem != nullptr)
            {
                m_appNotificationSystem->CancelAll();
            }
            
            if (m_localNotificationSystem != nullptr)
            {
                m_localNotificationSystem->CancelAll();
            }
        }
        //---------------------------------------------------
        //----------------------------------------------------
        void NotificationManager::Dismiss(const NotificationCSPtr& in_notification)
        {
            CS_ASSERT(m_notificationQueue.empty() == false, "Cannot dismiss from an empty notification queue!");
            CS_ASSERT(m_notificationQueue.front().m_notification == in_notification, "Trying to dismiss notification that is not the active notification!");
            
            if (m_notificationQueue.empty() == false && m_notificationQueue.front().m_notification == in_notification)
            {
                m_notificationQueue.front().m_dismissed = true;
            }
        }
        //---------------------------------------------------
        //----------------------------------------------------
        IConnectableEvent<NotificationManager::RecievedDelegate>& NotificationManager::GetRecievedEvent()
        {
            return m_recievedEvent;
        }
        //---------------------------------------------------
        //----------------------------------------------------
        IConnectableEvent<NotificationManager::DismissedDelegate>& NotificationManager::GetDismissedEvent()
        {
            return m_dismissedEvent;
        }
        //------------------------------------------------
        //------------------------------------------------
        void NotificationManager::OnInit()
        {
            //setup the app notification system
            m_appNotificationSystem = Application::Get()->GetSystem<AppNotificationSystem>();
            if (m_appNotificationSystem != nullptr)
            {
                m_appRecievedConnection = m_appNotificationSystem->GetRecievedEvent().OpenConnection(MakeDelegate(this, &NotificationManager::OnNotificationRecieved));
            }
            
            //setup the local notification system
            m_localNotificationSystem = Application::Get()->GetSystem<LocalNotificationSystem>();
            if (m_appNotificationSystem != nullptr)
            {
                m_localRecievedConnection = m_localNotificationSystem->GetRecievedEvent().OpenConnection(MakeDelegate(this, &NotificationManager::OnNotificationRecieved));
            }
            
            //setup the remote notification system
            m_remoteNotificationSystem = Application::Get()->GetSystem<RemoteNotificationSystem>();
            if (m_remoteNotificationSystem != nullptr)
            {
                m_remoteRecievedConnection = m_remoteNotificationSystem->GetRecievedEvent().OpenConnection(MakeDelegate(this, &NotificationManager::OnNotificationRecieved));
            }
        }
        //------------------------------------------------
        //------------------------------------------------
        void NotificationManager::OnNotificationRecieved(const NotificationCSPtr& in_notification)
        {
            NotificationContainer notificationContainer;
            notificationContainer.m_notification = std::make_shared<Notification>();
            notificationContainer.m_notification->m_id = in_notification->m_id;
            notificationContainer.m_notification->m_params = in_notification->m_params;
            notificationContainer.m_notification->m_priority = in_notification->m_priority;
            notificationContainer.m_dismissed = false;
            notificationContainer.m_triggered = false;
            
            switch (notificationContainer.m_notification->m_priority)
            {
                case Notification::Priority::k_standard:
                    m_notificationQueue.push_back(notificationContainer);
                    break;
                case Notification::Priority::k_high:
                    m_notificationQueue.push_front(notificationContainer);
                    break;
                default:
                    CS_LOG_ERROR("Invalid notification priority.");
            }
        }
        //------------------------------------------------
        //------------------------------------------------
        void NotificationManager::OnUpdate(f32 in_deltaTime)
        {
            m_timeSinceNotification += in_deltaTime;
            
            if(m_notificationQueue.empty() == false && m_timeSinceNotification >= k_minTimeBetweenNotifications)
            {
                if (m_notificationQueue.front().m_dismissed == true)
                {
                    m_dismissedEvent.NotifyConnections(m_notificationQueue.front().m_notification);
                    m_notificationQueue.pop_front();
                }
                
                if(m_notificationQueue.empty() == false && m_notificationQueue.front().m_triggered == false)
                {
                    m_recievedEvent.NotifyConnections(this, m_notificationQueue.front().m_notification);
                    m_notificationQueue.front().m_triggered = true;
                    m_timeSinceNotification = 0.0f;
                }
            }
        }
        //------------------------------------------------
        //------------------------------------------------
        void NotificationManager::OnDestroy()
        {
            //cleanup the app notification system.
            m_appRecievedConnection.reset();
            m_appNotificationSystem = nullptr;
            
            //cleanup the local notification system
            m_localRecievedConnection.reset();
            m_localNotificationSystem = nullptr;
            
            //cleanup the remote notification system
            m_remoteRecievedConnection.reset();
            m_remoteNotificationSystem = nullptr;
        }
    }
}
