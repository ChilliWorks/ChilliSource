//
//  ApplicationEvents.cpp
//  moFloTest
//
//  Created by Scott Downie on 13/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/ApplicationEvents.h>

namespace moFlo
{
	namespace Core
	{
		CEvent0<ApplicationSystemDelegate> CApplicationEvents::mResumeEvent;
		CEvent0<ApplicationSystemDelegate> CApplicationEvents::mSuspendEvent;
		CEvent0<ApplicationSystemDelegate> CApplicationEvents::mLateSuspendEvent;

		CEvent0<ApplicationSystemDelegate> CApplicationEvents::mLowMemoryEvent;

		CEvent1<ApplicationScreenOrientationDelegate> CApplicationEvents::mScreenOrientationChangedEvent;
		CEvent0<ApplicationSystemDelegate> CApplicationEvents::mGoBackEvent;
		CEvent2<ApplicationScreenResizeDelegate> CApplicationEvents::mScreenResizedEvent;
		CEvent1<ApplicationReceivesURLDelegate> CApplicationEvents::mReceivesURLEvent;

		//-----------------------------------------------------------------
		/// Get Application Resume Event
		///
		/// @return Event that is triggered when the application resumes
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& CApplicationEvents::GetResumeEvent()
		{
			return mResumeEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Suspend Event
		///
		/// @return Event that is triggered on application suspension
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& CApplicationEvents::GetSuspendEvent()
		{
			return mSuspendEvent;
		}
		//-----------------------------------------------------------------
		/// Get Late Suspend Event
		///
		/// @return Event that is triggered on application suspension after GetSuspendEvent
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& CApplicationEvents::GetLateSuspendEvent()
		{
			return mLateSuspendEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Low Memory Event
		///
		/// @return Event that is triggered when the application 
		/// receives memory warning
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& CApplicationEvents::GetLowMemoryEvent()
		{
			return mLowMemoryEvent;
		}
		//-----------------------------------------------------------------
		/// Get Screen Resized Event
		///
		/// @return Event that is triggered when screen resizes
		//-----------------------------------------------------------------
		CEvent2<ApplicationScreenResizeDelegate>& CApplicationEvents::GetScreenResizedEvent()
		{
			return mScreenResizedEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Screen Orientation Event
		///
		/// @return Event that is triggered when screen rotates
		//-----------------------------------------------------------------
		CEvent1<ApplicationScreenOrientationDelegate>& CApplicationEvents::GetScreenOrientationChangedEvent()
		{
			return mScreenOrientationChangedEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Screen Orientation Event
		///
		/// @return Event that is triggered when back is pressed
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& CApplicationEvents::GetGoBackEvent()
		{
			return mGoBackEvent;
		}
		//-----------------------------------------------------------------
		/// GetReceivesURLEvent
		///
		/// @return Event that is triggered when the app receives a URL string
		//-----------------------------------------------------------------
		CEvent1<ApplicationReceivesURLDelegate>& CApplicationEvents::GetReceivesURLEvent()
		{
			return mReceivesURLEvent;	
		}

	}
}
