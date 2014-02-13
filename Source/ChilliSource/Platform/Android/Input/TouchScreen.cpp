/*
 *  TouchScreen.cpp
 *  FatherTed
 *
 *  Created by Tag Games on 05/10/2010.
 *  Updated for Android use by Ian Copland on 23/03/2011
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Platform/Android/Input/TouchScreen.h>
#include <moFlo/Platform/Android/JavaInterface/TouchInputJavaInterface.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		CTouchScreen::CTouchScreen()
		{
			moFlo::AndroidPlatform::SCTouchInputJavaInterface::RegisterTouchScreen(this);
		}
		CTouchScreen::~CTouchScreen()
		{
			moFlo::AndroidPlatform::SCTouchInputJavaInterface::RegisterTouchScreen(NULL);
		}
		bool CTouchScreen::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::ITouchScreen::InterfaceID;
		}
		//---------------------------------------------------------------------
		/// On Touch Began
		//---------------------------------------------------------------------
		void CTouchScreen::OnTouchBegan(s32 indwID, f32 infX, f32 infY)
		{
			//Grab the touch location from the OS
			size_t InternalEventID = StartTouch(Core::CVector2(infX, infY), mffLastTimeStamp);
			mMapNativeTouchesToID.insert(std::make_pair(indwID,InternalEventID));
		}
		//---------------------------------------------------------------------
		/// On Touch Moved
		//---------------------------------------------------------------------
		void CTouchScreen::OnTouchMoved(s32 indwID, f32 infX, f32 infY)
		{
			MapNativeTouchToID::iterator pKey =  mMapNativeTouchesToID.find(indwID);

			if (pKey != mMapNativeTouchesToID.end())
			{
				MoveTouch(pKey->second, Core::CVector2(infX, infY), mffLastTimeStamp);
			}
		}
		//---------------------------------------------------------------------
		/// On Touch Ended
		//---------------------------------------------------------------------
		void CTouchScreen::OnTouchEnded(s32 indwID, f32 infX, f32 infY)
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
