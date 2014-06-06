//
//  LocalNotificationSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 01/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#include <ChilliSource/Backend/Platform/iOS/Core/Notification/LocalNotificationSystem.h>

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>

#import <UIKit/UIKit.h>

namespace ChilliSource
{
    namespace iOS
    {
        namespace
        {
            LocalNotificationSystem* g_localNotificationSystem;
            std::vector<Core::NotificationSPtr> g_queuedNotifications;
            
            //---------------------------------------------------------------
            /// Convert UILocalNotification to Notification
            ///
            /// @author Scott Downie
            ///
            /// @param Apple UILocalNotification
            /// @param [Out] Chilli Source notification
            //---------------------------------------------------------------
            Core::NotificationUPtr ConvertUILocalNotificationToNotification(UILocalNotification* in_uiLocal)
            {
                Core::NotificationUPtr notification = Core::NotificationUPtr(new Core::Notification());
                notification->m_id = (Core::Notification::ID)[[in_uiLocal.userInfo objectForKey:@"ID"] unsignedIntValue];
                notification->m_priority = (Core::Notification::Priority)[[in_uiLocal.userInfo objectForKey:@"Priority"] unsignedIntValue];
                
                NSDictionary* nsParams = (NSDictionary*)[in_uiLocal.userInfo objectForKey:@"Params"];
                for(id key in nsParams)
                {
                    notification->m_params.SetValue([NSStringUtils newUTF8StringWithNSString:[nsParams objectForKey:key]], [NSStringUtils newUTF8StringWithNSString:key]);
                }
                
                return notification;
            }
        }
        CS_DEFINE_NAMEDTYPE(LocalNotificationSystem);
        //----------------------------------------------------------
        //----------------------------------------------------------
        LocalNotificationSystem::LocalNotificationSystem()
        : m_enabled(true)
        {
        }
        //--------------------------------------------------------
        //-------------------------------------------------------
        bool LocalNotificationSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Core::LocalNotificationSystem::InterfaceID == in_interfaceId || LocalNotificationSystem::InterfaceID == in_interfaceId);
        }
        //--------------------------------------------------
        //---------------------------------------------------
        void LocalNotificationSystem::SetEnabled(bool in_enabled)
        {
            m_enabled = in_enabled;
            
            if (m_enabled == false)
            {
                CancelAll();
            }
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void LocalNotificationSystem::ScheduleNotificationForTime(Core::Notification::ID in_id, const Core::ParamDictionary& in_params, TimeIntervalSecs in_time, Core::Notification::Priority in_priority)
        {
            @autoreleasepool
            {
                if (m_enabled == true)
                {
                    //Create the notifications
                    UILocalNotification* nsNotification = [[[UILocalNotification alloc] init] autorelease];
                    nsNotification.fireDate = [NSDate dateWithTimeIntervalSince1970:(NSTimeInterval)in_time];
                    nsNotification.timeZone = [NSTimeZone defaultTimeZone];
                    nsNotification.alertAction = @"View";
                    
                    NSString* body = [NSStringUtils newNSStringWithUTF8String:in_params.GetValue("Body")];
                    nsNotification.alertBody = body;
                    [body release];
                    
                    if(in_params.HasKey("Sound") == true)
                    {
                        NSString* sound = [NSStringUtils newNSStringWithUTF8String:in_params.GetValue("Sound")];
                        nsNotification.soundName = sound;
                        [sound release];
                    }
                    else
                    {
                        nsNotification.soundName = UILocalNotificationDefaultSoundName;
                    }
                    
                    nsNotification.applicationIconBadgeNumber = 1;
                    
                    NSMutableDictionary* nsParams = [[NSMutableDictionary alloc] init];
                    for(Core::ParamDictionary::const_iterator it = in_params.begin(); it != in_params.end(); ++it)
                    {
                        NSString* key = [NSStringUtils newNSStringWithUTF8String:it->first];
                        NSString* value = [NSStringUtils newNSStringWithUTF8String:it->second];
                        [nsParams setObject:key forKey:value];
                        [key release];
                        [value release];
                    }
                    
                    //Encode the type ID into the notification so we can retrieve it at the other end
                    NSDictionary* pInfo = [NSDictionary dictionaryWithObjectsAndKeys:
                                           [NSNumber numberWithUnsignedInt:(u32)in_id], @"ID",
                                           [NSNumber numberWithUnsignedInt:(u32)in_priority], @"Priority",
                                           nsParams, @"Params",
                                           nil];
                    
                    nsNotification.userInfo = pInfo;
                    
                    // pInfo dictionary retains the pParams dictionary on insertion so we need to release
                    [nsParams release];
                    
                    //Schedule this baby
                    [[UIApplication sharedApplication] scheduleLocalNotification:nsNotification];
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void LocalNotificationSystem::GetScheduledNotifications(std::vector<Core::NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time, TimeIntervalSecs in_period) const
        {
            for(UILocalNotification* nsNotification in [[UIApplication sharedApplication] scheduledLocalNotifications])
			{
                TimeIntervalSecs triggerTime = (TimeIntervalSecs)[nsNotification.fireDate timeIntervalSince1970];
                s32 dwDeltaSecs = (s32)(in_time - triggerTime);
                
				if(std::abs(dwDeltaSecs) <= in_period)
				{
                    Core::NotificationSPtr notification = ConvertUILocalNotificationToNotification(nsNotification);
					out_notifications.push_back(notification);
				}
			}
        }
        //------------------------------------------------
        //------------------------------------------------
        void LocalNotificationSystem::CancelByID(Core::Notification::ID in_id)
        {
			for(UILocalNotification* nsNotification in [[UIApplication sharedApplication] scheduledLocalNotifications])
			{
				Core::Notification::ID notificationId = [[nsNotification.userInfo objectForKey:@"ID"] unsignedIntValue];
				
				if(notificationId == in_id)
				{
					[[UIApplication sharedApplication] cancelLocalNotification:nsNotification];
				}
			}
        }
        //------------------------------------------------
        //------------------------------------------------
        void LocalNotificationSystem::CancelAll()
        {
            [[UIApplication sharedApplication] cancelAllLocalNotifications];
        }
        //--------------------------------------------------
        //---------------------------------------------------
        Core::IConnectableEvent<Core::LocalNotificationSystem::RecievedDelegate>& LocalNotificationSystem::GetRecievedEvent()
        {
            return m_receivedEvent;
        }
        //------------------------------------------------
        //------------------------------------------------
        bool LocalNotificationSystem::ApplicationDidFinishLaunchingWithOptions(UIApplication* inpApplication, NSDictionary* inpOptions)
        {
            //Reset the badge number
            inpApplication.applicationIconBadgeNumber = 0;
            
            UILocalNotification* pLocalNotification = [inpOptions objectForKey:UIApplicationLaunchOptionsLocalNotificationKey];
            if(pLocalNotification) 
            {
                Core::NotificationSPtr notification = ConvertUILocalNotificationToNotification(pLocalNotification);
                if (g_localNotificationSystem != nullptr)
                {
                    g_localNotificationSystem->OnNotificationRecieved(notification);
                }
                else
                {
                    g_queuedNotifications.push_back(notification);
                }
                return true;
            }
            
            return false;
        }
        //------------------------------------------------
        //------------------------------------------------
		void LocalNotificationSystem::ApplicationDidBecomeActive(UIApplication* inpApplication)
		{
			//Reset the badge number
            inpApplication.applicationIconBadgeNumber = 0;
		}
        //------------------------------------------------
        //------------------------------------------------
        void LocalNotificationSystem::ApplicationDidReceiveLocalNotification(UIApplication* inpApplication, UILocalNotification* inpNotification)
        {
            //Reset the badge number
            inpApplication.applicationIconBadgeNumber = (inpApplication.applicationIconBadgeNumber - 1);
            
            Core::NotificationSPtr notification = ConvertUILocalNotificationToNotification(inpNotification);
            if (g_localNotificationSystem != nullptr)
            {
                g_localNotificationSystem->OnNotificationRecieved(notification);
            }
            else
            {
                g_queuedNotifications.push_back(notification);
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void LocalNotificationSystem::OnInit()
        {
            Core::LocalNotificationSystem::OnInit();
            
            CS_ASSERT(g_localNotificationSystem == nullptr, "Cannot create more than 1 Local Notification System!");
            g_localNotificationSystem = this;
            
            for (const Core::NotificationSPtr& notification : g_queuedNotifications)
            {
                OnNotificationRecieved(notification);
            }
            
            g_queuedNotifications.clear();
            g_queuedNotifications.shrink_to_fit();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void LocalNotificationSystem::OnNotificationRecieved(const Core::NotificationSPtr& in_notification)
        {
            m_receivedEvent.NotifyConnections(in_notification);
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void LocalNotificationSystem::OnDestroy()
        {
            g_localNotificationSystem = nullptr;
            Core::LocalNotificationSystem::OnDestroy();
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        LocalNotificationSystem::~LocalNotificationSystem()
        {
            g_localNotificationSystem = nullptr;
        }
    }
}

#endif