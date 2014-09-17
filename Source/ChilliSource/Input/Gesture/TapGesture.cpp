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

namespace ChilliSource
{
    namespace Input
    {
        CS_DEFINE_NAMEDTYPE(TapGesture);
        //----------------------------------------------------
        //----------------------------------------------------
        TapGesture::TapGesture(u32 in_numTaps, u32 in_numPointers, f32 in_maxTimeForTap, f32 in_maxTimeBetweenTaps)
            : m_numTaps(in_numTaps), m_numPointers(in_numPointers), m_maxTimeForTap(in_maxTimeForTap), m_maxTimeBetweenTaps(in_maxTimeBetweenTaps)
        {
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
        Core::IConnectableEvent<TapGesture::ActivatedDelegate>& TapGesture::GetActivatedEvent()
        {
            return m_activatedEvent;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter)
        {
            //if there is a tap already in progress
            for (auto& tapInfo : m_activeTaps)
            {
                if (tapInfo.m_pointers.size() < m_numPointers)
                {
                    if (in_timestamp - tapInfo.m_startTimestamp <= m_maxTimeForTap)
                    {
                        PointerInfo pointerInfo;
                        pointerInfo.m_initialPosition = in_pointer.GetPosition();
                        pointerInfo.m_pointerId = in_pointer.GetId();
                        tapInfo.m_pointers.push_back(pointerInfo);
                        return;
                    }
                }
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp, Filter& in_filter)
        {
            
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void TapGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter)
        {
            
        }
    }
}
