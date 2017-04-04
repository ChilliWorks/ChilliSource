//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#ifndef _CSBACKEND_PLATFORM_WINDOWS_INPUT_GAMEPAD_GAMEPADSYSTEM_H_
#define _CSBACKEND_PLATFORM_WINDOWS_INPUT_GAMEPAD_GAMEPADSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Input/Gamepad/GamepadSystem.h>

#include <SFML/Window/Joystick.hpp>

#include <array>

namespace CSBackend
{
	namespace Windows
	{
		/// The Windows backend for the gamepad system. Uses SFML
        /// to query the state of connected gamepads and forwards the information
        /// to the application
        ///
		class GamepadSystem final : public ChilliSource::GamepadSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(GamepadSystem);

            ///
            bool IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept override;


		private:
			friend class ChilliSource::GamepadSystem;

            ///
			GamepadSystem() = default;

			/// Start listening to SFML events
			///
			void OnInit() noexcept override;

			/// Called by SFML when a joystick is attached
			///
			/// @param index
			///		Index of joystick
			///
			void OnJoystickConnected(u32 index) noexcept;

			/// Called by SFML when a joystick is removed
			///
			/// @param index
			///		Index of joystick
			///
			void OnJoystickDisconnected(u32 index) noexcept;

			/// Called by SFML when a button is pressed
			///
			/// @param joystickIndex
			///		Index of joystick
			/// @param buttonIndex
			///		Index of button
			///
			void OnButtonPressed(u32 joystickIndex, u32 buttonIndex) noexcept;

			/// Called by SFML when a button is released
			///
			/// @param joystickIndex
			///		Index of joystick
			/// @param buttonIndex
			///		Index of button
			///
			void OnButtonReleased(u32 joystickIndex, u32 buttonIndex) noexcept;

			/// Called by SFML when an axis is moved on a connected joystick
			///
			/// @param joystickIndex
			///		Index of joystick
			/// @param axisId
			///		Identifier of the axis. SFML splits all axis into single floats
			/// @param pos
			///		Position of the axis along the line of the axis
			///
			void OnAxisMoved(u32 joystickIndex, sf::Joystick::Axis axisId, f32 pos) noexcept;

			/// Stop listening for SFML events
			///
			void OnDestroy() noexcept override;

			//SFML allows 8 joysticks
			std::array<ChilliSource::Gamepad::Id, ChilliSource::Gamepad::k_maxGamepads> m_mapSFMLToCSIds;
		};
	}
}

#endif

#endif
