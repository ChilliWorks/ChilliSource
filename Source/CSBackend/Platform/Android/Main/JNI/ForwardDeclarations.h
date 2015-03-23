//
//  ForwardDeclarations.h
//  Chilli Source
//  Created by Ian Copland on 09/08/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_FORWARDDECLARATIONS_H_
#define _CSBACKEND_PLATFORM_ANDROID_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------
		/// Core
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(JavaInterfaceManager);
		CS_FORWARDDECLARE_CLASS(IJavaInterface);
		CS_FORWARDDECLARE_CLASS(CoreJavaInterface);
		CS_FORWARDDECLARE_CLASS(DeviceJavaInterface);
		CS_FORWARDDECLARE_CLASS(PlatformSystem);
		CS_FORWARDDECLARE_CLASS(DialogueBoxJavaInterface);
		CS_FORWARDDECLARE_CLASS(DialogueBoxSystem);
		CS_FORWARDDECLARE_CLASS(FileStream);
		CS_FORWARDDECLARE_CLASS(FileStreamAPK);
		CS_FORWARDDECLARE_CLASS(FileSystem);
		CS_FORWARDDECLARE_CLASS(PngImage);
		CS_FORWARDDECLARE_CLASS(PNGImageProvider);
		CS_FORWARDDECLARE_CLASS(LocalNotificationJavaInterface);
		CS_FORWARDDECLARE_CLASS(LocalNotificationScheduler);
        CS_FORWARDDECLARE_CLASS(Device);
        CS_FORWARDDECLARE_CLASS(Screen);
		//------------------------------------------------------
		/// Extensions
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(AmazonIAPJavaInterface);
		CS_FORWARDDECLARE_CLASS(AmazonIAPSystem);
		CS_FORWARDDECLARE_CLASS(GooglePlayExpansionSystem);
		CS_FORWARDDECLARE_CLASS(GooglePlayExpansionJavaInterface);
		CS_FORWARDDECLARE_CLASS(GooglePlayIAPJavaInterface);
		CS_FORWARDDECLARE_CLASS(GooglePlayIAPSystem);
		CS_FORWARDDECLARE_CLASS(GooglePlayRemoteNotificationSystem);
		CS_FORWARDDECLARE_CLASS(GooglePlayRemoteNotificationJavaInterface);
		//------------------------------------------------------
		/// Input
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(Accelerometer);
		CS_FORWARDDECLARE_CLASS(AccelerometerJavaInterface);
		CS_FORWARDDECLARE_CLASS(DeviceButtonSystem);
		CS_FORWARDDECLARE_CLASS(DeviceButtonJavaInterface);
		CS_FORWARDDECLARE_CLASS(PointerSystem);
        CS_FORWARDDECLARE_CLASS(TextEntry);
		CS_FORWARDDECLARE_CLASS(TextEntryJavaInterface);
		CS_FORWARDDECLARE_CLASS(TouchInputJavaInterface);
		//------------------------------------------------------
		/// Networking
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(HttpRequestJavaInterface);
		CS_FORWARDDECLARE_CLASS(HttpRequest);
		CS_FORWARDDECLARE_CLASS(HttpRequestSystem);
		//------------------------------------------------------
		/// Social
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(EmailComposer);
		CS_FORWARDDECLARE_CLASS(EmailComposerJavaInterface);
		CS_FORWARDDECLARE_CLASS(FacebookAuthentication);
		CS_FORWARDDECLARE_CLASS(FacebookPostSystem);
		CS_FORWARDDECLARE_CLASS(FacebookJavaInterface);
		//------------------------------------------------------
		/// Video
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(VideoPlayerJavaInterface);
		CS_FORWARDDECLARE_CLASS(VideoPlayerActivity);
		//------------------------------------------------------
		/// Web
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(WebViewJavaInterface);
		CS_FORWARDDECLARE_CLASS(WebView);
	}
}

#endif

#endif
