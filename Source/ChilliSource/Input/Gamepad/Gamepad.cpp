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

#include <ChilliSource/Input/Gamepad/Gamepad.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    Gamepad::Gamepad(Id uniqueId, u32 index, u32 numButtons, u32 supportedAxisFlags, std::string name) noexcept
    : m_uniqueId(uniqueId), m_index(index), m_supportedAxisFlags(supportedAxisFlags), m_name(std::move(name))
    {
        m_buttonStates.resize(numButtons, 0.0f);
        m_axisStates = {{0.0f}};
        m_nameHash = HashCRC32::GenerateHashCode(m_name);
    }
    
    //------------------------------------------------------------------------------
    f32 Gamepad::GetButtonPressure(u32 buttonIndex) const noexcept
    {
        return buttonIndex < m_buttonStates.size() ? m_buttonStates[buttonIndex] : 0.0f;
    }
    
    //------------------------------------------------------------------------------
    f32 Gamepad::GetAxisPosition(GamepadAxis axis) const noexcept
    {
        return m_axisStates[(u32)axis];
    }
}
