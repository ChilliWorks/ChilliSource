//
//  AppNotificationSystem.cpp
//  Chilli Source
//
//  Created by I Copland on 10/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Notification/AppNotificationSystem.h>

#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
    namespace Core
    {
        CS_DEFINE_NAMEDTYPE(AppNotificationSystem);
        //--------------------------------------------------
        //--------------------------------------------------
        AppNotificationSystemUPtr AppNotificationSystem::Create()
        {
            return AppNotificationSystemUPtr(new AppNotificationSystem());
        }
        //--------------------------------------------------
        //--------------------------------------------------
        AppNotificationSystem::AppNotificationSystem()
        : m_enabled(true)
        {
        }
        //--------------------------------------------------
        //--------------------------------------------------
        bool AppNotificationSystem::IsA(Core::InterfaceIDType in_interfaceID) const
        {
            return (AppNotificationSystem::InterfaceID == in_interfaceID);
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void AppNotificationSystem::SetEnabled(bool in_enabled)
        {
            m_enabled = in_enabled;
            if (m_enabled == false)
            {
                CancelAll();
            }
        }
        //--------------------------------------------------
        //---------------------------------------------------
        void AppNotificationSystem::ScheduleNotification(Notification::ID in_id, const ParamDictionary& in_params, Notification::Priority in_priority)
        {
            if (m_enabled == true)
            {
                NotificationContainerSPtr notificationContainer = std::make_shared<NotificationContainer>();
                notificationContainer->m_notification = std::make_shared<Notification>(in_id, in_params, in_priority);
                notificationContainer->m_triggerTime = 0;
                notificationContainer->m_cancelled = false;
                m_notifications.push_back(notificationContainer);
            }
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void AppNotificationSystem::ScheduleNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority)
        {
            if (m_enabled == true)
            {
                NotificationContainerSPtr notificationContainer = std::make_shared<NotificationContainer>();
                notificationContainer->m_notification = std::make_shared<Notification>(in_id, in_params, in_priority);
                notificationContainer->m_triggerTime = in_time;
                notificationContainer->m_cancelled = false;
                m_notifications.push_back(notificationContainer);
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void AppNotificationSystem::GetScheduledNotifications(std::vector<NotificationSPtr>& out_notifications, TimeIntervalSecs in_time, TimeIntervalSecs in_peroid)
        {
            for (const NotificationContainerSPtr& notificationContainer : m_notifications)
            {
                if (notificationContainer->m_triggerTime > in_time && notificationContainer->m_triggerTime < in_time - in_peroid)
                {
                    out_notifications.push_back(notificationContainer->m_notification);
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void AppNotificationSystem::CancelByID(Notification::ID in_id)
        {
            for(auto it = m_notifications.begin(); it != m_notifications.end();)
            {
                if(in_id == (*it)->m_notification->GetID())
                {
                    it = m_notifications.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void AppNotificationSystem::CancelAll()
        {
            m_notifications.clear();
        }
        //--------------------------------------------------
        //---------------------------------------------------
        IConnectableEvent<AppNotificationSystem::RecievedDelegate>& AppNotificationSystem::GetRecievedEvent()
        {
            return m_recievedEvent;
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void AppNotificationSystem::OnUpdate(f32 in_deltaTime)
        {
            TimeIntervalSecs currentTime = Core::Application::Get()->GetSystemTime();
            
            for(auto it = m_notifications.begin(); it != m_notifications.end();)
            {
                if ((*it)->m_cancelled == true)
                {
                    it = m_notifications.erase(it);
                }
                else if(currentTime >= (*it)->m_triggerTime)
                {
                    m_recievedEvent.NotifyConnections((*it)->m_notification);
                    it = m_notifications.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }
}
