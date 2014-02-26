/*
 *  Timer.h
 *  moFlo
 *
 *  Created by Scott Downie on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_TIMER_H_
#define _MO_FLO_CORE_TIMER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Time/CoreTimer.h>
#include <vector>

namespace ChilliSource
{
	namespace Core 
	{
		//============================================
		/// Description:
		///
		/// Used to calculate the time taken between
		/// beginning and ending the timer.
		///
		/// Acts as an interface into the system 
		/// timer.
		//============================================
		
		class CTimer
		{
		public:
			CTimer();
			~CTimer();
			
			//--------------------------------------------
			/// Start
			///
			/// Begin the timer 
			//--------------------------------------------
			void Start();
			//--------------------------------------------
			/// Reset 
			///
			/// Reset the elapsed time
			//--------------------------------------------
			void Reset();
			//--------------------------------------------
			/// Stop 
			///
			/// Stop the timer
			//--------------------------------------------
			void Stop();
			//--------------------------------------------
			/// Check if Timer is active
			///
			/// check if timer is active
			/// @return mbIsTimerActive
			//--------------------------------------------
			const bool IsTimerActive() const;
			//--------------------------------------------
			/// Get Time Elapsed
			///
			/// Calculate the time elapsed since starting
			/// @return Delta time
			//--------------------------------------------
			const f32 GetTimeElapsed() const;
			
			//---Callbacks
			//----------------------------------------------------
			/// Register Time Elapsed Delegate
			///
			/// Register to be triggered when the timer reaches  
			/// a certain number of seconds
			/// @param Callback to be triggered
			/// @param Seconds after which callback is triggerd
			//----------------------------------------------------
			void RegisterTimeElapsedDelegate(TimeEventDelegate inDelegate, f32 infNumSecondsTilTrigger);
			//----------------------------------------------------
			/// Deregister Time Elapsed Delegate
			///
			/// Unsubscribe from being triggered after time period
			/// @param Callback to remove
			//----------------------------------------------------
			void DeregisterTimeElapsedDelegate(TimeEventDelegate inDelegate);
			//----------------------------------------------------
			/// Register Periodic Update Delegate
			///
			/// Register to be triggered every given number of
			/// seconds
			/// @param Callback to be triggered
			/// @param Seconds after which callback is triggerd
			//----------------------------------------------------
			void RegisterPeriodicUpdateDelegate(TimeEventDelegate inDelegate, f32 infNumSecondsBetweenTrigger);
			//----------------------------------------------------
			/// Deregister Periodic Update Delegate
			///
			/// Unsubscribe from being triggered after time period
			/// @param Callback to remove
			//----------------------------------------------------
			void DeregisterPeriodicUpdateDelegate(TimeEventDelegate inDelegate);
			
		private:
			//----------------------------------------------------
			/// Update
			///
			/// Called by the application to update the time 
			/// elapsed
			/// @param Time between frames
			//----------------------------------------------------
			void Update(const f32 dt);
			
		private:
			
			struct PeriodicUpdateData
			{
				TimeEventDelegate Delegate;
				f32 fTimeBetweenUpdates;
				f32 fTimeSinceLastUpdate;
			};
			
			f32 mfCurrentTime;
			
			bool mbIsTimerActive;
			
			typedef std::vector<TimeEventDelegate> TimeDelegatesList;
			typedef TimeDelegatesList::iterator TimeDelegatesListItr;
			TimeDelegatesList maTimerBeganDelegates;
			
			std::vector< std::pair<TimeEventDelegate, f32> > maTimeElapsedDelegates;
			std::vector<PeriodicUpdateData> maPeriodicUpdateDelegates;
			std::vector<TimeEventDelegate> maPeriodicUpdateDelegatesDelayedRemove;
		};
	}
}

#endif