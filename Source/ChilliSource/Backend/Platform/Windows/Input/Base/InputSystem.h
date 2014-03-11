//
//  InputSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 05/10/2010.
//  Copyright (c) 2010 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_INPUT_BASE_INPUTSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_INPUT_BASE_INPUTSYSTEM_H_

#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Input/Base/InputSystem.h>
#include <ChilliSource/Backend/Platform/Windows/Input/Pointer/Mouse.h>
#include <ChilliSource/Backend/Platform/Windows/Input/Pointer/Touchscreen.h>

namespace ChilliSource
{
	namespace Windows
	{
		class InputSystem final : public Input::InputSystem
		{
		public:
			//---------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface type to compare
			///
			/// @return Whether the system is of the given type
			//---------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceID) const override;
			//---------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface of input type to create
			///
			/// @return Whether the system can create input type
			//---------------------------------------------------------------
			bool CanCreateDeviceWithInterface(Core::InterfaceIDType in_interfaceID) const override;
			//---------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface of input type to retreive
			///
			/// @return Pointer to input type or nullptr
			//---------------------------------------------------------------
			Input::InputDevice* GetDeviceWithInterface(Core::InterfaceIDType in_interfaceID) override;
			//---------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Time since last update in seconds
			//---------------------------------------------------------------
			void Update(f32 in_timeSinceLastUpdate) override;
			//-----------------------------------------------------------
			/// Any buffered input is flushed to application listeners
			///
			/// @author S Downie
			//-----------------------------------------------------------
			void FlushBufferedInput() override;
			//---------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Pointer to mouse device
			//---------------------------------------------------------------
			Input::Mouse* GetMouse() override;
			//---------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Pointer to touch screen device
			//---------------------------------------------------------------
			Input::TouchScreen* GetTouchScreen() override;

		private:
			friend Input::InputSystemUPtr Input::InputSystem::Create();
			//-------------------------------------------------------
			/// Private constructor to force use of factory method
			///
			/// @author S Downie
			//-------------------------------------------------------
			InputSystem();

			TouchScreen m_touchScreen;
			Mouse m_mouse;
		};
	}
}

#endif
