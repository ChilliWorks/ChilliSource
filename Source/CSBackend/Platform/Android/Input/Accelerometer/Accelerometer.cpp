//
//  Accelerometer.cpp
//  Chilli Source
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

#include <CSBackend/Platform/Android/Input/Accelerometer/Accelerometer.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Input/Accelerometer/AccelerometerJavaInterface.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>

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
		bool Accelerometer::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == CSInput::Accelerometer::InterfaceID || in_interfaceId == Accelerometer::InterfaceID);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		bool Accelerometer::IsUpdating() const
		{
			return m_isUpdating;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void Accelerometer::StartUpdating()
		{
			if (false == m_isUpdating)
			{
				m_isUpdating = true;
				m_accelerometerJI->StartListening(CSCore::MakeDelegate(this, &Accelerometer::OnAccelerationChanged));
			}
		}
		//------------------------------------------------
		//------------------------------------------------
		CSCore::Vector3 Accelerometer::GetAcceleration() const
		{
			return m_acceleration;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		CSCore::IConnectableEvent<Accelerometer::AccelerationUpdatedDelegate>& Accelerometer::GetAccelerationUpdatedEvent()
		{
			return m_accelerationUpdatedEvent;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void Accelerometer::StopUpdating()
		{
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
        		m_accelerometerJI->StartListening(CSCore::MakeDelegate(this, &Accelerometer::OnAccelerationChanged));
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
		void Accelerometer::OnAccelerationChanged(const CSCore::Vector3& in_acceleration)
		{
			m_acceleration = in_acceleration;
			m_accelerationUpdatedEvent.NotifyConnections(m_acceleration);
		}
	}
}


#endif