//
//  IAPSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 10/06/2013.
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

#include <ChilliSource/Networking/IAP/IAPSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Networking/IAP/IAPSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY
#include <CSBackend/Platform/Android/Extensions/GooglePlay/GooglePlayIAPSystem.h>
#elif defined(CS_ANDROIDEXTENSION_AMAZON)
#include <CSBackend/Platform/Android/Extensions/Amazon/AmazonIAPSystem.h>
#endif
#endif

namespace ChilliSource
{
	namespace Networking
    {
        CS_DEFINE_NAMEDTYPE(IAPSystem);
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        IAPSystemUPtr IAPSystem::Create(const Core::ParamDictionary& inParams)
        {
#ifdef CS_TARGETPLATFORM_IOS
        	return IAPSystemUPtr(new CSBackend::iOS::IAPSystem());
#elif defined(CS_TARGETPLATFORM_ANDROID)
#	ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY
        	return IAPSystemUPtr(new CSBackend::Android::GooglePlayIAPSystem(inParams));
#	elif defined(CS_ANDROIDEXTENSION_AMAZON)
        	return IAPSystemUPtr(new CSBackend::Android::AmazonIAPSystem(inParams));
#	endif
#endif
        	return nullptr;
        }
    }
}

