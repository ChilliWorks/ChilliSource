//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
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
        CS_FORWARDDECLARE_CLASS(Keyboard);
        CS_FORWARDDECLARE_CLASS(PointerSystem);
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
