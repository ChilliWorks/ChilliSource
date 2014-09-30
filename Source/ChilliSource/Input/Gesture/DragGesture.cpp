//
//  DragGesture.cpp
//  Chilli Source
//  Created by Ian Copland on 19/09/2014.
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
        namespace
        {
            const f32 k_minDisplacement = 20.0f;
        }
        
        CS_DEFINE_NAMEDTYPE(DragGesture);
        //----------------------------------------------------
        //----------------------------------------------------
        DragGesture::DragGesture(u32 in_numPointers, Pointer::InputType in_inputType)
        : m_requiredPointerCount(in_numPointers), m_requiredInputType(in_inputType)
        {
            CS_ASSERT(m_requiredPointerCount > 0, "Cannot have a drag gesture with 0 required pointers.");
            
            Core::Screen* screen = Core::Application::Get()->GetScreen();
            m_minDisplacementSquared = (k_minDisplacement * screen->GetDensityScale()) * (k_minDisplacement * screen->GetDensityScale());
            
            m_pendingPointers.reserve(m_requiredPointerCount);
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
            return m_requiredPointerCount;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::InputType DragGesture::GetInputType() const
        {
            return m_requiredInputType;
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
        //--------------------------------------------------------
        //--------------------------------------------------------
        void DragGesture::TryStart(const Pointer& in_pointer)
        {
            u32 dragCount = 0;
            u32 existingActive = 0;
            for (const auto& pointer : m_pendingPointers)
            {
                if (pointer.m_isDrag == true)
                {
                    ++dragCount;
                }
                
                if (pointer.m_active == true)
                {
                    ++existingActive;
                }
            }
            
            CS_ASSERT(existingActive < m_requiredPointerCount, "gesture is already started if current active count is already at the required amount.");
            
            if (dragCount >= m_requiredPointerCount)
            {
                if (IsActive() == false && ResolveConflicts() == false)
                {
                    return;
                }
                
                u32 activeCount = existingActive;
                for (auto& pointer : m_pendingPointers)
                {
                    if (pointer.m_isDrag == true && pointer.m_active == false && activeCount < m_requiredPointerCount)
                    {
                        pointer.m_active = true;
                        ++activeCount;
                    }
                }
                
                m_paused = false;
                
                if (IsActive() == false)
                {
                    SetActive(true);
                    m_currentDragInfo.m_position = CalculatePosition();
                    m_currentDragInfo.m_displacement = Core::Vector2::k_zero;
                    m_currentDragInfo.m_delta = Core::Vector2::k_zero;
                    m_dragStartedEvent.NotifyConnections(this, m_currentDragInfo);
                }
                else
                {
                    m_currentDragInfo.m_position = CalculatePosition();
                    m_dragMovedEvent.NotifyConnections(this, m_currentDragInfo);
                }
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::Vector2 DragGesture::CalculatePosition() const
        {
            Core::Vector2 gesturePos = Core::Vector2::k_zero;
            if (m_pendingPointers.size() >= m_requiredPointerCount && IsActive() == true && m_paused == false)
            {
                for (const auto& pointer : m_pendingPointers)
                {
                    if (pointer.m_active == true)
                    {
                        gesturePos += pointer.m_currentPosition;
                    }
                }
                
                gesturePos /= m_requiredPointerCount;
            }
            
            return gesturePos;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void DragGesture::Cancel()
        {
            CS_ASSERT(IsActive() == true, "Cannot cancel a gesture that isn't active.");
            
            for (auto& pointer : m_pendingPointers)
            {
                pointer.m_active = false;
            }
            
            SetActive(false);
            m_paused = false;
            
            m_currentDragInfo.m_delta = Core::Vector2::k_zero;
            m_dragEndedEvent.NotifyConnections(this, m_currentDragInfo);
            
            m_currentDragInfo.m_displacement = Core::Vector2::k_zero;
            m_currentDragInfo.m_position = Core::Vector2::k_zero;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void DragGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            if (in_inputType == m_requiredInputType)
            {
                PointerInfo pointerInfo;
                pointerInfo.m_pointerId = in_pointer.GetId();
                pointerInfo.m_currentPosition = in_pointer.GetPosition();
                m_pendingPointers.push_back(pointerInfo);
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void DragGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp)
        {
            if (m_pendingPointers.size() > 0)
            {
                bool isPending = false;
                bool isActive = false;
                for (auto& pointer : m_pendingPointers)
                {
                    if (in_pointer.GetId() == pointer.m_pointerId)
                    {
                        pointer.m_currentPosition = in_pointer.GetPosition();
                        
                        Core::Vector2 displacement = pointer.m_currentPosition - pointer.m_initialPosition;
                        if (displacement.LengthSquared() > m_minDisplacementSquared)
                        {
                            pointer.m_isDrag = true;
                        }
                        
                        if (pointer.m_active == true)
                        {
                            isActive = true;
                        }
                        
                        isPending = true;
                    }
                }
                
                if (isPending == true)
                {
                    if (IsActive() == false || m_paused == true)
                    {
                        TryStart(in_pointer);
                    }
                    else if (isActive == true)
                    {
                        Core::Vector2 newPosition = CalculatePosition();
                        m_currentDragInfo.m_delta = newPosition - m_currentDragInfo.m_position;
                        m_currentDragInfo.m_displacement += m_currentDragInfo.m_delta;
                        m_currentDragInfo.m_position = newPosition;
                        m_dragMovedEvent.NotifyConnections(this, m_currentDragInfo);
                    }
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void DragGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            if (in_inputType == m_requiredInputType)
            {
                if (m_pendingPointers.size() > 0)
                {
                    for (auto pointerIt = m_pendingPointers.begin(); pointerIt != m_pendingPointers.end();)
                    {
                        if (in_pointer.GetId() == pointerIt->m_pointerId)
                        {
                            if (pointerIt->m_active == true)
                            {
                                m_paused = true;
                            }
                            
                            pointerIt = m_pendingPointers.erase(pointerIt);
                            break;
                        }
                        else
                        {
                            ++pointerIt;
                        }
                    }
                    
                    if (m_pendingPointers.empty() == true && IsActive() == true)
                    {
                        Cancel();
                    }
                }
            }
        }
    }
}
