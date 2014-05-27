//
//  RemoteNotificationSystem.cpp
//  Chill Source
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/Notification/RemoteNotificationSystem.h>

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Notification/Notification.h>

#include <UIKit/UIKit.h>

namespace ChilliSource
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
        bool RemoteNotificationSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (RemoteNotificationSystem::InterfaceID == in_interfaceId || Core::RemoteNotificationSystem::InterfaceID == in_interfaceId);
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
            UIRemoteNotificationType Types = UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert;
            [[UIApplication sharedApplication] registerForRemoteNotificationTypes:Types];
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
            m_token = Core::BaseEncoding::Base64Encode((const s8*)[in_token bytes], in_token.length);
            
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
                    
                    Core::NotificationSPtr notification = std::make_shared<Core::Notification>();
                    notification->m_id = 0;
                    notification->m_priority = Core::Notification::Priority::k_standard;
                    
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
        Core::IConnectableEvent<Core::RemoteNotificationSystem::NotificationReceivedDelegate>& RemoteNotificationSystem::GetRecievedEvent()
        {
            return m_receivedEvent;
        }
    }
}
