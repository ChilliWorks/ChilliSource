//
//  Accelerometer.cpp
//  moFlow
//
// Created by Ian Copland on 13/06/2013
// Copyright 2013 Tag Games Limited. All rights reserved
//

#include <ChilliSource/Input/Accelerometer/Accelerometer.h>

namespace ChilliSource
{
	namespace Input
	{
		CS_DEFINE_NAMEDTYPE(Accelerometer);
		//----------------------------------------------------
		/// Constructor
		//----------------------------------------------------
		Accelerometer::Accelerometer()
		{
		}
		//----------------------------------------------------
		/// Get Acceleration Updated Event
		//----------------------------------------------------
		Core::IConnectableEvent<Accelerometer::AccelerationUpdatedDelegate>& Accelerometer::GetAccelerationUpdatedEvent()
		{
			return mAccelerationUpdatedEvent;
		}
		//----------------------------------------------------
		/// Destructor
		//----------------------------------------------------
		Accelerometer::~Accelerometer()
		{
		}
	}
}
