//
//  RemoteNotificationSystem.h
//  Chilli Source
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_NOTIFICATIONS_REMOTENOTIFICATIONSYSTEM_H_
#define _CHILLISOURCE_CORE_NOTIFICATIONS_REMOTENOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>

namespace ChilliSource
{
	namespace Core
    {
        //---------------------------------------------------
        /// A system for listening for remote/push notifications.
        ///
        /// @author Robert Henning
        //---------------------------------------------------
        class RemoteNotificationSystem : public Core::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(RemoteNotificationSystem);
            //---------------------------------------------------
            /// Typedefs
            //---------------------------------------------------
            using TokenReceivedDelegate = std::function<void(const std::string&)>;
            using NotificationReceivedDelegate = std::function<void(const NotificationCSPtr&)>;
            //---------------------------------------------------
            /// Creates a new platform specfic instance of the remote
            /// notification system.
            ///
            /// @author Robert Henning
            //---------------------------------------------------
            static RemoteNotificationSystemUPtr Create();
            //--------------------------------------------------
            /// Enables and disables addition of remote notifications.
            /// All existing notifications will be cancelled
            /// when this is disabled. This is enabled by default.
            ///
            /// @author I Copland
            ///
            /// @param Whether or not to enable the scheduling
            /// of app notifications.
            //---------------------------------------------------
            virtual void SetEnabled(bool in_enabled) = 0;
            //--------------------------------------------------
            /// Request the remote token for this device for use
            /// with Push Notifications.
            ///
            /// @author Robert Henning
            ///
            /// @param The return delegate.
            //--------------------------------------------------
            virtual void RequestRemoteToken(const TokenReceivedDelegate& in_delegate) = 0;
            //--------------------------------------------------
            /// @author Robert Hennin
            ///
            /// @return The unique token that the push notification
            /// provider uses to identify the device
            //--------------------------------------------------
            virtual const std::string& GetRemoteToken() const = 0;
            //--------------------------------------------------
            /// @author Robert Hennin
            ///
            /// @return The ID for the Apple remote notification
            /// service
            //--------------------------------------------------
            virtual const std::string& GetProviderID() const = 0;
            //--------------------------------------------------
            /// @author I Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being recieved.
            //---------------------------------------------------
            virtual IConnectableEvent<NotificationReceivedDelegate>& GetRecievedEvent() = 0;
        };
    }
}

#endif
