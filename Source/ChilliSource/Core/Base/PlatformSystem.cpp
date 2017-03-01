//
//  PlatformSystem.cpp
//  ChilliSource
//  Created by Ian Copland on 24/02/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Core/Base/PlatformSystem.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Main/JNI/Core/Base/PlatformSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#import <CSBackend/Platform/iOS/Core/Base/PlatformSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Core/Base/PlatformSystem.h>
#endif

#ifdef CS_TARGET_PLATFORM_RPI
#include <CSBackend/Platform/RPi/Core/Base/PlatformSystem.h>
#endif

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(PlatformSystem);
    //-----------------------------------------
    //-----------------------------------------
    PlatformSystemUPtr PlatformSystem::Create()
    {
#ifdef CS_TARGETPLATFORM_IOS
        return PlatformSystemUPtr(new CSBackend::iOS::PlatformSystem());
#elif defined CS_TARGETPLATFORM_ANDROID
        return PlatformSystemUPtr(new CSBackend::Android::PlatformSystem());
#elif defined CS_TARGETPLATFORM_WINDOWS
        return PlatformSystemUPtr(new CSBackend::Windows::PlatformSystem());
#elif defined CS_TARGETPLATFORM_RPI
        return PlatformSystemUPtr(new CSBackend::RPi::PlatformSystem());
#else
        return nullptr;
#endif
    }
}
