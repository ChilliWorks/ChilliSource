//
//  TextImage.cpp
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

#include <ChilliSource/UI/Text/TextImage.h>

#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Texture.h>

namespace ChilliSource
{
    namespace UI
    {
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        TextImage::TextImage()
        {
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        TextImage::TextImage(const Rendering::TextureCSPtr& in_texture, f32 in_scale)
        :m_texture(in_texture), m_scale(in_scale)
        {
            CS_ASSERT(in_texture != nullptr, "Need to provide a texture for a Text Image");
            m_uvs = Rendering::UVs(0.0f, 0.0f, 1.0f, 1.0f);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        TextImage::TextImage(const Rendering::TextureCSPtr& in_texture, const Rendering::TextureAtlasCSPtr& in_atlas, const std::string in_atlasID, f32 in_scale)
        :TextImage(in_texture, in_scale)
        {
            CS_ASSERT(in_atlas != nullptr, "Need to provide a texture atlas for a Text Image");
            
            m_atlas = in_atlas;
            m_atlasID = in_atlasID;
            
            if(!m_atlasID.empty())
            {
                m_uvs = m_atlas->GetFrameUVs(m_atlasID);
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Rendering::TextureCSPtr& TextImage::GetTexture() const
        {
            return m_texture;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Rendering::TextureAtlasCSPtr& TextImage::GetTextureAtlas() const
        {
            return m_atlas;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::string& TextImage::GetTextureAtlasID() const
        {
            return m_atlasID;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextImage::GetScale() const
        {
            return m_scale;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Rendering::UVs& TextImage::GetUVs() const
        {
            return m_uvs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextImage::SetSize(const Core::Vector2& in_size)
        {
            m_size = in_size;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Core::Vector2& TextImage::GetSize() const
        {
            return m_size;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextImage::SetOffset(const Core::Vector2& in_offset)
        {
            m_offset = in_offset;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Core::Vector2& TextImage::GetOffset() const
        {
            return m_offset;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextImage::SetIndex(u32 in_index)
        {
            m_indexInText = in_index;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        u32 TextImage::GetIndex() const
        {
            return m_indexInText;
        }
    }
}
