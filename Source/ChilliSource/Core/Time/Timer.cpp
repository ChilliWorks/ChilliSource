/*
 *  Timer.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Time/Timer.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>

namespace ChilliSource
{
	namespace Core 
	{
		//--------------------------------------------
		/// Constructor 
		///
		/// Default
		//--------------------------------------------
		Timer::Timer() : mfCurrentTime(0.0f), mbIsTimerActive(false)
		{
            m_coreTimerUpdateConnection = CCoreTimer::GetTimerUpdateEvent().OpenConnection(MakeDelegate(this, &Timer::Update));
		}
		//--------------------------------------------
		/// Start
		///
		/// Begin the timer 
		//--------------------------------------------
		void Timer::Start()
		{
			mbIsTimerActive = true;
		}
		//--------------------------------------------
		/// Reset 
		///
		/// Reset the elapsed time
		//--------------------------------------------
		void Timer::Reset()
		{
			mfCurrentTime = 0.0f;
		}
		//--------------------------------------------
		/// Stop 
		///
		/// Stop the timer
		//--------------------------------------------
		void Timer::Stop()
		{
			mbIsTimerActive = false;
		}
        //--------------------------------------------
        /// Check if Timer is active
        ///
        /// check if timer is active
        /// @return mbIsTimerActive
        //--------------------------------------------
        const bool Timer::IsTimerActive() const
        {
            return mbIsTimerActive;
        }
		//--------------------------------------------
		/// Get Time Elapsed
		///
		/// Calculate the time elapsed since starting
		/// @return Delta time
		//--------------------------------------------
		const f32 Timer::GetTimeElapsed() const
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
		void Timer::Update(const f32 dt)
		{
			if(mbIsTimerActive)
			{
				mfCurrentTime += dt;
				
				//After time delegates
                
                if(!maTimeElapsedDelegates.empty())
                {
                    std::vector< std::pair<TimeEventDelegate, f32> > aTimeElapsedDelegatesCopy = maTimeElapsedDelegates;
                    for(std::vector< std::pair<TimeEventDelegate, f32> >::iterator it = aTimeElapsedDelegatesCopy.begin(); it != aTimeElapsedDelegatesCopy.end(); ++it)
                    {
                        if(mfCurrentTime >= it->second)
                        {
                            (it->first)(mfCurrentTime);
                        }
                    }
                }
                
				//Periodic delegates
				for(std::vector<TimeEventDelegate>::iterator itOut = maPeriodicUpdateDelegatesDelayedRemove.begin(); itOut != maPeriodicUpdateDelegatesDelayedRemove.end(); ++itOut)
				{
					for(std::vector<PeriodicUpdateData>::iterator itIn = maPeriodicUpdateDelegates.begin(); itIn != maPeriodicUpdateDelegates.end(); ++itIn)
					{
						if(*itOut == itIn->Delegate)
						{
							maPeriodicUpdateDelegates.erase(itIn);
							break;
						}
					}
				}
				maPeriodicUpdateDelegatesDelayedRemove.clear();
				for(std::vector<PeriodicUpdateData>::iterator it = maPeriodicUpdateDelegates.begin(); it != maPeriodicUpdateDelegates.end(); ++it)
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
		void Timer::RegisterTimeElapsedDelegate(TimeEventDelegate inDelegate, f32 infNumSecondsTilTrigger)
		{
			maTimeElapsedDelegates.push_back(std::pair<TimeEventDelegate, f32>(inDelegate, infNumSecondsTilTrigger));
		}
		//----------------------------------------------------
		/// Deregister Time Elapsed Delegate
		///
		/// Unsubscribe from being triggered after time period
		/// @param Callback to remove
		//----------------------------------------------------
		void Timer::DeregisterTimeElapsedDelegate(TimeEventDelegate inDelegate)
		{
			for(std::vector< std::pair<TimeEventDelegate, f32> >::iterator it = maTimeElapsedDelegates.begin(); it != maTimeElapsedDelegates.end(); ++it)
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
		void Timer::RegisterPeriodicUpdateDelegate(TimeEventDelegate inDelegate, f32 infNumSecondsBetweenTrigger)
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
		void Timer::DeregisterPeriodicUpdateDelegate(TimeEventDelegate inDelegate)
		{
			for(std::vector<PeriodicUpdateData>::iterator it = maPeriodicUpdateDelegates.begin(); it != maPeriodicUpdateDelegates.end(); ++it)
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
		Timer::~Timer()
		{
			maTimerBeganDelegates.clear();
			maTimeElapsedDelegates.clear();
		}
	}
}
