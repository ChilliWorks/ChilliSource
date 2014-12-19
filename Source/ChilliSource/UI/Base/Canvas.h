//
//  Canvas.h
//  Chilli Source
//  Created by Scott Downie on 21/04/2014.
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


#ifndef _CHILLISOURCE_UI_BASE_CANVAS_H_
#define _CHILLISOURCE_UI_BASE_CANVAS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Core/System/StateSystem.h>
#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
	namespace UI
	{
		//-----------------------------------------------------------
		/// A state system that holds and renders the UI
        /// scene.
        ///
        /// @author S Downie
		//-----------------------------------------------------------
		class Canvas final : public Core::StateSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(Canvas);
            
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID
            ///
            /// @return Whether the object is of the given type
            //----------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Size of the canvas in pixels
            //----------------------------------------------------
            const Core::Vector2& GetSize() const;
            //----------------------------------------------------
            /// Traverse the hierarchy and render each widget
            /// using the canvas renderer
            ///
            /// @author S Downie
            ///
            /// @param Renderer
            //----------------------------------------------------
            void Draw(Rendering::CanvasRenderer* in_renderer) const;
            //----------------------------------------------------
            /// Adds a widget to the canvas. The widget
            /// will be rendered and updated. Any relative coordinates
            /// will now be in relation to this widget.
            ///
            /// NOTE: Will assert if the widget already has a parent
            ///
            /// @author S Downie
            ///
            /// @param Widget
            //----------------------------------------------------
            void AddWidget(const WidgetSPtr& in_widget);
            //----------------------------------------------------
            /// Remove widget from the canvas.
            ///
            /// NOTE: Will assert if the widget is not on this canvas
            ///
            /// @author S Downie
            ///
            /// @param Widget
            //----------------------------------------------------
            void RemoveWidget(Widget* in_widget);
            
        private:
            friend class Core::State;
            
            //----------------------------------------------------
			/// Creates a new instance of this system.
            ///
            /// @author S Downie
            ///
            /// @return The new instance.
			//----------------------------------------------------
            static CanvasUPtr Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            Canvas() = default;
            //-------------------------------------------------------
            /// Called when the system is created to initialise the
            /// canvas widget
            ///
            /// @author S Downie
            //-------------------------------------------------------
            void OnInit() override;
            //-------------------------------------------------------
            /// Called when the owning state is resumed.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
            void OnResume() override;
            //-------------------------------------------------------
            /// Called when the owning state is foregrounded.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
            void OnForeground() override;
            //-------------------------------------------------------
            /// Called every frame that the canvas is active
            ///
            /// @author S Downie
            ///
            /// @param Time in seconds since last update
            //-------------------------------------------------------
            void OnUpdate(f32 in_timeSinceLastUpdate) override;
            //-------------------------------------------------------
            /// Called when the owning state is backgrounded.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
            void OnBackground() override;
            //-------------------------------------------------------
            /// Called when the owning state is suspended.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
            void OnSuspend() override;
            //-------------------------------------------------------
            /// Called when the system is destroyed to destroy the
            /// canvas widget
            ///
            /// @author S Downie
            //-------------------------------------------------------
            void OnDestroy() override;
            //-----------------------------------------------------------
			/// Triggered if the screen resizes
            ///
            /// @param The new resolution
			//-----------------------------------------------------------
			void OnScreenResolutionChanged(const Core::Vector2& in_resolution);
            //-----------------------------------------------------------
            /// Called when the canvas receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            /// @param The press type.
            /// @param Filter object to check if the event has been filtered or to filter it
            //-----------------------------------------------------------
            void OnPointerDown(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType, Input::Filter& in_filter);
            //-----------------------------------------------------------
            /// Called when the canvas receives cursor/touch move input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            //-----------------------------------------------------------
            void OnPointerMoved(const Input::Pointer& in_pointer, f64 in_timestamp);
            //-----------------------------------------------------------
            /// Called when the canvas receiving cursor/touch release input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------------
            void OnPointerUp(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType);
            
        private:
            
            WidgetUPtr m_canvas;
            Core::EventConnectionUPtr m_screenResizedConnection;
            Core::EventConnectionUPtr m_pointerDownConnection;
            Core::EventConnectionUPtr m_pointerMovedConnection;
            Core::EventConnectionUPtr m_pointerUpConnection;
            
            Core::Screen* m_screen;
		};
	}
}

#endif
