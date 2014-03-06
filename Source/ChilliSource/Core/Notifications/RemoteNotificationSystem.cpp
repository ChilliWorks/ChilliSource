//
//  RemoteNotificationSystem.cpp
//  MoFlow
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Notifications/RemoteNotificationSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#	include <ChilliSource/Backend/Platform/iOS/Core/Notification/RemoteNotificationSystem.h>
#elif defined(CS_TARGETPLATFORM_ANDROID)
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
        CS_DEFINE_NAMEDTYPE(RemoteNotificationSystem);
        //---------------------------------------------------------------
        /// Create
        //---------------------------------------------------------------
        RemoteNotificationSystemUPtr RemoteNotificationSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
        	return RemoteNotificationSystemUPtr(new iOS::RemoteNotificationSystem());
#elif defined(CS_TARGETPLATFORM_ANDROID)
#	ifdef MOFLOWSKU_REMOTENOTIFICATIONMETHOD_GOOGLEPLAY
        	return RemoteNotificationSystemUPtr(new Android::CGooglePlayRemoteNotificationSystem());
#	elif defined(MOFLOWSKU_REMOTENOTIFICATIONMETHOD_AMAZON)
        	return RemoteNotificationSystemUPtr(new Android::CAmazonRemoteNotificationSystem());
#	endif
#endif
        	return nullptr;
        }
        //---------------------------------------------------------------
        /// Is A
        //---------------------------------------------------------------
        bool RemoteNotificationSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == RemoteNotificationSystem::InterfaceID;
        }
    }
}
