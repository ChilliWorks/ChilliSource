//
//  ForwardDeclarations.h
//  Chilli Source
//  Created by Ian Copland on 04/03/2014.
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

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CSBACKEND_PLATFORM_IOS_FORWARDDECLARATIONS_H_
#define _CSBACKEND_PLATFORM_IOS_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace CSBackend
{
    namespace iOS
    {
        //------------------------------------------------------
		/// Core
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(PlatformSystem);
        CS_FORWARDDECLARE_CLASS(DialogueBoxSystem);
        CS_FORWARDDECLARE_CLASS(FileSystem);
        CS_FORWARDDECLARE_CLASS(PNGImageProvider);
        CS_FORWARDDECLARE_CLASS(LocalNotificationSystem);
        CS_FORWARDDECLARE_CLASS(RemoteNotificationSystem);
        CS_FORWARDDECLARE_CLASS(Device);
        CS_FORWARDDECLARE_CLASS(Screen);
        //------------------------------------------------------
		/// Input
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Accelerometer);
        CS_FORWARDDECLARE_CLASS(DeviceButtonSystem);
        CS_FORWARDDECLARE_CLASS(PointerSystem);
        CS_FORWARDDECLARE_CLASS(TextEntry);
        //------------------------------------------------------
		/// Networking
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(TouchScreen);
        CS_FORWARDDECLARE_CLASS(HttpRequestSystem);
        CS_FORWARDDECLARE_CLASS(HttpRequest);
        CS_FORWARDDECLARE_CLASS(IAPSystem);
        //------------------------------------------------------
		/// Social
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(EmailComposer);
        CS_FORWARDDECLARE_CLASS(FacebookAuthenticationSystem);
        CS_FORWARDDECLARE_CLASS(FacebookPostSystem);
        //------------------------------------------------------
		/// Video
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(VideoPlayer);
        //------------------------------------------------------
		/// Web
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(WebView);
    }
}

#endif

#endif