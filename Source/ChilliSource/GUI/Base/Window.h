//
//  GUIWindow.h
//  moFlo
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

//===============================================================================================
/// GUI Window
///
/// Manages the GUI hierarchy for a scene. Responsible for handling touchscreen input and
/// feeding it to subviews.
//===============================================================================================

#ifndef _MOFLO_GUIWINDOW_H_
#define _MOFLO_GUIWINDOW_H_

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
            
    		//-----------------------------------------------------
    		/// Set Input System
    		///
    		/// @param pointer to the input system
    		//-----------------------------------------------------
    		void SetInputSystem(Input::InputSystem* inpInputSystem);
            //-----------------------------------------------------
            /// Get Input System
            ///
            /// @return pointer to the input system
            //-----------------------------------------------------
            Input::InputSystem* GetInputSystem();
            void ListenForTouches();
            void UnlistenFromTouches();
            //-----------------------------------------------------------
			/// On Screen Orientation Changed
			///
			/// Triggered if the screen orientation changes so we can
			/// resize ourself
			//-----------------------------------------------------------
			void OnScreenOrientationChanged(Core::ScreenOrientation ineOrientation);
			//-----------------------------------------------------------
			/// On Screen Resized
			///
			/// Triggered if the screen resizes
			//-----------------------------------------------------------
			void OnScreenResized(u32 inudwWidth, u32 inudwHeight);
            
        protected:
            //-----------------------------------------------------------
            /// On Touch Began
            ///
            /// Called when the window receives cursor/touch input
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            void _OnTouchBegan(const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Touch Moved
            ///
            /// Called when the window receives cursor/touch input
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            void _OnTouchMoved(const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Touch Ended
            ///
            /// Called when the window stops receiving cursor/touch input
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            void _OnTouchEnded(const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Touch Began
            ///
            /// Called when the window receives cursor/touch input
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            bool OnTouchBegan(const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Touch Moved
            ///
            /// Called when the window receives cursor/touch input
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            bool OnTouchMoved(const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Touch Ended
            ///
            /// Called when the window stops receiving cursor/touch input
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            void OnTouchEnded(const Input::TouchInfo & insTouchInfo);
#ifdef DEBUG_STATS
            //-----------------------------------------------------
			/// Draw
			///
			/// Override this implementation to draw the GUI
			/// using the primitves within canvas renderer i.e.
			/// DrawBox, DrawString etc
			///
			/// @param Canvas renderer
			//-----------------------------------------------------
            void Draw(CanvasRenderer * inpCanvas);
#endif
			
            bool mbListeningForTouches;
            
			// Granting friendly access for the Scene
			friend void Core::CScene::BecomeInactive();
            
        private:
            Input::InputSystem* mpInputSystem;
        };
    }
}

#endif