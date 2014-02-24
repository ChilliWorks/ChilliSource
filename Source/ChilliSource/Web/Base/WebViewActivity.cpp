/*
 *  WebView.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 25/07/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Web/Base/WebViewActivity.h>

namespace ChilliSource
{
    namespace Web
    {
        DEFINE_NAMED_INTERFACE(IWebViewActivity);
        //--------------------------------------------------------------
        /// Is A
        ///
        /// @param Interface ID
        /// @param Whether activity is of given type
        //--------------------------------------------------------------
        bool IWebViewActivity::IsA(Core::InterfaceIDType inID) const
        {
            return inID == IWebViewActivity::InterfaceID;
        }
        //-----------------------------------------------
        /// Get Dismissed Event 
        ///
        /// @return Event triggered when activity is
        /// dismissed
        //-----------------------------------------------
        Core::IEvent<Core::ActivityDismissedEvent>& IWebViewActivity::GetDismissedEvent()
        {
            return mOnDismissedEvent;
        }
    }
}

