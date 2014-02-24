//
//  RemoteNotificationSystem.h
//  MoFlow
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_IOS_REMOTENOTIFICATIONSYSTEM_H_
#define _MOFLOW_PLATFORM_IOS_REMOTENOTIFICATIONSYSTEM_H_

#include <ChilliSource/Core/Notifications/RemoteNotificationSystem.h>

#include <UIKit/UIKit.h>

namespace ChilliSource
{
    namespace iOS
    {
        class CRemoteNotificationSystem : public Core::IRemoteNotificationSystem
        {
        public:
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
            void OnRemoteTokenReceived(NSData* inpToken);
            //-------------------------------------------------------------------------
            /// On Remote Notification Received
            ///
            /// Called when the app delegate receives a push notification
            ///
            /// @param UIApplication
            /// @param Dictionary payload of remote notification
            //-------------------------------------------------------------------------
            void OnRemoteNotificationReceived(UIApplication* inpApplication, NSDictionary* inpPayload);
            
        private:
            std::string                         mstrToken;
            Core::RemoteTokenReceivedDelegate   mDelegate;
        };
    }
}

#endif /* _MOFLOW_PLATFORM_IOS_REMOTENOTIFICATIONSYSTEM_H_ */
