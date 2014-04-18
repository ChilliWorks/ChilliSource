//
//  TextureDrawable.cpp
//  Chilli Source
//  Created by Scott Downie on 17/04/2014.
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

#include <ChilliSource/UI/Drawable/TextureDrawable.h>

#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        TextureDrawable::TextureDrawable()
        : m_UVs(Core::Rectangle(Core::Vector2::ZERO, Core::Vector2::ONE))
        {
            
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void TextureDrawable::SetTexture(const Rendering::TextureCSPtr& in_texture)
        {
            m_texture = in_texture;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void TextureDrawable::SetUVs(const Core::Rectangle& in_UVs)
        {
            m_UVs = in_UVs;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void TextureDrawable::Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3x3& in_transform, const Core::Colour& in_absColour)
        {
            CS_ASSERT(m_texture != nullptr, "TextureDrawable cannot draw without texture");
            //TODO: Check if the object is on screen
            //in_renderer->DrawBox(<#const Core::Matrix3x3 &inmatTransform#>, <#const Core::Vector2 &invSize#>, m_texture, m_UVs, in_absColour);
        }
    }
}