/*
*  InputSystem.cpp
*  FatherTed
*
*  Created by Tag Games on 05/10/2010.
*  Copyright 2010 Tag Games. All rights reserved.
*
*/

#include <ChilliSource/Backend/Platform/Windows/Input/Base/InputSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Time/Timer.h>

namespace ChilliSource
{
	namespace Windows
	{
		InputSystem::InputSystem() : mMouse(&mTouchScreen)
		{
		}
		InputSystem::~InputSystem()
		{

		}
		//---------------------------------------------------------------
		/// Is A
		///
		/// @param Interface type to compare
		/// @return Whether the system is of the given type
		//---------------------------------------------------------------
		bool InputSystem::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::InputSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
		}
		//---------------------------------------------------------------
		/// Can Create Device With Interface
		///
		/// @param Interface of input type to create
		/// @return Whether the system can create input type
		//---------------------------------------------------------------
		bool InputSystem::CanCreateDeviceWithInterface(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == TouchScreen::InterfaceID || inInterfaceID == Mouse::InterfaceID;
		}
		//---------------------------------------------------------------
		/// Get Device With Interface
		///
		/// @param Interface of input type to retreive
		/// @return Pointer to input type or nullptr
		//---------------------------------------------------------------
		Input::InputDevice * InputSystem::GetDeviceWithInterface(ChilliSource::Core::InterfaceIDType inInterfaceID)
		{
			if (inInterfaceID == TouchScreen::InterfaceID)
			{
				return &mTouchScreen;
			}
			else if (inInterfaceID == Mouse::InterfaceID)
			{
				return &mMouse;
			}

			return nullptr;
		}
		//---------------------------------------------------------------
		/// Get Touch Screen Pointer
		///
		/// @return Pointer to touch screen device
		//---------------------------------------------------------------
		Input::TouchScreen * InputSystem::GetTouchScreen()
		{
			return &mTouchScreen;
		}
		//---------------------------------------------------------------
		/// Get Mouse Pointer
		///
		/// @return Pointer to mouse device
		//---------------------------------------------------------------
		Input::Mouse * InputSystem::GetMouse()
		{
			return &mMouse;
		}
		//---------------------------------------------------------------
		/// Update
		///
		/// @param Time between frames
		//---------------------------------------------------------------
		void InputSystem::Update(f32 infDT)
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

