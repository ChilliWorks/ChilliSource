/*
 *  TouchScreen.cpp
 *  FatherTed
 *
 *  Created by Tag Games on 05/10/2010.
 *  Updated for Android use by Ian Copland on 23/03/2011
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchScreen.h>

#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchInputJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		TouchScreen::TouchScreen()
		{
			ChilliSource::Android::TouchInputJavaInterface::RegisterTouchScreen(this);
		}
		TouchScreen::~TouchScreen()
		{
			ChilliSource::Android::TouchInputJavaInterface::RegisterTouchScreen(nullptr);
		}
		bool TouchScreen::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::TouchScreen::InterfaceID;
		}
		//---------------------------------------------------------------------
		/// On Touch Began
		//---------------------------------------------------------------------
		void TouchScreen::OnTouchBegan(s32 indwID, f32 infX, f32 infY)
		{
			//Grab the touch location from the OS
			size_t InternalEventID = StartTouch(Core::Vector2(infX, infY), mffLastTimeStamp);
			mMapNativeTouchesToID.insert(std::make_pair(indwID,InternalEventID));
		}
		//---------------------------------------------------------------------
		/// On Touch Moved
		//---------------------------------------------------------------------
		void TouchScreen::OnTouchMoved(s32 indwID, f32 infX, f32 infY)
		{
			MapNativeTouchToID::iterator pKey =  mMapNativeTouchesToID.find(indwID);

			if (pKey != mMapNativeTouchesToID.end())
			{
				MoveTouch(pKey->second, Core::Vector2(infX, infY), mffLastTimeStamp);
			}
		}
		//---------------------------------------------------------------------
		/// On Touch Ended
		//---------------------------------------------------------------------
		void TouchScreen::OnTouchEnded(s32 indwID, f32 infX, f32 infY)
		{
			MapNativeTouchToID::iterator pKey =  mMapNativeTouchesToID.find(indwID);

			if (pKey != mMapNativeTouchesToID.end())
			{
				EndTouch(pKey->second, mffLastTimeStamp);
				mMapNativeTouchesToID.erase(pKey);
			}
		}
	}
}
