//
//  LocalNotificationSystem.cpp
//  Chilli Source
//
//  Created by Steven Hendrie on 13/12/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#include <ChilliSource/Backend/Platform/Android/Core/Notification/LocalNotificationSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/Notification/LocalNotificationJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(LocalNotificationSystem);
        //--------------------------------------------------
        //--------------------------------------------------
		LocalNotificationSystem::LocalNotificationSystem()
		: m_enabled(true)
		{
        	//get the media player java interface or create it if it doesn't yet exist.
			m_localNotificationJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<LocalNotificationJavaInterface>();
        	if (m_localNotificationJI == nullptr)
        	{
        		m_localNotificationJI = LocalNotificationJavaInterfaceSPtr(new LocalNotificationJavaInterface());
        		JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_localNotificationJI);
        	}
		}
        //--------------------------------------------------
        //--------------------------------------------------
        bool LocalNotificationSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
        	return (LocalNotificationSystem::InterfaceID == in_interfaceId || Core::LocalNotificationSystem::InterfaceID == in_interfaceId);
        }
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::SetEnabled(bool in_enabled)
        {
			m_enabled = in_enabled;

			if (m_enabled == false)
			{
				CancelAll();
			}
        }
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::ScheduleNotificationForTime(Core::Notification::ID in_id, const Core::ParamDictionary& in_params, TimeIntervalSecs in_time, Core::Notification::Priority in_priority)
        {
			if (m_enabled == true)
			{
				m_localNotificationJI->ScheduleNotificationForTime(in_id, in_params, in_time, in_priority);
			}
        }
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::GetScheduledNotifications(std::vector<Core::NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time, TimeIntervalSecs in_period) const
		{
			CS_LOG_WARNING("LocalNotificationScheduler::GetScheduledNotifications() is unimplemented on Android!");
		}
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::CancelByID(Core::Notification::ID in_id)
		{
			m_localNotificationJI->CancelByID(in_id);
		}
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::CancelAll()
		{
			m_localNotificationJI->CancelAll();
		}
        //--------------------------------------------------
        //--------------------------------------------------
		Core::IConnectableEvent<Core::LocalNotificationSystem::RecievedDelegate>& LocalNotificationSystem::GetRecievedEvent()
		{
        	return m_recievedEvent;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::OnNotificationReceived(Core::Notification::ID in_id, const Core::ParamDictionary& in_params, Core::Notification::Priority in_priority)
		{
			Core::NotificationSPtr notification = std::make_shared<Core::Notification>();
			notification->m_id = in_id;
			notification->m_params = in_params;
			notification->m_priority = in_priority;
			m_recievedEvent.NotifyConnections(notification);
		}
	}
}

#endif