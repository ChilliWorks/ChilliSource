/*
 *  LocalNotificationScheduler.h
 *  Funpark Friends
 *
 *  Created by Steven Hendrie on 13/12/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_ANDROID_LOCAL_NOTIFICATION_SCHEDULER_H_
#define _MOFLOW_ANDROID_LOCAL_NOTIFICATION_SCHEDULER_H_

#include <ChilliSource/Core/Notifications/LocalNotificationScheduler.h>
#include <ChilliSource/Platform/Android/ForwardDeclarations.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		class CLocalNotificationScheduler : public ILocalNotificationScheduler
		{
		public:
			CLocalNotificationScheduler();
			virtual ~CLocalNotificationScheduler();
			//------------------------------------------------------------------------------
			/// Schedule Notification
			///
			/// Once the time is reached the notification will be inserted into the queue.
			/// Upon reaching the head of the queue it will trigger.
			///
			/// @param Notification
			//------------------------------------------------------------------------------
			void ScheduleNotification(const Notification& insNotification);
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
	        bool TryGetNotificationsScheduledWithinTimePeriod(TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, DYNAMIC_ARRAY<Notification>& outaNotifications);
			//-------------------------------------------------------------------------
			/// Cancel By ID
			///
			/// Prevent any notifications with given ID type from firing
			///
			/// @param ID type
			//-------------------------------------------------------------------------
			void CancelByID(NotificationID inID);
			//-------------------------------------------------------------------------
			/// Cancel All
			///
			/// Terminate all currently scheduled notifications
			//-------------------------------------------------------------------------
			void CancelAll();
			//-------------------------------------------------------------------------
			/// Application Did Receive Local Notification
			///
			/// Called when game receives a local notification
			//-------------------------------------------------------------------------
			static void ApplicationDidReceiveLocalNotification(const Notification& insNotification);

		private:
			LocalNotificationJavaInterfacePtr mpLocalNotificationJavaInterface;
		};
	}
}
#endif
