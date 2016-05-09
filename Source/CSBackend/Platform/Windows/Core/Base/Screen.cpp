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
			ChilliSource::Integer2 size = SFMLWindow::Get()->GetWindowSize();
			m_resolution.x = (f32)size.x;
			m_resolution.y = (f32)size.y;

			m_densityScale = m_invDensityScale = 1.0f;

			m_displayModeChangeConnection = SFMLWindow::Get()->GetWindowDisplayModeEvent().OpenConnection(ChilliSource::MakeDelegate(this, &Screen::OnDisplayModeChanged));
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Screen::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
            return (ChilliSource::Screen::InterfaceID == in_interfaceId || Screen::InterfaceID == in_interfaceId);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        const ChilliSource::Vector2& Screen::GetResolution() const
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
        ChilliSource::IConnectableEvent<Screen::ResolutionChangedDelegate>& Screen::GetResolutionChangedEvent()
        {
            return m_resolutionChangedEvent;
        }
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		ChilliSource::IConnectableEvent<Screen::DisplayModeChangedDelegate>& Screen::GetDisplayModeChangedEvent()
		{
			return m_displayModeChangedEvent;
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		void Screen::SetResolution(const ChilliSource::Integer2& in_size)
		{
			SFMLWindow::Get()->SetSize(in_size);
			OnResolutionChanged(in_size);
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		void Screen::SetDisplayMode(DisplayMode in_mode)
		{
			switch (in_mode)
			{
			case DisplayMode::k_windowed:
				SFMLWindow::Get()->SetDisplayMode(SFMLWindow::DisplayMode::k_windowed);
				break;
			case DisplayMode::k_fullscreen:
				SFMLWindow::Get()->SetDisplayMode(SFMLWindow::DisplayMode::k_fullscreen);
				break;
			}
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		std::vector<ChilliSource::Integer2> Screen::GetSupportedResolutions() const
		{
			return SFMLWindow::Get()->GetSupportedResolutions();
		}
        //-----------------------------------------------------------
        //------------------------------------------------------------
        void Screen::OnResolutionChanged(const ChilliSource::Integer2& in_resolution)
        {
			m_resolution.x = (f32)in_resolution.x;
			m_resolution.y = (f32)in_resolution.y;

        	m_resolutionChangedEvent.NotifyConnections(m_resolution);
        }
		//----------------------------------------------------------
		//----------------------------------------------------------
		void Screen::OnDisplayModeChanged(SFMLWindow::DisplayMode in_mode)
		{
			switch (in_mode)
			{
			case SFMLWindow::DisplayMode::k_windowed:
				m_displayModeChangedEvent.NotifyConnections(DisplayMode::k_windowed);
				break;
			case SFMLWindow::DisplayMode::k_fullscreen:
				m_displayModeChangedEvent.NotifyConnections(DisplayMode::k_fullscreen);
				break;
			}
		}
		//------------------------------------------------
		//------------------------------------------------
		void Screen::OnInit()
		{
			m_windowResizeConnection = SFMLWindow::Get()->GetWindowResizedEvent().OpenConnection(ChilliSource::MakeDelegate(this, &Screen::OnResolutionChanged));
		}
		//------------------------------------------------
		//------------------------------------------------
		void Screen::OnDestroy()
		{
			m_windowResizeConnection = nullptr;
			m_displayModeChangeConnection = nullptr;
		}
    }
}

#endif
