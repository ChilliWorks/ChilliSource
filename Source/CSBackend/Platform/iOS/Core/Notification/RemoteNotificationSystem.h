//
//  RemoteNotificationSystem.h
//  ChilliSource
//  Created by Robert Henning on 22/01/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CSBACKEND_PLATFORM_IOS_CORE_NOTIFICATION_REMOTENOTIFICATIONSYSTEM_H_
#define _CSBACKEND_PLATFORM_IOS_CORE_NOTIFICATION_REMOTENOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Notification/RemoteNotificationSystem.h>

#include <UIKit/UIKit.h>

namespace CSBackend
{
    namespace iOS
    {
        //---------------------------------------------------------
        /// The iOS backend for the remote notification system.
        ///
        /// @author Robert Henning
        //---------------------------------------------------------
        class RemoteNotificationSystem : public ChilliSource::RemoteNotificationSystem
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
            bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
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
            /// @author Ian Copland
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
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being received.
            //---------------------------------------------------
            ChilliSource::IConnectableEvent<NotificationReceivedDelegate>& GetReceivedEvent() override;
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
            friend ChilliSource::RemoteNotificationSystemUPtr ChilliSource::RemoteNotificationSystem::Create();
            //----------------------------------------------------------
            /// Private constructor to force use of factory method.
            ///
            /// @author S Downie
            //----------------------------------------------------------
            RemoteNotificationSystem();
            //--------------------------------------------------
            /// Called when the system is initialised to request
            /// the permissions
            ///
            /// @author S Downie
            //--------------------------------------------------
            void OnInit() override;
            
            bool m_enabled;
            std::string m_token;
            TokenReceivedDelegate m_delegate;
            ChilliSource::Event<NotificationReceivedDelegate> m_receivedEvent;
        };
    }
}

#endif

#endif
