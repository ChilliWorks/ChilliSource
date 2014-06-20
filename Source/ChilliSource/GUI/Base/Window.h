//
//  Window.h
//  Chilli Source
//  Created by Scott Downie on 21/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

//===============================================================================================
/// GUI Window
///
/// Manages the GUI hierarchy for a scene. Responsible for handling touchscreen input and
/// feeding it to subviews.
//===============================================================================================

#ifndef _CHILLISOURCE_GUIWINDOW_H_
#define _CHILLISOURCE_GUIWINDOW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>
#include <ChilliSource/Core/Scene/Scene.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
    namespace GUI
    {
        class Window : public GUIView
        {
        public:
            Window();
            virtual ~Window();
            
            void StartListeningForPointerInput();
            void StopListeningForPointerInput();
			//-----------------------------------------------------------
			/// On Screen Resolution Changed
			///
			/// Triggered if the screen resizes
            ///
            /// @param The new resolution
			//-----------------------------------------------------------
			void OnScreenResolutionChanged(const Core::Vector2& in_resolution);
            
        protected:
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------------
            void _OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType);
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            //-----------------------------------------------------------
            void _OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp);
            //-----------------------------------------------------------
            /// Called when the window stops receiving cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------------
            void _OnPointerUp(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType);
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------------
            bool OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType) override;
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            //-----------------------------------------------------------
            bool OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp) override;
            //-----------------------------------------------------------
            /// Called when the window stops receiving cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------------
            void OnPointerUp(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType) override;
#ifdef CS_ENABLE_DEBUGSTATS
            //-----------------------------------------------------
			/// Draw
			///
			/// Override this implementation to draw the GUI
			/// using the primitves within canvas renderer i.e.
			/// DrawBox, DrawString etc
			///
			/// @param Canvas renderer
			//-----------------------------------------------------
            void Draw(Rendering::CanvasRenderer * inpCanvas);
#endif
			
			// Granting friendly access for the Scene
			friend void Core::Scene::OnBackground();
            
        private:
            Input::PointerSystem* m_pointerSystem;
            
            bool mbListeningForTouches;

            Core::EventConnectionUPtr m_screenOrientationChangedConnection;
            Core::EventConnectionUPtr m_screenResizedConnection;
            
            Core::EventConnectionUPtr m_pointerDownConnection;
            Core::EventConnectionUPtr m_pointerMovedConnection;
            Core::EventConnectionUPtr m_pointerUpConnection;
        };
    }
}

#endif
