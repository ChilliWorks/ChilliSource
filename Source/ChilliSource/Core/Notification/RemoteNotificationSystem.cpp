//
//  RemoteNotificationSystem.cpp
//  Chilli Source
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Notification/RemoteNotificationSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#import <CSBackend/Platform/iOS/Core/Notification/RemoteNotificationSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#ifdef MOFLOWSKU_REMOTENOTIFICATIONMETHOD_GOOGLEPLAY
#include <CSBackend/Platform/Android/GooglePlay/GooglePlayRemoteNotificationSystem.h>
#elif defined(MOFLOWSKU_REMOTENOTIFICATIONMETHOD_AMAZON)
#include <CSBackend/Platform/Android/Amazon/AmazonRemoteNotificationSystem.h>
#endif
#endif

namespace ChilliSource
{
	namespace Core
    {
        CS_DEFINE_NAMEDTYPE(RemoteNotificationSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        RemoteNotificationSystemUPtr RemoteNotificationSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
        	return RemoteNotificationSystemUPtr(new iOS::RemoteNotificationSystem());
#elif defined(CS_TARGETPLATFORM_ANDROID)
#	ifdef MOFLOWSKU_REMOTENOTIFICATIONMETHOD_GOOGLEPLAY
        	return RemoteNotificationSystemUPtr(new CSBackend::Android::CGooglePlayRemoteNotificationSystem());
#	elif defined(MOFLOWSKU_REMOTENOTIFICATIONMETHOD_AMAZON)
        	return RemoteNotificationSystemUPtr(new CSBackend::Android::CAmazonRemoteNotificationSystem());
#	endif
#endif
        	return nullptr;
        }
    }
}
