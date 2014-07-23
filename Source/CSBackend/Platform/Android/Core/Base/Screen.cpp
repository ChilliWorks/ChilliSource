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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Core/Base/Screen.h>

#include <CSBackend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace CSBackend
{
    namespace Android
    {
        CS_DEFINE_NAMEDTYPE(Screen);
        //-------------------------------------------------------
        //-------------------------------------------------------
        Screen::Screen()
        {
        	CoreJavaInterfaceSPtr javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();
        	CS_ASSERT(javaInterface != nullptr, "Cannot get CoreJavaInterface!");

			m_resolution = CSCore::Vector2((f32)javaInterface->GetScreenWidth(), (f32)javaInterface->GetScreenHeight());
			m_densityScale = javaInterface->GetScreenDensity();
			m_invDensityScale = 1.0f / m_densityScale;

            m_supportedResolutions.push_back(CSCore::Integer2((s32)m_resolution.x, (s32)m_resolution.y));
            m_supportedResolutions.push_back(CSCore::Integer2((s32)m_resolution.y, (s32)m_resolution.x));
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
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        CSCore::IConnectableEvent<Screen::DisplayModeChangedDelegate>& Screen::GetDisplayModeChangedEvent()
        {
            return m_displayModeChangedEvent;
        }
        //----------------------------------------------------------
		//----------------------------------------------------------
		void Screen::SetResolution(const CSCore::Integer2& in_size)
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
        std::vector<CSCore::Integer2> Screen::GetSupportedResolutions() const
        {
            return m_supportedResolutions;
        }
        //-----------------------------------------------------------
        //------------------------------------------------------------
        void Screen::OnResolutionChanged(const CSCore::Vector2& in_resolution)
        {
        	m_resolution = in_resolution;
        	m_resolutionChangedEvent.NotifyConnections(m_resolution);
        }
    }
}

#endif
