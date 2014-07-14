//
//  LocalNotificationJavaInterface.h
//  Chilli Source
//  Created by Steven Hendrie on 13/12/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_CORE_NOTIFICATION_LOCALNOTIFCATIONSJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID_CORE_NOTIFICATION_LOCALNOTIFCATIONSJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>
#include <ChilliSource/Core/Notification/Notification.h>

namespace CSBackend
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
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
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
			void ScheduleNotificationForTime(CSCore::Notification::ID in_id, const CSCore::ParamDictionary& in_params, TimeIntervalSecs in_time, CSCore::Notification::Priority in_priority);
			//--------------------------------------------------------
			/// Builds a list of all notifications currently scheduled
			/// within the given time period.
			///
			/// @author Ian Copland
			///
			/// @param [Out] The list of notifications.
			/// @param The start time.
			/// @param The time period.
			//--------------------------------------------------------
			void GetScheduledNotifications(std::vector<CSCore::NotificationCSPtr>& out_notifications, TimeIntervalSecs in_time, TimeIntervalSecs in_period) const;
			//---------------------------------------------------
			/// Prevent any notifications with given ID type from
			/// firing
			///
			/// @author Steven Hendrie
			///
			/// @param ID type
			//---------------------------------------------------
			void CancelByID(CSCore::Notification::ID in_id);
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

#endif
