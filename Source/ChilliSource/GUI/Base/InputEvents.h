//
//  InputEvents.h
//  Chilli Source
//  Created by Scott Downie on 16/06/2011.
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

#ifndef _CHILLISOURCE_GUI_INPUT_EVENTS_H_
#define _CHILLISOURCE_GUI_INPUT_EVENTS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Input/Pointer/Pointer.h>

#include <functional>

namespace ChilliSource
{
    namespace GUI
    {
        typedef std::function<void(GUIView*, const Input::Pointer&)> GUIEventDelegate;
        
        class InputEvents
        {
        public:
            //---User action delegates
            //-----------------------------------------------------------
            /// Get Pressed Inside Event
            ///
            /// A event that is triggered when input is started within
            /// the bounds of the view
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IConnectableEvent<GUIEventDelegate>& GetPressedInsideEvent();
            //-----------------------------------------------------------
            /// Get Released Inside Event
            ///
            /// A event that is triggered when input is ended within
            /// the bounds of the view
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IConnectableEvent<GUIEventDelegate>& GetReleasedInsideEvent();
            //-----------------------------------------------------------
            /// Get Released Outside Event
            ///
            /// A event that is triggered when input is ended outwith
            /// the bounds of the view having started within it
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IConnectableEvent<GUIEventDelegate>& GetReleasedOutsideEvent();
            //-----------------------------------------------------------
            /// Get Moved Outside Event
            ///
            /// A event that is triggered when input is registered outwith
            /// the bounds of the view having started within it
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IConnectableEvent<GUIEventDelegate>& GetMovedOutsideEvent();
            //-----------------------------------------------------------
            /// Get Moved Inside Event
            ///
            /// A event that is triggered when input is detected within
            /// the bounds of the view having started outwith it
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IConnectableEvent<GUIEventDelegate>& GetMovedInsideEvent();
            //-----------------------------------------------------------
            /// Get Moved Within Event
            ///
            /// A event that is triggered when input is detected within
            /// the bounds of the view having started within it
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IConnectableEvent<GUIEventDelegate>& GetMovedWithinEvent();
            
            //---Touch Delegates
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param Touch data
            /// @param Whether the view contains the touch
            //-----------------------------------------------------------
            void OnPointerDown(GUIView* in_view, const Input::Pointer& in_pointer, bool in_containsTouch);
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param Touch data
            ///
            /// @return Whether the view contains the touch
            //-----------------------------------------------------------
            bool OnPointerMoved(GUIView* in_view, const Input::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Called when the window stops receiving cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            void OnPointerUp(GUIView* in_view, const Input::Pointer& in_pointer);
            
        protected:
            
            //---View events
            Core::Event<GUIEventDelegate> mTouchPressedInside;
            Core::Event<GUIEventDelegate> mTouchReleasedInside;
            Core::Event<GUIEventDelegate> mTouchReleasedOutside;
            Core::Event<GUIEventDelegate> mTouchMoveEnter;
            Core::Event<GUIEventDelegate> mTouchMoveExit;
            Core::Event<GUIEventDelegate> mTouchMovedWithin;
            
        private: 
            
            std::vector<Input::Pointer::Id> mOpenTouches;
        };
    }
}

#endif