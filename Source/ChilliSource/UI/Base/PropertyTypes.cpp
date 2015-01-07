//
//  WidgetPropertyTypes.cpp
//  Chilli Source
//  Created by Ian Copland on 04/12/2014.
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

#include <ChilliSource/UI/Base/PropertyTypes.h>

#include <ChilliSource/UI/Base/SizePolicy.h>
#include <ChilliSource/UI/Drawable/DrawableDef.h>
#include <ChilliSource/UI/Layout/LayoutDef.h>
#include <ChilliSource/UI/ProgressBar/ProgressBarDirection.h>
#include <ChilliSource/UI/ProgressBar/ProgressBarType.h>
#include <ChilliSource/UI/Slider/SliderDirection.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Base/HorizontalTextJustification.h>
#include <ChilliSource/Rendering/Base/VerticalTextJustification.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace PropertyTypes
        {
            CS_DEFINE_PROPERTYTYPE(Rendering::AlignmentAnchor, AlignmentAnchor, Rendering::AlignmentAnchor::k_middleCentre, &Rendering::ParseAlignmentAnchor);
            CS_DEFINE_PROPERTYTYPE(UI::SizePolicy, SizePolicy, UI::SizePolicy::k_none, &ParseSizePolicy);
            CS_DEFINE_PROPERTYTYPE(DrawableDefCSPtr, DrawableDef, nullptr, nullptr);
            CS_DEFINE_PROPERTYTYPE(LayoutDefCSPtr, LayoutDef, nullptr, nullptr);
            CS_DEFINE_PROPERTYTYPE(Rendering::TextureCSPtr, Texture, nullptr, nullptr);
            CS_DEFINE_PROPERTYTYPE(Rendering::TextureAtlasCSPtr, TextureAtlas, nullptr, nullptr);
            CS_DEFINE_PROPERTYTYPE(Rendering::FontCSPtr, Font, nullptr, nullptr);
            CS_DEFINE_PROPERTYTYPE(Core::LocalisedTextCSPtr, LocalisedText, nullptr, nullptr);
            CS_DEFINE_PROPERTYTYPE(Rendering::HorizontalTextJustification, HorizontalTextJustification, Rendering::HorizontalTextJustification::k_centre, &Rendering::ParseHorizontalTextJustification);
            CS_DEFINE_PROPERTYTYPE(Rendering::VerticalTextJustification, VerticalTextJustification, Rendering::VerticalTextJustification::k_centre, &Rendering::ParseVerticalTextJustification);
            CS_DEFINE_PROPERTYTYPE(UI::SliderDirection, SliderDirection, UI::SliderDirection::k_horizontal, &ParseSliderDirection);
            CS_DEFINE_PROPERTYTYPE(UI::ProgressBarDirection, ProgressBarDirection, UI::ProgressBarDirection::k_horizontal, &ParseProgressBarDirection);
            CS_DEFINE_PROPERTYTYPE(UI::ProgressBarType, ProgressBarType, UI::ProgressBarType::k_stretch, &ParseProgressBarType);
        }
    }
}
