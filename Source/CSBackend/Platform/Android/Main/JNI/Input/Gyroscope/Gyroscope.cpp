//
//  Gyroscope.cpp
//  ChilliSource
//  Created by Jordan Brown on 20/01/2017
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#include <CSBackend/Platform/Android/Main/JNI/Input/Gyroscope/Gyroscope.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Main/JNI/Input/Gyroscope/GyroscopeJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(Gyroscope);
		//------------------------------------------------
		//------------------------------------------------
		Gyroscope::Gyroscope()
			: m_isUpdating(false)
		{
		}
		//------------------------------------------------
		//------------------------------------------------
		bool Gyroscope::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == ChilliSource::Gyroscope::InterfaceID || in_interfaceId == Gyroscope::InterfaceID);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		bool Gyroscope::IsUpdating() const
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to check if gyroscope was updating outside of the main thread.");
			return m_isUpdating;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void Gyroscope::StartUpdating()
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to start the gyroscope outside of the main thread.");
			if (false == m_isUpdating)
			{
				m_isUpdating = true;
				m_gyroscopeJI->StartListening(ChilliSource::MakeDelegate(this, &Gyroscope::OnOrientationChanged));
			}
		}
		//------------------------------------------------
		//------------------------------------------------
		ChilliSource::Quaternion Gyroscope::GetOrientation() const
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to read the gyroscope outside of the main thread.");
			return m_orientation;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		ChilliSource::IConnectableEvent<Gyroscope::OrientationUpdatedDelegate>& Gyroscope::GetOrientationUpdatedEvent()
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to get orientation update event outside of the main thread.");
			return m_orientationUpdatedEvent;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void Gyroscope::StopUpdating()
		{
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to stop the gyroscope outside of the main thread.");
			if (true == m_isUpdating)
			{
				m_gyroscopeJI->StopListening();
				m_isUpdating = false;
			}
		}
        //----------------------------------------------------
		//----------------------------------------------------
        void Gyroscope::OnInit()
        {
			m_gyroscopeJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<GyroscopeJavaInterface>();
			if (m_gyroscopeJI == nullptr)
			{
				m_gyroscopeJI = GyroscopeJavaInterfaceSPtr(new GyroscopeJavaInterface());
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_gyroscopeJI);
			}
        }
        //----------------------------------------------------
		//----------------------------------------------------
        void Gyroscope::OnResume()
        {
        	//Android continues to send gyroscope events even if
        	//the app is suspended so we may have had to manually stop this
        	//and now must restart listening
        	if(true == m_isUpdating)
        	{
        		m_gyroscopeJI->StartListening(ChilliSource::MakeDelegate(this, &Gyroscope::OnOrientationChanged));
        	}
        }
        //----------------------------------------------------
		//----------------------------------------------------
        void Gyroscope::OnSuspend()
        {
        	//Android continues to send gyroscope events even if
        	//the app is suspended. We don't want this
        	if(true == m_isUpdating)
        	{
        		m_gyroscopeJI->StopListening();
        	}
        }
        //----------------------------------------------------
		//----------------------------------------------------
        void Gyroscope::OnDestroy()
        {
        	m_gyroscopeJI.reset();
        }
		//------------------------------------------------
		//------------------------------------------------
		void Gyroscope::OnOrientationChanged(const ChilliSource::Quaternion& in_orientation)
		{
            m_orientation = in_orientation;
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
			    m_orientationUpdatedEvent.NotifyConnections(m_orientation);
			});
		}
	}
}


#endif
