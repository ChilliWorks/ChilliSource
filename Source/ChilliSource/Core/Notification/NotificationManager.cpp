//
//  NotificationSystem.cpp
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

#include <ChilliSource/Core/Notification/NotificationManager.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
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
        bool NotificationManager::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (NotificationManager::InterfaceID == in_interfaceId);
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
        void NotificationManager::GetScheduledNotifications(std::vector<NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time, TimeIntervalSecs in_period) const
        {
            if (m_appNotificationSystem != nullptr)
            {
                m_appNotificationSystem->GetScheduledNotifications(out_notifications, in_time, in_period);
            }
            
            if (m_localNotificationSystem != nullptr)
            {
                m_localNotificationSystem->GetScheduledNotifications(out_notifications, in_time, in_period);
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
        IConnectableEvent<NotificationManager::ReceivedDelegate>& NotificationManager::GetReceivedEvent()
        {
            return m_receivedEvent;
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
                m_appReceivedConnection = m_appNotificationSystem->GetReceivedEvent().OpenConnection(MakeDelegate(this, &NotificationManager::OnNotificationReceived));
            }
            
            //setup the local notification system
            m_localNotificationSystem = Application::Get()->GetSystem<LocalNotificationSystem>();
			if (m_localNotificationSystem != nullptr)
            {
                m_localReceivedConnection = m_localNotificationSystem->GetReceivedEvent().OpenConnection(MakeDelegate(this, &NotificationManager::OnNotificationReceived));
            }
            
            //setup the remote notification system
            m_remoteNotificationSystem = Application::Get()->GetSystem<RemoteNotificationSystem>();
            if (m_remoteNotificationSystem != nullptr)
            {
                m_remoteReceivedConnection = m_remoteNotificationSystem->GetReceivedEvent().OpenConnection(MakeDelegate(this, &NotificationManager::OnNotificationReceived));
            }
        }
        //------------------------------------------------
        //------------------------------------------------
        void NotificationManager::OnNotificationReceived(const NotificationCSPtr& in_notification)
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
                    m_receivedEvent.NotifyConnections(this, m_notificationQueue.front().m_notification);
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
            m_appReceivedConnection.reset();
            m_appNotificationSystem = nullptr;
            
            //cleanup the local notification system
            m_localReceivedConnection.reset();
            m_localNotificationSystem = nullptr;
            
            //cleanup the remote notification system
            m_remoteReceivedConnection.reset();
            m_remoteNotificationSystem = nullptr;
        }
    }
}
