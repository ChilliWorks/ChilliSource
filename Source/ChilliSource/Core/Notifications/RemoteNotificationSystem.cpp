//
//  RemoteNotificationSystem.cpp
//  MoFlow
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <moFlo/Core/Notifications/RemoteNotificationSystem.h>

#ifdef TARGET_OS_IPHONE
#	include <moFlo/Platform/iOS/RemoteNotificationSystem.h>
#elif defined(TARGET_ANDROID)
#	ifdef MOFLOWSKU_REMOTENOTIFICATIONMETHOD_GOOGLEPLAY
#		include <moFlo/Platform/Android/GooglePlay/GooglePlayRemoteNotificationSystem.h>
#	elif defined(MOFLOWSKU_REMOTENOTIFICATIONMETHOD_AMAZON)
#		include <moFlo/Platform/Android/Amazon/AmazonRemoteNotificationSystem.h>
#	endif
#endif

namespace moFlo
{
	namespace Core
    {
        DEFINE_NAMED_INTERFACE(IRemoteNotificationSystem);
        //---------------------------------------------------------------
        /// Create
        //---------------------------------------------------------------
        IRemoteNotificationSystem* IRemoteNotificationSystem::Create()
        {
#ifdef TARGET_OS_IPHONE
        	return new iOSPlatform::CRemoteNotificationSystem();
#elif defined(TARGET_ANDROID)
#	ifdef MOFLOWSKU_REMOTENOTIFICATIONMETHOD_GOOGLEPLAY
        	return new AndroidPlatform::CGooglePlayRemoteNotificationSystem();
#	elif defined(MOFLOWSKU_REMOTENOTIFICATIONMETHOD_AMAZON)
        	return new AndroidPlatform::CAmazonRemoteNotificationSystem();
#	endif
#endif
        	return NULL;
        }
        //---------------------------------------------------------------
        /// Is A
        //---------------------------------------------------------------
        bool IRemoteNotificationSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == IRemoteNotificationSystem::InterfaceID;
        }
    }
}
