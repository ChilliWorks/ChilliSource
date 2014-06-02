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

#include <ChilliSource/Backend/Platform/Windows/Core/Base/Screen.h>

//This must be included last.
#include <ChilliSource/Backend/Platform/Windows/GLFW/Base/GLFWManager.h>

namespace ChilliSource
{
    namespace Windows
    {
		namespace
		{
			Screen* g_screen = nullptr;
			//-------------------------------------------------
			//-------------------------------------------------
			void OnWindowResized(GLFWwindow* in_window, s32 in_width, s32 in_height)
			{
				if (g_screen != nullptr)
				{
					g_screen->OnResolutionChanged(Core::Vector2((f32)in_width, (f32)in_height));
				}
			}
		}
        CS_DEFINE_NAMEDTYPE(Screen);
        //-------------------------------------------------------
        //-------------------------------------------------------
        Screen::Screen()
        {
			s32 width, height = 0;
			GLFWManager::Get()->GetWindowSize(&width, &height);
			m_resolution.x = (f32)width;
			m_resolution.y = (f32)height;

			m_densityScale = m_invDensityScale = 1.0f;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Screen::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Core::Screen::InterfaceID == in_interfaceId || Screen::InterfaceID == in_interfaceId);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        const Core::Vector2& Screen::GetResolution() const
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
        Core::IConnectableEvent<Screen::ResolutionChangedDelegate>& Screen::GetResolutionChangedEvent()
        {
            return m_resolutionChangedEvent;
        }
        //-----------------------------------------------------------
        //------------------------------------------------------------
        void Screen::OnResolutionChanged(const Core::Vector2& in_resolution)
        {
        	m_resolution = in_resolution;
        	m_resolutionChangedEvent.NotifyConnections(m_resolution);
        }
		//------------------------------------------------
		//------------------------------------------------
		void Screen::OnInit()
		{
			g_screen = this;
			GLFWManager::Get()->SetWindowSizeDelegate((GLFWwindowsizefun)&OnWindowResized);
		}
		//------------------------------------------------
		//------------------------------------------------
		void Screen::OnDestroy()
		{
			GLFWManager::Get()->SetWindowSizeDelegate(nullptr);
			g_screen = nullptr;
		}
    }
}

#endif
