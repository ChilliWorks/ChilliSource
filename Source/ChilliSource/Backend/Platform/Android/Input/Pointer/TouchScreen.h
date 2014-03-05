/** 
 * File: TouchScreen.h
 * Date: 5 Oct 2010
 * Description: Concrete Android implementation of ChilliSource::Input::ITouchScreen;
 */

/** 
 *
 * Author Stuart / Ian Copland
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PLATFORM_ANDROID_TOUCHSCREEN_H_
#define _MOFLO_PLATFORM_ANDROID_TOUCHSCREEN_H_

#include <ChilliSource/Input/Pointer/TouchScreen.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Android
	{
		class TouchScreen : public Input::TouchScreen
		{
		public:
			TouchScreen();
			~TouchScreen();
			
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;

			//---------------------------------------------------------------------
			/// On Touch Began
			///
			/// registers when a touch is first pressed down.
			//---------------------------------------------------------------------
			void OnTouchBegan(s32 indwID, f32 infX, f32 infY);
			//---------------------------------------------------------------------
			/// On Touch Moved
			///
			///registers when a touch is moved
			//---------------------------------------------------------------------
			void OnTouchMoved(s32 indwID, f32 infX, f32 infY);
			//---------------------------------------------------------------------
			/// On Touch Ended
			///
			/// registers when a touch ends.
			//---------------------------------------------------------------------
			void OnTouchEnded(s32 indwID, f32 infX, f32 infY);

		private:

			typedef std::unordered_map<s32, size_t> MapNativeTouchToID;
			MapNativeTouchToID mMapNativeTouchesToID;
		};
	}
}

#endif
