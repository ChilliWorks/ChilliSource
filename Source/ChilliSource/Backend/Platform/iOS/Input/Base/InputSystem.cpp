/*
 *  InputSystem.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 05/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Input/Base/InputSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Time/Timer.h>

namespace ChilliSource
{
    namespace iOS
    {
        InputSystem::InputSystem()
        {
        }

        InputSystem::~InputSystem()
        {
        }

        bool InputSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == InputSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
        }

        bool InputSystem::CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == TouchScreen::InterfaceID;
        }
        Input::InputDevice * InputSystem::GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID)
        {
            if(inInterfaceID == TouchScreen::InterfaceID) 
            {
                return &mTouchScreen;
            }
            
            return nullptr;
        }

        Input::TouchScreen * InputSystem::GetTouchScreen()
        {
            return &mTouchScreen;
        }
        void InputSystem::Update(f32 infDT)
        {
            m_timeStamp += infDT;
            mTouchScreen.SetCurrentAppTime(m_timeStamp);
        }
        Input::VirtualKeyboard* InputSystem::GetVirtualKeyboard()
        {
            return &mVirtualKeyboard;
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
