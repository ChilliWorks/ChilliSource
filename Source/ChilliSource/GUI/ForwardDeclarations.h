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
        CS_FORWARD_DECLARE_CLASS(GUIView);
        CS_FORWARD_DECLARE_CLASS(GUIViewFactory);
        CS_FORWARD_DECLARE_CLASS(InputEvents);
        CS_FORWARD_DECLARE_CLASS(Window);
        //--------------------------------------------------------
        /// Button
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Button);
        CS_FORWARD_DECLARE_CLASS(HighlightButton);
        CS_FORWARD_DECLARE_CLASS(StretchableHighlightButton);
        CS_FORWARD_DECLARE_CLASS(ToggleButton);
        //--------------------------------------------------------
        /// Container
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(GridView);
        CS_FORWARD_DECLARE_CLASS(HorizontalList);
        CS_FORWARD_DECLARE_CLASS(ScrollView);
        CS_FORWARD_DECLARE_CLASS(VerticalList);
        //--------------------------------------------------------
        /// Debug
        //--------------------------------------------------------
#ifdef DEBUG_STATS
        CS_FORWARD_DECLARE_CLASS(DebugStatsView);
#endif
        //--------------------------------------------------------
        /// Image
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(HorizontalStretchableImage);
        CS_FORWARD_DECLARE_CLASS(ImageView);
        CS_FORWARD_DECLARE_CLASS(StretchableImage);
        CS_FORWARD_DECLARE_CLASS(VerticalStretchableImage);
        //--------------------------------------------------------
        /// Label
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(EditableLabel);
        CS_FORWARD_DECLARE_CLASS(FormattedLabel);
        CS_FORWARD_DECLARE_CLASS(Label);
        //--------------------------------------------------------
        /// Progress Bar
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(HorizontalClippingProgressBar);
        CS_FORWARD_DECLARE_CLASS(HorizontalEndcappedProgressBar);
        CS_FORWARD_DECLARE_CLASS(HorizontalProgressBar);
        CS_FORWARD_DECLARE_CLASS(ProgressBar);
        CS_FORWARD_DECLARE_CLASS(VerticalEndcappedProgressBar);
        //--------------------------------------------------------
        /// Slider Bar
        //--------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(HorizontalSliderBar);
        CS_FORWARD_DECLARE_CLASS(SliderBar);
        CS_FORWARD_DECLARE_CLASS(VerticalSliderBar);
    }
}

#endif
