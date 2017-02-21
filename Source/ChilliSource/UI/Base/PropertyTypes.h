//
//  WidgetPropertyTypes.h
//  ChilliSource
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

#ifndef _CHILLISOURCE_UI_BASE_PROPERTYTYPES_H_
#define _CHILLISOURCE_UI_BASE_PROPERTYTYPES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyType.h>
#include <ChilliSource/Input/TextEntry.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------
    /// Additional property types that are specific to UI.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------
    namespace PropertyTypes
    {
        CS_DECLARE_PROPERTYTYPE(AlignmentAnchor, AlignmentAnchor);
        CS_DECLARE_PROPERTYTYPE(SizePolicy, SizePolicy);
        CS_DECLARE_PROPERTYTYPE(UIDrawableDefCSPtr, UIDrawableDef);
        CS_DECLARE_PROPERTYTYPE(UILayoutDefCSPtr, UILayoutDef);
        CS_DECLARE_PROPERTYTYPE(TextureCSPtr, Texture);
        CS_DECLARE_PROPERTYTYPE(TextureAtlasCSPtr, TextureAtlas);
        CS_DECLARE_PROPERTYTYPE(FontCSPtr, Font);
        CS_DECLARE_PROPERTYTYPE(LocalisedTextCSPtr, LocalisedText);
        CS_DECLARE_PROPERTYTYPE(HorizontalTextJustification, HorizontalTextJustification);
        CS_DECLARE_PROPERTYTYPE(VerticalTextJustification, VerticalTextJustification);
        CS_DECLARE_PROPERTYTYPE(SliderDirection, SliderDirection);
        CS_DECLARE_PROPERTYTYPE(ProgressBarDirection, ProgressBarDirection);
        CS_DECLARE_PROPERTYTYPE(ProgressBarType, ProgressBarType);
        CS_DECLARE_PROPERTYTYPE(TextEntryType, InputType);
        CS_DECLARE_PROPERTYTYPE(TextEntryCapitalisation, CapitalisationFormat);
    }
}

#endif
