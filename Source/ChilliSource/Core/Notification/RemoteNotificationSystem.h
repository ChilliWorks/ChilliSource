//
//  RemoteNotificationSystem.h
//  Chilli Source
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

#ifndef _CHILLISOURCE_CORE_NOTIFICATION_REMOTENOTIFICATIONSYSTEM_H_
#define _CHILLISOURCE_CORE_NOTIFICATION_REMOTENOTIFICATIONSYSTEM_H_

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
            /// @author Ian Copland
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
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being received.
            //---------------------------------------------------
            virtual IConnectableEvent<NotificationReceivedDelegate>& GetReceivedEvent() = 0;
        };
    }
}

#endif
