/*
 *  TouchScreen.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 05/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Input/Pointer/TouchScreen.h>
#include <ChilliSource/Core/Main/Screen.h>

moFlo::iOSPlatform::CTouchScreen* gpTouchScreenInstance = NULL;

namespace moFlo
{
	namespace iOSPlatform
	{
		CTouchScreen::CTouchScreen() 
		{
			gpTouchScreenInstance = this;
            
            mfScale = Core::CScreen::GetDensity();
		}
		CTouchScreen::~CTouchScreen()
		{
			gpTouchScreenInstance = NULL;
		}
		bool CTouchScreen::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::ITouchScreen::InterfaceID;
		}
		void CTouchScreen::OnTouchBegan(UITouch* inpNativeTouch)
		{
			//Grab the touch location from the OS
			CGPoint TouchLocation = [inpNativeTouch locationInView:inpNativeTouch.view];
			u32 InternalEventID = StartTouch(Core::CVector2(TouchLocation.x * mfScale, TouchLocation.y * mfScale), mffLastTimeStamp);
			
			mMapNativeTouchesToID.insert(std::make_pair(inpNativeTouch,InternalEventID));
		}
		void CTouchScreen::OnTouchMoved(UITouch* inpNativeTouch)
		{
			MapNativeTouchToID::iterator pKey =  mMapNativeTouchesToID.find(inpNativeTouch);
			
			if (pKey != mMapNativeTouchesToID.end()) 
			{
				CGPoint TouchLocation = [inpNativeTouch locationInView:inpNativeTouch.view];			
				MoveTouch(pKey->second, Core::CVector2(TouchLocation.x * mfScale, TouchLocation.y * mfScale), mffLastTimeStamp);
			}
		}
		void CTouchScreen::OnTouchEnded(UITouch* inpNativeTouch)
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
