/**
 * File: Touchscreen.h
 * Date: 5 Oct 2010
 * Description: Interface for a TouchScreen input device
 */

/**
 *
 * Author Stuart
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved
 */

#include <ChilliSource/Input/Pointer/TouchScreen.h>
#include <ChilliSource/Input/Base/InputSystem.h>
#include <ChilliSource/Input/Accelerometer/Accelerometer.h>

#include <algorithm>
#include <thread>

namespace ChilliSource
{
	namespace Input
	{

		DEFINE_NAMED_INTERFACE(ITouchScreen);
		DEFINE_NAMED_INTERFACE(IInputSystem);

		ITouchScreen::ITouchScreen()
		:mLastTouchIndex(0), mffLastTimeStamp(0.0f), mudwOrientedScreenHeight(0), mbIsEnabled(true)
		{
			
		}
		ITouchScreen::~ITouchScreen()
		{
			
		}

		bool TouchInfo::operator==(const TouchInfo & inOther)
		{
			return (this->ID == inOther.ID);
		}
		Core::IEvent<TouchEventDelegate> & ITouchScreen::GetTouchBeganEvent()
		{
			return mTouchBeganEvent;
		}
		Core::IEvent<TouchEventDelegate> & ITouchScreen::GetTouchMovedEvent()
		{
			return mTouchMovedEvent;
		}
		Core::IEvent<TouchEventDelegate> & ITouchScreen::GetTouchEndEvent()
		{
			return mTouchEndedEvent;
		}
		void ITouchScreen::SetTouchTransformMatrix(const Core::CMatrix4x4 & inTouchTransformMat)
		{
			mTouchTransformMatrix = inTouchTransformMat;	
		}
		void ITouchScreen::SetCurrentAppTime(f64 inffTimeStamp)
		{
			mffLastTimeStamp = inffTimeStamp;
		}
		void ITouchScreen::SetScreenHeight(u32 inudwScreenHeight)
		{
			mudwOrientedScreenHeight = inudwScreenHeight;
		}
        void ITouchScreen::Enable()
        {
            mbIsEnabled = true;
        }
        void ITouchScreen::Disable()
        {
            mbIsEnabled = false;
        }
        //-----------------------------------------------------------
        /// Flush Buffered Input
        ///
        /// Have the input elements notify listeners of each
        /// buffered value then clear the buffered input
        //-----------------------------------------------------------
        void ITouchScreen::FlushBufferedInput()
        {
        	std::unique_lock<std::mutex> Lock(mMutex);

        	for(TouchListItr pTouch = mBufferedTouches.begin(); pTouch != mBufferedTouches.end(); ++pTouch)
            {
            	switch((*pTouch).eType)
                {
                    case TouchInputType::k_began:
                        AddActiveTouch(*pTouch);
                        mTouchBeganEvent.Invoke(*pTouch);
                        break;
                    case TouchInputType::k_moved:
                        UpdateActiveTouch(*pTouch);
                        mTouchMovedEvent.Invoke(*pTouch);
                        break;
                    case TouchInputType::k_ended:
                        mTouchEndedEvent.Invoke(*pTouch);
                        RemoveActiveTouch(*pTouch);
                        break;
                };
            }
            
            mBufferedTouches.clear();
        }
		u32 ITouchScreen::StartTouch(const Core::CVector2 & invTouchLocation, f64 inTimeStamp)
		{
            //Give the touch a unique ID
			u32 TouchID = ++mLastTouchIndex;
            
            if(!mbIsEnabled)
                return TouchID;
            
            //Transform the touch from crappy iOS space to our space
			Core::CVector2 TransformedPosition2(invTouchLocation.x, mudwOrientedScreenHeight - invTouchLocation.y);
			
            //Create the touch data
			TouchInfo NewTouch;
			NewTouch.ID = TouchID;
			NewTouch.vLocation = TransformedPosition2;
			NewTouch.vPreviousLocation = TransformedPosition2;
			NewTouch.TimeStamp = inTimeStamp;
            NewTouch.eType = TouchInputType::k_began;

            std::unique_lock<std::mutex> Lock(mMutex);
            mOpenTouches.push_back(NewTouch);
            mBufferedTouches.push_back(NewTouch);

			return TouchID;
		}
		void ITouchScreen::MoveTouch(u32 inID, const Core::CVector2 & invNewTouchLocation, f64 inTimeStamp)
		{
            if(!mbIsEnabled)
                return;
            
			//Find the most recent instance of this touch and duplicate it making sure to update
            //its current position
            TouchInfo NewTouch;
            NewTouch.ID = inID;
			NewTouch.TimeStamp = inTimeStamp;
            NewTouch.eType = TouchInputType::k_moved;
            
			for (TouchListItr pTouch = mOpenTouches.begin(); pTouch != mOpenTouches.end(); ++pTouch)
			{
				if (pTouch->ID == inID) 
				{
                    pTouch->vPreviousLocation = pTouch->vLocation;
                    NewTouch.vPreviousLocation = pTouch->vLocation;
					Core::CVector2 TransformedPosition2(invNewTouchLocation.x, mudwOrientedScreenHeight - invNewTouchLocation.y);
					NewTouch.vLocation = TransformedPosition2;
                    pTouch->vLocation = TransformedPosition2;

                    std::unique_lock<std::mutex> Lock(mMutex);
                    mBufferedTouches.push_back(NewTouch);
					return;
				}
			}
		}
		void ITouchScreen::EndTouch(u32 inID, f64 inTimeStamp)
		{
            //Find the most recent instance of this touch and duplicate it making sure to update
            //its current position
            TouchInfo NewTouch;
            NewTouch.ID = inID;
			NewTouch.TimeStamp = inTimeStamp;
            NewTouch.eType = TouchInputType::k_ended;
            
			for (TouchListItr pTouch = mOpenTouches.begin(); pTouch != mOpenTouches.end(); ++pTouch)
			{
				if (pTouch->ID == inID) 
				{
                    NewTouch.vPreviousLocation = pTouch->vPreviousLocation;
					NewTouch.vLocation = pTouch->vPreviousLocation;
                    
					std::unique_lock<std::mutex>  Lock(mMutex);
					mBufferedTouches.push_back(NewTouch);
                    mOpenTouches.erase(pTouch);
					return;
				}
			}
		}
        //-----------------------------------------------------------
        /// Add Active Touch
        //-----------------------------------------------------------
        void ITouchScreen::AddActiveTouch(const TouchInfo& inTouchInfo)
        {
            maActiveTouches.push_back(inTouchInfo);
        }
        //-----------------------------------------------------------
        /// Update Active Touch
        //-----------------------------------------------------------
        void ITouchScreen::UpdateActiveTouch(const TouchInfo& inTouchInfo)
        {
            for (TouchListItr touchIt = maActiveTouches.begin(); touchIt != maActiveTouches.end(); ++touchIt)
			{
				if (touchIt->ID == inTouchInfo.ID)
				{
                    (*touchIt) = inTouchInfo;
					return;
				}
			}
        }
        //-----------------------------------------------------------
        /// Remove Active Touch
        //-----------------------------------------------------------
        void ITouchScreen::RemoveActiveTouch(const TouchInfo& inTouchInfo)
        {
            for (TouchListItr touchIt = maActiveTouches.begin(); touchIt != maActiveTouches.end(); ++touchIt)
			{
				if (touchIt->ID == inTouchInfo.ID)
				{
                    touchIt = maActiveTouches.erase(touchIt);
					return;
				}
			}
        }
	}

}
