//
//  FacebookAuthenticationSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 01/06/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Social/Facebook/FacebookAuthenticationSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Social/Facebook/FacebookAuthenticationSystem.h>
#elif CS_TARGETPLATFORM_ANDROID
#ifdef CS_ANDROIDEXTENSION_FACEBOOK
#include <CSBackend/Platform/Android/Extensions/Facebook/FacebookAuthenticationSystem.h>
#endif
#endif

namespace ChilliSource
{
    namespace Social
    {
        CS_DEFINE_NAMEDTYPE(FacebookAuthenticationSystem);
        
        //------------------------------------------------
        //------------------------------------------------
        FacebookAuthenticationSystemUPtr FacebookAuthenticationSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return FacebookAuthenticationSystemUPtr(new CSBackend::iOS::FacebookAuthenticationSystem());
#elif CS_TARGETPLATFORM_ANDROID
#ifdef CS_ANDROIDEXTENSION_FACEBOOK
            return FacebookAuthenticationSystemUPtr(new CSBackend::Android::FacebookAuthenticationSystem());
#else
            return nullptr;
#endif
#else
			return nullptr;
#endif
        }
    }
}

