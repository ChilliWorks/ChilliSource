//
//  InputEvents.cpp
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

#include <ChilliSource/GUI/Base/InputEvents.h>
#include <ChilliSource/GUI/Base/GUIView.h>

#include <algorithm>

namespace ChilliSource
{
    namespace GUI
    {
        //-----------------------------------------------------------
        /// Get Pressed Inside Event
        ///
        /// A event that is triggered when input is started within
        /// the bounds of the view
        ///
        /// @return Event
        //-----------------------------------------------------------
        Core::IConnectableEvent<GUIEventDelegate>& InputEvents::GetPressedInsideEvent()
        {
            return mTouchPressedInside;
        }
        //-----------------------------------------------------------
        /// Get Released Inside Event
        ///
        /// A event that is triggered when input is ended within
        /// the bounds of the view
        ///
        /// @return Event
        //-----------------------------------------------------------
        Core::IConnectableEvent<GUIEventDelegate>& InputEvents::GetReleasedInsideEvent()
        {
            return mTouchReleasedInside;
        }
        //-----------------------------------------------------------
        /// Get Released Outside Event
        ///
        /// A event that is triggered when input is ended outwith
        /// the bounds of the view having started within it
        ///
        /// @return Event
        //-----------------------------------------------------------
        Core::IConnectableEvent<GUIEventDelegate>& InputEvents::GetReleasedOutsideEvent()
        {
            return mTouchReleasedOutside;
        }
        //-----------------------------------------------------------
        /// Get Moved Outside Event
        ///
        /// A event that is triggered when input is registered outwith
        /// the bounds of the view having started within it
        ///
        /// @return Event
        //-----------------------------------------------------------
        Core::IConnectableEvent<GUIEventDelegate>& InputEvents::GetMovedOutsideEvent()
        {   
            return mTouchMoveExit;    
        }
        //-----------------------------------------------------------
        /// Get Moved Inside Event
        ///
        /// A event that is triggered when input is detected within
        /// the bounds of the view having started outwith it
        ///
        /// @return Event
        //-----------------------------------------------------------
        Core::IConnectableEvent<GUIEventDelegate>& InputEvents::GetMovedInsideEvent()
        {
            return mTouchMoveEnter;
        }
        //-----------------------------------------------------------
        /// Get Moved Within Event
        ///
        /// A event that is triggered when input is detected within
        /// the bounds of the view having started within it
        ///
        /// @return Event
        //-----------------------------------------------------------
        Core::IConnectableEvent<GUIEventDelegate>& InputEvents::GetMovedWithinEvent()
        {
            return mTouchMovedWithin;
        }
        //---Touch Delegates
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void InputEvents::OnPointerDown(GUIView* in_view, const Input::PointerSystem::Pointer& in_pointer, bool in_containsTouch)
        {
            //We must contain this point to be notified so we can trigger an event
            //Events: PressedInside
            //Possible: Released outside, Moved Outside, Moved Within
            if(in_containsTouch == true)
            {
                mTouchPressedInside.NotifyConnections(in_view, in_pointer);
            }
            
            mOpenTouches.push_back(in_pointer.m_uniqueId);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        bool InputEvents::OnPointerMoved(GUIView* in_view, const Input::PointerSystem::Pointer& in_pointer)
        {
            auto it = std::find(mOpenTouches.begin(), mOpenTouches.end(), in_pointer.m_uniqueId);
            
            bool bContains = in_view->Contains(in_pointer.m_location);
            
            if(it == mOpenTouches.end() && bContains)
            {
                //Event: Moved inside
                //The touch has moved into us raise an event
                mTouchMoveEnter.NotifyConnections(in_view, in_pointer);
            }
            else if(it != mOpenTouches.end() && bContains)
            {
                //Event: Moved within
                mTouchMovedWithin.NotifyConnections(in_view, in_pointer);
            }
            else if(it != mOpenTouches.end() && !bContains)
            {
                //If the touch started within us then we can trigger an event
                //Event: Moved outside
                mTouchMoveExit.NotifyConnections(in_view, in_pointer);
            }
            
            return bContains;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void InputEvents::OnPointerUp(GUIView* in_view, const Input::PointerSystem::Pointer& in_pointer)
        {
            auto it = std::find(mOpenTouches.begin(), mOpenTouches.end(), in_pointer.m_uniqueId);
            
            if(in_view->Contains(in_pointer.m_location))
            {
                if(it != mOpenTouches.end())
                {
                    mOpenTouches.erase(it);
                }
                
                //We contained the touch when it was released we can raise an event
                mTouchReleasedInside.NotifyConnections(in_view, in_pointer);
            }
            else if(it != mOpenTouches.end())
            {
                mOpenTouches.erase(it);
                
                //If the touch started within us then we can trigger an event
                mTouchReleasedOutside.NotifyConnections(in_view, in_pointer);
            }
        }  
    }
}
