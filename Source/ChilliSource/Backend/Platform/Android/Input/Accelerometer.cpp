//
//  Accelerometer.cpp
//  moFlow
//
//  Created by Ian Copland on 10/06/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Platform/Android/Input/Accelerometer.h>
#include <ChilliSource/Platform/Android/JavaInterface/AccelerometerJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>


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
			return inInterfaceID == IAccelerometer::InterfaceID;
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
				mpAccelerometerJI->StartListening(CAccelerometerJavaInterface::AccelerationChangedDelegate(this, &CAccelerometer::OnAccelerationChanged));
			}
		}
		//------------------------------------------------
		/// Get Acceleration
		//------------------------------------------------
		const Core::CVector3& CAccelerometer::GetAcceleration() const
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
		void CAccelerometer::OnAccelerationChanged(const Core::CVector3& invAcceleration)
		{
			mvAcceleration = invAcceleration;
			mAccelerationUpdatedEvent.Invoke(mvAcceleration);
		}
		//------------------------------------------------
		/// Destructor
		//------------------------------------------------
		CAccelerometer::~CAccelerometer()
		{
		}
	}
}
