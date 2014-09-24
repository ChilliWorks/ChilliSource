//
//  Pointer.cpp
//  Chilli Source
//  Created by Scott Downie on 08/07/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Input/Pointer/Pointer.h>

namespace ChilliSource
{
    namespace Input
    {
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::InputType Pointer::GetDefaultInputType()
        {
#if defined CS_TARGETPLATFORM_ANDROID
            return Pointer::InputType::k_touch;
#elif defined CS_TARGETPLATFORM_IOS
            return Pointer::InputType::k_touch;
#elif defined CS_TARGETPLATFORM_WINDOWS
            return Pointer::InputType::k_leftMouseButton;
#else
            return nullptr;
#endif
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::Pointer(Id in_uniqueId, u32 in_index, const Core::Vector2& in_initialPosition)
        : m_uniqueId(in_uniqueId), m_index(in_index), m_position(in_initialPosition), m_previousPosition(in_initialPosition)
        {
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::Vector2 Pointer::GetPosition() const
        {
            return m_position;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::Vector2 Pointer::GetPreviousPosition() const
        {
            return m_previousPosition;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool Pointer::IsInputDown(InputType in_type) const
        {
            return m_activeInput.find(in_type) != m_activeInput.end();
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool Pointer::IsInputUp(InputType in_type) const
        {
            return m_activeInput.find(in_type) == m_activeInput.end();
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::Id Pointer::GetId() const
        {
            return m_uniqueId;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        u32 Pointer::GetIndex() const
        {
            return m_index;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const std::set<Pointer::InputType>& Pointer::GetActiveInputs() const
        {
            return m_activeInput;
        }
    }
}
