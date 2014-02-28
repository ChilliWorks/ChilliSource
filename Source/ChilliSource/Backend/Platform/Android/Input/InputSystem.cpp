/*
 *  InputSystem.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 23/03/2011
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Input/InputSystem.h>

#include <ChilliSource/Core/Time/Timer.h>
#include <ChilliSource/Core/Base/Application.h>

using namespace ChilliSource::Input;
using namespace ChilliSource::Core;
using namespace ChilliSource::Android;

bool CInputSystem::IsA(Core::InterfaceIDType inInterfaceID) const
{
	return inInterfaceID == InputSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
}

bool CInputSystem::CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const
{
	return inInterfaceID == TouchScreen::InterfaceID;
}
InputDevice* CInputSystem::GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID)
{
	if (inInterfaceID == TouchScreen::InterfaceID)
	{
		return &mTouchScreen;
	}
	
	return NULL;
}
Accelerometer* CInputSystem::GetAccelerometerPtr()
{
	return &mAccelerometer;
}

TouchScreen* CInputSystem::GetTouchScreenPtr()
{
	return &mTouchScreen;
}
void CInputSystem::Update(float infDT)
{
	mffTimeStamp += infDT;
	mTouchScreen.SetCurrentAppTime(mffTimeStamp);
}
VirtualKeyboard* CInputSystem::GetVirtualKeyboardPtr()
{
	return &mVirtualKeyboard;
}
//-----------------------------------------------------------
/// Flush Buffered Input
///
/// Have the input elements notify listeners of each
/// buffered value then clear the buffered input
//-----------------------------------------------------------
void CInputSystem::FlushBufferedInput()
{
    mTouchScreen.FlushBufferedInput();
}
