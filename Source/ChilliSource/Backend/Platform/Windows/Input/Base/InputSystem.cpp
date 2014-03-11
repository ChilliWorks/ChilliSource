//
//  InputSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 05/10/2010.
//  Copyright (c) 2010 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Windows/Input/Base/InputSystem.h>

namespace ChilliSource
{
	namespace Windows
	{
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		InputSystem::InputSystem() 
		: m_mouse(&m_touchScreen)
		{
		}
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		bool InputSystem::IsA(ChilliSource::Core::InterfaceIDType in_interfaceID) const
		{
			return in_interfaceID == Input::InputSystem::InterfaceID || in_interfaceID == IUpdateable::InterfaceID;
		}
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		bool InputSystem::CanCreateDeviceWithInterface(Core::InterfaceIDType in_interfaceID) const
		{
			return in_interfaceID == TouchScreen::InterfaceID || in_interfaceID == Mouse::InterfaceID;
		}
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		Input::InputDevice* InputSystem::GetDeviceWithInterface(ChilliSource::Core::InterfaceIDType in_interfaceID)
		{
			if (in_interfaceID == TouchScreen::InterfaceID)
			{
				return &m_touchScreen;
			}
			else if (in_interfaceID == Mouse::InterfaceID)
			{
				return &m_mouse;
			}

			return nullptr;
		}
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		Input::TouchScreen* InputSystem::GetTouchScreen()
		{
			return &m_touchScreen;
		}
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		Input::Mouse* InputSystem::GetMouse()
		{
			return &m_mouse;
		}
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		void InputSystem::Update(f32 in_timeSinceLastUpdate)
		{
			m_timeStamp += in_timeSinceLastUpdate;
			m_touchScreen.SetCurrentAppTime(m_timeStamp);
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void InputSystem::FlushBufferedInput()
		{
			m_touchScreen.FlushBufferedInput();
		}
	}
}

