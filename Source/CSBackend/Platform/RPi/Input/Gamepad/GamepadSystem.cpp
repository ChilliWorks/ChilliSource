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
#include <libudev.h>

#include <array>
#include <cstring>

namespace CSBackend
{
	namespace RPi
	{
		CS_DEFINE_NAMEDTYPE(GamepadSystem);

		namespace
		{
			const std::array<int, 8> k_axes = {{ABS_X, ABS_Y, ABS_THROTTLE, ABS_RUDDER, ABS_RX, ABS_RY, ABS_HAT0X, ABS_HAT0Y}}; //X, Y, Z, R, U, V, PovX, PovY
			const u32 k_maxButtons = 32;

			/// @param monitorFd
			///		Monitor to check
			///
			/// @return TRUE if there is a new monitoring event meaning a new device has been connected
			///
			bool HasUdevMonitorEvent(int monitorFd)
			{
				fd_set descriptorSet;
				FD_ZERO(&descriptorSet);
				FD_SET(monitorFd, &descriptorSet);
				timeval timeout = {0, 0};

				return (select(monitorFd + 1, &descriptorSet, NULL, NULL, &timeout) > 0) && FD_ISSET(monitorFd, &descriptorSet);
			}

			/// @param device
			///		Device to check
			///
			/// @return TRUE if device is a joystick
			///
			bool IsDeviceJoystick(udev_device* device)
			{
				const char* devnode = udev_device_get_devnode(device);
				return devnode != nullptr && std::strstr(devnode, "event") != nullptr && udev_device_get_property_value(device, "ID_INPUT_JOYSTICK");
			}
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

			m_udev = udev_new();
			CS_RELEASE_ASSERT(m_udev != nullptr, "Cannot create udev");

			m_udevMonitor = udev_monitor_new_from_netlink(m_udev, "udev");
			CS_RELEASE_ASSERT(m_udevMonitor != nullptr, "Cannot create udev monitor");

			auto result = udev_monitor_filter_add_match_subsystem_devtype(m_udevMonitor, "input", NULL);
			CS_RELEASE_ASSERT(result >= 0, "Failed to add udev monitor filter");

			//Start listening for new  device connections - NOTE: Monitoring needs to be enabled before enumeration according to the manual
			udev_monitor_enable_receiving(m_udevMonitor);
			m_monitorFd = udev_monitor_get_fd(m_udevMonitor);

			//Check for already connected devices
			CheckForExistingGamepadConnections();
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnUpdate(f32 timeSinceLastUpdate) noexcept
		{
			CheckForNewGamepadConnections();

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
						close(m_gamepadConnections[i].m_fileDescriptor);
						m_gamepadConnections[i] = GamepadData();
					}
				}
				while(eventResult == LIBEVDEV_READ_STATUS_SYNC || eventResult == LIBEVDEV_READ_STATUS_SUCCESS);
			}
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::CheckForExistingGamepadConnections() noexcept
		{
			udev_enumerate* enumerator = udev_enumerate_new(m_udev);
			CS_RELEASE_ASSERT(enumerator != nullptr, "Cannot create udev enumerator");
			auto result = udev_enumerate_add_match_subsystem(enumerator, "input");
			CS_RELEASE_ASSERT(result >= 0, "Failed to add filter to udev enumerator");
			result = udev_enumerate_scan_devices(enumerator);
			CS_RELEASE_ASSERT(result >= 0, "Failed to scan udev for devices");

			udev_list_entry* devices = udev_enumerate_get_list_entry(enumerator);
			udev_list_entry* deviceEntry;
			udev_list_entry_foreach(deviceEntry, devices)
			{
				const char* sysPath = udev_list_entry_get_name(deviceEntry);
				udev_device* device = udev_device_new_from_syspath(m_udev, sysPath);
				if(device != nullptr)
				{
					TryAddGamepadDevice(device);
					udev_device_unref(device);
				}
			}

			udev_enumerate_unref(enumerator);
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::CheckForNewGamepadConnections() noexcept
		{
			//Check for new events
			if(HasUdevMonitorEvent(m_monitorFd) == false)
				return;

			//Check if the device causing the event is a gamepad
			udev_device* device = udev_monitor_receive_device(m_udevMonitor);
			if(device == nullptr)
			{
				return;
			}

			//Check to make sure it is a connected event
			const char* action = udev_device_get_action(device);
			if(std::strcmp(action, "add") != 0)
			{
				udev_device_unref(device);
				return;
			}

			TryAddGamepadDevice(device);
			udev_device_unref(device);
		}

		//------------------------------------------------------------------------------
		bool GamepadSystem::TryAddGamepadDevice(udev_device* device) noexcept
		{
			if(IsDeviceJoystick(device) == false)
			{
				return false;
			}

			//Check if we can accomodate new gamepads
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
				return false;
			}

			//Get the file that the kernel writes gamepad events to and hand it to evdev to poll
			const char* devNode = udev_device_get_devnode(device);
			auto fileDescriptor = open(devNode, O_RDONLY|O_NONBLOCK);

			struct libevdev* dev = nullptr;
			auto result = libevdev_new_from_fd(fileDescriptor, &dev);
			if(result < 0)
			{
				CS_LOG_ERROR_FMT("Failed to init gamepad libevdev: %d\n", result);
				if(fileDescriptor != -1)
				{
					close(fileDescriptor);
				}
				return false;
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

			CS_LOG_VERBOSE_FMT("Gamepad Connected: %s\n", deviceName.c_str());

			GamepadData data;
			data.m_uid = AddGamepadCreateEvent(std::move(deviceName), numButtons, supportedAxisFlags);
			data.m_fileDescriptor = fileDescriptor;
			data.m_dev = dev;
			m_gamepadConnections[freeIndex] = std::move(data);

			return true;
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

			udev_monitor_unref(m_udevMonitor);
			udev_unref(m_udev);
		}
	}
}

#endif
