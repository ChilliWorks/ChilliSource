//
//  AppNotificationSystem.cpp
//  Chilli Source
//
//  Created by Ian Copland on 10/03/2014.
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
        bool AppNotificationSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (AppNotificationSystem::InterfaceID == in_interfaceId);
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
        //---------------------------------------------------
        //---------------------------------------------------
        void AppNotificationSystem::ScheduleNotificationForTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority)
        {
            if (m_enabled == true)
            {
                NotificationContainer notificationContainer;
                notificationContainer.m_notification = std::make_shared<Notification>();
                notificationContainer.m_notification->m_id = in_id;
                notificationContainer.m_notification->m_params = in_params;
                notificationContainer.m_notification->m_priority = in_priority;
                notificationContainer.m_triggerTime = in_time;
                m_notifications.push_back(notificationContainer);
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void AppNotificationSystem::GetScheduledNotifications(std::vector<NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time, TimeIntervalSecs in_period) const
        {
            for (const NotificationContainer& notificationContainer : m_notifications)
            {
                if (notificationContainer.m_triggerTime > in_time && notificationContainer.m_triggerTime <= in_time + in_period)
                {
                    out_notifications.push_back(notificationContainer.m_notification);
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void AppNotificationSystem::CancelByID(Notification::ID in_id)
        {
            for(auto it = m_notifications.begin(); it != m_notifications.end();)
            {
                if(in_id == it->m_notification->m_id)
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
        IConnectableEvent<AppNotificationSystem::ReceivedDelegate>& AppNotificationSystem::GetReceivedEvent()
        {
            return m_receivedEvent;
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void AppNotificationSystem::OnUpdate(f32 in_deltaTime)
        {
            TimeIntervalSecs currentTime = Application::Get()->GetSystemTime();
            
            for(auto it = m_notifications.begin(); it != m_notifications.end();)
            {
                if(currentTime >= it->m_triggerTime)
                {
                    m_receivedEvent.NotifyConnections(it->m_notification);
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
