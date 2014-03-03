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
		CAccelerometer::CAccelerometer()
			: mbIsUpdating(false)
		{
			//get the accelerometer java interface or create it if it doesn't yet exist.
			mpAccelerometerJI = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CAccelerometerJavaInterface>();
			if (mpAccelerometerJI == NULL)
			{
				mpAccelerometerJI = AccelerometerJavaInterfacePtr(new CAccelerometerJavaInterface());
				CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpAccelerometerJI);
			}
		}
		//------------------------------------------------
		/// Is A
		//------------------------------------------------
		bool CAccelerometer::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Accelerometer::InterfaceID;
		}
		//----------------------------------------------------
		/// Is Updating
		///
		/// @return whether or not the accelerometer is
        /// currently updating.
		//----------------------------------------------------
		bool CAccelerometer::IsUpdating() const
		{
			return mbIsUpdating;
		}
		//----------------------------------------------------
		/// Start Updating
		//----------------------------------------------------
		void CAccelerometer::StartUpdating()
		{
			if (false == mbIsUpdating)
			{
				mbIsUpdating = true;
				mpAccelerometerJI->StartListening(Core::MakeDelegate(this, &CAccelerometer::OnAccelerationChanged));
			}
		}
		//------------------------------------------------
		/// Get Acceleration
		//------------------------------------------------
		Core::Vector3 CAccelerometer::GetAcceleration() const
		{
			return mvAcceleration;
		}
		//----------------------------------------------------
		/// Stop Updating
		//----------------------------------------------------
		void CAccelerometer::StopUpdating()
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
		void CAccelerometer::OnAccelerationChanged(const Core::Vector3& invAcceleration)
		{
			mvAcceleration = invAcceleration;
			mAccelerationUpdatedEvent.NotifyConnections(mvAcceleration);
		}
	}
}
