/*
 *  TouchScreen.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 05/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Input/Pointer/TouchScreen.h>

#include <ChilliSource/Core/Base/Screen.h>

ChilliSource::iOS::TouchScreen* gpTouchScreenInstance = nullptr;

namespace ChilliSource
{
	namespace iOS
	{
		TouchScreen::TouchScreen() 
		{
			gpTouchScreenInstance = this;
            
            mfScale = Core::Screen::GetDensity();
		}
		TouchScreen::~TouchScreen()
		{
			gpTouchScreenInstance = nullptr;
		}
		bool TouchScreen::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::TouchScreen::InterfaceID;
		}
		void TouchScreen::OnTouchBegan(UITouch* inpNativeTouch)
		{
			//Grab the touch location from the OS
			CGPoint TouchLocation = [inpNativeTouch locationInView:inpNativeTouch.view];
			u32 InternalEventID = StartTouch(Core::Vector2(TouchLocation.x * mfScale, TouchLocation.y * mfScale), mffLastTimeStamp);
			
			mMapNativeTouchesToID.insert(std::make_pair(inpNativeTouch,InternalEventID));
		}
		void TouchScreen::OnTouchMoved(UITouch* inpNativeTouch)
		{
			MapNativeTouchToID::iterator pKey =  mMapNativeTouchesToID.find(inpNativeTouch);
			
			if (pKey != mMapNativeTouchesToID.end()) 
			{
				CGPoint TouchLocation = [inpNativeTouch locationInView:inpNativeTouch.view];			
				MoveTouch(pKey->second, Core::Vector2(TouchLocation.x * mfScale, TouchLocation.y * mfScale), mffLastTimeStamp);
			}
		}
		void TouchScreen::OnTouchEnded(UITouch* inpNativeTouch)
		{
			MapNativeTouchToID::iterator pKey =  mMapNativeTouchesToID.find(inpNativeTouch);
			
			if (pKey != mMapNativeTouchesToID.end()) 
			{
				EndTouch(pKey->second, mffLastTimeStamp);
				mMapNativeTouchesToID.erase(pKey);
			}
		}
	}
}

extern "C" {
	void TouchBegan(UITouch* inTouch)
	{
		gpTouchScreenInstance->OnTouchBegan(inTouch);
	}
	void TouchMoved(UITouch* inTouch)
	{
		gpTouchScreenInstance->OnTouchMoved(inTouch);
	}
	void TouchEnded(UITouch* inTouch)
	{
		gpTouchScreenInstance->OnTouchEnded(inTouch);
	}
};
