//
//  RemoteNotificationSystem.cpp
//  MoFlow
//
//  Created by Robert Henning on 22/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/Notification/RemoteNotificationSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>

#include <UIKit/UIKit.h>


namespace ChilliSource
{
    namespace iOS
    {
        namespace
        {
            const std::string kstrProviderID = "Apple";
        }
        //-------------------------------------------------------------------------
        /// Request Remote Token
        //-------------------------------------------------------------------------
        void CRemoteNotificationSystem::RequestRemoteToken(const Core::RemoteTokenReceivedDelegate& inDelegate)
        {
            mDelegate = inDelegate;
            UIRemoteNotificationType Types = UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert;
            [[UIApplication sharedApplication] registerForRemoteNotificationTypes:Types];
        }
        //-------------------------------------------------------------------------
        /// Get Remote Token
        //-------------------------------------------------------------------------
        const std::string& CRemoteNotificationSystem::GetRemoteToken() const
        {
            return mstrToken;
        }
        //-------------------------------------------------------------------------
        /// Get Provider ID
        //-------------------------------------------------------------------------
        const std::string& CRemoteNotificationSystem::GetProviderID() const
        {
            return kstrProviderID;
        }
        //-------------------------------------------------------------------------
        /// On Remote Token Received
        //-------------------------------------------------------------------------
        void CRemoteNotificationSystem::OnRemoteTokenReceived(NSData* inpToken)
        {
            mstrToken = Core::BaseEncoding::Base64Encode((const s8*)[inpToken bytes], inpToken.length);
            
            if(mDelegate != nullptr)
            {
                mDelegate(mstrToken);
                mDelegate = nullptr;
            }
        }
        //-------------------------------------------------------------------------
        /// On Remote Notification Received
        //-------------------------------------------------------------------------
        void CRemoteNotificationSystem::OnRemoteNotificationReceived(UIApplication* inpApplication, NSDictionary* inpPayload)
        {
            inpApplication.applicationIconBadgeNumber = 0;
            
            NSLog(@"Recieved Remote Notification %@", inpPayload);
            
            Core::Notification sNotification;
            sNotification.bDismissed = false;
            sNotification.eType = Core::NotificationType::k_push;
            sNotification.TriggerTime = Core::Application::GetSystemTime();
            sNotification.ID = 0;
            sNotification.ePriority = Core::NotificationPriority::k_standard;
            
            // Add the message
            NSObject* pApsObject = [inpPayload objectForKey:@"aps"];
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
                        sNotification.sParams.SetValueForKey("message", Core::StringUtils::NSStringToString(pstrBody));
                    }
                }
            }
            
            // Get the extra data
            for(NSString* key in inpPayload)
            {
                if([key isKindOfClass:[NSString class]])
                {
                    NSObject* pPayloadObject = [inpPayload objectForKey:key];
                    if([pPayloadObject isKindOfClass:[NSString class]])
                    {
                        NSString* pstrPayloadString = (NSString*)pPayloadObject;
                        sNotification.sParams.SetValueForKey(Core::StringUtils::NSStringToString(key),
                                                             Core::StringUtils::NSStringToString(pstrPayloadString));
                    }
                }
            }
            
            Core::NotificationScheduler::OnNotificationReceived(sNotification);
        }
    }
}
