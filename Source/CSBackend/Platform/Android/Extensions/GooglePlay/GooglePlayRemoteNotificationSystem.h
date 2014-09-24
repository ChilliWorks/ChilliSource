//
//  GooglePlayRemoteNotificationSystem.h
//  Chilli Source
//  Created by Robert Henning on 23/01/2014.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY

#ifndef _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_REMOTENOTIFICATIONSYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_REMOTENOTIFICATIONSYSTEM_H_

#include <CSBackend/Platform/Android/Extensions/GooglePlay/GooglePlayRemoteNotificationJavaInterface.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Notification/RemoteNotificationSystem.h>

namespace CSBackend
{
    namespace Android
    {
		//-------------------------------------------------------
		/// Google play specific backend for the remote notification
    	/// system.
    	///
    	/// @author Robert Henning
		//-------------------------------------------------------
        class GooglePlayRemoteNotificationSystem : public CSCore::RemoteNotificationSystem
        {
        public:
        	CS_DECLARE_NAMEDTYPE(GooglePlayRemoteNotificationSystem);
            //--------------------------------------------------
            /// @author Ian Copland
            ///
        	/// @param The interface ID to check against.
            /// @return Whether this implements the passed in
            /// interface id.
            //--------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
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
            //------------------------------------------------
            /// Request the remote token for this device for use
            /// with Push Notifications.
            ///
            ///  @author Robert Henning
            ///
            /// @param The return delegate.
            //------------------------------------------------
            void RequestRemoteToken(const TokenReceivedDelegate& in_delegate) override;
            //------------------------------------------------
            /// @author Robert Henning
            ///
            /// @return The unique token that the push notification
            /// provider uses to identify the device
            //------------------------------------------------
            const std::string& GetRemoteToken() const override;
            //------------------------------------------------
            /// @author Robert Henning
            ///
            /// @return The ID for the remote notification
            /// service
            //------------------------------------------------
            const std::string& GetProviderID() const override;
            //--------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being recieved.
            //---------------------------------------------------
            CSCore::IConnectableEvent<NotificationReceivedDelegate>& GetReceivedEvent() override;
            //------------------------------------------------
            /// @author Robert Henning
            ///
            /// @param The unique token that the push notification
            /// provider uses to identify the device
            //------------------------------------------------
            void OnRemoteTokenReceived(const std::string& in_token);
            //------------------------------------------------
            /// Called when the app delegate receives a push
            /// notification
            ///
            /// @author Robert Henning
            ///
            /// @param Dictionary payload of remote notification
            //------------------------------------------------
            void OnRemoteNotificationReceived(const CSCore::ParamDictionary& in_params);
        private:
            friend CSCore::RemoteNotificationSystemUPtr CSCore::RemoteNotificationSystem::Create();
            //------------------------------------------------
            /// Private constructor to force use of factory method.
            ///
            /// @author Ian Copland
            //-----------------------------------------------
            GooglePlayRemoteNotificationSystem();

            bool m_enabled;
            GooglePlayRemoteNotificationJavaInterfaceSPtr m_javaInterface;
            std::string	m_token;
            TokenReceivedDelegate m_delegate;
            CSCore::Event<NotificationReceivedDelegate> m_receivedEvent;
        };
    }
}

#endif

#endif

#endif
