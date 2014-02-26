//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_GUI_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_GUI_FORWARDDECLARATIONS_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace GUI
	{
        //--------------------------------------------------------
        /// Base
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CGUIView);
        CS_FORWARD_DECLARE_CLASS(CGUIViewFactory);
        CS_FORWARD_DECLARE_CLASS(CInputEvents);
        CS_FORWARD_DECLARE_CLASS(CWindow);
        //--------------------------------------------------------
        /// Button
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(IButton);
        CS_FORWARD_DECLARE_CLASS(CHighlightButton);
        CS_FORWARD_DECLARE_CLASS(CStretchableHighlightButton);
        CS_FORWARD_DECLARE_CLASS(CToggleButton);
        //--------------------------------------------------------
        /// Container
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CGridView);
        CS_FORWARD_DECLARE_CLASS(CHorizontalList);
        CS_FORWARD_DECLARE_CLASS(CScrollView);
        CS_FORWARD_DECLARE_CLASS(CVerticalList);
        //--------------------------------------------------------
        /// Debug
        //--------------------------------------------------------
#ifdef DEBUG_STATS
        CS_FORWARD_DECLARE_CLASS(CDebugStatsView);
#endif
        //--------------------------------------------------------
        /// Image
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CHorizontalStretchableImage);
        CS_FORWARD_DECLARE_CLASS(CImageView);
        CS_FORWARD_DECLARE_CLASS(CStretchableImage);
        CS_FORWARD_DECLARE_CLASS(CVerticalStretchableImage);
        //--------------------------------------------------------
        /// Label
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CEditableLabel);
        CS_FORWARD_DECLARE_CLASS(CFormattedLabel);
        CS_FORWARD_DECLARE_CLASS(CLabel);
        //--------------------------------------------------------
        /// Progress Bar
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CHorizontalClippingProgressBar);
        CS_FORWARD_DECLARE_CLASS(CHorizontalEndcappedProgressBar);
        CS_FORWARD_DECLARE_CLASS(CHorizontalProgressBar);
        CS_FORWARD_DECLARE_CLASS(CProgressBar);
        CS_FORWARD_DECLARE_CLASS(CVerticalEndcappedProgressBar);
        //--------------------------------------------------------
        /// Slider Bar
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CHorizontalSliderBar);
        CS_FORWARD_DECLARE_CLASS(CSliderBar);
        CS_FORWARD_DECLARE_CLASS(CVerticalSliderBar);
    }
}

#endif
