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

#ifndef _CHILLISOURCE_INPUT_GAMEPAD_GAMEPAD_H_
#define _CHILLISOURCE_INPUT_GAMEPAD_GAMEPAD_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Input/Gamepad/GamepadAxis.h>

#include <array>
#include <vector>

namespace ChilliSource
{
    /// Contains the information for a single gamepad and is accessed
    /// through the GamepadSystem.
    ///
    /// A gamepad contains information on the states of all buttons or inputs
    /// provided by the gamepad as well as a unique id for the pad.
    ///
    /// All gamepad buttons are treated as analogue even if the hardware is digital (in that case the returned values
    /// will just be 0.0 or 1.0).
    ///
    /// Axis are defined individually e.g. an analogue stick has 2 separate axes x and y.
    ///
    /// Button and axis indices and the directions of axis are all controller and platform dependent
    ///
    /// The index describes the active index of the pad at the point it was created, so if there were already
    /// two active gamepads existing at creation, it will be have an index of 2.
    ///
    /// NOTE: Restrictions on the backend mean we can only support up to 32 buttons and 8 axis inputs (e.g. 4 analogue sticks).
    ///
    class Gamepad final
    {
    public:
        
        static const u32 k_maxGamepads = 8;

        using Id = u64;
        
        ///
        /// Default constructor required for TryGetPointer
        ///
        Gamepad() = default;
        
        /// @param uniqueId
        ///     Unique id for the gamepad
        /// @param index
        ///     Index of the gamepad (describes the order it was created)
        /// @param numButtons
        ///     The number of buttons on the control pad
        /// @param supportedAxisFlags
        ///     Bits flagged on for each supported axis
        /// @param name
        ///     The identifying name of the control pad (as reported by the pad drivers).
        ///
        Gamepad(Id uniqueId, u32 index, u32 numButtons, u32 supportedAxisFlags, std::string name) noexcept;
        
        /// @param buttonIndex
        ///     Index of the button to check
        ///
        /// @return The pressure applied to the button with 1.0 being full
        ///     and 0.0 being none. For digital buttons the results are always 0 or 1. Returns 0.0 if unsupported
        ///
        f32 GetButtonPressure(u32 buttonIndex) const noexcept;
        
        /// @param axisIndex
        ///     Id of the axis to check
        ///
        /// @return Position of the axis with from 0 in the range -1 : 1. Returns 0.0 if unsupported
        ///
        f32 GetAxisPosition(GamepadAxis axis) const noexcept;

        /// @return Unique id of the gamepad
        ///
        Id GetId() const noexcept { return m_uniqueId; }
        
        /// @return Index describing the order the gamepad was attached (can be used for P1, P2, etc)
        ///
        u32 GetIndex() const noexcept { return m_index; }
        
        /// @return Number of buttons supported by this pad
        ///
        u32 GetNumButtons() const noexcept { return (u32)m_buttonStates.size(); }
        
        /// @param axis
        ///     Axis to check for support
        ///
        /// @return TRUE if gamepad supports the given axis
        ///
        bool IsAxisSupported(GamepadAxis axis) const noexcept { return ((m_supportedAxisFlags >> (u32)axis) & 1u) > 0u; }
        
        /// @return Name as reported by gamepad drivers
        ///
        const std::string& GetName() const noexcept { return m_name; }
        
        /// @return CRC32 hash of the name as reported by gamepad drivers
        ///
        u32 GetNameHash() const noexcept { return m_nameHash; }
        
    private:
        
        friend class GamepadSystem;
        
        Id m_uniqueId;
        u32 m_index;
        u32 m_supportedAxisFlags = 0;
        std::string m_name;
        u32 m_nameHash;
        
        std::vector<f32> m_buttonStates;
        std::array<f32, (u32)GamepadAxis::k_total> m_axisStates;
    };
}

#endif
