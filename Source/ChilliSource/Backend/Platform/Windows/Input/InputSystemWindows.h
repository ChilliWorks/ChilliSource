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

#include <ChilliSource/Input/Base/InputSystem.h>

#include <ChilliSource/Backend/Platform/Windows/Input/MouseWindows.h>
#include <ChilliSource/Backend/Platform/Windows/Input/TouchscreenWindows.h>

namespace ChilliSource
{
	namespace Windows
	{
		class CInputSystem : public Input::InputSystem
		{
		public:
			CInputSystem();
			virtual ~CInputSystem();
			//---------------------------------------------------------------
			/// Is A
			///
			/// @param Interface type to compare
			/// @return Whether the system is of the given type
			//---------------------------------------------------------------
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//---------------------------------------------------------------
			/// Can Create Device With Interface
			///
			/// @param Interface of input type to create
			/// @return Whether the system can create input type
			//---------------------------------------------------------------
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const;
			//---------------------------------------------------------------
			/// Get Device With Interface
			///
			/// @param Interface of input type to retreive
			/// @return Pointer to input type or NULL
			//---------------------------------------------------------------
			virtual Input::InputDevice * GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID);
			//---------------------------------------------------------------
			/// Update
			///
			/// @param Time between frames
			//---------------------------------------------------------------
			virtual void Update(float infDT);
			//-----------------------------------------------------------
			/// Flush Buffered Input
			///
			/// Have the input elements notify listeners of each
			/// buffered value then clear the buffered input
			//-----------------------------------------------------------
			void FlushBufferedInput();
			//---------------------------------------------------------------
			/// Get Mouse Pointer
			///
			/// @return Pointer to mouse device
			//---------------------------------------------------------------
			Input::Mouse * GetMousePtr();
			//---------------------------------------------------------------
			/// Get Touch Screen Pointer
			///
			/// @return Pointer to touch screen device
			//---------------------------------------------------------------
			Input::TouchScreen * GetTouchScreenPtr();

		protected:

			CTouchScreen mTouchScreen;
			CMouse mMouse;
		};
	}
}

#endif
