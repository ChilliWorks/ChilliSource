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

#include <ChilliSource/UI/Base/WidgetPropertyTypes.h>

#include <ChilliSource/UI/Base/SizePolicy.h>
#include <ChilliSource/UI/Drawable/DrawableDesc.h>
#include <ChilliSource/UI/Layout/LayoutDesc.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace WidgetPropertyTypes
        {
            const Core::PropertyType<Rendering::AlignmentAnchor> k_alignmentAnchor("AlignmentAnchor", Rendering::AlignmentAnchor::k_middleCentre, &Rendering::ParseAlignmentAnchor);
            const Core::PropertyType<SizePolicy> k_sizePolicy("SizePolicy", SizePolicy::k_none, &ParseSizePolicy);
            const Core::PropertyType<LayoutDesc> k_layoutDesc("LayoutDesc", LayoutDesc(), nullptr);
            const Core::PropertyType<ILayoutSPtr> k_layout("Layout", nullptr, nullptr);
            const Core::PropertyType<DrawableDesc> k_drawableDesc("DrawableDesc", DrawableDesc(), nullptr);
            const Core::PropertyType<IDrawableSPtr> k_drawable("Drawable", nullptr, nullptr);
            const Core::PropertyType<Rendering::TextureCSPtr> k_texture("Texture", nullptr, nullptr);
            const Core::PropertyType<Rendering::TextureAtlasCSPtr> k_textureAtlas("TextureAtlas", nullptr, nullptr);
            const Core::PropertyType<Rendering::FontCSPtr> k_font("Font", nullptr, nullptr);
            const Core::PropertyType<Core::LocalisedTextCSPtr> k_localisedText("LocalisedText", nullptr, nullptr);
        }
    }
}
