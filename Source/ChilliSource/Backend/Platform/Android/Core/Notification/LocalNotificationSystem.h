//
//  LocalNotificationSystem.h
//  Chilli Source
//
//  Created by Steven Hendrie on 13/12/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Notification/LocalNotificationSystem.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>

namespace ChilliSource
{
	namespace Android
	{
		//--------------------------------------------------------
		/// Android backend for the local notification system
		///
		/// @author Steven Hendrie
		//--------------------------------------------------------
		class LocalNotificationSystem : public Core::LocalNotificationSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(LocalNotificationSystem);
            //---------------------------------------------------
            /// @author I Copland
            ///
			/// @param The interface ID to check against.
            /// @return Whether this implements the passed in
			/// interface id.
            //--------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//--------------------------------------------------
			/// Enables and disables addition of local notifications.
			/// All existing notifications will be cancelled
			/// when this is disabled. This is enabled by default.
			///
			/// @author I Copland
			///
			/// @param Whether or not to enable the scheduling
			/// of app notifications.
			//---------------------------------------------------
			void SetEnabled(bool in_enabled) override;
			//---------------------------------------------------
			/// Schedules a Local Notifcation which should fire
			/// at the given time. A Local Notification uses
			/// the system specfic notification alarms and can
			/// be received even if it was scheduled during a
			/// previous instance of the application.
			///
			/// @author I Copland
			///
			/// @param The notification id
			/// @param The notification params.
			/// @param Time in seconds at which it should trigger.
			/// @param [Optional] The notification priority. Defaults
			/// to standard priority.
			//---------------------------------------------------
			void ScheduleNotificationForTime(Core::Notification::ID in_id, const Core::ParamDictionary& in_params, TimeIntervalSecs in_time, Core::Notification::Priority in_priority = Core::Notification::Priority::k_standard) override;
			//--------------------------------------------------------
			/// Builds a list of all notifications currently scheduled
			/// within the given time period.
			///
			/// @author I Copland
			///
			/// @param [Out] The list of notifications.
			/// @param [Optional] The start time.
			/// @param [Optional] The end time.
			//--------------------------------------------------------
			void GetScheduledNotifications(std::vector<Core::NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time = 0, TimeIntervalSecs in_period = std::numeric_limits<TimeIntervalSecs>::max()) const override;
			//-------------------------------------------------------
			/// Prevent any notifications with given ID type from firing
			///
			/// @author Steven Hendrie
			///
			/// @param ID type
			//------------------------------------------------------
			void CancelByID(Core::Notification::ID in_id) override;
			//------------------------------------------------------
			/// @author Steven Hendrie
			///
			/// Terminate all currently scheduled notifications
			//-----------------------------------------------------
			void CancelAll() override;
            //--------------------------------------------------
            /// @author I Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being recieved.
            //---------------------------------------------------
            Core::IConnectableEvent<RecievedDelegate>& GetRecievedEvent() override;
			//-----------------------------------------------------
			/// Called when game receives a local notification.
			///
			/// @author Steven Hendrie
            ///
            /// @param The notification ID
            /// @param The notification params.
            /// @param The notification priority.
			//----------------------------------------------------
			void OnNotificationReceived(Core::Notification::ID in_id, const Core::ParamDictionary& in_params, Core::Notification::Priority in_priority);
		private:
            friend Core::LocalNotificationSystemUPtr Core::LocalNotificationSystem::Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            LocalNotificationSystem();

		private:
            bool m_enabled;
            Core::Event<RecievedDelegate> m_recievedEvent;
			LocalNotificationJavaInterfaceSPtr m_localNotificationJI;
		};
	}
}
#endif
