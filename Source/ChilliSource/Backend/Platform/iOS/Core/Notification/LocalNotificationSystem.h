//
//  LocalNotifcationSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 01/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_CORE_NOTIFICATION_LOCALNOTIFICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/Notifications/LocalNotificationSystem.h>

@class UIApplication;
@class UILocalNotification;
@class NSDictionary;

namespace ChilliSource
{
    namespace iOS
    {
        class LocalNotificationSystem : public Core::LocalNotificationSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(LocalNotificationSystem);
            //--------------------------------------------------------
            /// @author I Copland
            ///
            /// @return Whether this implements the passed in interface
            /// id.
            //-------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceID) const override;
            //------------------------------------------------------
            /// Schedule Notification
            ///
            /// Once the time is reached the notification will be
            /// inserted into the queue. Upon reaching the head of
            /// the queue it will trigger.
            ///
            /// @author Scott Downie
            ///
            /// @param Notification
            //-------------------------------------------------------
            void ScheduleNotification(const Core::NotificationSPtr& in_notification, TimeIntervalSecs in_time) override;
            //--------------------------------------------------------
            /// Builds a list of all notifications currently scheduled
            /// within the given time peroid.
            ///
            /// @author I Copland
            ///
            /// @param [Out] The list of notifications.
            /// @param [Optional] The start time.
            /// @param [Optional] The end time.
            //--------------------------------------------------------
            void GetScheduledNotifications(std::vector<Core::NotificationSPtr>& out_notifications, TimeIntervalSecs in_time = 0, TimeIntervalSecs in_peroid = std::numeric_limits<TimeIntervalSecs>::max()) override;
            //--------------------------------------------------------
            /// Prevent any notifications with given ID type from firing
            ///
            /// @author Scott Downie
            ///
            /// @param ID type
            //--------------------------------------------------------
            void CancelByID(Core::Notification::ID in_id) override;
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
            Core::IConnectableEvent<Core::LocalNotificationSystem::RecievedDelegate>& GetRecievedEvent() override;
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
            friend Core::LocalNotificationSystemUPtr Core::LocalNotificationSystem::Create();
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
            void OnInitialise() override;
            //--------------------------------------------------------
            /// Called when a notification is received.
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void OnNotificationRecieved(const Core::NotificationSPtr& in_notification);
            //--------------------------------------------------------
            /// destroys the local notification system.
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void OnDestroy() override;
            
            Core::Event<Core::LocalNotificationSystem::RecievedDelegate> m_receivedEvent;
        };
    }
}

#endif