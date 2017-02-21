//
//  Gyroscope.cpp
//  ChilliSource
//  Created by Jordan Brown on 19/01/2017
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

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Input/Gyroscope/Gyroscope.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Main/JNI/Input/Gyroscope/Gyroscope.h>
#include <CSBackend/Platform/Android/Main/JNI/Input/Gyroscope/GyroscopeJavaInterface.h>
#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Input/Gyroscope/Gyroscope.h>
#endif

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Gyroscope);
    //----------------------------------------------------
    //----------------------------------------------------
    GyroscopeUPtr Gyroscope::Create()
    {
#if defined CS_TARGETPLATFORM_ANDROID
        // Check if avail. on android
        CSBackend::Android::GyroscopeJavaInterfaceSPtr gyroscopeJI = CSBackend::Android::GyroscopeJavaInterfaceSPtr(new CSBackend::Android::GyroscopeJavaInterface());

        if(gyroscopeJI->IsAvailable())
        {
            return GyroscopeUPtr(new CSBackend::Android::Gyroscope());
        }
        else
        {
            return nullptr;
        }
#elif defined CS_TARGETPLATFORM_IOS
        if (CSBackend::iOS::Gyroscope::IsSupportedByDevice() == true)
        {
            return GyroscopeUPtr(new CSBackend::iOS::Gyroscope());
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
