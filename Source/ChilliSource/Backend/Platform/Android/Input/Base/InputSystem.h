/** 
 * File: InputSystem.h
 * Date: 5 Oct 2010
 * Description: Concrete iOS3_x implementation of ChilliSource::Input::IInputSystem;
 */

/** 
 *
 * Author Stuart / Ian Copland
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PLATFORM_ANDROID_INPUTSYSTEM_H_
#define _MOFLO_PLATFORM_ANDROID_INPUTSYSTEM_H_

#include <ChilliSource/Backend/Platform/Android/Input/Accelerometer/Accelerometer.h>
#include <ChilliSource/Backend/Platform/Android/Input/Keyboard/VirtualKeyboard.h>
#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchScreen.h>
#include <ChilliSource/Input/Base/InputSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		class CInputSystem : public Input::InputSystem
		{
		public:

			bool IsA(Core::InterfaceIDType inInterfaceID) const override;

			bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const override;
			Input::InputDevice* GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID) override;

			Input::TouchScreen* GetTouchScreenPtr() override;
			Input::Accelerometer* GetAccelerometerPtr() override;
			Input::VirtualKeyboard* GetVirtualKeyboardPtr() override;
			
			void Update(float infDT);
			
			void FlushBufferedInput() override;
			
		private:
			
			CTouchScreen mTouchScreen;
			CAccelerometer mAccelerometer;
			CVirtualKeyboard mVirtualKeyboard;
		};
	}
}

#endif
