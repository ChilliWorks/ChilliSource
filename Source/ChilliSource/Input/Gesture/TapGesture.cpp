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
            const f64 k_minTimeBetweenTaps = 0.015;
        }
        
        CS_DEFINE_NAMEDTYPE(TapGesture);
        //----------------------------------------------------
        //----------------------------------------------------
        TapGesture::TapGesture(u32 in_numTaps, u32 in_numPointers, f32 in_maxTimeForTap, f32 in_maxTimeBetweenTaps, f32 in_tapRadius, Pointer::InputType in_inputType)
            : m_numTaps(in_numTaps), m_numPointers(in_numPointers), m_maxTimeForTap(in_maxTimeForTap), m_maxTimeBetweenTaps(in_maxTimeBetweenTaps), m_tapRadius(in_tapRadius), m_inputType(in_inputType)
        {
            Core::Screen* screen = Core::Application::Get()->GetScreen();
            
            m_maxTapMoveDistSquared = (m_tapRadius * screen->GetDensityScale()) * (m_tapRadius * screen->GetDensityScale());
            m_activeTapPointers.reserve(m_numPointers);
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
            return m_numTaps;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        u32 TapGesture::GetNumPointers() const
        {
            return m_numPointers;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        f32 TapGesture::GetMaxTimeForTap() const
        {
            return m_maxTimeForTap;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        f32 TapGesture::GetMaxTimeBetweenTaps() const
        {
            return m_maxTimeBetweenTaps;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        f32 TapGesture::GetTapRadius() const
        {
            return m_tapRadius;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::InputType TapGesture::GetInputType() const
        {
            return m_inputType;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<TapGesture::ActivatedDelegate>& TapGesture::GetActivatedEvent()
        {
            return m_activatedEvent;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::CheckForExpiration(f64 in_timestamp)
        {
            //Check whether an active tap has expired
            if (m_activeTap == true && in_timestamp - m_activeTapStartTimestamp > m_maxTimeForTap)
            {
                ResetTap();
            }
            
            if (m_activeTap == false && m_numTaps > 0 && in_timestamp - m_lastTapEndTimestamp > m_maxTimeBetweenTaps)
            {
                ResetGesture();
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::ResetTap()
        {
            m_activeTap = false;
            m_activeTapStartTimestamp = 0.0;
            m_activeTapPointers.clear();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::ResetGesture()
        {
            ResetTap();
            m_tapCount = 0;
            m_lastTapEndTimestamp = 0.0;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter)
        {
            CheckForExpiration(in_timestamp);
            
            if (in_inputType == m_inputType)
            {
                if (m_activeTap == true)
                {
                    if (m_activeTapPointers.size() < m_numPointers)
                    {
                        PointerInfo pointerInfo;
                        pointerInfo.m_initialPosition = in_pointer.GetPosition();
                        pointerInfo.m_pointerId = in_pointer.GetId();
                        pointerInfo.m_isDown = true;
                        m_activeTapPointers.push_back(pointerInfo);
                    }
                    else
                    {
                        ResetTap();
                    }
                }
                else if (in_timestamp - m_lastTapStartTimestamp > k_minTimeBetweenTaps)
                {
                    m_activeTap = true;
                    m_activeTapStartTimestamp = in_timestamp;
                    m_lastTapStartTimestamp = m_activeTapStartTimestamp;
                    
                    PointerInfo pointerInfo;
                    pointerInfo.m_initialPosition = in_pointer.GetPosition();
                    pointerInfo.m_pointerId = in_pointer.GetId();
                    pointerInfo.m_isDown = true;
                    m_activeTapPointers.push_back(pointerInfo);
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp, Filter& in_filter)
        {
            CheckForExpiration(in_timestamp);
            
            if (m_activeTap == true)
            {
                bool shouldReset = false;
                
                for (const auto& pointerInfo : m_activeTapPointers)
                {
                    if (in_pointer.GetId() == pointerInfo.m_pointerId)
                    {
                        const Core::Vector2 displacement = in_pointer.GetPosition() - pointerInfo.m_initialPosition;
                        if (displacement.LengthSquared() > m_maxTapMoveDistSquared)
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
        void TapGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter)
        {
            CheckForExpiration(in_timestamp);
            
            if (in_inputType == m_inputType && m_activeTap == true)
            {
                if (m_activeTapPointers.size() == m_numPointers)
                {
                    bool tapFinished = true;
                    
                    for (auto& pointerInfo : m_activeTapPointers)
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
                        ResetTap();
                        m_tapCount++;
                        m_lastTapEndTimestamp = in_timestamp;
                        
                        if (m_tapCount == m_numTaps)
                        {
                            ResetGesture();
                            
                            //TODO: !? Check for collisions.
                            m_activatedEvent.NotifyConnections(this);
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
