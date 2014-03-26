//
//  InputEvents.cpp
//  moFloTest
//
//  Created by Scott Downie on 16/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
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
            std::vector<u32>::iterator it = std::find(mOpenTouches.begin(), mOpenTouches.end(), in_pointer.m_uniqueId);
            
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
            std::vector<u32>::iterator it = std::find(mOpenTouches.begin(), mOpenTouches.end(), in_pointer.m_uniqueId);
            
            if(in_view->Contains(in_pointer.m_location))
            {
                //We contained the touch when it was released we can raise an event
                mTouchReleasedInside.NotifyConnections(in_view, in_pointer);
                
                if(it != mOpenTouches.end())
                {
                    mOpenTouches.erase(it);
                }
                
            }
            else if(it != mOpenTouches.end())
            {
                //If the touch started within us then we can trigger an event
                mTouchReleasedOutside.NotifyConnections(in_view, in_pointer);
                mOpenTouches.erase(it);
            }
        }  
    }
}
