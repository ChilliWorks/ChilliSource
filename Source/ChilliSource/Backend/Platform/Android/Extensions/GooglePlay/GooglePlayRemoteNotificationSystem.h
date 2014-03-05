//
//  GooglePlayRemoteNotificationSystem.h
//  MoFlow
//
//  Created by Robert Henning on 23/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_ANDROID_GOOGLEPLAY_REMOTENOTIFICATIONSYSTEM_H_
#define _MOFLOW_PLATFORM_ANDROID_GOOGLEPLAY_REMOTENOTIFICATIONSYSTEM_H_

#include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayRemoteNotificationJavaInterface.h>
#include <ChilliSource/Core/Notifications/RemoteNotificationSystem.h>

namespace ChilliSource
{
    namespace Android
    {
        class GooglePlayRemoteNotificationSystem : public Core::RemoteNotificationSystem
        {
        public:
            //---------------------------------------------------------------
            /// Constructor
            //---------------------------------------------------------------
            GooglePlayRemoteNotificationSystem();
            //-------------------------------------------------------------------------
            /// Request Remote Token
            ///
            /// Request the remote token for this device for use with Push Notifications.
            ///
            /// @param The return delegate.
            //-------------------------------------------------------------------------
            void RequestRemoteToken(const Core::RemoteTokenReceivedDelegate& inDelegate);
            //-------------------------------------------------------------------------
            /// Get Remote Token
            ///
            /// @return The unique token that the push notification provider uses to
            /// identify the device
            //-------------------------------------------------------------------------
            const std::string& GetRemoteToken() const;
            //-------------------------------------------------------------------------
            /// Get Provider ID
            ///
            /// @return The ID for the Apple remote notification service
            //-------------------------------------------------------------------------
            const std::string& GetProviderID() const;
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
            /// Called when the app delegate receives a push notification
            ///
            /// @param Dictionary payload of remote notification
            //-------------------------------------------------------------------------
            void OnRemoteNotificationReceived(const Core::ParamDictionary& insParams);

        private:
            GooglePlayRemoteNotificationJavaInterfaceSPtr	mpJavaInterface;
            std::string										mstrToken;
            Core::RemoteTokenReceivedDelegate				mDelegate;
        };
    }
}

#endif /* _MOFLOW_PLATFORM_ANDROID_GOOGLEPLAY_REMOTENOTIFICATIONSYSTEM_H_ */
