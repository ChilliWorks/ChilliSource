//
//  RemoteNotificationSystem.cpp
//  MoFlow
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Notifications/RemoteNotificationSystem.h>

#ifdef TARGET_OS_IPHONE
#	include <ChilliSource/Backend/Platform/iOS/Core/Notification/RemoteNotificationSystem.h>
#elif defined(TARGET_ANDROID)
#	ifdef MOFLOWSKU_REMOTENOTIFICATIONMETHOD_GOOGLEPLAY
#		include <ChilliSource/Backend/Platform/Android/GooglePlay/GooglePlayRemoteNotificationSystem.h>
#	elif defined(MOFLOWSKU_REMOTENOTIFICATIONMETHOD_AMAZON)
#		include <ChilliSource/Backend/Platform/Android/Amazon/AmazonRemoteNotificationSystem.h>
#	endif
#endif

namespace ChilliSource
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
        	return new iOS::CRemoteNotificationSystem();
#elif defined(TARGET_ANDROID)
#	ifdef MOFLOWSKU_REMOTENOTIFICATIONMETHOD_GOOGLEPLAY
        	return new Android::CGooglePlayRemoteNotificationSystem();
#	elif defined(MOFLOWSKU_REMOTENOTIFICATIONMETHOD_AMAZON)
        	return new Android::CAmazonRemoteNotificationSystem();
#	endif
#endif
        	return nullptr;
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
