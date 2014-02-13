/** 
 * File: InputSystem.h
 * Date: 5 Oct 2010
 * Description: Concrete iOS3_x implementation of moflo::Input::IInputSystem;
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PLATFORM_IOS3X_INPUTSYSTEM_H_
#define _MOFLO_PLATFORM_IOS3X_INPUTSYSTEM_H_

#include <UIKit/UIKit.h>

#include <moFlo/Input/InputSystem.h>

#include <moFlo/Platform/iOS/Input/TouchScreen.h>
#include <moFlo/Platform/iOS/Input/Accelerometer.h>
#include <moFlo/Platform/iOS/Input/VirtualKeyboard.h>

namespace moFlo
{
	namespace iOSPlatform
	{
		class CInputSystem : public Input::IInputSystem
		{
		public:
			virtual ~CInputSystem();
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;
			
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const;
			virtual Input::IInputDevice * GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID);
			
			
			virtual Input::ITouchScreen * GetTouchScreenPtr();
			virtual Input::IAccelerometer * GetAccelerometerPtr();
			virtual void Update(f32 infDT);
			
			virtual Input::IVirtualKeyboard* GetVirtualKeyboardPtr();
            
            //-----------------------------------------------------------
            /// Flush Buffered Input
            ///
            /// Have the input elements notify listeners of each
            /// buffered value then clear the buffered input
            //-----------------------------------------------------------
            void FlushBufferedInput();
			
		protected:
			CTouchScreen mTouchScreen;
			CAccelerometer mAccelerometer;
			CVirtualKeyboard mVirtualKeyboard;
		};
	}
}

#endif
