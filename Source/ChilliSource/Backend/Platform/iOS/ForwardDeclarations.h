//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_FORWARDDECLARATIONS_H_

namespace ChilliSource
{
    namespace iOS
    {
        //------------------------------------------------------
		/// Core
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(PlatformSystem);
        CS_FORWARDDECLARE_CLASS(FileSystem);
        CS_FORWARDDECLARE_CLASS(ImageLoader);
        CS_FORWARDDECLARE_CLASS(LocalNotificationScheduler);
        CS_FORWARDDECLARE_CLASS(RemoteNotificationSystem);
        //------------------------------------------------------
		/// Input
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Accelerometer);
        CS_FORWARDDECLARE_CLASS(InputSystem);
        CS_FORWARDDECLARE_CLASS(VirtualKeyboard);
        CS_FORWARDDECLARE_CLASS(TouchScreen);
        //------------------------------------------------------
		/// Networking
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(TouchScreen);
        CS_FORWARDDECLARE_CLASS(iCloudSystem);
        CS_FORWARDDECLARE_CLASS(HttpConnectionSystem);
        CS_FORWARDDECLARE_CLASS(IAPSystem);
        //------------------------------------------------------
		/// Social
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(ContactInformationProvider);
        CS_FORWARDDECLARE_CLASS(EmailCompositionActivity);
        CS_FORWARDDECLARE_CLASS(SMSCompositionActivity);
        CS_FORWARDDECLARE_CLASS(FacebookAuthenticationSystem);
        CS_FORWARDDECLARE_CLASS(FacebookPostSystem);
        CS_FORWARDDECLARE_CLASS(GameCentreSystem);
        CS_FORWARDDECLARE_CLASS(TwitterAuthenticationActivity);
        CS_FORWARDDECLARE_CLASS(TwitterPostSystem);
        //------------------------------------------------------
		/// Video
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(VideoPlayerActivity);
        //------------------------------------------------------
		/// Web
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(WebViewActivity);
    }
}

#endif
