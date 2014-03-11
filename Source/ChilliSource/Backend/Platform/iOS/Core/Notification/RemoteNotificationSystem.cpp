//
//  RemoteNotificationSystem.cpp
//  Chill Source
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/Notification/RemoteNotificationSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Notifications/Notification.h>

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
        bool RemoteNotificationSystem::IsA(Core::InterfaceIDType in_interfaceID) const
        {
            return (RemoteNotificationSystem::InterfaceID == in_interfaceID || Core::RemoteNotificationSystem::InterfaceID == in_interfaceID);
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
                    
                    Core::ParamDictionary params;
                    
                    // Add the message
                    NSObject* pApsObject = [in_payload objectForKey:@"aps"];
                    if(pApsObject != nil && [pApsObject isKindOfClass:[NSDictionary class]])
                    {
                        NSDictionary* pApsDictionary = (NSDictionary*)pApsObject;
                        NSObject* pAlertObject = [pApsDictionary objectForKey:@"alert"];
                        if(pAlertObject != nil && [pAlertObject isKindOfClass:[NSDictionary class]])
                        {
                            NSDictionary* pAlertDictionary = (NSDictionary*)pAlertObject;
                            NSObject* pBodyObject = [pAlertDictionary objectForKey:@"body"];
                            if(pBodyObject != nil && [pBodyObject isKindOfClass:[NSString class]])
                            {
                                NSString* pstrBody = (NSString*) pBodyObject;
                                params.SetValueForKey("message", Core::StringUtils::NSStringToString(pstrBody));
                            }
                        }
                    }
                    
                    // Get the extra data
                    for(NSString* key in in_payload)
                    {
                        if([key isKindOfClass:[NSString class]])
                        {
                            NSObject* pPayloadObject = [in_payload objectForKey:key];
                            if([pPayloadObject isKindOfClass:[NSString class]])
                            {
                                NSString* pstrPayloadString = (NSString*)pPayloadObject;
                                params.SetValueForKey(Core::StringUtils::NSStringToString(key),
                                                                     Core::StringUtils::NSStringToString(pstrPayloadString));
                            }
                        }
                    }
                    
                    Core::NotificationSPtr notification(std::make_shared<Core::Notification>(0, params, Core::Notification::Priority::k_standard));
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
