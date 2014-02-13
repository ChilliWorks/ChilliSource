//
//  Accelerometer.cpp
//  moFlow
//
// Created by Ian Copland on 13/06/2013
// Copyright 2013 Tag Games Limited. All rights reserved
//

#include <ChilliSource/Input/Accelerometer.h>

namespace moFlo
{
	namespace Input
	{
		DEFINE_NAMED_INTERFACE(IAccelerometer);
		//----------------------------------------------------
		/// Constructor
		//----------------------------------------------------
		IAccelerometer::IAccelerometer()
		{
		}
		//----------------------------------------------------
		/// Get Acceleration Updated Event
		//----------------------------------------------------
		IEvent<IAccelerometer::AccelerationUpdatedDelegate>& IAccelerometer::GetAccelerationUpdatedEvent()
		{
			return mAccelerationUpdatedEvent;
		}
		//----------------------------------------------------
		/// Destructor
		//----------------------------------------------------
		IAccelerometer::~IAccelerometer()
		{
		}
	}
}
