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

#include <array>

struct libevdev;
struct input_event;
struct udev;
struct udev_monitor;
struct udev_device;

namespace CSBackend
{
	namespace RPi
	{
		/// The Raspberry Pi backend for the gamepad system.
		/// Uses libudev to check for new devices and libevdev to
		/// query the input files for changes to buttons and axes
        ///
		class GamepadSystem final : public ChilliSource::GamepadSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(GamepadSystem);

            ///
            bool IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept override;


		private:
			friend class ChilliSource::GamepadSystem;

			static const u32 k_maxButtons = 32;

			/// Clusters the unique id of the gamepad, with the file it is streaming from
			/// and the event handler that is polling the file
			///
			struct GamepadData
			{
				std::array<s32, k_maxButtons> m_buttonIndexMappings;
				std::array<f32, (u32)ChilliSource::GamepadAxis::k_total> m_axisMinValues;
				std::array<f32, (u32)ChilliSource::GamepadAxis::k_total> m_axisMaxValues;
				ChilliSource::Gamepad::Id m_uid = 0;
				libevdev* m_dev = nullptr;
				int m_fileDescriptor = 0;
			};

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

			/// On Linux gamepads are handled as files. We check for existing joypad event files to grab the file descriptor
			/// we then store this and use evdev to poll for state changes every update
			///
			void CheckForExistingGamepadConnections() noexcept;

			/// On Linux gamepads are handled as files. We use udev to check for new devices and to grab the file descriptor
			/// we then store this and use evdev to poll for state changes every update
			///
			void CheckForNewGamepadConnections() noexcept;

			/// Attempt to add the given device (must not be nullptr) as a gamepad if it meets the gamepad
			/// criteria.
			///
			/// @param device
			///		Device to check and add if gamepad
			///
			/// @return TRUE if added
			///
			bool TryAddGamepadDevice(udev_device* device) noexcept;

			/// Process the axis moved event forwarding it to the main CS application
			///
			/// @param ev
			///		libevdev input event of type EV_ABS
			/// @param gamepadData
			///		Holds the uid of the gamepad, used for communicating with CS and the max and min for axis conversion
			/// @param device
			///		Device that received the vent used to perform axis normalisation
			///
			void ProcessAxisEvent(const input_event& ev, const GamepadData& gamepadData) noexcept;

			/// Process the button pressure changed event forwarding it to the main CS application
			///
			/// @param ev
			///		libevdev input event of type EV_KEY
			/// @param gamepadData
			///		Holds the uid of the gamepad, used for communicating with CS and the button mapping to CS index
			///
			void ProcessButtonEvent(const input_event& ev, const GamepadData& gamepadData) noexcept;

			/// Close any open file connections to input files
			///
			void OnDestroy() noexcept override;

			std::array<GamepadData, ChilliSource::Gamepad::k_maxGamepads> m_gamepadConnections;

			udev* m_udev;
			udev_monitor* m_udevMonitor;
			int m_monitorFd = -1;
		};
	}
}

#endif

#endif
