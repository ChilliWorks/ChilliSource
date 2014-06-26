//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 09/08/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
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
		CS_FORWARDDECLARE_CLASS(UDIDManager);
		CS_FORWARDDECLARE_CLASS(PlatformSystem);
		CS_FORWARDDECLARE_CLASS(DialogueBoxJavaInterface);
		CS_FORWARDDECLARE_CLASS(DialogueBoxSystem);
		CS_FORWARDDECLARE_CLASS(FileStream);
		CS_FORWARDDECLARE_CLASS(FileStreamAPK);
		CS_FORWARDDECLARE_CLASS(FileSystem);
		CS_FORWARDDECLARE_CLASS(SharedPreferencesJavaInterface);
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
		CS_FORWARDDECLARE_CLASS(VirtualKeyboard);
		CS_FORWARDDECLARE_CLASS(KeyboardJavaInterface);
		CS_FORWARDDECLARE_CLASS(PointerSystem);
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