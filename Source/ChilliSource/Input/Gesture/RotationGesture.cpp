//
//  RotationGesture.cpp
//  Chilli Source
//  Created by Ian Copland on 23/09/2014.
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

#include <ChilliSource/Input/Gesture/RotationGesture.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
    namespace
    {
        const f32 k_minDisplacement = 20.0f;
        const u32 k_requiredPointerCount = 2;
    }
    
    namespace Input
    {
        CS_DEFINE_NAMEDTYPE(RotationGesture);
        //----------------------------------------------------
        //----------------------------------------------------
        RotationGesture::RotationGesture(Pointer::InputType in_inputType)
        : m_requiredInputType(in_inputType)
        {
            Core::Screen* screen = Core::Application::Get()->GetScreen();
            m_minDisplacementSquared = (k_minDisplacement * screen->GetDensityScale()) * (k_minDisplacement * screen->GetDensityScale());
            
            m_pendingPointers.reserve(k_requiredPointerCount);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool RotationGesture::IsA(Core::InterfaceIDType in_gestureInterfaceId) const
        {
            return (Gesture::InterfaceID == in_gestureInterfaceId || RotationGesture::InterfaceID == in_gestureInterfaceId);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::InputType RotationGesture::GetInputType() const
        {
            return m_requiredInputType;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<RotationGesture::Delegate>& RotationGesture::GetRotationStartedEvent()
        {
            return m_rotationStartedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<RotationGesture::Delegate>& RotationGesture::GetRotationMovedEvent()
        {
            return m_rotationMovedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<RotationGesture::Delegate>& RotationGesture::GetRotationEndedEvent()
        {
            return m_rotationEndedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void RotationGesture::TryStart(const Pointer& in_pointer)
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
            
            CS_ASSERT(existingActive < k_requiredPointerCount, "gesture is already started if current active count is already at the required amount.");
            
            if (dragCount >= k_requiredPointerCount)
            {
                if (IsActive() == false && ResolveConflicts() == false)
                {
                    return;
                }
                
                u32 activeCount = existingActive;
                for (auto& pointer : m_pendingPointers)
                {
                    if (pointer.m_isDrag == true && pointer.m_active == false && activeCount < k_requiredPointerCount)
                    {
                        pointer.m_active = true;
                        ++activeCount;
                    }
                }
                
                m_paused = false;
                
                if (IsActive() == false)
                {
                    SetActive(true);
                    m_initialAngle = CalculateAngle();
                    m_currentRotationInfo.m_position = CalculatePosition();
                    m_currentRotationInfo.m_rotation = CalculateRelativeRotation();
                    m_currentRotationInfo.m_rotationDelta = 0.0f;
                    m_rotationStartedEvent.NotifyConnections(this, m_currentRotationInfo);
                }
                else
                {
                    //If we're re-placing a finger for a rotation we want to update the initial angle such that the rotation at the new
                    //finger position is the same as it was when the finger was removed.
                    m_initialAngle = CalculateAngle() - m_currentRotationInfo.m_rotation;
                    m_currentRotationInfo.m_position = CalculatePosition();
                    m_currentRotationInfo.m_rotation = CalculateRelativeRotation();
                    m_currentRotationInfo.m_rotationDelta = 0.0f;
                    m_rotationMovedEvent.NotifyConnections(this, m_currentRotationInfo);
                }
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::Vector2 RotationGesture::CalculatePosition() const
        {
            Core::Vector2 gesturePos = Core::Vector2::k_zero;
            if (m_pendingPointers.size() >= k_requiredPointerCount && IsActive() == true && m_paused == false)
            {
                for (const auto& pointer : m_pendingPointers)
                {
                    if (pointer.m_active == true)
                    {
                        gesturePos += pointer.m_currentPosition;
                    }
                }
                
                gesturePos /= k_requiredPointerCount;
            }
            
            return gesturePos;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        f32 RotationGesture::CalculateAngle() const
        {
            f32 angle = 0.0f;
            if (m_pendingPointers.size() >= k_requiredPointerCount && IsActive() == true && m_paused == false)
            {
                std::vector<Core::Vector2> positions;
                for (const auto& pointer : m_pendingPointers)
                {
                    if (pointer.m_active == true)
                    {
                        positions.push_back(pointer.m_currentPosition);
                    }
                }
                
                CS_ASSERT(positions.size() == k_requiredPointerCount, "There are more active pointers than required.");
                
                CSCore::Vector2 displacement = positions[1] - positions[0];
                
                angle = -Core::Vector2::Angle(CSCore::Vector2::k_unitPositiveY, displacement);
                
                while (angle < 0.0f)
                {
                    angle += CSCore::MathUtils::k_pi * 2.0f;
                }

                while (angle >= CSCore::MathUtils::k_pi * 2.0f)
                {
                    angle -= CSCore::MathUtils::k_pi * 2.0f;
                }
            }
            
            return angle;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        f32 RotationGesture::CalculateRelativeRotation() const
        {
            f32 angle = CalculateAngle() - m_initialAngle;
            
            while (angle < -CSCore::MathUtils::k_pi)
            {
                angle += CSCore::MathUtils::k_pi * 2.0f;
            }
            
            while (angle >= CSCore::MathUtils::k_pi)
            {
                angle -= CSCore::MathUtils::k_pi * 2.0f;
            }
            
            return angle;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void RotationGesture::Cancel()
        {
            CS_ASSERT(IsActive() == true, "Cannot cancel a gesture that isn't active.");
            
            for (auto& pointer : m_pendingPointers)
            {
                pointer.m_active = false;
            }
            
            SetActive(false);
            m_paused = false;
            
            m_currentRotationInfo.m_rotationDelta = 0.0f;
            m_rotationEndedEvent.NotifyConnections(this, m_currentRotationInfo);
            m_currentRotationInfo.m_position = Core::Vector2::k_zero;
            m_currentRotationInfo.m_rotation = 0.0f;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void RotationGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
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
        void RotationGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp)
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
                        m_currentRotationInfo.m_position = CalculatePosition();
                        f32 newRotation = CalculateRelativeRotation();
                        m_currentRotationInfo.m_rotationDelta = newRotation - m_currentRotationInfo.m_rotation;
                        m_currentRotationInfo.m_rotation = newRotation;
                        m_rotationMovedEvent.NotifyConnections(this, m_currentRotationInfo);
                    }
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void RotationGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
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
