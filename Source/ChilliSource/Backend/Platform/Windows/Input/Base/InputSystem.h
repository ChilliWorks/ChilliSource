/** 
* File: InputSystem.h
* Date: 5 Oct 2010
* Description: Concrete iOS3_x implementation of ChilliSource::Input::IInputSystem;
*/

/** 
*
* Author Stuart 
* Version 1.0 - moFlo
* Copyright ©2010 Tag Games Limited - All rights reserved 
*/

#ifndef _MOFLO_PLATFORM_WINDOWS_INPUTSYSTEM_H_
#define _MOFLO_PLATFORM_WINDOWS_INPUTSYSTEM_H_

#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Windows/Input/Pointer/Mouse.h>
#include <ChilliSource/Backend/Platform/Windows/Input/Pointer/Touchscreen.h>
#include <ChilliSource/Input/Base/InputSystem.h>

namespace ChilliSource
{
	namespace Windows
	{
		class InputSystem : public Input::InputSystem
		{
		public:
			InputSystem();
			virtual ~InputSystem();
			//---------------------------------------------------------------
			/// Is A
			///
			/// @param Interface type to compare
			/// @return Whether the system is of the given type
			//---------------------------------------------------------------
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//---------------------------------------------------------------
			/// Can Create Device With Interface
			///
			/// @param Interface of input type to create
			/// @return Whether the system can create input type
			//---------------------------------------------------------------
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const override;
			//---------------------------------------------------------------
			/// Get Device With Interface
			///
			/// @param Interface of input type to retreive
			/// @return Pointer to input type or nullptr
			//---------------------------------------------------------------
			virtual Input::InputDevice * GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID) override;
			//---------------------------------------------------------------
			/// Update
			///
			/// @param Time between frames
			//---------------------------------------------------------------
			virtual void Update(float infDT) override;
			//-----------------------------------------------------------
			/// Flush Buffered Input
			///
			/// Have the input elements notify listeners of each
			/// buffered value then clear the buffered input
			//-----------------------------------------------------------
			void FlushBufferedInput() override;
			//---------------------------------------------------------------
			/// Get Mouse Pointer
			///
			/// @return Pointer to mouse device
			//---------------------------------------------------------------
			Input::Mouse * GetMouse() override;
			//---------------------------------------------------------------
			/// Get Touch Screen Pointer
			///
			/// @return Pointer to touch screen device
			//---------------------------------------------------------------
			Input::TouchScreen * GetTouchScreen() override;

		protected:

			TouchScreen mTouchScreen;
			Mouse mMouse;
		};
	}
}

#endif
