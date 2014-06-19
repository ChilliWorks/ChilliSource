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

#ifndef _CHILLISOURCE_CORE_CORE_TIMER_H_
#define _CHILLISOURCE_CORE_CORE_TIMER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>

#include <functional>

namespace ChilliSource
{
	namespace Core
	{
		typedef std::function<void(const f32)> TimeEventDelegate;
		
		class CoreTimer
		{
		public:
			
			//----------------------------------------------------
			/// Get Timer Update Event
			///
			/// @return The event
			//----------------------------------------------------
			static IConnectableEvent<TimeEventDelegate>& GetTimerUpdateEvent();

		private:
            
            //----------------------------------------------------
            /// Update
            ///
            /// @param Time since last frame
            //----------------------------------------------------
			static void Update(f32 infDT);
			
		private:
			
			static Event<TimeEventDelegate> mOnTimerUpdateEvent;
            
			friend class Application; //Only application can invoke the update
		};
	}
}

#endif