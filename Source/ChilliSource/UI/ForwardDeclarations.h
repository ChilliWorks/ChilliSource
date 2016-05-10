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
    //---------------------------------------------------------
    /// Base
    //---------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(Canvas);
    CS_FORWARDDECLARE_CLASS(UIComponent);
    CS_FORWARDDECLARE_CLASS(UIComponentDesc);
    CS_FORWARDDECLARE_CLASS(UIComponentFactory);
    CS_FORWARDDECLARE_CLASS(PropertyLink);
    CS_FORWARDDECLARE_CLASS(Widget);
    CS_FORWARDDECLARE_CLASS(WidgetDef);
    CS_FORWARDDECLARE_CLASS(WidgetDesc);
    CS_FORWARDDECLARE_CLASS(WidgetDefProvider);
    CS_FORWARDDECLARE_CLASS(WidgetFactory);
    CS_FORWARDDECLARE_CLASS(WidgetTemplate);
    CS_FORWARDDECLARE_CLASS(WidgetTemplateProvider);
    //---------------------------------------------------------
    /// Button
    //---------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(HighlightUIComponent);
    CS_FORWARDDECLARE_CLASS(ToggleHighlightUIComponent);
    //---------------------------------------------------------
    /// UIDrawable
    //---------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(UIDrawable);
    CS_FORWARDDECLARE_CLASS(DrawableUIComponent);
    CS_FORWARDDECLARE_CLASS(UIDrawableDef);
    CS_FORWARDDECLARE_CLASS(NinePatchUIDrawable);
    CS_FORWARDDECLARE_CLASS(NinePatchUIDrawableDef);
    CS_FORWARDDECLARE_CLASS(StandardUIDrawable);
    CS_FORWARDDECLARE_CLASS(StandardUIDrawableDef);
    CS_FORWARDDECLARE_CLASS(ThreePatchUIDrawable);
    CS_FORWARDDECLARE_CLASS(ThreePatchUIDrawableDef);
    //---------------------------------------------------------
    /// UILayout
    //---------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(UILayout);
    CS_FORWARDDECLARE_CLASS(LayoutUIComponent);
    CS_FORWARDDECLARE_CLASS(UILayoutDef);
    CS_FORWARDDECLARE_CLASS(GridUILayout);
    CS_FORWARDDECLARE_CLASS(GridUILayoutDef);
    CS_FORWARDDECLARE_CLASS(HListUILayout);
    CS_FORWARDDECLARE_CLASS(HListUILayoutDef);
    CS_FORWARDDECLARE_CLASS(VListUILayout);
    CS_FORWARDDECLARE_CLASS(VListUILayoutDef);
    //---------------------------------------------------------
    /// Progress Bar
    //---------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(ProgressBarUIComponent);
    enum class ProgressBarDirection;
    enum class ProgressBarType;
    //---------------------------------------------------------
    /// Slider
    //---------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(SliderUIComponent);
    enum class SliderDirection;
    //---------------------------------------------------------
    /// Text
    //---------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(TextUIComponent);
    CS_FORWARDDECLARE_CLASS(TextIcon);
}

#endif
