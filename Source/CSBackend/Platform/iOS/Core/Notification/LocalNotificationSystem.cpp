//
//  LocalNotificationSystem.cpp
//  ChilliSource
//  Created by Scott Downie on 01/07/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <CSBackend/Platform/iOS/Core/Notification/LocalNotificationSystem.h>
#include <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#import <UIKit/UIKit.h>

namespace CSBackend
{
    namespace iOS
    {
        namespace
        {
            LocalNotificationSystem* g_localNotificationSystem;
            std::vector<ChilliSource::NotificationSPtr> g_queuedNotifications;
            
            //---------------------------------------------------------------
            /// Convert UILocalNotification to Notification
            ///
            /// @author Scott Downie
            ///
            /// @param Apple UILocalNotification
            /// @param [Out] ChilliSource notification
            //---------------------------------------------------------------
            ChilliSource::NotificationUPtr ConvertUILocalNotificationToNotification(UILocalNotification* in_uiLocal)
            {
                ChilliSource::NotificationUPtr notification = ChilliSource::NotificationUPtr(new ChilliSource::Notification());
                notification->m_id = (ChilliSource::Notification::ID)[[in_uiLocal.userInfo objectForKey:@"ID"] unsignedIntValue];
                notification->m_priority = (ChilliSource::Notification::Priority)[[in_uiLocal.userInfo objectForKey:@"Priority"] unsignedIntValue];
                
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
            m_recentlyAddedNotifications = [[NSMutableArray alloc] init];
        }
        //--------------------------------------------------------
        //-------------------------------------------------------
        bool LocalNotificationSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
            return (ChilliSource::LocalNotificationSystem::InterfaceID == in_interfaceId || LocalNotificationSystem::InterfaceID == in_interfaceId);
        }
        //--------------------------------------------------
        //---------------------------------------------------
        void LocalNotificationSystem::SetEnabled(bool in_enabled)
        {
            CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to enable/disable notifications outside of main thread.");
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                m_enabled = in_enabled;
                
                if (m_enabled == false)
                {
                    CancelAll();
                }
            });
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void LocalNotificationSystem::ScheduleNotificationForTime(ChilliSource::Notification::ID in_id, const ChilliSource::ParamDictionary& in_params, TimeIntervalSecs in_time, ChilliSource::Notification::Priority in_priority)
        {
            @autoreleasepool
            {
                CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to schedule notification outside of main thread.");
                
                
                ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
                {
                    //Clean-up any duplicates from recently added
                    for(UILocalNotification* nsNotification in [[UIApplication sharedApplication] scheduledLocalNotifications])
                    {
                        if([m_recentlyAddedNotifications containsObject:nsNotification] == YES)
                        {
                            [m_recentlyAddedNotifications removeObject:nsNotification];
                        }
                    }
                    
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
                        for(ChilliSource::ParamDictionary::const_iterator it = in_params.begin(); it != in_params.end(); ++it)
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
                        
                        //Unfortunately it seems that scheduling a notification doesn't immediately add it to the notifications list
                        //and subsequent attempts to cancel or retrieve it may fail. In order to workaround this we need to maintain our
                        //own list of recently added notifications.
                        [m_recentlyAddedNotifications addObject:nsNotification];
                    }
                });
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void LocalNotificationSystem::GetScheduledNotifications(const GetScheduledNotificationsDelegate& in_delegate, TimeIntervalSecs in_time, TimeIntervalSecs in_period) const
        {
            CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to retrieve scheduled notifications outside of main thread.");
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                std::vector<ChilliSource::NotificationCSPtr> notificationList;
                
                for(UILocalNotification* nsNotification in [[UIApplication sharedApplication] scheduledLocalNotifications])
                {
                    TimeIntervalSecs triggerTime = (TimeIntervalSecs)[nsNotification.fireDate timeIntervalSince1970];
                    s32 dwDeltaSecs = (s32)(in_time - triggerTime);
                    
                    if(std::abs(dwDeltaSecs) <= in_period)
                    {
                        ChilliSource::NotificationSPtr notification = ConvertUILocalNotificationToNotification(nsNotification);
                        notificationList.push_back(notification);
                    }
                    
                    if([m_recentlyAddedNotifications containsObject:nsNotification] == YES)
                    {
                        [m_recentlyAddedNotifications removeObject:nsNotification];
                    }
                }
                
                for(UILocalNotification* nsNotification in m_recentlyAddedNotifications)
                {
                    TimeIntervalSecs triggerTime = (TimeIntervalSecs)[nsNotification.fireDate timeIntervalSince1970];
                    s32 dwDeltaSecs = (s32)(in_time - triggerTime);
                    
                    if(std::abs(dwDeltaSecs) <= in_period)
                    {
                        ChilliSource::NotificationSPtr notification = ConvertUILocalNotificationToNotification(nsNotification);
                        notificationList.push_back(notification);
                    }
                }
                
                ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
                {
                    if(in_delegate)
                    {
                        in_delegate(notificationList);
                    }
                });
            });
        }
        //------------------------------------------------
        //------------------------------------------------
        void LocalNotificationSystem::CancelByID(ChilliSource::Notification::ID in_id)
        {
            CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to cancel notification outside of main thread.");
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                for(UILocalNotification* nsNotification in [[UIApplication sharedApplication] scheduledLocalNotifications])
                {
                    ChilliSource::Notification::ID notificationId = [[nsNotification.userInfo objectForKey:@"ID"] unsignedIntValue];
                    
                    if(notificationId == in_id)
                    {
                        [[UIApplication sharedApplication] cancelLocalNotification:nsNotification];
                    }
                    
                    if([m_recentlyAddedNotifications containsObject:nsNotification] == YES)
                    {
                        [m_recentlyAddedNotifications removeObject:nsNotification];
                    }
                }
                
                for(UILocalNotification* nsNotification in m_recentlyAddedNotifications)
                {
                    ChilliSource::Notification::ID notificationId = [[nsNotification.userInfo objectForKey:@"ID"] unsignedIntValue];
                    
                    if(notificationId == in_id)
                    {
                        [[UIApplication sharedApplication] cancelLocalNotification:nsNotification];
                    }
                }
            });
        }
        //------------------------------------------------
        //------------------------------------------------
        void LocalNotificationSystem::CancelAll()
        {
            CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to cancel all notifications outside of main thread.");
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                [m_recentlyAddedNotifications removeAllObjects];
                [[UIApplication sharedApplication] cancelAllLocalNotifications];
            });
        }
        //--------------------------------------------------
        //---------------------------------------------------
        ChilliSource::IConnectableEvent<ChilliSource::LocalNotificationSystem::ReceivedDelegate>& LocalNotificationSystem::GetReceivedEvent()
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
                ChilliSource::NotificationSPtr notification = ConvertUILocalNotificationToNotification(pLocalNotification);
                if (g_localNotificationSystem != nullptr)
                {
                    g_localNotificationSystem->OnNotificationReceived(notification);
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
            
            ChilliSource::NotificationSPtr notification = ConvertUILocalNotificationToNotification(inpNotification);
            if (g_localNotificationSystem != nullptr)
            {
                g_localNotificationSystem->OnNotificationReceived(notification);
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
#ifdef __IPHONE_8_0
            if ([[UIApplication sharedApplication] respondsToSelector:@selector(registerUserNotificationSettings:)])
            {
                //From iOS 8 we need to request permissions to display notifications, to badge the app icon and to play a sound
                UIUserNotificationSettings* notificationSettings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound categories:nil];
                [[UIApplication sharedApplication] registerUserNotificationSettings:notificationSettings];
            }
#endif
            
            ChilliSource::LocalNotificationSystem::OnInit();
            
            CS_ASSERT(g_localNotificationSystem == nullptr, "Cannot create more than 1 Local Notification System!");
            g_localNotificationSystem = this;
            
            for (const ChilliSource::NotificationSPtr& notification : g_queuedNotifications)
            {
                OnNotificationReceived(notification);
            }
            
            g_queuedNotifications.clear();
            g_queuedNotifications.shrink_to_fit();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void LocalNotificationSystem::OnNotificationReceived(const ChilliSource::NotificationSPtr& in_notification)
        {
            m_receivedEvent.NotifyConnections(in_notification);
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void LocalNotificationSystem::OnDestroy()
        {
            g_localNotificationSystem = nullptr;
            ChilliSource::LocalNotificationSystem::OnDestroy();
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        LocalNotificationSystem::~LocalNotificationSystem()
        {
            [m_recentlyAddedNotifications release];
            g_localNotificationSystem = nullptr;
        }
    }
}

#endif
