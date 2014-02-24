/** 
 * File: TouchScreen.h
 * Date: 5 Oct 2010
 * Description: Concrete Android implementation of moflo::Input::ITouchScreen;
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

namespace ChilliSource
{
	namespace Android
	{
		class CTouchScreen : public Input::ITouchScreen
		{
		public:
			CTouchScreen();
			~CTouchScreen();
			
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;

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
		protected:

			
			typedef HASH_MAP<s32, size_t> MapNativeTouchToID;
			MapNativeTouchToID mMapNativeTouchesToID;
		};
	}
}

#endif
