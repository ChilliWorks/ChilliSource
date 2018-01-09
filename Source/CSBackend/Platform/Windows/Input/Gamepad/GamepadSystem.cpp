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

#include <CSBackend/Platform/Windows/Input/Gamepad/GamepadSystem.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Input/Gamepad/GamepadAxis.h>

#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>

namespace CSBackend
{
	namespace Windows
	{
		CS_DEFINE_NAMEDTYPE(GamepadSystem);

        //------------------------------------------------------------------------------
		bool GamepadSystem::IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept
		{
			return (ChilliSource::GamepadSystem::InterfaceID == interfaceId || GamepadSystem::InterfaceID == interfaceId);
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnInit() noexcept
		{
			SFMLWindow::Get()->SetJoystickDelegates(ChilliSource::MakeDelegate(this, &GamepadSystem::OnJoystickConnected), ChilliSource::MakeDelegate(this, &GamepadSystem::OnJoystickDisconnected),
													ChilliSource::MakeDelegate(this, &GamepadSystem::OnButtonPressed), ChilliSource::MakeDelegate(this, &GamepadSystem::OnButtonReleased),
													ChilliSource::MakeDelegate(this, &GamepadSystem::OnAxisMoved));

			sf::Joystick::update();
			//SFML doesn't notify us about already connected joysticks.
			for(u32 i=0; i<ChilliSource::Gamepad::k_maxGamepads; ++i)
			{
				if(sf::Joystick::isConnected(i))
				{
					OnJoystickConnected(i);
				}
			}
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnJoystickConnected(u32 index) noexcept
		{
			CS_LOG_VERBOSE_FMT("Gamepad Connected: %s\n", sf::Joystick::getIdentification(index).name.toAnsiString().c_str());

			u32 supportedAxisFlags = 0;
			for(u32 i=0; i<=(u32)sf::Joystick::Axis::PovY; ++i)
			{
				if(sf::Joystick::hasAxis(index, (sf::Joystick::Axis)i))
				{
					supportedAxisFlags |= (1u << i);
				}
			}
			m_mapSFMLToCSIds[index] = AddGamepadCreateEvent(sf::Joystick::getIdentification(index).name.toAnsiString(), sf::Joystick::getButtonCount(index), supportedAxisFlags);
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnJoystickDisconnected(u32 index) noexcept
		{
			AddGamepadRemoveEvent(m_mapSFMLToCSIds[index]);
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnButtonPressed(u32 joystickIndex, u32 buttonIndex) noexcept
		{
			AddButtonPressureChangedEvent(m_mapSFMLToCSIds[joystickIndex], buttonIndex, 1.0f);
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnButtonReleased(u32 joystickIndex, u32 buttonIndex) noexcept
		{
			AddButtonPressureChangedEvent(m_mapSFMLToCSIds[joystickIndex], buttonIndex, 0.0f);
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnAxisMoved(u32 joystickIndex, sf::Joystick::Axis axisId, f32 pos) noexcept
		{
			AddAxisPositionChangedEvent(m_mapSFMLToCSIds[joystickIndex], (ChilliSource::GamepadAxis)axisId, pos * 0.01f);
		}

		//------------------------------------------------------------------------------
		void GamepadSystem::OnDestroy() noexcept
		{
            SFMLWindow::Get()->RemoveJoystickDelegates();
		}
	}
}

#endif
