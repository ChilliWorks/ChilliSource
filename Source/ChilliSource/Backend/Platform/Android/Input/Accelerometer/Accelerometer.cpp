//
//  Accelerometer.cpp
//  moFlow
//
//  Created by Ian Copland on 10/06/2013.
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
		//------------------------------------------------
		/// Constructor
		//------------------------------------------------
		Accelerometer::Accelerometer()
			: mbIsUpdating(false)
		{
			//get the accelerometer java interface or create it if it doesn't yet exist.
			mpAccelerometerJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<AccelerometerJavaInterface>();
			if (mpAccelerometerJI == nullptr)
			{
				mpAccelerometerJI = AccelerometerJavaInterfaceSPtr(new AccelerometerJavaInterface());
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpAccelerometerJI);
			}
		}
		//------------------------------------------------
		/// Is A
		//------------------------------------------------
		bool Accelerometer::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::Accelerometer::InterfaceID;
		}
		//----------------------------------------------------
		/// Is Updating
		///
		/// @return whether or not the accelerometer is
        /// currently updating.
		//----------------------------------------------------
		bool Accelerometer::IsUpdating() const
		{
			return mbIsUpdating;
		}
		//----------------------------------------------------
		/// Start Updating
		//----------------------------------------------------
		void Accelerometer::StartUpdating()
		{
			if (false == mbIsUpdating)
			{
				mbIsUpdating = true;
				mpAccelerometerJI->StartListening(Core::MakeDelegate(this, &Accelerometer::OnAccelerationChanged));
			}
		}
		//------------------------------------------------
		/// Get Acceleration
		//------------------------------------------------
		Core::Vector3 Accelerometer::GetAcceleration() const
		{
			return mvAcceleration;
		}
		//----------------------------------------------------
		/// Stop Updating
		//----------------------------------------------------
		void Accelerometer::StopUpdating()
		{
			if (true == mbIsUpdating)
			{
				mpAccelerometerJI->StopListening();
				mbIsUpdating = false;
			}
		}
		//------------------------------------------------
		/// On Acceleration Changed
		//------------------------------------------------
		void Accelerometer::OnAccelerationChanged(const Core::Vector3& invAcceleration)
		{
			mvAcceleration = invAcceleration;
			mAccelerationUpdatedEvent.NotifyConnections(mvAcceleration);
		}
	}
}
