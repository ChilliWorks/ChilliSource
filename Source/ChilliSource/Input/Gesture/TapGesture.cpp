//
//  TapGesture.cpp
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

#include <ChilliSource/Input/Gesture/TapGesture.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
    namespace Input
    {
        namespace
        {
            const f32 k_minTimeBetweenTaps = 0.015f;
            const f32 k_maxTimeForTap = 0.15f;
            const f32 k_maxTimeBetweenTaps = 0.25f;
            const f32 k_maxTapDisplacement = 20.0f;
            const f32 k_maxRepeatTapDisplacement = 40.0f;
        }
        
        CS_DEFINE_NAMEDTYPE(TapGesture);
        //----------------------------------------------------
        //----------------------------------------------------
        TapGesture::TapGesture(u32 in_numTaps, u32 in_numPointers, Pointer::InputType in_inputType)
            : m_requiredTapCount(in_numTaps), m_requiredPointerCount(in_numPointers), m_requiredInputType(in_inputType)
        {
            CS_ASSERT(m_requiredTapCount > 0, "Cannot have a tap gesture which requres 0 taps.");
            CS_ASSERT(m_requiredPointerCount > 0, "Cannot have a tap gesture which requres 0 pointers.");
            
            Core::Screen* screen = Core::Application::Get()->GetScreen();
            
            m_maxTapDisplacementSquared = (k_maxTapDisplacement * screen->GetDensityScale()) * (k_maxTapDisplacement * screen->GetDensityScale());
            m_maxRepeatTapDisplacementSquared = (k_maxRepeatTapDisplacement * screen->GetDensityScale()) * (k_maxRepeatTapDisplacement * screen->GetDensityScale());
            m_pendingPointers.reserve(m_requiredPointerCount);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool TapGesture::IsA(Core::InterfaceIDType in_gestureInterfaceId) const
        {
            return (Gesture::InterfaceID == in_gestureInterfaceId || TapGesture::InterfaceID == in_gestureInterfaceId);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        u32 TapGesture::GetNumTaps() const
        {
            return m_requiredTapCount;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        u32 TapGesture::GetNumPointers() const
        {
            return m_requiredPointerCount;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::InputType TapGesture::GetInputType() const
        {
            return m_requiredInputType;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<TapGesture::Delegate>& TapGesture::GetTappedEvent()
        {
            return m_tappedEvent;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::CheckForExpiration(f64 in_timestamp)
        {
            //Check whether an active tap has expired
            if (m_tapPending == true && in_timestamp - m_currentTapStartTimestamp > k_maxTimeForTap)
            {
                ResetTap();
            }
            
            if (m_tapPending == false && m_requiredTapCount > 0 && in_timestamp - m_lastTapEndTimestamp > k_maxTimeBetweenTaps)
            {
                ResetGesture();
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::ResetTap()
        {
            m_tapPending = false;
            m_currentTapStartTimestamp = 0.0;
            m_pendingPointers.clear();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::ResetGesture()
        {
            ResetTap();
            m_tapCount = 0;
            m_lastTapEndTimestamp = 0.0;
            m_firstTapPendingPointers.clear();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            CheckForExpiration(in_timestamp);
            
            if (in_inputType == m_requiredInputType)
            {
                bool tapValid = false;
                
                if (m_tapCount == 0)
                {
                    tapValid = true;
                }
                else
                {
                    for (const auto& pointerInfo : m_firstTapPendingPointers)
                    {
                        const Core::Vector2 displacement = in_pointer.GetPosition() - pointerInfo.m_initialPosition;
                        if (displacement.LengthSquared() <= m_maxRepeatTapDisplacementSquared)
                        {
                            tapValid = true;
                            break;
                        }
                    }
                }
                
                if (tapValid == true)
                {
                    if (m_tapPending == true)
                    {
                        if (m_pendingPointers.size() < m_requiredPointerCount)
                        {
                            PointerInfo pointerInfo;
                            pointerInfo.m_initialPosition = in_pointer.GetPosition();
                            pointerInfo.m_pointerId = in_pointer.GetId();
                            pointerInfo.m_isDown = true;
                            m_pendingPointers.push_back(pointerInfo);
                        }
                        else
                        {
                            ResetTap();
                        }
                    }
                    else if (m_tapCount == 0 || in_timestamp - m_lastTapStartTimestamp > k_minTimeBetweenTaps)
                    {
                        m_tapPending = true;
                        m_currentTapStartTimestamp = in_timestamp;
                        m_lastTapStartTimestamp = m_currentTapStartTimestamp;
                        
                        PointerInfo pointerInfo;
                        pointerInfo.m_initialPosition = in_pointer.GetPosition();
                        pointerInfo.m_pointerId = in_pointer.GetId();
                        pointerInfo.m_isDown = true;
                        m_pendingPointers.push_back(pointerInfo);
                    }
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp)
        {
            CheckForExpiration(in_timestamp);
            
            if (m_tapPending == true)
            {
                bool shouldReset = false;
                
                for (const auto& pointerInfo : m_pendingPointers)
                {
                    if (in_pointer.GetId() == pointerInfo.m_pointerId)
                    {
                        const Core::Vector2 displacement = in_pointer.GetPosition() - pointerInfo.m_initialPosition;
                        if (displacement.LengthSquared() > m_maxTapDisplacementSquared)
                        {
                            shouldReset = true;
                        }
                        
                        break;
                    }
                }
                
                if (shouldReset == true)
                {
                    ResetTap();
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            CheckForExpiration(in_timestamp);
            
            if (in_inputType == m_requiredInputType && m_tapPending == true)
            {
                if (m_pendingPointers.size() == m_requiredPointerCount)
                {
                    bool tapFinished = true;
                    
                    for (auto& pointerInfo : m_pendingPointers)
                    {
                        if (in_pointer.GetId() == pointerInfo.m_pointerId)
                        {
                            pointerInfo.m_isDown = false;
                        }
                        else if (pointerInfo.m_isDown == true)
                        {
                            tapFinished = false;
                        }
                    }
                    
                    if (tapFinished == true)
                    {
                        if (m_tapCount == 0)
                        {
                            m_firstTapPendingPointers = m_pendingPointers;
                        }
                        
                        ResetTap();
                        m_tapCount++;
                        m_lastTapEndTimestamp = in_timestamp;
                        
                        if (m_tapCount == m_requiredTapCount)
                        {
                            Core::Vector2 gesturePosition = Core::Vector2::k_zero;
                            for (auto& pointerInfo : m_firstTapPendingPointers)
                            {
                                gesturePosition += pointerInfo.m_initialPosition;
                            }
                            gesturePosition /= m_firstTapPendingPointers.size();
                            
                            ResetGesture();
                            
                            if (ResolveConflicts() == true)
                            {
                                m_tappedEvent.NotifyConnections(this, gesturePosition);
                            }
                        }
                    }
                }
                else
                {
                    ResetTap();
                }
            }
        }
    }
}
