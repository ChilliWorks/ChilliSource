/*
 *  WebView.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 25/07/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Web/Base/WebViewActivity.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Web/Base/WebViewActivity.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Web/Base/WebViewActivity.h>
#endif

namespace ChilliSource
{
    namespace Web
    {
        CS_DEFINE_NAMEDTYPE(WebViewActivity);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        WebViewActivityUPtr WebViewActivity::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return WebViewActivityUPtr(new iOS::WebViewActivity());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return WebViewActivityUPtr(new Android::WebViewActivity());
#endif
            return nullptr;
        }
        //--------------------------------------------------------------
        /// Is A
        ///
        /// @param Interface ID
        /// @param Whether activity is of given type
        //--------------------------------------------------------------
        bool WebViewActivity::IsA(Core::InterfaceIDType inID) const
        {
            return inID == WebViewActivity::InterfaceID;
        }
        //-----------------------------------------------
        /// Get Dismissed Event 
        ///
        /// @return Event triggered when activity is
        /// dismissed
        //-----------------------------------------------
        Core::IConnectableEvent<Core::ActivityDismissedEvent>& WebViewActivity::GetDismissedEvent()
        {
            return mOnDismissedEvent;
        }
    }
}

