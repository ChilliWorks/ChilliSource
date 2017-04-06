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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_PLATFORM_RPI_INPUT_GAMEPAD_GAMEPADSYSTEM_H_
#define _CSBACKEND_PLATFORM_RPI_INPUT_GAMEPAD_GAMEPADSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <ChilliSource/Input/Gamepad/GamepadSystem.h>

#include <vector>

struct libevdev;
struct input_event;

namespace CSBackend
{
	namespace RPi
	{
		/// The Raspberry Pi backend for the gamepad system. TODO:
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

			///
			void OnInit() noexcept override;

			/// Checks for gamepad connections and disconnections and
			/// updates attached gamepad states
			///
			/// @param timeSinceLastUpdate
			///		Time in seconds since last update
			///
			void OnUpdate(f32 timeSinceLastUpdate) noexcept override;

			/// On Linux gamepads are handled as files. We check for the addition of a new file to check whether a controller has been connected.
			/// Connected controller files are added to the internal list and checked every update for input.
			///
			void CheckForNewGamepadConnections() noexcept;

			/// Process the axis moved event forwarding it to the main CS application
			///
			/// @param ev
			///		libevdev input event of type EV_ABS
			/// @param gamepadId
			///		Holds the uid of the gamepad, used for communicating with CS
			///
			void ProcessAxisEvent(const input_event& ev, ChilliSource::Gamepad::Id gamepadId) noexcept;

			/// Process the button pressure changed event forwarding it to the main CS application
			///
			/// @param ev
			///		libevdev input event of type EV_KEY
			/// @param gamepadId
			///		Holds the uid of the gamepad, used for communicating with CS
			///
			void ProcessButtonEvent(const input_event& ev, ChilliSource::Gamepad::Id gamepadId) noexcept;

			/// Close any open file connections to input files
			///
			void OnDestroy() noexcept override;

			/// Clusters the unique id of the gamepad, with the file it is streaming from
			/// and the event handler that is polling the file
			///
			struct GamepadData
			{
				ChilliSource::Gamepad::Id m_uid = 0;
				libevdev* m_dev = nullptr;
				int m_fileDescriptor = 0;
			};

			GamepadData m_gamepadConnections[ChilliSource::Gamepad::k_maxGamepads];
		};
	}
}

#endif

#endif
