//
//  ScrollView.h
//  Chilli Source
//  Created by Scott Downie on 27/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifndef _CHILLISOURCE_GUI_SCROLLVIEW_H_
#define _CHILLISOURCE_GUI_SCROLLVIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>

namespace ChilliSource
{
    namespace GUI
    {
        class ScrollView : public GUIView
        {
        public:

			DECLARE_META_CLASS(ScrollView)

            ScrollView();
            ScrollView(const Core::ParamDictionary& insParams);
            //-----------------------------------------------------
            /// Add Subview
            ///
            /// Add a view to the hierarchy
            ///
            /// @param GUIView shared pointer
            //-----------------------------------------------------
            void AddSubview(const GUIViewSPtr& inpSubview);
            //-----------------------------------------------------
            /// Remove Subview (Internal)
            ///
            /// Remove a view from our hierarchy
            ///
            /// @param GUIView pointer
            //-----------------------------------------------------
            void RemoveSubview(GUIView* inpSubview);
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer.
            /// @return Whether touch has been consumed
            //-----------------------------------------------------------
            bool OnPointerDown(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType) override;
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer.
            /// @param The timestamp
            //-----------------------------------------------------------
            void OnPointerMoved(const Input::Pointer& in_pointer, f64 in_timestamp) override;
			//-----------------------------------------------------------
			/// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
			///
			/// @param The pointer.
            /// @param The timestamp
            /// @param The press type.
			//-----------------------------------------------------------
			void OnPointerUp(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType) override;
            //-----------------------------------------------------------
            /// Enable Horizontal Scrolling
            ///
            /// @param Whether the scroll view allows sideways scrolling
            //-----------------------------------------------------------
            void EnableHorizontalScrolling(bool inbEnabled);
            //-----------------------------------------------------------
            /// Enable Vertical Scrolling
            ///
            /// @param Whether the scroll view allows vertical scrolling
            //-----------------------------------------------------------
            void EnableVerticalScrolling(bool inbEnabled);
			//-----------------------------------------------------------
			/// Is Horizontal Scrolling Enabled
			///
			/// @return Whether the scroll view allows sideways scrolling
			//-----------------------------------------------------------
			bool IsHorizontalScrollingEnabled() const;
			//-----------------------------------------------------------
			/// Is Vertical Scrolling Enabled
			///
			/// @return Whether the scroll view allows vertical scrolling
			//-----------------------------------------------------------
			bool IsVerticalScrollingEnabled() const;
            //-----------------------------------------------------
            /// Update
            ///
            /// @param Time between frames
            //-----------------------------------------------------
            void Update(f32 infDt);
            //-----------------------------------------------------
            /// Reset
            ///
            /// Resets the scroller back to the default
            //-----------------------------------------------------
            void Reset();
            //-----------------------------------------------------
            /// Jump To
            ///
            /// Jumps to the given position
            ///
            /// @param The new position
            //-----------------------------------------------------
            void JumpTo(const Core::UnifiedVector2& inuvPosition);
            //-----------------------------------------------------
            /// Set Velocity
            ///
            /// @param Velocity
            //-----------------------------------------------------
            void SetVelocity(const Core::Vector2& invVelocity);
            //-----------------------------------------------------
            /// Set Absolute Content Size
            ///
            /// @param Content size
            //-----------------------------------------------------
            void SetAbsoluteContentSize(const Core::Vector2& invSize);
            //-----------------------------------------------------
            /// Get Absolute Content Size
            ///
            /// @return Content size
            //-----------------------------------------------------
            Core::Vector2 GetAbsoluteContentSize() const;
            //-------------------------------------------------------
			/// Draw
			///
			/// Draw all our subviews in neat rows and columns. Each
			/// cell is based on the size of the largest content
			///
			/// @param Canvas renderer pointer
			//-------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvas);
			//-------------------------------------------------------
			/// Sets Debug Drawing
			///
			/// Enables/Disables debug drawing
			///
			/// @param New value for this flag. DEBUG_DRAWING must be
			/// set to TRUE
			//-------------------------------------------------------
			void EnableDebugDrawing(bool inbValue);
			//-----------------------------------------------------------
			/// Get Absolute Content Position
			///
			/// @return The current absolute position of the scroll view
			//-----------------------------------------------------------
            Core::Vector2 GetAbsoluteContentPosition() const;
         	//-----------------------------------------------------------
			/// Get Container View
			///
			/// @return The GUIView that contains all scrollable subviews
			/// within the scroll view.
			//-----------------------------------------------------------
            const GUIViewSPtr& GetContainerView() const;
            
        private:
            
            Core::Vector2 mvVelocity;
            
            GUIViewSPtr mpContainerView;
            
			Core::Vector2 mvRealPreviousTouchPosition;
			Core::Vector2 mvNextRealPreviousTouchPosition;
			bool mbTouchActive;
			bool mbTouchMoved;
			
			f32 mfTouchTravel;

			DECLARE_PROPERTY_A(bool, ScrollHorizontally, EnableHorizontalScrolling, IsHorizontalScrollingEnabled);
			DECLARE_PROPERTY_A(bool, ScrollVertically, EnableVerticalScrolling, IsVerticalScrollingEnabled);
            
            bool mbDrawDebug;
        };
    }
}

#endif