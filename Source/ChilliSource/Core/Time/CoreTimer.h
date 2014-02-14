/*
 * File: CoreTimer.h
 * Date: 09/11/2010 2010 
 * Description: Core timer that updates all other game timers.
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_CORE_CORE_TIMER_H_
#define _MO_FLO_CORE_CORE_TIMER_H_

#include <ChilliSource/Core/Event/GenericEvent.h>

namespace moFlo
{
	namespace Core
	{
		typedef fastdelegate::FastDelegate1<const f32> TimeEventDelegate;
		
		class CCoreTimer
		{
		public:
			
			//----------------------------------------------------
			/// Get Timer Update Event
			///
			/// @return The event
			//----------------------------------------------------
			static IEvent<TimeEventDelegate>& GetTimerUpdateEvent();

		private:
            
            //----------------------------------------------------
            /// Update
            ///
            /// @param Time since last frame
            //----------------------------------------------------
			static void Update(f32 infDT);
			
		private:
			
			static CEvent1<TimeEventDelegate> mOnTimerUpdateEvent;
            
			friend class CApplication; //Only application can invoke the update
		};
	}
}

#endif