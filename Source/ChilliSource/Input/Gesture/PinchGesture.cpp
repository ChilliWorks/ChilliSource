//
//  PinchGesture.cpp
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

#include <ChilliSource/Input/Gesture/PinchGesture.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
    namespace
    {
        const u32 k_requiredPointerCount = 2;
    }
    
    namespace Input
    {
        CS_DEFINE_NAMEDTYPE(PinchGesture);
        //----------------------------------------------------
        //----------------------------------------------------
        PinchGesture::PinchGesture(Pointer::InputType in_inputType)
        : m_requiredInputType(in_inputType)
        {
            m_pendingPointers.reserve(2);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool PinchGesture::IsA(Core::InterfaceIDType in_gestureInterfaceId) const
        {
            return (Gesture::InterfaceID == in_gestureInterfaceId || PinchGesture::InterfaceID == in_gestureInterfaceId);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::InputType PinchGesture::GetInputType() const
        {
            return m_requiredInputType;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PinchGesture::Delegate>& PinchGesture::GetPinchStartedEvent()
        {
            return m_pinchStartedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PinchGesture::Delegate>& PinchGesture::GetPinchMovedEvent()
        {
            return m_pinchMovedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PinchGesture::Delegate>& PinchGesture::GetPinchEndedEvent()
        {
            return m_pinchEndedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::Vector2 PinchGesture::CalculatePosition() const
        {
            Core::Vector2 gesturePos = Core::Vector2::k_zero;
            if (m_pendingPointers.size() > 0)
            {
                for (const auto& pointer : m_pendingPointers)
                {
                    gesturePos += pointer.m_currentPosition;
                }
                
                gesturePos /= f32(m_pendingPointers.size());
            }
            
            return gesturePos;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        f32 PinchGesture::CalculateDistance() const
        {
            //TODO: !?
//            Core::Vector2 gesturePos = Core::Vector2::k_zero;
//            if (m_pendingPointers.size() > 0)
//            {
//                for (const auto& pointer : m_pendingPointers)
//                {
//                    gesturePos += pointer.m_currentPosition;
//                }
//                
//                gesturePos /= f32(m_pendingPointers.size());
//            }
//            
            return 0.0f;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        f32 PinchGesture::CalculateScale() const
        {
            //TODO: !?
            
            return 0.0f;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void PinchGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter)
        {
//            Core::Vector2 currentGesturePos = CalculatePosition();
//            f32 currentGestureScale = CalculateScale();
//            
//            PointerInfo pointerInfo;
//            pointerInfo.m_pointerId = in_pointer.GetId();
//            pointerInfo.m_currentPosition = in_pointer.GetPosition();
//            m_pendingPointers.push_back(pointerInfo);
//            
//            if (IsActive() == true)
//            {
//                SetActive(false);
//                m_pinchEndedEvent.NotifyConnections(this, currentGesturePos, currentGestureScale);
//            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void PinchGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp, Filter& in_filter)
        {
//            if (m_pendingPointers.size() > 0)
//            {
//                for (auto& pointer : m_pendingPointers)
//                {
//                    if (in_pointer.GetId() == pointer.m_pointerId)
//                    {
//                        pointer.m_currentPosition = in_pointer.GetPosition();
//                        break;
//                    }
//                }
//                
//                if (IsActive() == false && m_pendingPointers.size() == k_requiredPointerCount)
//                {
//                    SetActive(true);
//                    m_initialDistance = CalculateDistance();
//                    m_pinchStartedEvent.NotifyConnections(this, CalculatePosition(), CalculateScale());
//                }
//                else if (IsActive() == true)
//                {
//                    m_pinchMovedEvent.NotifyConnections(this, CalculatePosition(), CalculateScale());
//                }
//            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void PinchGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter)
        {
//            if (m_pendingPointers.size() > 0)
//            {
//                for (auto pointerIt = m_pendingPointers.begin(); pointerIt != m_pendingPointers.end();)
//                {
//                    if (in_pointer.GetId() == pointerIt->m_pointerId)
//                    {
//                        if (IsActive() == true)
//                        {
//                            Core::Vector2 gesturePos = CalculatePosition();
//                            f32 gestureScale = CalculateScale();
//                            SetActive(false);
//                            m_pinchEndedEvent.NotifyConnections(this, gesturePos, gestureScale);
//                        }
//                        pointerIt = m_pendingPointers.erase(pointerIt);
//                        break;
//                    }
//                    else
//                    {
//                        ++pointerIt;
//                    }
//                }
//            }
        }
    }
}
