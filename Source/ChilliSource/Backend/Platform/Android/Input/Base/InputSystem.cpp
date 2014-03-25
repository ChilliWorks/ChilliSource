/*
 *  InputSystem.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 23/03/2011
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Input/Base/InputSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Time/Timer.h>

using namespace ChilliSource::Input;
using namespace ChilliSource::Core;
using namespace ChilliSource::Android;

namespace ChilliSource
{
	namespace Android
	{
		bool InputSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::InputSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
		}

		bool InputSystem::CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == TouchScreen::InterfaceID;
		}
		InputDevice* InputSystem::GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID)
		{
			if (inInterfaceID == TouchScreen::InterfaceID)
			{
				return &mTouchScreen;
			}

			return nullptr;
		}
		Input::TouchScreen* InputSystem::GetTouchScreen()
		{
			return &mTouchScreen;
		}
		void InputSystem::Update(float infDT)
		{
			m_timeStamp += infDT;
			mTouchScreen.SetCurrentAppTime(m_timeStamp);
		}
		//-----------------------------------------------------------
		/// Flush Buffered Input
		///
		/// Have the input elements notify listeners of each
		/// buffered value then clear the buffered input
		//-----------------------------------------------------------
		void InputSystem::FlushBufferedInput()
		{
			mTouchScreen.FlushBufferedInput();
		}
	}
}
