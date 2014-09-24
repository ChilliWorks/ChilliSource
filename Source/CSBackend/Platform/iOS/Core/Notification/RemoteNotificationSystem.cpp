//
//  RemoteNotificationSystem.cpp
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

#ifdef CS_TARGETPLATFORM_IOS

#include <CSBackend/Platform/iOS/Core/Notification/RemoteNotificationSystem.h>

#include <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Notification/Notification.h>

#include <UIKit/UIKit.h>

namespace CSBackend
{
    namespace iOS
    {
        namespace
        {
            const std::string k_providerID = "Apple";
        }
        
        CS_DEFINE_NAMEDTYPE(RemoteNotificationSystem);
        //--------------------------------------------------
        //--------------------------------------------------
        RemoteNotificationSystem::RemoteNotificationSystem()
            : m_enabled(true)
        {
        }
        //--------------------------------------------------
        //--------------------------------------------------
        bool RemoteNotificationSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (RemoteNotificationSystem::InterfaceID == in_interfaceId || CSCore::RemoteNotificationSystem::InterfaceID == in_interfaceId);
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void RemoteNotificationSystem::OnInit()
        {
#ifdef __IPHONE_8_0
            if ([[UIApplication sharedApplication] respondsToSelector:@selector(registerUserNotificationSettings:)])
            {
                //From iOS 8 we need to request permissions to display notifications, to badge the app icon and to play a sound
                UIUserNotificationSettings* notificationSettings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound categories:nil];
                [[UIApplication sharedApplication] registerUserNotificationSettings:notificationSettings];
            }
#endif
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void RemoteNotificationSystem::SetEnabled(bool in_enabled)
        {
            m_enabled = in_enabled;
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void RemoteNotificationSystem::RequestRemoteToken(const TokenReceivedDelegate& in_delegate)
        {
            m_delegate = in_delegate;

#ifdef __IPHONE_8_0
            if ([[UIApplication sharedApplication] respondsToSelector:@selector(registerForRemoteNotifications)])
            {
                [[UIApplication sharedApplication] registerForRemoteNotifications];
            }
            else
#endif
            {
                UIRemoteNotificationType Types = UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert;
                [[UIApplication sharedApplication] registerForRemoteNotificationTypes:Types];
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        const std::string& RemoteNotificationSystem::GetRemoteToken() const
        {
            return m_token;
        }
        //--------------------------------------------------
        //--------------------------------------------------
        const std::string& RemoteNotificationSystem::GetProviderID() const
        {
            return k_providerID;
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void RemoteNotificationSystem::OnRemoteTokenReceived(NSData* in_token)
        {
            m_token = CSCore::BaseEncoding::Base64Encode((const s8*)[in_token bytes], in_token.length);
            
            if(m_delegate != nullptr)
            {
                m_delegate(m_token);
                m_delegate = nullptr;
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void RemoteNotificationSystem::OnRemoteNotificationReceived(UIApplication* in_application, NSDictionary* in_payload)
        {
            @autoreleasepool
            {
                if (m_enabled == true)
                {
                    in_application.applicationIconBadgeNumber = 0;
                    
                    CSCore::NotificationSPtr notification = std::make_shared<CSCore::Notification>();
                    notification->m_id = 0;
                    notification->m_priority = CSCore::Notification::Priority::k_standard;
                    
                    // Add the message
                    NSObject* apsObject = [in_payload objectForKey:@"aps"];
                    if(apsObject != nil && [apsObject isKindOfClass:[NSDictionary class]])
                    {
                        NSDictionary* apsDictionary = (NSDictionary*)apsObject;
                        NSObject* alertObject = [apsDictionary objectForKey:@"alert"];
                        if(alertObject != nil && [alertObject isKindOfClass:[NSDictionary class]])
                        {
                            NSDictionary* alertDictionary = (NSDictionary*)alertObject;
                            NSObject* bodyObject = [alertDictionary objectForKey:@"body"];
                            if(bodyObject != nil && [bodyObject isKindOfClass:[NSString class]])
                            {
                                NSString* body = (NSString*) bodyObject;
                                notification->m_params.SetValue("message", [NSStringUtils newUTF8StringWithNSString:body]);
                            }
                        }
                    }
                    
                    // Get the extra data
                    for(NSString* key in in_payload)
                    {
                        if([key isKindOfClass:[NSString class]])
                        {
                            NSObject* payloadObject = [in_payload objectForKey:key];
                            if([payloadObject isKindOfClass:[NSString class]])
                            {
                                NSString* payloadString = (NSString*)payloadObject;
                                notification->m_params.SetValue([NSStringUtils newUTF8StringWithNSString:key], [NSStringUtils newUTF8StringWithNSString:payloadString]);
                            }
                        }
                    }
                    
                    
                    m_receivedEvent.NotifyConnections(notification);
                }
            }
        }
        //--------------------------------------------------
        //---------------------------------------------------
        CSCore::IConnectableEvent<CSCore::RemoteNotificationSystem::NotificationReceivedDelegate>& RemoteNotificationSystem::GetReceivedEvent()
        {
            return m_receivedEvent;
        }
    }
}

#endif