/*
 *  InputSystem.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 05/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Input/Base/InputSystem.h>

#include <ChilliSource/Core/Time/Timer.h>
#include <ChilliSource/Core/Base/Application.h>

using namespace moFlo::Input;
using namespace moFlo::Core;
using namespace moFlo::iOSPlatform;


CInputSystem::~CInputSystem()
{
	
}

bool CInputSystem::IsA(Core::InterfaceIDType inInterfaceID) const
{
	return inInterfaceID == IInputSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
}

bool CInputSystem::CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const
{
	return inInterfaceID == ITouchScreen::InterfaceID;
}
IInputDevice * CInputSystem::GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID)
{
	if(inInterfaceID == ITouchScreen::InterfaceID) 
    {
		return &mTouchScreen;
	}
    
	return NULL;
}
IAccelerometer * CInputSystem::GetAccelerometerPtr()
{
	return &mAccelerometer;
}

ITouchScreen * CInputSystem::GetTouchScreenPtr()
{
	return &mTouchScreen;
}
void CInputSystem::Update(f32 infDT)
{
    mffTimeStamp += infDT;
	mTouchScreen.SetCurrentAppTime(mffTimeStamp);
}
IVirtualKeyboard* CInputSystem::GetVirtualKeyboardPtr()
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
