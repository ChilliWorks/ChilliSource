//
//  Accelerometer.cpp
//  Chilli Source
//
// Created by Ian Copland on 13/06/2013
// Copyright 2013 Tag Games Limited. All rights reserved
//

#include <ChilliSource/Input/Accelerometer/Accelerometer.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#   include <ChilliSource/Backend/Platform/Android/Input/Accelerometer/Accelerometer.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#   include <ChilliSource/Backend/Platform/iOS/Input/Accelerometer/Accelerometer.h>
#endif

namespace ChilliSource
{
	namespace Input
	{
		CS_DEFINE_NAMEDTYPE(Accelerometer);
        //----------------------------------------------------
        //----------------------------------------------------
        AccelerometerUPtr Accelerometer::Create()
        {
#if defined CS_TARGETPLATFORM_ANDROID
            return AccelerometerUPtr(new Android::Accelerometer());
#elif defined CS_TARGETPLATFORM_IOS
            return AccelerometerUPtr(new iOS::Accelerometer());
#else
            return nullptr;
#endif
        }
	}
}
