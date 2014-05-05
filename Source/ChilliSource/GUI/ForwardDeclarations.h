//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_GUI_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_GUI_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
	namespace GUI
	{
        //--------------------------------------------------------
        /// Base
        //--------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(GUIView);
        CS_FORWARDDECLARE_CLASS(GUIViewFactory);
        CS_FORWARDDECLARE_CLASS(InputEvents);
        CS_FORWARDDECLARE_CLASS(Window);
        //--------------------------------------------------------
        /// Button
        //--------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Button);
        CS_FORWARDDECLARE_CLASS(HighlightButton);
        CS_FORWARDDECLARE_CLASS(StretchableHighlightButton);
        CS_FORWARDDECLARE_CLASS(ToggleButton);
        //--------------------------------------------------------
        /// Container
        //--------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(GridView);
        CS_FORWARDDECLARE_CLASS(HorizontalList);
        CS_FORWARDDECLARE_CLASS(ScrollView);
        CS_FORWARDDECLARE_CLASS(VerticalList);
        //--------------------------------------------------------
        /// Image
        //--------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(HorizontalStretchableImage);
        CS_FORWARDDECLARE_CLASS(ImageView);
        CS_FORWARDDECLARE_CLASS(StretchableImage);
        CS_FORWARDDECLARE_CLASS(VerticalStretchableImage);
        //--------------------------------------------------------
        /// Label
        //--------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(EditableLabel);
        CS_FORWARDDECLARE_CLASS(FormattedLabel);
        CS_FORWARDDECLARE_CLASS(Label);
        enum class TextOverflowBehaviour;
        enum class TextJustification;
        //--------------------------------------------------------
        /// Progress Bar
        //--------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(HorizontalClippingProgressBar);
        CS_FORWARDDECLARE_CLASS(HorizontalEndcappedProgressBar);
        CS_FORWARDDECLARE_CLASS(HorizontalProgressBar);
        CS_FORWARDDECLARE_CLASS(ProgressBar);
        CS_FORWARDDECLARE_CLASS(VerticalEndcappedProgressBar);
        //--------------------------------------------------------
        /// Slider Bar
        //--------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(HorizontalSliderBar);
        CS_FORWARDDECLARE_CLASS(SliderBar);
        CS_FORWARDDECLARE_CLASS(VerticalSliderBar);
    }
}

#endif
