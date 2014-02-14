/*
 *  LocalNotificationJavaInterface.h
 *  moFlow
 *
 *  Created by Steven Hendrie on 28/09/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_LOCALNOTIFCATIONSJAVAINTERFACE_H_
#define _MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_LOCALNOTIFCATIONSJAVAINTERFACE_H_

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterface.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>

namespace moFlo
{
	namespace AndroidPlatform
	{

		//=================================================================
		/// Media Player Java Interface
		///
		/// A Java Interface for calls to the Android MediaPlayer.
		//=================================================================
		class CLocalNotificationJavaInterface : public IJavaInterface
		{
		public:
			DECLARE_NAMED_INTERFACE();
			//--------------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------------
			CLocalNotificationJavaInterface();
			//--------------------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//--------------------------------------------------------------------------------------
			/// Schedule Notification
			///
			/// Schedules a notification to be displayed to the user
			///
			/// @param a reference to the notification to be sent
			//--------------------------------------------------------------------------------------
			void ScheduleNotification(const Notification& insNotification);
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
		private:

		};
	}
}

#endif
