//
//  CNotificationFactory.cpp
//  moFloTest
//
//  Created by Scott Downie on 01/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/Notification/LocalNotificationScheduler.h>

#import <UIKit/UIKit.h>

namespace ChilliSource
{
    namespace iOS
    {
        //------------------------------------------------------------------------------
        /// Schedule Notification 
        ///
        /// Once the time is reached the notification will be inserted into the queue.
        /// Upon reaching the head of the queue it will trigger.
        ///
        /// @param Notification 
        //------------------------------------------------------------------------------
        void CLocalNotificationScheduler::ScheduleNotification(const Core::NotificationSPtr& insNotification)
        {
            NSDate* pDate = [NSDate dateWithTimeIntervalSince1970:(NSTimeInterval)insNotification->TriggerTime];
            
            //Create the notifications
			UILocalNotification* pNotification = [[[UILocalNotification alloc] init] autorelease];
			pNotification.fireDate = pDate;
			pNotification.timeZone = [NSTimeZone defaultTimeZone];
			
			pNotification.alertAction = @"View";
			pNotification.alertBody = Core::StringUtils::StringToNSString(insNotification->sParams.ValueForKey("Body"));
			
            if( insNotification->sParams.HasValue("Sound") )
            {
                pNotification.soundName = Core::StringUtils::StringToNSString(insNotification->sParams.ValueForKey("Sound"));
            }
            else {
                pNotification.soundName = UILocalNotificationDefaultSoundName;
            }
			pNotification.applicationIconBadgeNumber = 1;
            
            NSMutableDictionary* pParams = [[NSMutableDictionary alloc] init];
            for(Core::StringToStringMap::const_iterator it = insNotification->sParams.begin(); it != insNotification->sParams.end(); ++it)
            {
                [pParams setObject:Core::StringUtils::StringToNSString(it->first) forKey:Core::StringUtils::StringToNSString(it->second)];
            }
			
			//Encode the type ID into the notification so we can retrieve it at the other end
			NSDictionary* pInfo = [NSDictionary dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithUnsignedInt:(u32)insNotification->ID], @"ID",
                                   [NSNumber numberWithUnsignedInt:(u32)insNotification->ePriority], @"Priority",
                                   pParams, @"Params",
                                   nil];
            
			pNotification.userInfo = pInfo;
			
            // pInfo dictionary retains the pParams dictionary on insertion so we need to release
            [pParams release];
            
			//Schedule this baby
			[[UIApplication sharedApplication] scheduleLocalNotification:pNotification];
        }
        //-------------------------------------------------------------------------
        /// Cancel By ID
        ///
        /// Prevent any notifications with given ID type from firing
        ///
        /// @param ID type
        //-------------------------------------------------------------------------
        void CLocalNotificationScheduler::CancelByID(Core::NotificationID inID)
        {
			for(UILocalNotification* pNotification in [[UIApplication sharedApplication] scheduledLocalNotifications]) 
			{
				Core::NotificationID ID = [[pNotification.userInfo objectForKey:@"ID"] unsignedIntValue];
				
				if(ID == inID) 
				{
					[[UIApplication sharedApplication] cancelLocalNotification:pNotification];
				}
			}
        }
        //-------------------------------------------------------------------------
        /// Try Get Notifications Scheduled Within Time Period
        ///
        /// Checks if any notifications have been scheduled to trigger
        /// within the given window of the given time
        ///
        /// @param Time
        /// @param Timeframe
        /// @param Out: Notifications that meet criteria
        /// @return Whether any notifications exist within that time period
        //-------------------------------------------------------------------------
        bool CLocalNotificationScheduler::TryGetNotificationsScheduledWithinTimePeriod(TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, std::vector<Core::NotificationSPtr>& outaNotifications)
        {
            outaNotifications.clear();
            
            for(UILocalNotification* pNotification in [[UIApplication sharedApplication] scheduledLocalNotifications]) 
			{
                TimeIntervalSecs TriggerTime = (TimeIntervalSecs)[pNotification.fireDate timeIntervalSince1970];
                s32 dwDeltaSecs = inTime - TriggerTime;
                
				if(std::abs(dwDeltaSecs) <= inPeriod) 
				{
                    Core::NotificationSPtr notification(std::make_shared<Core::Notification>());
                    ConvertUILocalNotificationToNotification(pNotification, notification);
					outaNotifications.push_back(notification);
				}
			}
            
            return !outaNotifications.empty();
        }
        //-------------------------------------------------------------------------
        /// Cancel All 
        ///
        /// Terminate all currently scheduled notifications
        //-------------------------------------------------------------------------
        void CLocalNotificationScheduler::CancelAll()
        {
            [[UIApplication sharedApplication] cancelAllLocalNotifications];
        }
        //-------------------------------------------------------------------------
        /// Application Did Finish Launching With Options
        ///
        /// Called by the app delegate when the app launches. Used to pull
        /// local notification data from the options
        ///
        /// @param UIApplication
        /// @param Launching options dictionary
        /// @return Whether the app was launched with local notifications
        //-------------------------------------------------------------------------
        bool CLocalNotificationScheduler::ApplicationDidFinishLaunchingWithOptions(UIApplication* inpApplication, NSDictionary* inpOptions)
        {
            //Reset the badge number
            inpApplication.applicationIconBadgeNumber = 0;
            
            UILocalNotification* pLocalNotification = [inpOptions objectForKey:UIApplicationLaunchOptionsLocalNotificationKey];
            if(pLocalNotification) 
            {
                Core::NotificationSPtr notification(std::make_shared<Core::Notification>());
                ConvertUILocalNotificationToNotification(pLocalNotification, notification);
                Core::NotificationScheduler::OnNotificationReceived(notification);
                return true;
            }
            
            return false;
        }
		//-------------------------------------------------------------------------
		/// Application Did Become Active
		///
		/// Called by the app delegate when the app becomes active. Used to clear
		/// the icon badge number
		///
		/// @param UIApplication
		//-------------------------------------------------------------------------
		void CLocalNotificationScheduler::ApplicationDidBecomeActive(UIApplication* inpApplication)
		{
			//Reset the badge number
            inpApplication.applicationIconBadgeNumber = 0;
		}
        //-------------------------------------------------------------------------
        /// Application Did Receive Local Notification
        ///
        /// Called by the app delegate when it receives a local notification
        ///
        /// @param UILocalNotification
        //-------------------------------------------------------------------------
        void CLocalNotificationScheduler::ApplicationDidReceiveLocalNotification(UIApplication* inpApplication, UILocalNotification* inpNotification)
        {
            //Reset the badge number
            inpApplication.applicationIconBadgeNumber = (inpApplication.applicationIconBadgeNumber - 1);
            
            Core::NotificationSPtr notification(std::make_shared<Core::Notification>());
            ConvertUILocalNotificationToNotification(inpNotification, notification);
            Core::NotificationScheduler::OnNotificationReceived(notification);
        }
        //-------------------------------------------------------------------------
        /// Convert UILocalNotification to Notification
        ///
        /// @param Apple UILocalNotification
        /// @param Out: moFlow notification
        //-------------------------------------------------------------------------
        void CLocalNotificationScheduler::ConvertUILocalNotificationToNotification(UILocalNotification* inpUILocal, Core::NotificationSPtr& out_notification)
        {
            out_notification->bDismissed = false;
            out_notification->eType = Core::NotificationType::k_system;
            out_notification->TriggerTime = (TimeIntervalSecs)[inpUILocal.fireDate timeIntervalSince1970];
            
            out_notification->ID = (Core::NotificationID)[[inpUILocal.userInfo objectForKey:@"ID"] unsignedIntValue];
            out_notification->ePriority = (Core::NotificationPriority)[[inpUILocal.userInfo objectForKey:@"Priority"] unsignedIntValue];
            
            NSDictionary* pParams = (NSDictionary*)[inpUILocal.userInfo objectForKey:@"Params"];
            
            for(id key in pParams)
            {
                out_notification->sParams.SetValueForKey(Core::StringUtils::NSStringToString([pParams objectForKey:key]), Core::StringUtils::NSStringToString(key));
            }                                       
        }
    }
}
