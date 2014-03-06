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
		CS_DEFINE_NAMEDTYPE(TouchScreen);

		TouchScreen::TouchScreen()
		:mLastTouchIndex(0), mffLastTimeStamp(0.0f), mudwOrientedScreenHeight(0), mbIsEnabled(true)
		{
			
		}
		TouchScreen::~TouchScreen()
		{
			
		}

		bool TouchInfo::operator==(const TouchInfo & inOther)
		{
			return (this->ID == inOther.ID);
		}
		Core::IConnectableEvent<TouchEventDelegate> & TouchScreen::GetTouchBeganEvent()
		{
			return mTouchBeganEvent;
		}
		Core::IConnectableEvent<TouchEventDelegate> & TouchScreen::GetTouchMovedEvent()
		{
			return mTouchMovedEvent;
		}
		Core::IConnectableEvent<TouchEventDelegate> & TouchScreen::GetTouchEndEvent()
		{
			return mTouchEndedEvent;
		}
		void TouchScreen::SetTouchTransformMatrix(const Core::Matrix4x4 & inTouchTransformMat)
		{
			mTouchTransformMatrix = inTouchTransformMat;	
		}
		void TouchScreen::SetCurrentAppTime(f64 inffTimeStamp)
		{
			mffLastTimeStamp = inffTimeStamp;
		}
		void TouchScreen::SetScreenHeight(u32 inudwScreenHeight)
		{
			mudwOrientedScreenHeight = inudwScreenHeight;
		}
        void TouchScreen::Enable()
        {
            mbIsEnabled = true;
        }
        void TouchScreen::Disable()
        {
            mbIsEnabled = false;
        }
        //-----------------------------------------------------------
        /// Flush Buffered Input
        ///
        /// Have the input elements notify listeners of each
        /// buffered value then clear the buffered input
        //-----------------------------------------------------------
        void TouchScreen::FlushBufferedInput()
        {
        	std::unique_lock<std::mutex> Lock(mMutex);

        	for(TouchListItr pTouch = mBufferedTouches.begin(); pTouch != mBufferedTouches.end(); ++pTouch)
            {
            	switch((*pTouch).eType)
                {
                    case TouchInputType::k_began:
                        AddActiveTouch(*pTouch);
                        mTouchBeganEvent.NotifyConnections(*pTouch);
                        break;
                    case TouchInputType::k_moved:
                        UpdateActiveTouch(*pTouch);
                        mTouchMovedEvent.NotifyConnections(*pTouch);
                        break;
                    case TouchInputType::k_ended:
                        mTouchEndedEvent.NotifyConnections(*pTouch);
                        RemoveActiveTouch(*pTouch);
                        break;
                };
            }
            
            mBufferedTouches.clear();
        }
		u32 TouchScreen::StartTouch(const Core::Vector2 & invTouchLocation, f64 inTimeStamp)
		{
            //Give the touch a unique ID
			u32 TouchID = ++mLastTouchIndex;
            
            if(!mbIsEnabled)
                return TouchID;
            
            //Transform the touch from crappy iOS space to our space
			Core::Vector2 TransformedPosition2(invTouchLocation.x, mudwOrientedScreenHeight - invTouchLocation.y);
			
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
		void TouchScreen::MoveTouch(u32 inID, const Core::Vector2 & invNewTouchLocation, f64 inTimeStamp)
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
					Core::Vector2 TransformedPosition2(invNewTouchLocation.x, mudwOrientedScreenHeight - invNewTouchLocation.y);
					NewTouch.vLocation = TransformedPosition2;
                    pTouch->vLocation = TransformedPosition2;

                    std::unique_lock<std::mutex> Lock(mMutex);
                    mBufferedTouches.push_back(NewTouch);
					return;
				}
			}
		}
		void TouchScreen::EndTouch(u32 inID, f64 inTimeStamp)
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
        void TouchScreen::AddActiveTouch(const TouchInfo& inTouchInfo)
        {
            maActiveTouches.push_back(inTouchInfo);
        }
        //-----------------------------------------------------------
        /// Update Active Touch
        //-----------------------------------------------------------
        void TouchScreen::UpdateActiveTouch(const TouchInfo& inTouchInfo)
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
        void TouchScreen::RemoveActiveTouch(const TouchInfo& inTouchInfo)
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
