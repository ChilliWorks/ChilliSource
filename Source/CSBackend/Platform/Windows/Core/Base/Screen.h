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

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_BASE_SCREEN_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_BASE_SCREEN_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Event/Event.h>

namespace ChilliSource
{
    namespace Windows
    {
        //----------------------------------------------------------------
        /// The Windows backend for the screen. This provides information on
        /// the include device's screen such as resolution and density scale.
        ///
        /// @author I Copland
        //----------------------------------------------------------------
		class Screen final : public Core::Screen
		{
		public:
            CS_DECLARE_NAMEDTYPE(Screen);
            //-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author I Copland
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//-----------------------------------------------------------
			/// @author I Copland
			///
			/// @return Vector containing the width and height of screen
            /// space available to the application. For a desktop app
            /// this will be the current size of the window. For a mobile
            /// application this will be full size of the screen.
			//-----------------------------------------------------------
			const Core::Vector2& GetResolution() const override;
            //-----------------------------------------------------------
            /// The density scale factor as reported by the device. What
            /// this factor relates to is platform dependant. On iOS it
            /// is relative to a non-retina screen resolution. On Android
            /// it is a factor that changes depending on whether the
            /// screen is considered, low, medium, high or extra high
            /// density.
            ///
            /// @author I Copland
            ///
            /// @return The density scale factor of the screen
            //-----------------------------------------------------------
            f32 GetDensityScale() const override;
            //-----------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The inverse of density scale factor of the screen
            //-----------------------------------------------------------
            f32 GetInverseDensityScale() const override;
            //-----------------------------------------------------------
            /// @author I Copland
			///
			/// @return An event that is called when the screen resolution
            /// changes.
			//-----------------------------------------------------------
            Core::IConnectableEvent<ResolutionChangedDelegate>& GetResolutionChangedEvent() override;
            //-----------------------------------------------------------
            /// Called when the screen resolution changes. This will update
            /// the screen resolution and notify listeners that the resolution
            /// has changed. This is for internal use and should not be
            /// called manually.
            ///
			/// @author I Copland
            ///
            /// @param The new resolution.
			//------------------------------------------------------------
			void OnResolutionChanged(const Core::Vector2& in_resolution);
        private:
            friend Core::ScreenUPtr Core::Screen::Create();
            //-------------------------------------------------------
			/// Private constructor to force the use of the Create()
            /// factory method.
			///
            /// @author I Copland
			//-------------------------------------------------------
			Screen();
			//------------------------------------------------
			/// Initialisation method called at a time when
			/// all App Systems have been created. System
			/// initialisation occurs in the order they were
			/// created.
			///
			/// @author I Copland
			//------------------------------------------------
			void OnInit() override;
			//------------------------------------------------
			/// Called when the application is being destroyed.
			/// This should be used to cleanup memory and
			/// references to other systems. System destruction
			/// occurs in the reverse order to which they
			/// were created
			///
			/// @author I Copland
			//------------------------------------------------
			void OnDestroy() override;
            
            Core::Vector2 m_resolution;
            f32 m_densityScale;
            f32 m_invDensityScale;
            Core::Event<ResolutionChangedDelegate> m_resolutionChangedEvent;
        };
    }
}

#endif

#endif
