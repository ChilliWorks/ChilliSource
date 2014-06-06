//
//  RemoteNotificationSystem.h
//  Chilli Source
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_CORE_NOTIFICATION_REMOTENOTIFICATIONSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_CORE_NOTIFICATION_REMOTENOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Notification/RemoteNotificationSystem.h>

#include <UIKit/UIKit.h>

namespace ChilliSource
{
    namespace iOS
    {
        //---------------------------------------------------------
        /// The iOS backend for the remote notification system.
        ///
        /// @author Robert Henning
        //---------------------------------------------------------
        class RemoteNotificationSystem : public Core::RemoteNotificationSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(RemoteNotificationSystem);
            //--------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The interface ID to check against.
            ///
            /// @return Whether this implements the passed in
            /// interface id.
            //--------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------
            /// Request the remote token for this device for use
            /// with Push Notifications.
            ///
            /// @author Robert Henning
            ///
            /// @param The return delegate.
            //--------------------------------------------------
            void RequestRemoteToken(const TokenReceivedDelegate& in_delegate) override;
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
            void SetEnabled(bool in_enabled) override;
            //---------------------------------------------------
            /// @author Robert Henning
            ///
            /// @return The unique token that the push notification
            /// provider uses to identify the device
            //---------------------------------------------------
            const std::string& GetRemoteToken() const override;
            //---------------------------------------------------
            /// @author Robert Henning
            ///
            /// @return The ID for the Apple remote notification
            /// service
            //---------------------------------------------------
            const std::string& GetProviderID() const override;
            //--------------------------------------------------
            /// @author I Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being recieved.
            //---------------------------------------------------
            Core::IConnectableEvent<NotificationReceivedDelegate>& GetRecievedEvent() override;
            //---------------------------------------------------
            /// @author Robert Henning
            ///
            /// @param The unique token that the push notification
            /// provider uses to identify the device
            //---------------------------------------------------
            void OnRemoteTokenReceived(NSData* in_token);
            //---------------------------------------------------
            /// Called when the app delegate receives a push
            /// notification
            ///
            /// @author Robert Henning
            ///
            /// @param UIApplication
            /// @param Dictionary payload of remote notification
            //---------------------------------------------------
            void OnRemoteNotificationReceived(UIApplication* in_application, NSDictionary* in_payload);
            
        private:
            friend Core::RemoteNotificationSystemUPtr Core::RemoteNotificationSystem::Create();
            //----------------------------------------------------------
            /// Private constructor to force use of factory method.
            ///
            /// @author S Downie
            //----------------------------------------------------------
            RemoteNotificationSystem();
            
            bool m_enabled;
            std::string m_token;
            TokenReceivedDelegate m_delegate;
            Core::Event<NotificationReceivedDelegate> m_receivedEvent;
        };
    }
}

#endif

#endif