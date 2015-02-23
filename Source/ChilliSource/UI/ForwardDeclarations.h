//
//  ForwardDeclarations.h
//  Chilli Source
//  Created by Scott Downie on 18/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_UI_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_UI_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource 
{
	namespace UI
	{
        //---------------------------------------------------------
        /// Base
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Canvas);
        CS_FORWARDDECLARE_CLASS(Component);
        CS_FORWARDDECLARE_CLASS(ComponentDesc);
        CS_FORWARDDECLARE_CLASS(ComponentFactory);
        CS_FORWARDDECLARE_CLASS(PropertyLink);
        CS_FORWARDDECLARE_CLASS(Widget);
        CS_FORWARDDECLARE_CLASS(WidgetDef);
        CS_FORWARDDECLARE_CLASS(WidgetDesc);
        CS_FORWARDDECLARE_CLASS(WidgetDefProvider);
        CS_FORWARDDECLARE_CLASS(WidgetFactory);
        CS_FORWARDDECLARE_CLASS(WidgetTemplate);
        CS_FORWARDDECLARE_CLASS(WidgetTemplateProvider);
        enum class SizePolicy;
        //---------------------------------------------------------
        /// Button
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(HighlightComponent);
        CS_FORWARDDECLARE_CLASS(ToggleHighlightComponent);
        //---------------------------------------------------------
        /// Drawable
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Drawable);
        CS_FORWARDDECLARE_CLASS(DrawableComponent);
        CS_FORWARDDECLARE_CLASS(DrawableDef);
        CS_FORWARDDECLARE_CLASS(NinePatchDrawable);
        CS_FORWARDDECLARE_CLASS(NinePatchDrawableDef);
        CS_FORWARDDECLARE_CLASS(StandardDrawable);
        CS_FORWARDDECLARE_CLASS(StandardDrawableDef);
        CS_FORWARDDECLARE_CLASS(ThreePatchDrawable);
        CS_FORWARDDECLARE_CLASS(ThreePatchDrawableDef);
        //---------------------------------------------------------
        /// Layout
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Layout);
        CS_FORWARDDECLARE_CLASS(LayoutComponent);
        CS_FORWARDDECLARE_CLASS(LayoutDef);
        CS_FORWARDDECLARE_CLASS(GridLayout);
        CS_FORWARDDECLARE_CLASS(GridLayoutDef);
        CS_FORWARDDECLARE_CLASS(HListLayout);
        CS_FORWARDDECLARE_CLASS(HListLayoutDef);
        CS_FORWARDDECLARE_CLASS(VListLayout);
        CS_FORWARDDECLARE_CLASS(VListLayoutDef);
        //---------------------------------------------------------
        /// Progress Bar
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(ProgressBarComponent);
        enum class ProgressBarDirection;
        enum class ProgressBarType;
        //---------------------------------------------------------
        /// Slider
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(SliderComponent);
        enum class SliderDirection;
        //---------------------------------------------------------
        /// Text
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(TextComponent);
        CS_FORWARDDECLARE_CLASS(TextIcon);
	}
}

#endif
