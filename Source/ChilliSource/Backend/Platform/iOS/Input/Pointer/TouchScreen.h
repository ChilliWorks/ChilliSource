/** 
 * File: TouchScreen.h
 * Date: 5 Oct 2010
 * Description: Concrete iOS3_x implementation of ChilliSource::Input::TouchScreen;
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PLATFORM_IOS_TOUCHSCREEN_H_
#define _MOFLO_PLATFORM_IOS_TOUCHSCREEN_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Input/Pointer/TouchScreen.h>

#include <UIKit/UIKit.h>

namespace ChilliSource
{
	namespace iOS
	{
		class TouchScreen : public Input::TouchScreen
		{
		public:
			TouchScreen();
			~TouchScreen();
			
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			
			void OnTouchBegan(UITouch* inpNativeTouch);
			void OnTouchMoved(UITouch* inpNativeTouch);
			void OnTouchEnded(UITouch* inpNativeTouch);
            
		protected:
			
			typedef std::unordered_map<UITouch*, size_t> MapNativeTouchToID;
			MapNativeTouchToID mMapNativeTouchesToID;
            
            f32 mfScale;
		};
	}
}

extern "C" 
{
	void TouchBegan(UITouch* inTouch);
	void TouchMoved(UITouch* inTouch);
	void TouchEnded(UITouch* inTouch);
};
	
#endif
