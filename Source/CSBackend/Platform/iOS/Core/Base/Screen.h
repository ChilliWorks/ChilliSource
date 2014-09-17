//
//  Screen.h
//  Chilli Source
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

#import <ChilliSource/ChilliSource.h>
#import <CSBackend/Platform/iOS/ForwardDeclarations.h>
#import <ChilliSource/Core/Base/Screen.h>
#import <ChilliSource/Core/Event/Event.h>

#import <UIKit/UIKit.h>

namespace CSBackend
{
    namespace iOS
    {
        //----------------------------------------------------------------
        /// The iOS backend for the screen. This provides information on
        /// the iOS device's screen such as resolution and density scale.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------------
		class Screen final : public CSCore::Screen
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
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
			//-----------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return Vector containing the width and height of screen
            /// space available to the application. For a desktop app
            /// this will be the current size of the window. For a mobile
            /// application this will be full size of the screen.
			//-----------------------------------------------------------
			const CSCore::Vector2& GetResolution() const override;
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
            CSCore::IConnectableEvent<ResolutionChangedDelegate>& GetResolutionChangedEvent() override;
            //-----------------------------------------------------------
            /// @author S Downie
			///
			/// @return An event that is called when the screen display
            /// mode changes.
			//-----------------------------------------------------------
            CSCore::IConnectableEvent<DisplayModeChangedDelegate>& GetDisplayModeChangedEvent() override;
            //----------------------------------------------------------
            /// Does nothing on iOS due to fixed screen size
            ///
            /// @author S Downie
            //
			/// @param Screen size in pixels
			//----------------------------------------------------------
			void SetResolution(const CSCore::Integer2& in_size) override;
            //----------------------------------------------------------
            /// Does nothing on iOS due to fixed screen state
            ///
            /// @author S Downie
            //----------------------------------------------------------
            void SetDisplayMode(DisplayMode in_mode) override;;
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return A list of resolutions supported by the display
            //----------------------------------------------------------
            std::vector<CSCore::Integer2> GetSupportedResolutions() const override;
            //-----------------------------------------------------------
            /// Called when the screen orientation changes. This will update
            /// the screen resolution and notify listeners that the resolution
            /// has changed. This is for internal use and should not be
            /// called manually. Note: As of iOS 8 this is now deprecated.
            ///
            /// @author Ian Copland
            ///
            /// @param The new orientation.
            //------------------------------------------------------------
            void OnOrientationChanged(UIInterfaceOrientation in_orientation);
            //-----------------------------------------------------------
            /// Called when the screen resolution changes. This will update
            /// the screen resolution and notify listeners that the resolution
            /// has changed. This is for internal use and should not be
            /// called manually. This will only be called from iOS 8 onwards.
            ///
            /// @author Ian Copland
            ///
            /// @param The screen size in DIPS.
            //------------------------------------------------------------
            void OnResolutionChanged(CGSize in_dipsSize);
        private:
            friend CSCore::ScreenUPtr CSCore::Screen::Create();
            //-------------------------------------------------------
			/// Private constructor to force the use of the Create()
            /// factory method.
			///
            /// @author Ian Copland
			//-------------------------------------------------------
			Screen();
    
            std::vector<CSCore::Integer2> m_supportedResolutions;
            CSCore::Vector2 m_resolution;
            f32 m_densityScale;
            f32 m_invDensityScale;
            CSCore::Event<ResolutionChangedDelegate> m_resolutionChangedEvent;
            CSCore::Event<DisplayModeChangedDelegate> m_displayModeChangedEvent;
        };
    }
}

#endif