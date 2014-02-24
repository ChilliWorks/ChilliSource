//
//  InputEvents.cpp
//  moFloTest
//
//  Created by Scott Downie on 16/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/GUI/InputEvents.h>
#include <ChilliSource/Rendering/GUI/GUIView.h>

#include <algorithm>

namespace ChilliSource
{
    namespace Rendering
    {
        //-----------------------------------------------------------
        /// Get Pressed Inside Event
        ///
        /// A event that is triggered when input is started within
        /// the bounds of the view
        ///
        /// @return Event
        //-----------------------------------------------------------
        Core::IEvent<GUIEventDelegate>& CInputEvents::GetPressedInsideEvent()
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
        Core::IEvent<GUIEventDelegate>& CInputEvents::GetReleasedInsideEvent()
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
        Core::IEvent<GUIEventDelegate>& CInputEvents::GetReleasedOutsideEvent()
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
        Core::IEvent<GUIEventDelegate>& CInputEvents::GetMovedOutsideEvent()
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
        Core::IEvent<GUIEventDelegate>& CInputEvents::GetMovedInsideEvent()
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
        Core::IEvent<GUIEventDelegate>& CInputEvents::GetMovedWithinEvent()
        {
            return mTouchMovedWithin;
        }
        //---Touch Delegates
        //-----------------------------------------------------------
        /// On Touch Began
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        /// @return Whether the view contains the touch
        //-----------------------------------------------------------
        bool CInputEvents::OnTouchBegan(CGUIView* inpView, const Input::TouchInfo & insTouchInfo)
        {
            //We must contain this point to be notified so we can trigger an event
            //Events: PressedInside
            //Possible: Released outside, Moved Outside, Moved Within
            mTouchPressedInside.Invoke(inpView, insTouchInfo);
            mOpenTouches.push_back(insTouchInfo.ID);
            
            return true;
        }
        //-----------------------------------------------------------
        /// On Touch Moved
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        /// @return Whether the view contains the touch
        //-----------------------------------------------------------
        bool CInputEvents::OnTouchMoved(CGUIView* inpView, const Input::TouchInfo & insTouchInfo)
        {
            DYNAMIC_ARRAY<u32>::iterator it = std::find(mOpenTouches.begin(), mOpenTouches.end(), insTouchInfo.ID);
            
            bool bContains = inpView->Contains(insTouchInfo.vLocation);
            
            if(it == mOpenTouches.end() && bContains)
            {
                //Event: Moved inside
                //The touch has moved into us raise an event
                mTouchMoveEnter.Invoke(inpView, insTouchInfo);
            }
            else if(it != mOpenTouches.end() && bContains)
            {
                //Event: Moved within
                mTouchMovedWithin.Invoke(inpView, insTouchInfo);
            }
            else if(it != mOpenTouches.end() && !bContains)
            {
                //If the touch started within us then we can trigger an event
                //Event: Moved outside
                mTouchMoveExit.Invoke(inpView, insTouchInfo);
            }
            
            return bContains;
        }
        //-----------------------------------------------------------
        /// On Touch Ended
        ///
        /// Called when the window stops receiving cursor/touch input
        ///
        /// @param Touch data
        //-----------------------------------------------------------
        void CInputEvents::OnTouchEnded(CGUIView* inpView, const Input::TouchInfo & insTouchInfo)
        {
            DYNAMIC_ARRAY<u32>::iterator it = std::find(mOpenTouches.begin(), mOpenTouches.end(), insTouchInfo.ID);
            
            if(inpView->Contains(insTouchInfo.vLocation))
            {
                //We contained the touch when it was released we can raise an event
                mTouchReleasedInside.Invoke(inpView, insTouchInfo);
                
                if(it != mOpenTouches.end())
                {
                    mOpenTouches.erase(it);
                }
                
            }
            else if(it != mOpenTouches.end())
            {
                //If the touch started within us then we can trigger an event
                mTouchReleasedOutside.Invoke(inpView, insTouchInfo);
                mOpenTouches.erase(it);
            }
        }  
    }
}
