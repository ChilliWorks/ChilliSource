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

#include <CSBackend/Platform/RPi/Input/Gamepad/GamepadSystem.h>

#include <fcntl.h>
#include <unistd.h>
#include <libevdev/libevdev.h>

#include <array>

namespace CSBackend
{
	namespace RPi
	{
		CS_DEFINE_NAMEDTYPE(GamepadSystem);

		namespace
		{
			const std::string k_inputFileDirectory = "/dev/input/by-path/";
			const std::array<int, 8> k_axes = {{ABS_X, ABS_Y, ABS_THROTTLE, ABS_RUDDER, ABS_RX, ABS_RY, ABS_HAT0X, ABS_HAT0Y}}; //X, Y, Z, R, U, V, PovX, PovY
			const u32 k_maxButtons = 32;
		}

        //------------------------------------------------------------------------------
		bool GamepadSystem::IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept
		{
			return (ChilliSource::GamepadSystem::InterfaceID == interfaceId || GamepadSystem::InterfaceID == interfaceId);
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnInit() noexcept
		{
			std::fill(std::begin(m_gamepadConnections), std::end(m_gamepadConnections), GamepadData());
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnUpdate(f32 timeSinceLastUpdate) noexcept
		{
			if(m_gamepadConnections[0].m_dev == nullptr)
			{
				CheckForNewGamepadConnections();
			}

			for(u32 i=0; i<ChilliSource::Gamepad::k_maxGamepads; ++i)
			{
				if(m_gamepadConnections[i].m_dev == nullptr)
					continue;

				int eventResult = LIBEVDEV_READ_STATUS_SYNC;
				do
				{
					input_event ev;
					eventResult = libevdev_next_event(m_gamepadConnections[i].m_dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);

					if(eventResult == LIBEVDEV_READ_STATUS_SUCCESS)
					{
						printf("Event: %s %s %d\n",libevdev_event_type_get_name(ev.type),libevdev_event_code_get_name(ev.type, ev.code),ev.value);
						switch(ev.type)
						{
							case EV_ABS:
								ProcessAxisEvent(ev, m_gamepadConnections[i].m_uid);
								break;
							case EV_KEY:
								ProcessButtonEvent(ev, m_gamepadConnections[i].m_uid);
								break;
							default:
								break;
						}
					}
					else if(eventResult == -ENODEV)
					{
						//We have lost connection to the device.
						AddGamepadRemoveEvent(m_gamepadConnections[i].m_uid);
						m_gamepadConnections[i] = GamepadData();
					}
				}
				while(eventResult == LIBEVDEV_READ_STATUS_SYNC || eventResult == LIBEVDEV_READ_STATUS_SUCCESS);
			}
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::CheckForNewGamepadConnections() noexcept
		{
			s32 freeIndex = -1;
			for(s32 i=0; i<(s32)ChilliSource::Gamepad::k_maxGamepads; ++i)
			{
				if(m_gamepadConnections[i].m_dev == nullptr)
				{
					freeIndex = i;
					break;
				}
			}

			//Too many gamepads
			if(freeIndex == -1)
			{
				return;
			}

			//Check the /dev/input/by-path directory for joypad event files. Each file
			//corresponds to a gamepad.

			//Check for new files that we haven't seen before
			std::string filename = "platform-3f980000.usb-usb-0:1.2:1.0-event-joystick";
			auto fileDescriptor = open((k_inputFileDirectory + filename).c_str(), O_RDONLY|O_NONBLOCK);

			struct libevdev* dev = nullptr;
			auto result = libevdev_new_from_fd(fileDescriptor, &dev);
			if(result < 0)
			{
				CS_LOG_ERROR_FMT("Failed to init gamepad libevdev: %d\n", result);
				if(fileDescriptor != -1)
				{
					close(fileDescriptor);
				}
				return;
			}

			//Read the configuration of the device
			std::string deviceName = libevdev_get_name(dev);

			u32 numButtons = 0;
			for(s32 i=BTN_TRIGGER; i<BTN_TRIGGER+k_maxButtons; ++i)
			{
				if(libevdev_has_event_code(dev, EV_KEY, i))
				{
					++numButtons;
				}
			}

			u32 supportedAxisFlags = 0;
			for(std::size_t i=0; i<=k_axes.size(); ++i)
			{
				if(libevdev_has_event_code(dev, EV_ABS, k_axes[i]))
				{
					supportedAxisFlags |= (1u << i);
				}
			}

			GamepadData data;
			data.m_uid = AddGamepadCreateEvent(deviceName, numButtons, supportedAxisFlags);
			data.m_fileDescriptor = fileDescriptor;
			data.m_dev = dev;
			m_gamepadConnections[freeIndex] = std::move(data);
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::ProcessAxisEvent(const input_event& ev, ChilliSource::Gamepad::Id gamepadId) noexcept
		{
			//Values are reported between 0 : 255
			s32 axis = -1;
			switch(ev.code)
			{
				case ABS_X:
					axis = (s32)ChilliSource::GamepadAxis::k_x;
					break;
				case ABS_Y:
					axis = (s32)ChilliSource::GamepadAxis::k_y;
					break;
				case ABS_THROTTLE:
					axis = (s32)ChilliSource::GamepadAxis::k_z;
					break;
				case ABS_RUDDER:
					axis = (s32)ChilliSource::GamepadAxis::k_r;
					break;
				case ABS_RX:
					axis = (s32)ChilliSource::GamepadAxis::k_u;
					break;
				case ABS_RY:
					axis = (s32)ChilliSource::GamepadAxis::k_v;
					break;
				case ABS_HAT0X:
					axis = (s32)ChilliSource::GamepadAxis::k_povX;
					break;
				case ABS_HAT0Y:
					axis = (s32)ChilliSource::GamepadAxis::k_povY;
					break;
			}

			if(axis >= 0)
			{
				f32 halfRange = (255.0f * 0.5f);
				f32 normalised = ((f32)ev.value - halfRange)/halfRange;
				AddAxisPositionChangedEvent(gamepadId, (ChilliSource::GamepadAxis)axis, normalised);
			}
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::ProcessButtonEvent(const input_event& ev, ChilliSource::Gamepad::Id gamepadId) noexcept
		{
			u32 buttonIndex = ev.code - BTN_TRIGGER;
			if(buttonIndex < k_maxButtons)
			{
				AddButtonPressureChangedEvent(gamepadId, buttonIndex, (f32)ev.value);
			}
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnDestroy() noexcept
		{
			for(auto gamepadData : m_gamepadConnections)
			{
				if(gamepadData.m_dev == nullptr)
					continue;

				libevdev_free(gamepadData.m_dev);
				close(gamepadData.m_fileDescriptor);
			}
		}
	}
}

#endif
