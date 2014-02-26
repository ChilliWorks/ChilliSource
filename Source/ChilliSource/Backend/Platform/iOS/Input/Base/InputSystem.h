/** 
 * File: InputSystem.h
 * Date: 5 Oct 2010
 * Description: Concrete iOS3_x implementation of ChilliSource::Input::InputSystem;
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PLATFORM_IOS3X_INPUTSYSTEM_H_
#define _MOFLO_PLATFORM_IOS3X_INPUTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Input/Base/InputSystem.h>
#include <ChilliSource/Backend/Platform/iOS/Input/Pointer/TouchScreen.h>
#include <ChilliSource/Backend/Platform/iOS/Input/Accelerometer/Accelerometer.h>
#include <ChilliSource/Backend/Platform/iOS/Input/Keyboard/VirtualKeyboard.h>

#include <UIKit/UIKit.h>

namespace ChilliSource
{
	namespace iOS
	{
		class CInputSystem : public Input::InputSystem
		{
		public:
            CInputSystem();
			virtual ~CInputSystem();
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;
			
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const;
			virtual Input::InputDevice * GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID);
			
			
			virtual Input::TouchScreen * GetTouchScreenPtr();
			virtual Input::Accelerometer * GetAccelerometerPtr();
			virtual void Update(f32 infDT);
			
			virtual Input::VirtualKeyboard* GetVirtualKeyboardPtr();
            
            //-----------------------------------------------------------
            /// Flush Buffered Input
            ///
            /// Have the input elements notify listeners of each
            /// buffered value then clear the buffered input
            //-----------------------------------------------------------
            void FlushBufferedInput();
			
		protected:
			CTouchScreen mTouchScreen;
			CAccelerometer* mpAccelerometer;
			CVirtualKeyboard mVirtualKeyboard;
		};
	}
}

#endif
