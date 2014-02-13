//
//  CNotificationFactory.cpp
//  moFloTest
//
//  Created by Scott Downie on 01/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Platform/iOS/LocalNotificationScheduler.h>

#import <UIKit/UIKit.h>

namespace moFlo
{
    namespace iOSPlatform
    {
        //------------------------------------------------------------------------------
        /// Schedule Notification 
        ///
        /// Once the time is reached the notification will be inserted into the queue.
        /// Upon reaching the head of the queue it will trigger.
        ///
        /// @param Notification 
        //------------------------------------------------------------------------------
        void CLocalNotificationScheduler::ScheduleNotification(const Notification& insNotification)
        {
            NSDate* pDate = [NSDate dateWithTimeIntervalSince1970:(NSTimeInterval)insNotification.TriggerTime];
            
            //Create the notifications
			UILocalNotification* pNotification = [[[UILocalNotification alloc] init] autorelease];
			pNotification.fireDate = pDate;
			pNotification.timeZone = [NSTimeZone defaultTimeZone];
			
			pNotification.alertAction = @"View";
			pNotification.alertBody = Core::CStringUtils::StringToNSString(insNotification.sParams.ValueForKey("Body"));
			
            if( insNotification.sParams.HasValue("Sound") )
            {
                pNotification.soundName = Core::CStringUtils::StringToNSString(insNotification.sParams.ValueForKey("Sound"));
            }
            else {
                pNotification.soundName = UILocalNotificationDefaultSoundName;
            }
			pNotification.applicationIconBadgeNumber = 1;
            
            NSMutableDictionary* pParams = [[NSMutableDictionary alloc] init];
            for(Core::StringToStringMap::const_iterator it = insNotification.sParams.begin(); it != insNotification.sParams.end(); ++it)
            {
                [pParams setObject:Core::CStringUtils::StringToNSString(it->first) forKey:Core::CStringUtils::StringToNSString(it->second)];
            }
			
			//Encode the type ID into the notification so we can retrieve it at the other end
			NSDictionary* pInfo = [NSDictionary dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithUnsignedInt:(u32)insNotification.ID], @"ID",
                                   [NSNumber numberWithUnsignedInt:(u32)insNotification.ePriority], @"Priority",
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
        void CLocalNotificationScheduler::CancelByID(NotificationID inID)
        {
			for(UILocalNotification* pNotification in [[UIApplication sharedApplication] scheduledLocalNotifications]) 
			{
				NotificationID ID = [[pNotification.userInfo objectForKey:@"ID"] unsignedIntValue];
				
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
        bool CLocalNotificationScheduler::TryGetNotificationsScheduledWithinTimePeriod(TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, DYNAMIC_ARRAY<Notification>& outaNotifications)
        {
            outaNotifications.clear();
            
            for(UILocalNotification* pNotification in [[UIApplication sharedApplication] scheduledLocalNotifications]) 
			{
                TimeIntervalSecs TriggerTime = (TimeIntervalSecs)[pNotification.fireDate timeIntervalSince1970];
                s32 dwDeltaSecs = inTime - TriggerTime;
                
				if(std::abs(dwDeltaSecs) <= inPeriod) 
				{
                    Notification sNotification;
                    ConvertUILocalNotificationToNotification(pNotification, sNotification);
					outaNotifications.push_back(sNotification);
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
                Notification sNotification;
                ConvertUILocalNotificationToNotification(pLocalNotification, sNotification);
                CNotificationScheduler::OnNotificationReceived(sNotification);
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
            
            Notification sNotification;
            ConvertUILocalNotificationToNotification(inpNotification, sNotification);
            CNotificationScheduler::OnNotificationReceived(sNotification);
        }
        //-------------------------------------------------------------------------
        /// Convert UILocalNotification to Notification
        ///
        /// @param Apple UILocalNotification
        /// @param Out: moFlow notification
        //-------------------------------------------------------------------------
        void CLocalNotificationScheduler::ConvertUILocalNotificationToNotification(UILocalNotification* inpUILocal, Notification& outsNotification)
        {
            outsNotification.bDismissed = false;
            outsNotification.eType = NOTICE_SYSTEM;
            outsNotification.TriggerTime = (TimeIntervalSecs)[inpUILocal.fireDate timeIntervalSince1970];
            
            outsNotification.ID = (NotificationID)[[inpUILocal.userInfo objectForKey:@"ID"] unsignedIntValue];
            outsNotification.ePriority = (NotificationPriority)[[inpUILocal.userInfo objectForKey:@"Priority"] unsignedIntValue];
            
            NSDictionary* pParams = (NSDictionary*)[inpUILocal.userInfo objectForKey:@"Params"];
            
            for(id key in pParams)
            {
                outsNotification.sParams.SetValueForKey(Core::CStringUtils::NSStringToString([pParams objectForKey:key]), Core::CStringUtils::NSStringToString(key));
            }                                       
        }
    }
}
