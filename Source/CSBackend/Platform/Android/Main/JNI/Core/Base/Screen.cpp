//
//  Screen.cpp
//  ChilliSource
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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Main/JNI/Core/Base/Screen.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Base/CoreJavaInterface.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>

namespace CSBackend
{
    namespace Android
    {
        CS_DEFINE_NAMEDTYPE(Screen);
        //-------------------------------------------------------
        //-------------------------------------------------------
        Screen::Screen(const ChilliSource::ScreenInfo& screenInfo)
            : m_screenInfo(screenInfo)
        {
            m_resolution = m_screenInfo.GetInitialResolution();
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
            return m_screenInfo.GetDensityScale();
        }
        //----------------------------------------------------------
        //-----------------------------------------------------------
        f32 Screen::GetInverseDensityScale() const
        {
            return m_screenInfo.GetInverseDensityScale();
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
			CS_LOG_WARNING("Screen::SetResolution has no effect on Android");
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Screen::SetDisplayMode(DisplayMode in_mode)
        {
        	CS_LOG_WARNING("Screen::SetDisplayMode has no effect on Android");
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        std::vector<ChilliSource::Integer2> Screen::GetSupportedFullscreenResolutions() const
        {
            return m_screenInfo.GetSupportedFullscreenResolutions();
        }
        //-----------------------------------------------------------
        //------------------------------------------------------------
        void Screen::OnResolutionChanged(const ChilliSource::Vector2& in_resolution)
        {
        	m_resolution = in_resolution;
        	m_resolutionChangedEvent.NotifyConnections(m_resolution);
        }
    }
}

#endif
