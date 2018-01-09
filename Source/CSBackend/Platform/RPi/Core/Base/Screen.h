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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_PLATFORM_RPI_CORE_BASE_SCREEN_H_
#define _CSBACKEND_PLATFORM_RPI_CORE_BASE_SCREEN_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/ScreenInfo.h>
#include <ChilliSource/Core/Event/Event.h>
#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <CSBackend/Platform/RPi/Core/Base/DispmanWindow.h>

namespace CSBackend
{
    namespace RPi
    {
        /// The Raspberry Pi backend for the screen. This provides information on
        /// the include device's screen such as resolution and density scale.
        ///
		class Screen final : public ChilliSource::Screen
		{
		public:
            CS_DECLARE_NAMEDTYPE(Screen);

            /// Queries whether or not this system implements the
			/// interface with the given Id.
			///
			/// @param interfaceId
			///		The id of the interface that we need to check if this system implements
			///
			/// @retrun TRUE if the system 'IsA' interface
			///
			bool IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept override;

			/// @return Vector containing the width and height of screen
            /// space available to the application. For the RPi we always run
            /// in fullscreen mode so this will be the resolution of the display
            ///
			const ChilliSource::Vector2& GetResolution() const noexcept override;

            /// @return The density scale factor of the screen
            ///
            f32 GetDensityScale() const noexcept override;

            /// @return The reciprocal density scale factor of the screen
            ///
            f32 GetInverseDensityScale() const noexcept override;

			/// @return An event that is called when the screen resolution
            /// changes (as we don't run windowed this will only be if the display changes).
            ///
            ChilliSource::IConnectableEvent<ResolutionChangedDelegate>& GetResolutionChangedEvent() noexcept override;

            /// @return An event that is called when the screen display mode changes (we only run fullscreen on Pi so this won't change)
            ///
			ChilliSource::IConnectableEvent<DisplayModeChangedDelegate>& GetDisplayModeChangedEvent() noexcept override;

            /// This has no effect on the Raspberry Pi as we always run fullscreen
            ///
            /// @param size
            ///     Size to set the window to
            ///
			void SetResolution(const ChilliSource::Integer2& size) noexcept override;

            /// This has no effect on the Raspberry Pi as we always run fullscreen
            ///
            /// @param mode
            ///     Display mode to use
            ///
			void SetDisplayMode(DisplayMode mode) noexcept override;

			/// @return A list of resolutions supported by the display
			///
			std::vector<ChilliSource::Integer2> GetSupportedFullscreenResolutions() const noexcept override;

        private:
            friend ChilliSource::ScreenUPtr ChilliSource::Screen::Create(const ChilliSource::ScreenInfo& screenInfo);

            /// @param screenInfo
            ///     Holds the information about the display
            ///
			Screen(const ChilliSource::ScreenInfo& screenInfo) noexcept;

            /// @param resolution
            ///     New window size
            ///
            void OnResolutionChanged(const ChilliSource::Integer2& resolution) noexcept;

            /// @param mode
            ///     New window mode
            ///
            void OnDisplayModeChanged(DisplayMode mode) noexcept;

            /// Stops listening for window events
            ///
            void OnDestroy() noexcept override;

            ChilliSource::Vector2 m_resolution;
            ChilliSource::ScreenInfo m_screenInfo;

            ChilliSource::Event<ResolutionChangedDelegate> m_resolutionChangedEvent;
			ChilliSource::Event<DisplayModeChangedDelegate> m_displayModeChangedEvent;
			ChilliSource::EventConnectionUPtr m_windowResizeConnection;
			ChilliSource::EventConnectionUPtr m_displayModeChangeConnection;
        };
    }
}

#endif

#endif
