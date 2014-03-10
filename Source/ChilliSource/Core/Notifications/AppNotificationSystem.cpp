//
//  AppNotificationSystem.cpp
//  Chilli Source
//
//  Created by I Copland on 10/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Notifications/AppNotificationSystem.h>

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
                m_timedAppNotifications.push_back(notificationContainer);
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
                m_timedAppNotifications.push_back(notificationContainer);
            }
        }
        //---------------------------------------------------
        //----------------------------------------------------
        void AppNotificationSystem::ScheduleNotificationAfterTime(Notification::ID in_id, const ParamDictionary& in_params, TimeIntervalSecs in_time, Notification::Priority in_priority)
        {
            if (m_enabled == true)
            {
                NotificationContainerSPtr notificationContainer = std::make_shared<NotificationContainer>();
                notificationContainer->m_notification = std::make_shared<Notification>(in_id, in_params, in_priority);
                notificationContainer->m_triggerTime = Application::Get()->GetSystemTime() + in_time;
                m_timedAppNotifications.push_back(notificationContainer);
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void AppNotificationSystem::OnUpdate(f32 in_deltaTime)
        {
            TimeIntervalSecs currentTime = Core::Application::Get()->GetSystemTime();
            
            for(auto it = m_timedAppNotifications.begin(); it != m_timedAppNotifications.end();)
            {
                if(currentTime >= (*it)->m_triggerTime)
                {
                    m_recievedEvent.NotifyConnections((*it)->m_notification);
                    it = m_timedAppNotifications.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void AppNotificationSystem::CancelAll()
        {
            m_timedAppNotifications.clear();
        }
    }
}
