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
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>

namespace ChilliSource
{
	namespace Android
	{
		class CLocalNotificationScheduler : public Core::LocalNotificationScheduler
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
			void ScheduleNotification(const Core::NotificationSPtr& insNotification) override;
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
	        bool TryGetNotificationsScheduledWithinTimePeriod(TimeIntervalSecs inTime, TimeIntervalSecs inPeriod, std::vector<Core::NotificationSPtr>& outaNotifications) override;
			//-------------------------------------------------------------------------
			/// Cancel By ID
			///
			/// Prevent any notifications with given ID type from firing
			///
			/// @param ID type
			//-------------------------------------------------------------------------
			void CancelByID(Core::NotificationID inID) override;
			//-------------------------------------------------------------------------
			/// Cancel All
			///
			/// Terminate all currently scheduled notifications
			//-------------------------------------------------------------------------
			void CancelAll() override;
			//-------------------------------------------------------------------------
			/// Application Did Receive Local Notification
			///
			/// Called when game receives a local notification
			//-------------------------------------------------------------------------
			static void ApplicationDidReceiveLocalNotification(const Core::NotificationSPtr& insNotification);

		private:
			LocalNotificationJavaInterfacePtr mpLocalNotificationJavaInterface;
		};
	}
}
#endif
