//
//  ApplicationEvents.cpp
//  moFloTest
//
//  Created by Scott Downie on 13/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/ApplicationEvents.h>

namespace ChilliSource
{
	namespace Core
	{
		CEvent0<ApplicationSystemDelegate> ApplicationEvents::mResumeEvent;
		CEvent0<ApplicationSystemDelegate> ApplicationEvents::mSuspendEvent;
		CEvent0<ApplicationSystemDelegate> ApplicationEvents::mLateSuspendEvent;

		CEvent0<ApplicationSystemDelegate> ApplicationEvents::mLowMemoryEvent;

		CEvent1<ApplicationScreenOrientationDelegate> ApplicationEvents::mScreenOrientationChangedEvent;
		CEvent0<ApplicationSystemDelegate> ApplicationEvents::mGoBackEvent;
		CEvent2<ApplicationScreenResizeDelegate> ApplicationEvents::mScreenResizedEvent;
		CEvent1<ApplicationReceivesURLDelegate> ApplicationEvents::mReceivesURLEvent;

		//-----------------------------------------------------------------
		/// Get Application Resume Event
		///
		/// @return Event that is triggered when the application resumes
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& ApplicationEvents::GetResumeEvent()
		{
			return mResumeEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Suspend Event
		///
		/// @return Event that is triggered on application suspension
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& ApplicationEvents::GetSuspendEvent()
		{
			return mSuspendEvent;
		}
		//-----------------------------------------------------------------
		/// Get Late Suspend Event
		///
		/// @return Event that is triggered on application suspension after GetSuspendEvent
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& ApplicationEvents::GetLateSuspendEvent()
		{
			return mLateSuspendEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Low Memory Event
		///
		/// @return Event that is triggered when the application 
		/// receives memory warning
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& ApplicationEvents::GetLowMemoryEvent()
		{
			return mLowMemoryEvent;
		}
		//-----------------------------------------------------------------
		/// Get Screen Resized Event
		///
		/// @return Event that is triggered when screen resizes
		//-----------------------------------------------------------------
		CEvent2<ApplicationScreenResizeDelegate>& ApplicationEvents::GetScreenResizedEvent()
		{
			return mScreenResizedEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Screen Orientation Event
		///
		/// @return Event that is triggered when screen rotates
		//-----------------------------------------------------------------
		CEvent1<ApplicationScreenOrientationDelegate>& ApplicationEvents::GetScreenOrientationChangedEvent()
		{
			return mScreenOrientationChangedEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Screen Orientation Event
		///
		/// @return Event that is triggered when back is pressed
		//-----------------------------------------------------------------
		CEvent0<ApplicationSystemDelegate>& ApplicationEvents::GetGoBackEvent()
		{
			return mGoBackEvent;
		}
		//-----------------------------------------------------------------
		/// GetReceivesURLEvent
		///
		/// @return Event that is triggered when the app receives a URL string
		//-----------------------------------------------------------------
		CEvent1<ApplicationReceivesURLDelegate>& ApplicationEvents::GetReceivesURLEvent()
		{
			return mReceivesURLEvent;	
		}

	}
}
