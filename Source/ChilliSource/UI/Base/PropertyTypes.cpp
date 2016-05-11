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

#include <ChilliSource/UI/Drawable/UIDrawableDef.h>
#include <ChilliSource/UI/Layout/UILayoutDef.h>
#include <ChilliSource/UI/ProgressBar/ProgressBarDirection.h>
#include <ChilliSource/UI/ProgressBar/ProgressBarType.h>
#include <ChilliSource/UI/Slider/SliderDirection.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Base/HorizontalTextJustification.h>
#include <ChilliSource/Rendering/Base/SizePolicy.h>
#include <ChilliSource/Rendering/Base/VerticalTextJustification.h>

namespace ChilliSource
{
    namespace PropertyTypes
    {
        CS_DEFINE_PROPERTYTYPE(ChilliSource::AlignmentAnchor, AlignmentAnchor, ChilliSource::AlignmentAnchor::k_middleCentre, &ParseAlignmentAnchor);
        CS_DEFINE_PROPERTYTYPE(ChilliSource::SizePolicy, SizePolicy, ChilliSource::SizePolicy::k_none, &ParseSizePolicy);
        CS_DEFINE_PROPERTYTYPE(UIDrawableDefCSPtr, UIDrawableDef, nullptr, nullptr);
        CS_DEFINE_PROPERTYTYPE(UILayoutDefCSPtr, UILayoutDef, nullptr, nullptr);
        CS_DEFINE_PROPERTYTYPE(TextureCSPtr, Texture, nullptr, nullptr);
        CS_DEFINE_PROPERTYTYPE(TextureAtlasCSPtr, TextureAtlas, nullptr, nullptr);
        CS_DEFINE_PROPERTYTYPE(FontCSPtr, Font, nullptr, nullptr);
        CS_DEFINE_PROPERTYTYPE(LocalisedTextCSPtr, LocalisedText, nullptr, nullptr);
        CS_DEFINE_PROPERTYTYPE(ChilliSource::HorizontalTextJustification, HorizontalTextJustification, ChilliSource::HorizontalTextJustification::k_centre, &ParseHorizontalTextJustification);
        CS_DEFINE_PROPERTYTYPE(ChilliSource::VerticalTextJustification, VerticalTextJustification, ChilliSource::VerticalTextJustification::k_centre, &ParseVerticalTextJustification);
        CS_DEFINE_PROPERTYTYPE(ChilliSource::SliderDirection, SliderDirection, ChilliSource::SliderDirection::k_horizontal, &ParseSliderDirection);
        CS_DEFINE_PROPERTYTYPE(ChilliSource::ProgressBarDirection, ProgressBarDirection, ChilliSource::ProgressBarDirection::k_horizontal, &ParseProgressBarDirection);
        CS_DEFINE_PROPERTYTYPE(ChilliSource::ProgressBarType, ProgressBarType, ChilliSource::ProgressBarType::k_stretch, &ParseProgressBarType);
    }
}
