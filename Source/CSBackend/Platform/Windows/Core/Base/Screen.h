//
//  Screen.h
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

#ifndef _CSBACKEND_PLATFORM_WINDOWS_CORE_BASE_SCREEN_H_
#define _CSBACKEND_PLATFORM_WINDOWS_CORE_BASE_SCREEN_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Event/Event.h>
#include <CSBackend/Platform/Windows/ForwardDeclarations.h>
#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>

namespace CSBackend
{
    namespace Windows
    {
        //----------------------------------------------------------------
        /// The Windows backend for the screen. This provides information on
        /// the include device's screen such as resolution and density scale.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------------
		class Screen final : public ChilliSource::Screen
		{
		public:
            CS_DECLARE_NAMEDTYPE(Screen);
            //-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author Ian Copland
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
			//-----------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return Vector containing the width and height of screen
            /// space available to the application. For a desktop app
            /// this will be the current size of the window. For a mobile
            /// application this will be full size of the screen.
			//-----------------------------------------------------------
			const ChilliSource::Vector2& GetResolution() const override;
            //-----------------------------------------------------------
            /// The density scale factor as reported by the device. What
            /// this factor relates to is platform dependant. On iOS it
            /// is relative to a non-retina screen resolution. On Android
            /// it is a factor that changes depending on whether the
            /// screen is considered, low, medium, high or extra high
            /// density.
            ///
            /// @author Ian Copland
            ///
            /// @return The density scale factor of the screen
            //-----------------------------------------------------------
            f32 GetDensityScale() const override;
            //-----------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The inverse of density scale factor of the screen
            //-----------------------------------------------------------
            f32 GetInverseDensityScale() const override;
            //-----------------------------------------------------------
            /// @author Ian Copland
			///
			/// @return An event that is called when the screen resolution
            /// changes.
			//-----------------------------------------------------------
            ChilliSource::IConnectableEvent<ResolutionChangedDelegate>& GetResolutionChangedEvent() override;
			//-----------------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when the screen display
			/// mode changes.
			//-----------------------------------------------------------
			ChilliSource::IConnectableEvent<DisplayModeChangedDelegate>& GetDisplayModeChangedEvent() override;
			//----------------------------------------------------------
			/// Changes the size of the application window
			///
			/// @author S Downie
			///
			/// @param Screen size in pixels
			//----------------------------------------------------------
			void SetResolution(const ChilliSource::Integer2& in_size) override;
			//----------------------------------------------------------
			/// Set the screen to fullscreen more or windowed mode
			/// on platforms where that is allowed. This will include the
			/// removal or addition of any status or menu bars
			///
			/// @author S Downie
			//----------------------------------------------------------
			void SetDisplayMode(DisplayMode in_mode) override;
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return A list of resolutions supported by the display
			//----------------------------------------------------------
			std::vector<ChilliSource::Integer2> GetSupportedResolutions() const override;
        private:
            friend ChilliSource::ScreenUPtr ChilliSource::Screen::Create();
            //-------------------------------------------------------
			/// Private constructor to force the use of the Create()
            /// factory method.
			///
            /// @author Ian Copland
			//-------------------------------------------------------
			Screen();
			//------------------------------------------------
			/// Initialisation method called at a time when
			/// all App Systems have been created. System
			/// initialisation occurs in the order they were
			/// created.
			///
			/// @author Ian Copland
			//------------------------------------------------
			void OnInit() override;
			//-----------------------------------------------------------
			/// Called when the screen resolution changes. This will update
			/// the screen resolution and notify listeners that the resolution
			/// has changed.
			///
			/// @author Ian Copland
			///
			/// @param The new resolution.
			//------------------------------------------------------------
			void OnResolutionChanged(const ChilliSource::Integer2& in_resolution);
			//----------------------------------------------------------
			/// Triggered when window display mode changes
			///
			/// @author S Downie
			///
			/// @param Display mode
			//----------------------------------------------------------
			void OnDisplayModeChanged(SFMLWindow::DisplayMode in_mode);
			//------------------------------------------------
			/// Called when the application is being destroyed.
			/// This should be used to cleanup memory and
			/// references to other systems. System destruction
			/// occurs in the reverse order to which they
			/// were created
			///
			/// @author Ian Copland
			//------------------------------------------------
			void OnDestroy() override;
            
            ChilliSource::Vector2 m_resolution;
            f32 m_densityScale;
            f32 m_invDensityScale;
            ChilliSource::Event<ResolutionChangedDelegate> m_resolutionChangedEvent;
			ChilliSource::Event<DisplayModeChangedDelegate> m_displayModeChangedEvent;
			ChilliSource::EventConnectionUPtr m_windowResizeConnection;
			ChilliSource::EventConnectionUPtr m_displayModeChangeConnection;
        };
    }
}

#endif

#endif
