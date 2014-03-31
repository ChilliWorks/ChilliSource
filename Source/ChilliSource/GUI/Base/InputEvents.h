//
//  InputEvents.h
//  moFloTest
//
//  Created by Scott Downie on 16/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_GUI_INPUT_EVENTS_H_
#define _MO_FLO_GUI_INPUT_EVENTS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>

namespace ChilliSource
{
    namespace GUI
    {
        typedef std::function<void(GUIView*, const Input::PointerSystem::Pointer&)> GUIEventDelegate;
        
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
            void OnPointerDown(GUIView* in_view, const Input::PointerSystem::Pointer& in_pointer, bool in_containsTouch);
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param Touch data
            ///
            /// @return Whether the view contains the touch
            //-----------------------------------------------------------
            bool OnPointerMoved(GUIView* in_view, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Called when the window stops receiving cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            void OnPointerUp(GUIView* in_view, const Input::PointerSystem::Pointer& in_pointer);
            
        protected:
            
            //---View events
            Core::Event<GUIEventDelegate> mTouchPressedInside;
            Core::Event<GUIEventDelegate> mTouchReleasedInside;
            Core::Event<GUIEventDelegate> mTouchReleasedOutside;
            Core::Event<GUIEventDelegate> mTouchMoveEnter;
            Core::Event<GUIEventDelegate> mTouchMoveExit;
            Core::Event<GUIEventDelegate> mTouchMovedWithin;
            
        private: 
            
            std::vector<u32> mOpenTouches;  
        };
    }
}

#endif