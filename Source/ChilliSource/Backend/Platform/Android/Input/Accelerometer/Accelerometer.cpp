//
//  Accelerometer.cpp
//  Chilli Source
//
//  Created by I Copland on 10/06/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Input/Accelerometer/Accelerometer.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Input/Accelerometer/AccelerometerJavaInterface.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>

namespace ChilliSource
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
		bool Accelerometer::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == Input::Accelerometer::InterfaceID || in_interfaceId == Accelerometer::InterfaceID);
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
				m_accelerometerJI->StartListening(Core::MakeDelegate(this, &Accelerometer::OnAccelerationChanged));
			}
		}
		//------------------------------------------------
		//------------------------------------------------
		Core::Vector3 Accelerometer::GetAcceleration() const
		{
			return m_acceleration;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		Core::IConnectableEvent<Accelerometer::AccelerationUpdatedDelegate>& Accelerometer::GetAccelerationUpdatedEvent()
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
        void Accelerometer::OnDestroy()
        {
        	m_accelerometerJI.reset();
        }
		//------------------------------------------------
		//------------------------------------------------
		void Accelerometer::OnAccelerationChanged(const Core::Vector3& in_acceleration)
		{
			m_acceleration = in_acceleration;
			m_accelerationUpdatedEvent.NotifyConnections(m_acceleration);
		}
	}
}
