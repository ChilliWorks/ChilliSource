//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 09/08/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
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
		CS_FORWARDDECLARE_CLASS(ImageProvider);
		CS_FORWARDDECLARE_CLASS(LocalNotificationJavaInterface);
		CS_FORWARDDECLARE_CLASS(LocalNotificationScheduler);
		//------------------------------------------------------
		/// Extensions
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(AmazonIAPJavaInterface);
		CS_FORWARDDECLARE_CLASS(AmazonIAPSystem);
		CS_FORWARDDECLARE_CLASS(GooglePlayExpansionSystem);
		CS_FORWARDDECLARE_CLASS(GooglePlayIAPJavaInterface);
		CS_FORWARDDECLARE_CLASS(GooglePlayIAPSystem);
		CS_FORWARDDECLARE_CLASS(GooglePlayRemoteNotificationSystem);
		CS_FORWARDDECLARE_CLASS(GooglePlayRemoteNotificationJavaInterface);
		//------------------------------------------------------
		/// Input
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(Accelerometer);
		CS_FORWARDDECLARE_CLASS(AccelerometerJavaInterface);
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
		CS_FORWARDDECLARE_CLASS(ContactInformationProvider);
		CS_FORWARDDECLARE_CLASS(ContactInformationProviderJavaInterface);
		CS_FORWARDDECLARE_CLASS(EmailCompositionActivity);
		CS_FORWARDDECLARE_CLASS(EmailCompositionJavaInterface);
		CS_FORWARDDECLARE_CLASS(SMSCompositionActivity);
		CS_FORWARDDECLARE_CLASS(SMSCompositionActivityJavaInterface);
		CS_FORWARDDECLARE_CLASS(FacebookAuthentication);
		CS_FORWARDDECLARE_CLASS(FacebookPostSystem);
		CS_FORWARDDECLARE_CLASS(FacebookJavaInterface);
		CS_FORWARDDECLARE_CLASS(TwitterAuthenticationActivity);
		CS_FORWARDDECLARE_CLASS(TwitterPostSystem);
		CS_FORWARDDECLARE_CLASS(TwitterAuthenticationActivityJavaInterface);
		//------------------------------------------------------
		/// Video
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(VideoPlayerJavaInterface);
		CS_FORWARDDECLARE_CLASS(VideoPlayerActivity);
		//------------------------------------------------------
		/// Web
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(WebViewJavaInterface);
		CS_FORWARDDECLARE_CLASS(WebViewActivity);
	}
}

#endif
