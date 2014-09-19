//
//  DragGesture.cpp
//  Chilli Source
//  Created by Ian Copland on 17/09/2014.
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

#include <ChilliSource/Input/Gesture/DragGesture.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
    namespace Input
    {
        CS_DEFINE_NAMEDTYPE(DragGesture);
        //----------------------------------------------------
        //----------------------------------------------------
        DragGesture::DragGesture(u32 in_numPointers, Pointer::InputType in_inputType)
        : m_numPointers(in_numPointers), m_inputType(in_inputType)
        {
            CS_ASSERT(in_numPointers > 0, "Cannot have a drag gesture with 0 required pointers.");
            
            m_activePointers.reserve(m_numPointers);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool DragGesture::IsA(Core::InterfaceIDType in_gestureInterfaceId) const
        {
            return (Gesture::InterfaceID == in_gestureInterfaceId || DragGesture::InterfaceID == in_gestureInterfaceId);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        u32 DragGesture::GetNumPointers() const
        {
            return m_numPointers;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::InputType DragGesture::GetInputType() const
        {
            return m_inputType;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<DragGesture::Delegate>& DragGesture::GetDragStartedEvent()
        {
            return m_dragStartedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<DragGesture::Delegate>& DragGesture::GetDragMovedEvent()
        {
            return m_dragMovedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<DragGesture::Delegate>& DragGesture::GetDragEndedEvent()
        {
            return m_dragEndedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::Vector2 DragGesture::CalculatePosition() const
        {
            Core::Vector2 gesturePos = Core::Vector2::k_zero;
            if (m_activePointers.size() > 0)
            {
                for (const auto& pointer : m_activePointers)
                {
                    gesturePos += pointer.m_currentPosition;
                }
                
                gesturePos /= f32(m_activePointers.size());
            }
            
            return gesturePos;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void DragGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter)
        {
            PointerInfo pointerInfo;
            pointerInfo.m_pointerId = in_pointer.GetId();
            pointerInfo.m_currentPosition = in_pointer.GetPosition();
            m_activePointers.push_back(pointerInfo);
            
            if (IsActive() == true)
            {
                Core::Vector2 gesturePos = CalculatePosition();
                SetActive(false);
                m_dragEndedEvent.NotifyConnections(this, gesturePos);
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void DragGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp, Filter& in_filter)
        {
            if (m_activePointers.size() > 0)
            {
                for (auto& pointer : m_activePointers)
                {
                    if (in_pointer.GetId() == pointer.m_pointerId)
                    {
                        pointer.m_currentPosition = in_pointer.GetPosition();
                        break;
                    }
                }
                
                if (IsActive() == false && m_activePointers.size() == m_numPointers)
                {
                    SetActive(true);
                    m_dragStartedEvent.NotifyConnections(this, CalculatePosition());
                }
                else if (IsActive() == true)
                {
                    m_dragMovedEvent.NotifyConnections(this, CalculatePosition());
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void DragGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter)
        {
            if (m_activePointers.size() > 0)
            {
                for (auto pointerIt = m_activePointers.begin(); pointerIt != m_activePointers.end();)
                {
                    if (in_pointer.GetId() == pointerIt->m_pointerId)
                    {
                        if (IsActive() == true)
                        {
                            Core::Vector2 gesturePos = CalculatePosition();
                            SetActive(false);
                            m_dragEndedEvent.NotifyConnections(this, gesturePos);
                        }
                        pointerIt = m_activePointers.erase(pointerIt);
                        break;
                    }
                    else
                    {
                        ++pointerIt;
                    }
                }
            }
        }
    }
}
