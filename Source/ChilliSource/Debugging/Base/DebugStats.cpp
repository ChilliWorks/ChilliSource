//
//  DebugStats.cpp
//  Chilli Source
//  Created by Scott Downie on 04/08/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifdef CS_ENABLE_DEBUGSTATS

#include <ChilliSource/Debugging/Base/DebugStats.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Debugging/Base/DebugStatsView.h>
#include <ChilliSource/GUI/Base/Window.h>

namespace ChilliSource
{
    namespace Debugging
    {
        namespace
        {
            const TimeIntervalMs k_msBetweenRefreshes = 100;
        }
        
        CS_DEFINE_NAMEDTYPE(DebugStats);
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        DebugStatsUPtr DebugStats::Create()
        {
            return DebugStatsUPtr(new DebugStats());
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        DebugStats::DebugStats()
            : m_enabled(true), m_lastRefresh(0)
        {
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool DebugStats::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (DebugStats::InterfaceID == in_interfaceId);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool DebugStats::IsEnabled() const
        {
            return m_enabled;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void DebugStats::SetEnabled(bool in_enabled)
        {
            m_enabled = in_enabled;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void DebugStats::AddToEvent(const std::string& in_eventName, u32 in_value)
        {
            if(m_enabled == false)
            {
                return;
            }
            
            auto it = m_events.find(in_eventName);
            if(it != m_events.end())
            {
                u32 newValue = Core::ParseU32(it->second) + in_value;
                it->second = Core::ToString(newValue);
            }
            else
            {
                m_events[in_eventName] = Core::ToString(in_value);
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void DebugStats::AddToEvent(const std::string& in_eventName, s32 in_value)
        {
            if(m_enabled == false)
            {
                return;
            }
            
            auto it = m_events.find(in_eventName);
            if(it != m_events.end())
            {
                s32 newValue = Core::ParseS32(it->second) + in_value;
                it->second = Core::ToString(newValue);
            }
            else
            {
                m_events[in_eventName] = Core::ToString(in_value);
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void DebugStats::AddToEvent(const std::string& in_eventName, f32 in_value)
        {
            if(m_enabled == false)
            {
                return;
            }
            
            auto it = m_events.find(in_eventName);
            if(it != m_events.end())
            {
                f32 newValue = Core::ParseF32(it->second) + in_value;
                it->second = Core::ToString(newValue);
            }
            else
            {
                m_events[in_eventName] = Core::ToString(in_value);
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void DebugStats::Clear()
        {
            m_events.clear();
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void DebugStats::DrawStats(Rendering::CanvasRenderer* in_canvas, GUI::Window* in_window)
        {
            if(m_enabled == true)
            {
                TimeIntervalMs timeNow = Core::Application::Get()->GetSystemTimeInMilliseconds();
                TimeIntervalMs timeSince = timeNow - m_lastRefresh;
                if (timeSince > k_msBetweenRefreshes)
                {
                    m_lastRefresh = timeNow;
                    m_view->Refresh(m_events);
                }
                
                m_view->SetParentView(in_window);
                m_view->SetRootWindow(in_window);
                m_view->Draw(in_canvas);
            }
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        void DebugStats::OnInit()
        {
            m_view = DebugStatsViewSPtr(new DebugStatsView());
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        void DebugStats::OnDestroy()
        {
            m_view.reset();
        }
    }
}

#endif
