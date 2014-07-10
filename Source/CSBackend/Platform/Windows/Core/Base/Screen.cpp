//
//  Screen.cpp
//  Chilli Source
//  Created by Ian Copland on 28/04/2014.
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

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/Core/Base/Screen.h>

#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>

namespace CSBackend
{
    namespace Windows
    {
        CS_DEFINE_NAMEDTYPE(Screen);
        //-------------------------------------------------------
        //-------------------------------------------------------
        Screen::Screen()
        {
			CSCore::Integer2 size = SFMLWindow::Get()->GetWindowSize();
			m_resolution.x = (f32)size.x;
			m_resolution.y = (f32)size.y;

			m_densityScale = m_invDensityScale = 1.0f;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Screen::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (CSCore::Screen::InterfaceID == in_interfaceId || Screen::InterfaceID == in_interfaceId);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        const CSCore::Vector2& Screen::GetResolution() const
        {
            return m_resolution;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        f32 Screen::GetDensityScale() const
        {
            return m_densityScale;
        }
        //----------------------------------------------------------
        //-----------------------------------------------------------
        f32 Screen::GetInverseDensityScale() const
        {
            return m_invDensityScale;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        CSCore::IConnectableEvent<Screen::ResolutionChangedDelegate>& Screen::GetResolutionChangedEvent()
        {
            return m_resolutionChangedEvent;
        }
		//----------------------------------------------------------
		//----------------------------------------------------------
		void Screen::SetResolution(u32 in_width, u32 in_height)
		{
			SFMLWindow::Get()->SetSize(in_width, in_height);
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		void Screen::SetFullscreen()
		{
			SFMLWindow::Get()->SetFullscreen();
		}
        //-----------------------------------------------------------
        //------------------------------------------------------------
        void Screen::OnResolutionChanged(const CSCore::Integer2& in_resolution)
        {
			m_resolution.x = (f32)in_resolution.x;
			m_resolution.y = (f32)in_resolution.y;

        	m_resolutionChangedEvent.NotifyConnections(m_resolution);
        }
		//------------------------------------------------
		//------------------------------------------------
		void Screen::OnInit()
		{
			m_windowResizeConnection = SFMLWindow::Get()->GetWindowResizedEvent().OpenConnection(CSCore::MakeDelegate(this, &Screen::OnResolutionChanged));
		}
		//------------------------------------------------
		//------------------------------------------------
		void Screen::OnDestroy()
		{
			m_windowResizeConnection = nullptr;
		}
    }
}

#endif
