//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#include <CSBackend/Platform/RPi/Core/Base/Screen.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
    namespace RPi
    {
        CS_DEFINE_NAMEDTYPE(Screen);

        //---------------------------------------------------------------------------------
        Screen::Screen(const ChilliSource::ScreenInfo& screenInfo) noexcept
            : m_screenInfo(screenInfo)
        {
            m_resolution.x = screenInfo.GetInitialResolution().x;
            m_resolution.y = screenInfo.GetInitialResolution().y;

            DispmanWindow::Get()->SetWindowDelegates(ChilliSource::MakeDelegate(this, &Screen::OnResolutionChanged));
        }

        //---------------------------------------------------------------------------------
        bool Screen::IsA(ChilliSource::InterfaceIDType in_interfaceId) const noexcept
        {
            return (ChilliSource::Screen::InterfaceID == in_interfaceId || Screen::InterfaceID == in_interfaceId);
        }

        //---------------------------------------------------------------------------------
        const ChilliSource::Vector2& Screen::GetResolution() const noexcept
        {
            return m_resolution;
        }

        //---------------------------------------------------------------------------------
        f32 Screen::GetDensityScale() const noexcept
        {
            return m_screenInfo.GetDensityScale();
        }

        //---------------------------------------------------------------------------------
        f32 Screen::GetInverseDensityScale() const noexcept
        {
            return m_screenInfo.GetInverseDensityScale();
        }

        //---------------------------------------------------------------------------------
        ChilliSource::IConnectableEvent<Screen::ResolutionChangedDelegate>& Screen::GetResolutionChangedEvent() noexcept
        {
            return m_resolutionChangedEvent;
        }

        //---------------------------------------------------------------------------------
		ChilliSource::IConnectableEvent<Screen::DisplayModeChangedDelegate>& Screen::GetDisplayModeChangedEvent() noexcept
		{
			return m_displayModeChangedEvent;
		}

        //---------------------------------------------------------------------------------
		std::vector<ChilliSource::Integer2> Screen::GetSupportedFullscreenResolutions() const noexcept
		{
            return m_screenInfo.GetSupportedFullscreenResolutions();
		}

        //---------------------------------------------------------------------------------
        void Screen::SetResolution(const ChilliSource::Integer2& size) noexcept
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                DispmanWindow::Get()->SetSize(size);
                OnResolutionChanged(size);
            });
        }

        //---------------------------------------------------------------------------------
        void Screen::SetDisplayMode(DisplayMode mode) noexcept
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                DispmanWindow::Get()->SetDisplayMode(mode);
                OnDisplayModeChanged(mode);
            });
        }

        //---------------------------------------------------------------------------------
        void Screen::OnResolutionChanged(const ChilliSource::Integer2& resolution) noexcept
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
                m_resolution.x = (f32)resolution.x;
                m_resolution.y = (f32)resolution.y;

                m_resolutionChangedEvent.NotifyConnections(m_resolution);
            });
        }

        //---------------------------------------------------------------------------------
        void Screen::OnDisplayModeChanged(DisplayMode mode) noexcept
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
                m_displayModeChangedEvent.NotifyConnections(mode);
            });
        }

        //---------------------------------------------------------------------------------
        void Screen::OnDestroy() noexcept
        {
            DispmanWindow::Get()->RemoveWindowDelegates();
        }
    }
}

#endif
