//
//  LocalNotificationJavaInterface.h
//  Chilli Source
//
//  Created by Steven Hendrie on 28/09/2012.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_NOTIFICATION_LOCALNOTIFCATIONSJAVAINTERFACE_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_NOTIFICATION_LOCALNOTIFCATIONSJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterface.h>
#include <ChilliSource/Core/Notification/Notification.h>

namespace ChilliSource
{
	namespace Android
	{
		//---------------------------------------------------------
		/// A Java Interface for calls to the Alarm Manager.
		///
		/// @author Steven Hendrie
		//---------------------------------------------------------
		class LocalNotificationJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(LocalNotificationJavaInterface);
			//---------------------------------------------------
			/// Constructor
			///
			/// @author Steven Hendrie
			//---------------------------------------------------
			LocalNotificationJavaInterface();
			//---------------------------------------------------
			/// @author Steven Hendrie
			///
			/// @param The interface ID to check against.
			/// @return whether or not this object implements the
			/// given interface.
			//---------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceID) const override;
			//---------------------------------------------------
			/// Schedules a notification to be displayed to the user
			///
			/// @author Steven Hendrie
			///
			/// @param The notification id
			/// @param The notification params.
			/// @param Time in seconds at which it should trigger.
			/// @param [Optional] The notification priority. Defaults
			/// to standard priority.
			//---------------------------------------------------
			void ScheduleNotificationForTime(Core::Notification::ID in_id, const Core::ParamDictionary& in_params, TimeIntervalSecs in_time, Core::Notification::Priority in_priority);
			//---------------------------------------------------
			/// Prevent any notifications with given ID type from
			/// firing
			///
			/// @author Steven Hendrie
			///
			/// @param ID type
			//---------------------------------------------------
			void CancelByID(Core::Notification::ID in_id);
			//---------------------------------------------------
			/// Terminate all currently scheduled notifications.
			///
			/// @author Steven Hendrie
			//---------------------------------------------------
			void CancelAll();
		};
	}
}

#endif
