//
//  Accelerometer.cpp
//  ChilliSource
//  Created by Ian Copland on 10/06/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#include <CSBackend/Platform/Android/Main/JNI/Input/Accelerometer/Accelerometer.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Main/JNI/Input/Accelerometer/AccelerometerJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(Accelerometer);
		//------------------------------------------------
		//------------------------------------------------
		Accelerometer::Accelerometer()
			: m_isUpdating(false)
		{
		}
		//------------------------------------------------
		//------------------------------------------------
		bool Accelerometer::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == ChilliSource::Accelerometer::InterfaceID || in_interfaceId == Accelerometer::InterfaceID);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		bool Accelerometer::IsUpdating() const
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to check if accelerometer was updating outside of the main thread.");
			return m_isUpdating;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void Accelerometer::StartUpdating()
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to start the accelerometer outside of the main thread.");
			if (false == m_isUpdating)
			{
				m_isUpdating = true;
				m_accelerometerJI->StartListening(ChilliSource::MakeDelegate(this, &Accelerometer::OnAccelerationChanged));
			}
		}
		//------------------------------------------------
		//------------------------------------------------
		ChilliSource::Vector3 Accelerometer::GetAcceleration() const
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to read the accelerometer outside of the main thread.");
			return m_acceleration;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		ChilliSource::IConnectableEvent<Accelerometer::AccelerationUpdatedDelegate>& Accelerometer::GetAccelerationUpdatedEvent()
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to get acceleration update event outside of the main thread.");
			return m_accelerationUpdatedEvent;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void Accelerometer::StopUpdating()
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to stop the accelerometer outside of the main thread.");
			if (true == m_isUpdating)
			{
				m_accelerometerJI->StopListening();
				m_isUpdating = false;
			}
		}
        //----------------------------------------------------
		//----------------------------------------------------
        void Accelerometer::OnInit()
        {
			m_accelerometerJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<AccelerometerJavaInterface>();
			if (m_accelerometerJI == nullptr)
			{
				m_accelerometerJI = AccelerometerJavaInterfaceSPtr(new AccelerometerJavaInterface());
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_accelerometerJI);
			}
        }
        //----------------------------------------------------
		//----------------------------------------------------
        void Accelerometer::OnResume()
        {
        	//Android continues to send accelerometer events even if
        	//the app is suspended so we may have had to manually stop this
        	//and now must restart listening
        	if(true == m_isUpdating)
        	{
        		m_accelerometerJI->StartListening(ChilliSource::MakeDelegate(this, &Accelerometer::OnAccelerationChanged));
        	}
        }
        //----------------------------------------------------
		//----------------------------------------------------
        void Accelerometer::OnSuspend()
        {
        	//Android continues to send accelerometer events even if
        	//the app is suspended. We don't want this
        	if(true == m_isUpdating)
        	{
        		m_accelerometerJI->StopListening();
        	}
        }
        //----------------------------------------------------
		//----------------------------------------------------
        void Accelerometer::OnDestroy()
        {
        	m_accelerometerJI.reset();
        }
		//------------------------------------------------
		//------------------------------------------------
		void Accelerometer::OnAccelerationChanged(const ChilliSource::Vector3& in_acceleration)
		{
            m_acceleration = in_acceleration;
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
			    m_accelerationUpdatedEvent.NotifyConnections(m_acceleration);
			});
		}
	}
}


#endif
