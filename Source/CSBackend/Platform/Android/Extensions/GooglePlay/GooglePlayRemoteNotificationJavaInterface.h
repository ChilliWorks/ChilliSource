//
//  GooglePlayRemoteNotificationJavaInterface.h
//  MoFlow
//
//  Created by Robert Henning on 23/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_GOOGLEPLAY_GOOGLEPLAYREMOTENOTIFICATIONJAVAINTERFACE_H_
#define _CHILLISOURCE_PLATFORM_ANDROID_GOOGLEPLAY_GOOGLEPLAYREMOTENOTIFICATIONJAVAINTERFACE_H_

#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

namespace CSBackend
{
    namespace Android
    {
        class GooglePlayRemoteNotificationJavaInterface : public IJavaInterface
        {
        public:
        	CS_DECLARE_NAMEDTYPE(GooglePlayRemoteNotificationJavaInterface);
        	//--------------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------------
        	GooglePlayRemoteNotificationJavaInterface();
        	//--------------------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType inInterfaceID) const;
			//--------------------------------------------------------------
			/// Request Remote Token
			///
			/// Requests a new token for Google Cloud Messaging
			//--------------------------------------------------------------
			void RequestRemoteToken();
			//-------------------------------------------------------------------------
			/// On Remote Token Received
			///
			/// @param The unique token that the push notification provider uses to
			/// identify the device
			//-------------------------------------------------------------------------
			void OnRemoteTokenReceived(const std::string& instrToken);
			//-------------------------------------------------------------------------
			/// On Remote Notification Received
			///
			/// @param Dictionary of notification keys and values.
			//-------------------------------------------------------------------------
			void OnRemoteNotificationReceived(const CSCore::ParamDictionary& insParams);
        };
    }
}

#endif

#endif

#endif
