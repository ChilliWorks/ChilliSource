//
//  StandardDrawable.cpp
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

#include <ChilliSource/UI/Drawable/StandardDrawable.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/UI/Drawable/DrawableType.h>
#include <ChilliSource/UI/Drawable/DrawableDesc.h>
#include <ChilliSource/UI/Drawable/DrawableUtils.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        StandardDrawable::StandardDrawable(const Rendering::TextureCSPtr& in_texture)
        {
            SetTexture(in_texture);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        StandardDrawable::StandardDrawable(const Rendering::TextureCSPtr& in_texture, const Rendering::TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId)
        {
            CS_ASSERT(in_atlas != nullptr, "The given texture atlas cannot be null.");
            CS_ASSERT(in_atlas->HasFrameWithId(in_atlasId) == true, "The texture atlas id must exist in the atlas.");
            
            SetTexture(in_texture);
            SetTextureAtlas(in_atlas);
            SetTextureAtlasId(in_atlasId);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        StandardDrawable::StandardDrawable(const DrawableDesc& in_desc)
        {
            SetUVs(in_desc.GetUVs());
            
            Core::StorageLocation textureLocation = in_desc.GetTextureLocation();
            std::string texturePath = in_desc.GetTexturePath();
            CS_ASSERT(texturePath.empty() == false, "Must provide a texture path in a widget drawable.");
            
            auto resPool = Core::Application::Get()->GetResourcePool();
            SetTexture(resPool->LoadResource<Rendering::Texture>(textureLocation, texturePath));
            
            Core::StorageLocation atlasLocation = in_desc.GetAtlasLocation();
            std::string atlasPath = in_desc.GetAtlasPath();
            
            if(atlasPath.empty() == false)
            {
                SetTextureAtlas(resPool->LoadResource<Rendering::TextureAtlas>(atlasLocation, atlasPath));
                CS_ASSERT(m_atlas != nullptr, "Invalid atlas provided for widget drawable: " + atlasPath);
                
                std::string atlasId = in_desc.GetAtlasId();
                CS_ASSERT(m_atlas->HasFrameWithId(atlasId) == true, "Invalid atlas id provided for widget drawable: " + atlasId);
                
                SetTextureAtlasId(atlasId);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        DrawableType StandardDrawable::GetType() const
        {
            return DrawableType::k_standard;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StandardDrawable::SetTexture(const Rendering::TextureCSPtr& in_texture)
        {
            CS_ASSERT(in_texture != nullptr, "Cannot set a null texture on a drawable.");
            
            m_texture = in_texture;
            
            m_atlasFrame = DrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StandardDrawable::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& in_atlas)
        {
            m_atlas = in_atlas;
            m_atlasId = "";
            
            m_atlasFrame = DrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StandardDrawable::SetTextureAtlasId(const std::string& in_atlasId)
        {
            CS_ASSERT(m_atlas != nullptr, "StandardDrawable::SetTextureAtlasId: Atlas Id cannot be set without first setting an atlas");
            
            m_atlasId = in_atlasId;
            
            m_atlasFrame = DrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StandardDrawable::SetUVs(const Rendering::UVs& in_UVs)
        {
            m_uvs = in_UVs;
            
            m_atlasFrame = DrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 StandardDrawable::GetPreferredSize() const
        {
            return m_atlasFrame.m_originalSize;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StandardDrawable::Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            CS_ASSERT(m_texture != nullptr, "StandardDrawable cannot draw without texture");
            
            //When textures are packed into an atlas their alpha space is cropped. This functionality restores the alpha space by resizing and offsetting the box.
            Core::Vector2 offsetTL
            (
                (-m_atlasFrame.m_originalSize.x * 0.5f) + (m_atlasFrame.m_croppedSize.x * 0.5f) + m_atlasFrame.m_offset.x,
                (m_atlasFrame.m_originalSize.y * 0.5f) - (m_atlasFrame.m_croppedSize.y * 0.5f) - m_atlasFrame.m_offset.y
            );
            offsetTL = in_absSize/m_atlasFrame.m_originalSize * offsetTL;
            Core::Vector2 size = in_absSize/m_atlasFrame.m_originalSize * m_atlasFrame.m_croppedSize;
            
            in_renderer->DrawBox(in_transform, size, offsetTL, m_texture, m_atlasFrame.m_uvs, in_absColour, Rendering::AlignmentAnchor::k_middleCentre);
        }
    }
}