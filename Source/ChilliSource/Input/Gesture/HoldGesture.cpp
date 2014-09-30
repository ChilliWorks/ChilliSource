//
//  HoldGesture.cpp
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

#include <ChilliSource/Input/Gesture/HoldGesture.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
    namespace Input
    {
        namespace
        {
            const f32 k_minTimeBetweenAttempts = 0.015f;
            const f32 k_maxDisplacement = 20.0f;
            const f32 k_maxTimeBetweenPointers = 0.15f;
            const f32 k_holdTime = 0.75f;
        }
        
        CS_DEFINE_NAMEDTYPE(HoldGesture);
        //----------------------------------------------------
        //----------------------------------------------------
        HoldGesture::HoldGesture(u32 in_numPointers, Pointer::InputType in_inputType)
        : m_requiredPointerCount(in_numPointers), m_requiredInputType(in_inputType)
        {
            CS_ASSERT(m_requiredPointerCount > 0, "Cannot have a hold gesture which requres 0 pointers.");
            
            Core::Screen* screen = Core::Application::Get()->GetScreen();
            
            m_maxDisplacementSquared = (k_maxDisplacement * screen->GetDensityScale()) * (k_maxDisplacement * screen->GetDensityScale());
            m_pendingPointers.reserve(m_requiredPointerCount);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool HoldGesture::IsA(Core::InterfaceIDType in_gestureInterfaceId) const
        {
            return (Gesture::InterfaceID == in_gestureInterfaceId || HoldGesture::InterfaceID == in_gestureInterfaceId);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        u32 HoldGesture::GetNumPointers() const
        {
            return m_requiredPointerCount;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::InputType HoldGesture::GetInputType() const
        {
            return m_requiredInputType;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<HoldGesture::Delegate>& HoldGesture::GetHeldEvent()
        {
            return m_heldEvent;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void HoldGesture::Reset()
        {
            m_holdPending = false;
            m_currentStartTimestamp = 0.0;
            m_pendingPointers.clear();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void HoldGesture::OnUpdate(f32 in_deltaTime)
        {
            if (m_holdPending == true && m_pendingPointers.size() == m_requiredPointerCount)
            {
                f64 timestamp = ((f64)Core::Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0;
                if (timestamp - m_currentStartTimestamp > k_holdTime)
                {
                    Core::Vector2 gesturePosition = Core::Vector2::k_zero;
                    for (auto& pointerInfo : m_pendingPointers)
                    {
                        gesturePosition += pointerInfo.m_initialPosition;
                    }
                    gesturePosition /= m_pendingPointers.size();
                    
                    Reset();
                    
                    if (ResolveConflicts() == true)
                    {
                        m_heldEvent.NotifyConnections(this, gesturePosition);
                    }
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void HoldGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            if (in_inputType == m_requiredInputType)
            {
                if (m_holdPending == true && (m_pendingPointers.size() >= m_requiredPointerCount || in_timestamp - m_currentStartTimestamp > k_maxTimeBetweenPointers))
                {
                    Reset();
                    m_lastEndTimestamp = in_timestamp;
                }
                else if (in_timestamp - m_lastEndTimestamp > k_minTimeBetweenAttempts)
                {
                    if (m_holdPending == false && m_pendingPointers.size() == 0)
                    {
                        m_holdPending = true;
                        m_currentStartTimestamp = in_timestamp;
                    }
                    
                    PointerInfo pointerInfo;
                    pointerInfo.m_initialPosition = in_pointer.GetPosition();
                    pointerInfo.m_pointerId = in_pointer.GetId();
                    m_pendingPointers.push_back(pointerInfo);
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void HoldGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp)
        {
            if (m_holdPending == true)
            {
                bool shouldReset = false;
                
                for (const auto& pointerInfo : m_pendingPointers)
                {
                    if (in_pointer.GetId() == pointerInfo.m_pointerId)
                    {
                        const Core::Vector2 displacement = in_pointer.GetPosition() - pointerInfo.m_initialPosition;
                        if (displacement.LengthSquared() > m_maxDisplacementSquared)
                        {
                            shouldReset = true;
                        }
                        
                        break;
                    }
                }
                
                if (shouldReset == true)
                {
                    Reset();
                    m_lastEndTimestamp = in_timestamp;
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void HoldGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            if (in_inputType == m_requiredInputType && m_holdPending == true)
            {
                bool contains = false;
                for (auto pointer : m_pendingPointers)
                {
                    if (pointer.m_pointerId == in_pointer.GetId())
                    {
                        contains = true;
                    }
                }
                
                if (contains == true)
                {
                    Reset();
                    m_lastEndTimestamp = in_timestamp;
                }
            }
        }
    }
}
