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
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/iOS/Input/Accelerometer/Accelerometer.h>
#include <ChilliSource/Backend/Platform/iOS/Input/Pointer/TouchScreen.h>
#include <ChilliSource/Input/Base/InputSystem.h>

#include <UIKit/UIKit.h>

namespace ChilliSource
{
	namespace iOS
	{
		class InputSystem : public Input::InputSystem
		{
		public:
			virtual ~InputSystem();
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const override;
			virtual Input::InputDevice * GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID) override;
			
			
			virtual Input::TouchScreen * GetTouchScreen() override;
			virtual void Update(f32 infDT) override;
            
            //-----------------------------------------------------------
            /// Flush Buffered Input
            ///
            /// Have the input elements notify listeners of each
            /// buffered value then clear the buffered input
            //-----------------------------------------------------------
            void FlushBufferedInput() override;
            
        private:
            
            friend Input::InputSystemUPtr Input::InputSystem::Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            InputSystem();
			
		protected:
			TouchScreen mTouchScreen;
		};
	}
}

#endif
