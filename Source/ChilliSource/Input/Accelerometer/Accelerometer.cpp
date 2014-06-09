//
//  Accelerometer.cpp
//  Chilli Source
//  Created by Ian Copland on 13/06/2013
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

#include <ChilliSource/Input/Accelerometer/Accelerometer.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Input/Accelerometer/Accelerometer.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Input/Accelerometer/Accelerometer.h>
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
            return AccelerometerUPtr(new CSBackend::Android::Accelerometer());
#elif defined CS_TARGETPLATFORM_IOS
            if (CSBackend::iOS::Accelerometer::IsSupportedByDevice() == true)
            {
                return AccelerometerUPtr(new CSBackend::iOS::Accelerometer());
            }
            else
            {
                return nullptr;
            }
#else
            return nullptr;
#endif
        }
	}
}
