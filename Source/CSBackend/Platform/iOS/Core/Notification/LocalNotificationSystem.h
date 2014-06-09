//
//  LocalNotifcationSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 01/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CSBACKEND_PLATFORM_IOS_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_
#define _CSBACKEND_PLATFORM_IOS_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/Notification/LocalNotificationSystem.h>

@class UIApplication;
@class UILocalNotification;
@class NSDictionary;

namespace CSBackend
{
    namespace iOS
    {
        class LocalNotificationSystem final : public CSCore::LocalNotificationSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(LocalNotificationSystem);
            //--------------------------------------------------------
            /// @author I Copland
            ///
            /// @param The interface ID to check against.
            /// @return Whether this implements the passed in interface
            /// id.
            //-------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
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
            void ScheduleNotificationForTime(CSCore::Notification::ID in_id, const CSCore::ParamDictionary& in_params, TimeIntervalSecs in_time, CSCore::Notification::Priority in_priority = CSCore::Notification::Priority::k_standard) override;
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
            void GetScheduledNotifications(std::vector<CSCore::NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time = 0, TimeIntervalSecs in_period = std::numeric_limits<TimeIntervalSecs>::max()) const override;
            //--------------------------------------------------------
            /// Prevent any notifications with given ID type from firing
            ///
            /// @author Scott Downie
            ///
            /// @param ID type
            //--------------------------------------------------------
            void CancelByID(CSCore::Notification::ID in_id) override;
            //--------------------------------------------------------
            /// Terminate all currently scheduled notifications
            ///
            /// @author Scott Downie
            //--------------------------------------------------------
            void CancelAll() override;
            //--------------------------------------------------
            /// @author I Copland
            ///
            /// @return An event that can be used to listen for
            /// new notifications being recieved.
            //---------------------------------------------------
            CSCore::IConnectableEvent<CSCore::LocalNotificationSystem::RecievedDelegate>& GetRecievedEvent() override;
            //--------------------------------------------------------
            /// Called by the app delegate when the app launches. Used
            /// to pull local notification data from the options
            ///
            /// @author Scott Downie
            ///
            /// @param UIApplication
            /// @param Launching options dictionary
            /// @return Whether the app was launched with local
            /// notifications
            //---------------------------------------------------------
            static bool ApplicationDidFinishLaunchingWithOptions(UIApplication* in_application, NSDictionary* in_options);
			//---------------------------------------------------------
            /// Called by the app delegate when the app becomes active.
            /// Used to clear the icon badge number
            ///
            /// @author Scott Downie
            ///
            /// @param UIApplication
            //----------------------------------------------------------
            static void ApplicationDidBecomeActive(UIApplication* in_application);
            //----------------------------------------------------------
            /// Called by the app delegate when it receives a local
            /// notification.
            ///
            /// @author Scott Downie
            ///
            /// @param UILocalNotification
            //----------------------------------------------------------
            static void ApplicationDidReceiveLocalNotification(UIApplication* in_application, UILocalNotification* in_notification);
            //----------------------------------------------------------
            /// Destructor.
            ///
            /// @author Scott Downie
            //----------------------------------------------------------
            ~LocalNotificationSystem();
        private:
            friend CSCore::LocalNotificationSystemUPtr CSCore::LocalNotificationSystem::Create();
            //----------------------------------------------------------
            /// Private constructor to force use of factory method.
            ///
            /// @author S Downie
            //----------------------------------------------------------
            LocalNotificationSystem();
            //--------------------------------------------------------
            /// Initalises the local notification system.
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void OnInit() override;
            //--------------------------------------------------------
            /// Called when a notification is received.
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void OnNotificationRecieved(const CSCore::NotificationSPtr& in_notification);
            //--------------------------------------------------------
            /// destroys the local notification system.
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void OnDestroy() override;
            
            bool m_enabled;
            CSCore::Event<CSCore::LocalNotificationSystem::RecievedDelegate> m_receivedEvent;
        };
    }
}

#endif

#endif