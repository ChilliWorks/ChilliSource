//
//  LocalNotificationSystem.cpp
//  Chilli Source
//  Created by Steven Hendrie on 13/12/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Main/JNI/Core/Notification/LocalNotificationSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Threading.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Notification/LocalNotificationJavaInterface.h>

namespace CSBackend
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
        bool LocalNotificationSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
        	return (LocalNotificationSystem::InterfaceID == in_interfaceId || ChilliSource::LocalNotificationSystem::InterfaceID == in_interfaceId);
        }
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::SetEnabled(bool in_enabled)
        {
            CS::Application::Get()->GetTaskScheduler()->ScheduleTask(CS::TaskType::k_system, [=](const CS::TaskContext& in_taskContext)
            {
                std::mutex enabledMutex;
                std::unique_lock<std::mutex> lock(enabledMutex);

                m_enabled = in_enabled;

                if (m_enabled == false)
                {
                    CancelAll();
                }
            });
        }
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::ScheduleNotificationForTime(ChilliSource::Notification::ID in_id, const ChilliSource::ParamDictionary& in_params, TimeIntervalSecs in_time, ChilliSource::Notification::Priority in_priority)
        {
            CS::Application::Get()->GetTaskScheduler()->ScheduleTask(CS::TaskType::k_system, [=](const CS::TaskContext& in_taskContext)
            {
                if (m_enabled == true)
                {
                    m_localNotificationJI->ScheduleNotificationForTime(in_id, in_params, in_time, in_priority);
                }
            });
        }
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::GetScheduledNotifications(std::vector<ChilliSource::NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time, TimeIntervalSecs in_period) const
		{
            std::vector<ChilliSource::NotificationCSPtr> notifications;
            TimeIntervalSecs time = in_time;
            TimeIntervalSecs period = in_period;
            CS::Application::Get()->GetTaskScheduler()->ScheduleTask(CS::TaskType::k_system, [&](const CS::TaskContext& in_taskContext)
            {
                m_localNotificationJI->GetScheduledNotifications(notifications, time, period);
            });
            out_notifications = notifications;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::CancelByID(ChilliSource::Notification::ID in_id)
		{
            CS::Application::Get()->GetTaskScheduler()->ScheduleTask(CS::TaskType::k_system, [=](const CS::TaskContext& in_taskContext)
            {
                m_localNotificationJI->CancelByID(in_id);
            });
		}
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::CancelAll()
		{
            CS::Application::Get()->GetTaskScheduler()->ScheduleTask(CS::TaskType::k_system, [=](const CS::TaskContext& in_taskContext)
            {
                m_localNotificationJI->CancelAll();
            });
		}
        //--------------------------------------------------
        //--------------------------------------------------
		ChilliSource::IConnectableEvent<ChilliSource::LocalNotificationSystem::ReceivedDelegate>& LocalNotificationSystem::GetReceivedEvent()
		{
            ChilliSource::IConnectableEvent<ChilliSource::LocalNotificationSystem::ReceivedDelegate>* receivedEvent = nullptr;
            CS::Application::Get()->GetTaskScheduler()->ScheduleTask(CS::TaskType::k_system, [&](const CS::TaskContext& in_taskContext)
            {
                receivedEvent = &m_recievedEvent;
            });
            return *receivedEvent;        	
		}
		//--------------------------------------------------
		//--------------------------------------------------
		void LocalNotificationSystem::OnNotificationReceived(ChilliSource::Notification::ID in_id, const ChilliSource::ParamDictionary& in_params, ChilliSource::Notification::Priority in_priority)
		{
            CS::Application::Get()->GetTaskScheduler()->ScheduleTask(CS::TaskType::k_system, [=](const CS::TaskContext& in_taskContext)
            {
                ChilliSource::NotificationSPtr notification = std::make_shared<ChilliSource::Notification>();
                notification->m_id = in_id;
                notification->m_params = in_params;
                notification->m_priority = in_priority;
                m_recievedEvent.NotifyConnections(notification);
            });
		}
	}
}

#endif
