//
//  ForwardDeclarations.h
//  Chilli Source
//  Created by Scott Downie on 21/04/2011.
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
        /// Slider
        //--------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(HorizontalSliderBar);
        CS_FORWARDDECLARE_CLASS(SliderBar);
        CS_FORWARDDECLARE_CLASS(VerticalSliderBar);
    }
}

#endif
