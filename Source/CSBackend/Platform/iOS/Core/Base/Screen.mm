//
//  Screen.mm
//  ChilliSource
//  Created by Ian Copland on 25/04/2014.
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

#ifdef CS_TARGETPLATFORM_IOS

#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Threading/TaskScheduler.h>

#import <CSBackend/Platform/iOS/Core/Base/CSAppDelegate.h>
#import <CSBackend/Platform/iOS/Core/Base/Screen.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

namespace CSBackend
{
    namespace iOS
    {
        namespace
        {
            //----------------------------------------------------
            /// Calculate the resolution of the screen, pre-iOS 8.
            ///
            /// @author Ian Copland
            ///
            /// @param The orientation of the application.
            /// @param The size of the screen in portrait DIPS.
            /// @param The device pixel scale factor.
            ///
            /// @return The iOS device resolution.
            //----------------------------------------------------
            ChilliSource::Vector2 CalculateResolution(UIInterfaceOrientation in_orientation, CGSize in_dipsSize, f32 in_pixelScaleFactor)
            {
                ChilliSource::Vector2 resolution(in_dipsSize.width * in_pixelScaleFactor, in_dipsSize.height * in_pixelScaleFactor);
                
                if (in_orientation == UIInterfaceOrientationLandscapeLeft || in_orientation == UIInterfaceOrientationLandscapeRight)
                {
                    resolution = ChilliSource::Vector2(resolution.y, resolution.x);
                }
                
                return resolution;
            }
            //----------------------------------------------------
            /// Calculate the resolution of the screen in iOS 8
            /// and above.
            ///
            /// @author Ian Copland
            ///
            /// @param The size of the screen in DIPS.
            /// @param The device pixel scale factor.
            ///
            /// @return The iOS device resolution.
            //----------------------------------------------------
            ChilliSource::Vector2 CalculateResolution(CGSize in_dipsSize, f32 in_pixelScaleFactor)
            {
                ChilliSource::Vector2 resolution(std::round(in_dipsSize.width * in_pixelScaleFactor), std::round(in_dipsSize.height * in_pixelScaleFactor));
                return resolution;
            }
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not the resolution should be
            /// calculated based on the screen orientation. This
            /// is only the case below iOS 8, and this is
            /// effectively a check for pre-iOS 8.
            //----------------------------------------------------
#if DEBUG
            bool ShouldCalculateBasedOnOrientation()
            {
#ifdef NSFoundationVersionNumber_iOS_7_1
                return (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_7_1);
#else
                return true;
#endif
            }
#endif
        }
        
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
        std::vector<ChilliSource::Integer2> Screen::GetSupportedResolutions() const
        {
            return m_screenInfo.GetSupportedResolutions();
        }
        //----------------------------------------------------------
		//----------------------------------------------------------
		void Screen::SetResolution(const ChilliSource::Integer2& in_size)
		{
			CS_LOG_WARNING("Screen::SetResolution has no effect on iOS");
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Screen::SetDisplayMode(DisplayMode in_mode)
        {
        	CS_LOG_WARNING("Screen::SetDisplayMode has no effect on iOS");
        }
        //-----------------------------------------------------------
        //------------------------------------------------------------
        void Screen::OnOrientationChanged(UIInterfaceOrientation in_orientation)
        {
            CS_ASSERT(ShouldCalculateBasedOnOrientation() == true, "OnOrientationChanged() should not get called on devices that do not require orientation based calculations.");
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
                m_resolution = CalculateResolution(in_orientation, [[UIScreen mainScreen] bounds].size, [UIScreen mainScreen].scale);
                m_resolutionChangedEvent.NotifyConnections(m_resolution);
            });
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void Screen::OnResolutionChanged(CGSize in_dipsSize)
        {
            CS_ASSERT(ShouldCalculateBasedOnOrientation() == false, "OnResolutionChanged() should not get called on devices that require orientation based calculations.");
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
                m_resolution = CalculateResolution(in_dipsSize, m_screenInfo.GetDensityScale());
                m_resolutionChangedEvent.NotifyConnections(m_resolution);
            });
        }
    }
}

#endif
