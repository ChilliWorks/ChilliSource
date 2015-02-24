//
//  TextIcon.cpp
//  Chilli Source
//  Created by Nicolas Tanda on 23/02/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <ChilliSource/UI/Text/TextIcon.h>

#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
    namespace UI
    {
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        TextIcon::TextIcon(const Rendering::TextureCSPtr& in_texture, f32 in_scale)
        :m_texture(in_texture), m_scale(in_scale)
        {
            CS_ASSERT(in_texture != nullptr, "Need to provide a texture for a Text Icon");
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        TextIcon::TextIcon(const Rendering::TextureCSPtr& in_texture, const Rendering::TextureAtlasCSPtr& in_atlas, const std::string& in_atlasID, f32 in_scale)
        :TextIcon(in_texture, in_scale)
        {
            CS_ASSERT(in_atlas != nullptr, "Need to provide a texture atlas for a Text Icon");
            
            m_atlas = in_atlas;
            m_atlasID = in_atlasID;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Rendering::TextureCSPtr& TextIcon::GetTexture() const
        {
            return m_texture;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Rendering::TextureAtlasCSPtr& TextIcon::GetTextureAtlas() const
        {
            return m_atlas;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::string& TextIcon::GetTextureAtlasID() const
        {
            return m_atlasID;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextIcon::GetScale() const
        {
            return m_scale;
        }
    }
}
