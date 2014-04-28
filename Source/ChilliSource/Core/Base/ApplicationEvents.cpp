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
		Event<ApplicationSystemDelegate> ApplicationEvents::mResumeEvent;
		Event<ApplicationSystemDelegate> ApplicationEvents::mSuspendEvent;
		Event<ApplicationSystemDelegate> ApplicationEvents::mLateSuspendEvent;

		Event<ApplicationSystemDelegate> ApplicationEvents::mLowMemoryEvent;

		Event<ApplicationSystemDelegate> ApplicationEvents::mGoBackEvent;
		Event<ApplicationReceivesURLDelegate> ApplicationEvents::mReceivesURLEvent;

		//-----------------------------------------------------------------
		/// Get Application Resume Event
		///
		/// @return Event that is triggered when the application resumes
		//-----------------------------------------------------------------
		Event<ApplicationSystemDelegate>& ApplicationEvents::GetResumeEvent()
		{
			return mResumeEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Suspend Event
		///
		/// @return Event that is triggered on application suspension
		//-----------------------------------------------------------------
		Event<ApplicationSystemDelegate>& ApplicationEvents::GetSuspendEvent()
		{
			return mSuspendEvent;
		}
		//-----------------------------------------------------------------
		/// Get Late Suspend Event
		///
		/// @return Event that is triggered on application suspension after GetSuspendEvent
		//-----------------------------------------------------------------
		Event<ApplicationSystemDelegate>& ApplicationEvents::GetLateSuspendEvent()
		{
			return mLateSuspendEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Low Memory Event
		///
		/// @return Event that is triggered when the application 
		/// receives memory warning
		//-----------------------------------------------------------------
		Event<ApplicationSystemDelegate>& ApplicationEvents::GetLowMemoryEvent()
		{
			return mLowMemoryEvent;
		}
		//-----------------------------------------------------------------
		/// Get Application Screen Orientation Event
		///
		/// @return Event that is triggered when back is pressed
		//-----------------------------------------------------------------
		Event<ApplicationSystemDelegate>& ApplicationEvents::GetGoBackEvent()
		{
			return mGoBackEvent;
		}
		//-----------------------------------------------------------------
		/// GetReceivesURLEvent
		///
		/// @return Event that is triggered when the app receives a URL string
		//-----------------------------------------------------------------
		Event<ApplicationReceivesURLDelegate>& ApplicationEvents::GetReceivesURLEvent()
		{
			return mReceivesURLEvent;	
		}

	}
}
