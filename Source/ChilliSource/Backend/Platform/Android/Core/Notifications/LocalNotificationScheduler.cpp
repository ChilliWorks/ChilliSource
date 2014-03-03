/*
 *  LocalNotificationScheduler.cpp
 *  Funpark Friends
 *
 *  Created by Steven Hendrie on 13/12/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Core/Notifications/LocalNotificationScheduler.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/Notifications/LocalNotificationJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		CLocalNotificationScheduler::CLocalNotificationScheduler()
		{
        	//get the media player java interface or create it if it doesn't yet exist.
			mpLocalNotificationJavaInterface = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CLocalNotificationJavaInterface>();
        	if (mpLocalNotificationJavaInterface == NULL)
        	{
        		mpLocalNotificationJavaInterface = LocalNotificationJavaInterfacePtr(new CLocalNotificationJavaInterface());
        		CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpLocalNotificationJavaInterface);
        	}
		}
		CLocalNotificationScheduler::~CLocalNotificationScheduler()
		{

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
        	CS_LOG_WARNING("Unimplemented method: CLocalNotificationScheduler::TryGetNotificationsScheduledWithinTimePeriod");
        	return false;
        }
		//------------------------------------------------------------------------------
		/// Schedule Notification
		///
		/// Calls java interface schedule method. This is done so that all java interface
		///	functions are held within JavaInterface.
		///
		/// @param Notification
		//------------------------------------------------------------------------------
		void CLocalNotificationScheduler::ScheduleNotification(const Core::NotificationSPtr& insNotification)
		{
			mpLocalNotificationJavaInterface->ScheduleNotification(insNotification);
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
			mpLocalNotificationJavaInterface->CancelByID(inID);
		}
		//-------------------------------------------------------------------------
		/// Cancel All
		///
		/// Terminate all currently scheduled notifications
		//-------------------------------------------------------------------------
		void CLocalNotificationScheduler::CancelAll()
		{
			mpLocalNotificationJavaInterface->CancelAll();
		}
		//-------------------------------------------------------------------------
		/// Application Did Receive Local Notification
		///
		/// Called when game receives a local notification
		//-------------------------------------------------------------------------
		void CLocalNotificationScheduler::ApplicationDidReceiveLocalNotification(const Core::NotificationSPtr& insNotification)
		{
			Core::NotificationScheduler::OnNotificationReceived(insNotification);
		}
	}
}
