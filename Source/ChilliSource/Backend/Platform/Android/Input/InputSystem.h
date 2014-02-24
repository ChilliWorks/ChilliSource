/** 
 * File: InputSystem.h
 * Date: 5 Oct 2010
 * Description: Concrete iOS3_x implementation of moflo::Input::IInputSystem;
 */

/** 
 *
 * Author Stuart / Ian Copland
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PLATFORM_ANDROID_INPUTSYSTEM_H_
#define _MOFLO_PLATFORM_ANDROID_INPUTSYSTEM_H_

#include <ChilliSource/Input/Main/InputSystem.h>

#include <ChilliSource/Platform/Android/Input/TouchScreen.h>
#include <ChilliSource/Platform/Android/Input/Accelerometer.h>
#include <ChilliSource/Platform/Android/Input/VirtualKeyboard.h>

namespace ChilliSource
{
	namespace Android
	{
		class CInputSystem : public Input::IInputSystem
		{
		public:
			CInputSystem();
			virtual ~CInputSystem();
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;
			
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const;
			virtual Input::IInputDevice * GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID);
			
			
			virtual Input::ITouchScreen * GetTouchScreenPtr();
			virtual Input::IAccelerometer * GetAccelerometerPtr();
			virtual void Update(float infDT);
			
			virtual Input::IVirtualKeyboard* GetVirtualKeyboardPtr();
			virtual void FlushBufferedInput();
		protected:
			CTouchScreen mTouchScreen;
			CAccelerometer mAccelerometer;
			CVirtualKeyboard mVirtualKeyboard;
		};
	}
}

#endif
