/*
 *  Timer.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Time/Timer.h>

namespace moFlo
{
	namespace Core 
	{
		#define MS_TO_SECS(x) (f32)(x) * 0.001f
		#define NS_TO_SECS(x) (f32)(x) * 0.000000001f
		//--------------------------------------------
		/// Constructor 
		///
		/// Default
		//--------------------------------------------
		CTimer::CTimer() : mfCurrentTime(0.0f), mbIsTimerActive(false)
		{
            CCoreTimer::GetTimerUpdateEvent().AddListener(TimeEventDelegate(this, &CTimer::Update));
		}
		//--------------------------------------------
		/// Start
		///
		/// Begin the timer 
		//--------------------------------------------
		void CTimer::Start()
		{
			mbIsTimerActive = true;
		}
		//--------------------------------------------
		/// Reset 
		///
		/// Reset the elapsed time
		//--------------------------------------------
		void CTimer::Reset()
		{
			mfCurrentTime = 0.0f;
		}
		//--------------------------------------------
		/// Stop 
		///
		/// Stop the timer
		//--------------------------------------------
		void CTimer::Stop()
		{
			mbIsTimerActive = false;
		}
        //--------------------------------------------
        /// Check if Timer is active
        ///
        /// check if timer is active
        /// @return mbIsTimerActive
        //--------------------------------------------
        const bool CTimer::IsTimerActive() const
        {
            return mbIsTimerActive;
        }
		//--------------------------------------------
		/// Get Time Elapsed
		///
		/// Calculate the time elapsed since starting
		/// @return Delta time
		//--------------------------------------------
		const f32 CTimer::GetTimeElapsed() const
		{
			return mfCurrentTime;
		}
		//----------------------------------------------------
		/// Update
		///
		/// Called by the application to update the time 
		/// elapsed
		/// @param Time between frames
		//----------------------------------------------------
		void CTimer::Update(const f32 dt)
		{
			if(mbIsTimerActive)
			{
				mfCurrentTime += dt;
				
				//After time delegates
                
                if(!maTimeElapsedDelegates.empty())
                {
                    DYNAMIC_ARRAY< std::pair<TimeEventDelegate, f32> > aTimeElapsedDelegatesCopy = maTimeElapsedDelegates;
                    for(DYNAMIC_ARRAY< std::pair<TimeEventDelegate, f32> >::iterator it = aTimeElapsedDelegatesCopy.begin(); it != aTimeElapsedDelegatesCopy.end(); ++it)
                    {
                        if(mfCurrentTime >= it->second)
                        {
                            (it->first)(mfCurrentTime);
                        }
                    }
                }
                
				//Periodic delegates
				for(DYNAMIC_ARRAY<TimeEventDelegate>::iterator itOut = maPeriodicUpdateDelegatesDelayedRemove.begin(); itOut != maPeriodicUpdateDelegatesDelayedRemove.end(); ++itOut)
				{
					for(DYNAMIC_ARRAY<PeriodicUpdateData>::iterator itIn = maPeriodicUpdateDelegates.begin(); itIn != maPeriodicUpdateDelegates.end(); ++itIn)
					{
						if(*itOut == itIn->Delegate)
						{
							maPeriodicUpdateDelegates.erase(itIn);
							break;
						}
					}
				}
				maPeriodicUpdateDelegatesDelayedRemove.clear();
				for(DYNAMIC_ARRAY<PeriodicUpdateData>::iterator it = maPeriodicUpdateDelegates.begin(); it != maPeriodicUpdateDelegates.end(); ++it)
				{
					if((it->fTimeSinceLastUpdate += dt) >= it->fTimeBetweenUpdates)
					{
						it->fTimeSinceLastUpdate = 0.0f;
						(it->Delegate)(mfCurrentTime);
					}
				}
			}
		}
		//---Callbacks
		//----------------------------------------------------
		/// Register Time Elapsed Delegate
		///
		/// Register to be triggered when the timer reaches  
		/// a certain number of seconds
		/// @param Callback to be triggered
		/// @param Seconds after which callback is triggerd
		//----------------------------------------------------
		void CTimer::RegisterTimeElapsedDelegate(TimeEventDelegate inDelegate, f32 infNumSecondsTilTrigger)
		{
			maTimeElapsedDelegates.push_back(std::pair<TimeEventDelegate, f32>(inDelegate, infNumSecondsTilTrigger));
		}
		//----------------------------------------------------
		/// Deregister Time Elapsed Delegate
		///
		/// Unsubscribe from being triggered after time period
		/// @param Callback to remove
		//----------------------------------------------------
		void CTimer::DeregisterTimeElapsedDelegate(TimeEventDelegate inDelegate)
		{
			for(DYNAMIC_ARRAY< std::pair<TimeEventDelegate, f32> >::iterator it = maTimeElapsedDelegates.begin(); it != maTimeElapsedDelegates.end(); ++it)
			{
				if(inDelegate == it->first)
				{
					maTimeElapsedDelegates.erase(it);
					break;
				}
			}
		}
		//----------------------------------------------------
		/// Register Periodic Update Delegate
		///
		/// Register to be triggered every given number of
		/// seconds
		/// @param Callback to be triggered
		/// @param Seconds after which callback is triggerd
		//----------------------------------------------------
		void CTimer::RegisterPeriodicUpdateDelegate(TimeEventDelegate inDelegate, f32 infNumSecondsBetweenTrigger)
		{
			PeriodicUpdateData Data;
			Data.Delegate = inDelegate;
			Data.fTimeBetweenUpdates = infNumSecondsBetweenTrigger;
			Data.fTimeSinceLastUpdate = 0.0f;
			
			maPeriodicUpdateDelegates.push_back(Data);
		}
		//----------------------------------------------------
		/// Deregister Periodic Update Delegate
		///
		/// Unsubscribe from being triggered after time period
		/// @param Callback to remove
		//----------------------------------------------------
		void CTimer::DeregisterPeriodicUpdateDelegate(TimeEventDelegate inDelegate)
		{
			for(DYNAMIC_ARRAY<PeriodicUpdateData>::iterator it = maPeriodicUpdateDelegates.begin(); it != maPeriodicUpdateDelegates.end(); ++it)
			{
				if(inDelegate == it->Delegate)
				{
					maPeriodicUpdateDelegatesDelayedRemove.push_back(it->Delegate);
					break;
				}
			}
		}
		//--------------------------------------------
		/// Destructor
		///
		//--------------------------------------------
		CTimer::~CTimer()
		{
			maTimerBeganDelegates.clear();
			maTimeElapsedDelegates.clear();
			
            CCoreTimer::GetTimerUpdateEvent().RemoveListener(TimeEventDelegate(this, &CTimer::Update));
		}
	}
}