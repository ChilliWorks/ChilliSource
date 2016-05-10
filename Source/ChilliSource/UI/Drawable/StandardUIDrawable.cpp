//
//  StandardUIDrawable.cpp
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

#include <ChilliSource/UI/Drawable/StandardUIDrawable.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/UI/Drawable/UIDrawableUtils.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(StandardUIDrawable);
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    StandardUIDrawable::StandardUIDrawable(const TextureCSPtr& in_texture)
    {
        SetTexture(in_texture);
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    StandardUIDrawable::StandardUIDrawable(const TextureCSPtr& in_texture, const TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId)
    {
        CS_ASSERT(in_atlas != nullptr, "The given texture atlas cannot be null.");
        CS_ASSERT(in_atlas->HasFrameWithId(in_atlasId) == true, "The texture atlas id \"" + in_atlasId + "\" must exist in the atlas.");
        
        SetTexture(in_texture);
        SetTextureAtlas(in_atlas);
        SetTextureAtlasId(in_atlasId);
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    bool StandardUIDrawable::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIDrawable::InterfaceID == in_interfaceId || StandardUIDrawable::InterfaceID == in_interfaceId);
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const TextureCSPtr& StandardUIDrawable::GetTexture() const
    {
        return m_texture;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const TextureAtlasCSPtr& StandardUIDrawable::GetTextureAtlas() const
    {
        return m_atlas;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const std::string& StandardUIDrawable::GetTextureAtlasId() const
    {
        return m_atlasId;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const UVs& StandardUIDrawable::GetUVs() const
    {
        return m_uvs;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const Colour& StandardUIDrawable::GetColour() const
    {
        return m_colour;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void StandardUIDrawable::SetTexture(const TextureCSPtr& in_texture)
    {
        CS_ASSERT(in_texture != nullptr, "Cannot set a null texture on a drawable.");
        
        m_texture = in_texture;
        
        m_atlasFrame = UIDrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void StandardUIDrawable::SetTextureAtlas(const TextureAtlasCSPtr& in_atlas)
    {
        m_atlas = in_atlas;
        m_atlasId = "";
        
        m_atlasFrame = UIDrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void StandardUIDrawable::SetTextureAtlasId(const std::string& in_atlasId)
    {
        CS_ASSERT(m_atlas != nullptr, "StandardUIDrawable::SetTextureAtlasId: Atlas Id cannot be set without first setting an atlas");
        
        m_atlasId = in_atlasId;
        
        m_atlasFrame = UIDrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void StandardUIDrawable::SetUVs(const UVs& in_UVs)
    {
        m_uvs = in_UVs;
        
        m_atlasFrame = UIDrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void StandardUIDrawable::SetColour(const Colour& in_colour)
    {
        m_colour = in_colour;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    Vector2 StandardUIDrawable::GetPreferredSize() const
    {
        return m_atlasFrame.m_originalSize;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void StandardUIDrawable::Draw(CanvasRenderer* in_renderer, const Matrix3& in_transform, const Vector2& in_absSize, const Colour& in_absColour)
    {
        CS_ASSERT(m_texture != nullptr, "StandardUIDrawable cannot draw without texture");
        
        //When textures are packed into an atlas their alpha space is cropped. This functionality restores the alpha space by resizing and offsetting the box.
        Vector2 offsetTL
        (
            (-m_atlasFrame.m_originalSize.x * 0.5f) + (m_atlasFrame.m_croppedSize.x * 0.5f) + m_atlasFrame.m_offset.x,
            (m_atlasFrame.m_originalSize.y * 0.5f) - (m_atlasFrame.m_croppedSize.y * 0.5f) - m_atlasFrame.m_offset.y
        );
        offsetTL = in_absSize/m_atlasFrame.m_originalSize * offsetTL;
        Vector2 size = in_absSize/m_atlasFrame.m_originalSize * m_atlasFrame.m_croppedSize;
        
        in_renderer->DrawBox(in_transform, size, offsetTL, m_texture, m_atlasFrame.m_uvs, in_absColour * m_colour, AlignmentAnchor::k_middleCentre);
    }
}