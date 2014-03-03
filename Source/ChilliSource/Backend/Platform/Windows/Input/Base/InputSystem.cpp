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

using namespace ChilliSource::Input;
using namespace ChilliSource::Core;
using namespace ChilliSource::Windows;

CInputSystem::CInputSystem() : mMouse(&mTouchScreen)
{
}
CInputSystem::~CInputSystem()
{

}
//---------------------------------------------------------------
/// Is A
///
/// @param Interface type to compare
/// @return Whether the system is of the given type
//---------------------------------------------------------------
bool CInputSystem::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
{
	return inInterfaceID == InputSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
}
//---------------------------------------------------------------
/// Can Create Device With Interface
///
/// @param Interface of input type to create
/// @return Whether the system can create input type
//---------------------------------------------------------------
bool CInputSystem::CanCreateDeviceWithInterface(ChilliSource::Core::InterfaceIDType inInterfaceID) const
{
	return inInterfaceID == TouchScreen::InterfaceID || inInterfaceID == Mouse::InterfaceID;
}
//---------------------------------------------------------------
/// Get Device With Interface
///
/// @param Interface of input type to retreive
/// @return Pointer to input type or NULL
//---------------------------------------------------------------
InputDevice * CInputSystem::GetDeviceWithInterface(ChilliSource::Core::InterfaceIDType inInterfaceID)
{
	if(inInterfaceID == TouchScreen::InterfaceID) 
	{
		return &mTouchScreen;
	}
	else if(inInterfaceID == Mouse::InterfaceID)
	{
		return &mMouse;
	}

	return NULL;
}
//---------------------------------------------------------------
/// Get Touch Screen Pointer
///
/// @return Pointer to touch screen device
//---------------------------------------------------------------
TouchScreen * CInputSystem::GetTouchScreenPtr()
{
	return &mTouchScreen;
}
//---------------------------------------------------------------
/// Get Mouse Pointer
///
/// @return Pointer to mouse device
//---------------------------------------------------------------
Mouse * CInputSystem::GetMousePtr()
{
	return &mMouse;
}
//---------------------------------------------------------------
/// Update
///
/// @param Time between frames
//---------------------------------------------------------------
void CInputSystem::Update(f32 infDT)
{
	mffTimeStamp += infDT;
	mTouchScreen.SetCurrentAppTime(mffTimeStamp);
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
