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

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyType::k_string, "Type", "Standard"},
                {PropertyType::k_vec4, "UVs", "0 0 1 1"},
                {PropertyType::k_string, "TextureLocation", "Package"},
                {PropertyType::k_string, "TexturePath", ""}
            };
        }
        
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        StandardDrawable::StandardDrawable(const PropertyMap& in_properties)
        {
            Core::Vector4 uvs(in_properties.GetProperty<Core::Vector4>("UVs"));
            SetUVs(Rendering::UVs(uvs.x, uvs.y, uvs.z, uvs.w));
            
            std::string location(in_properties.GetProperty<std::string>("TextureLocation"));
            std::string path(in_properties.GetProperty<std::string>("TexturePath"));
            
            if(location.empty() == false && path.empty() == false)
            {
                auto resPool = Core::Application::Get()->GetResourcePool();
                m_texture = resPool->LoadResource<Rendering::Texture>(Core::ParseStorageLocation(location), path);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::vector<PropertyMap::PropertyDesc> StandardDrawable::GetPropertyDescs()
        {
            return k_propertyDescs;
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
            m_texture = in_texture;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StandardDrawable::SetUVs(const Rendering::UVs& in_UVs)
        {
            m_UVs = in_UVs;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 StandardDrawable::GetPreferredSize() const
        {
            CS_ASSERT(m_texture != nullptr, "StandardDrawable cannot get preferred size without texture");
            return Core::Vector2((f32)m_texture->GetWidth() * m_UVs.m_s, (f32)m_texture->GetHeight() * m_UVs.m_t);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StandardDrawable::Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            CS_ASSERT(m_texture != nullptr, "StandardDrawable cannot draw without texture");
            in_renderer->DrawBox(in_transform, in_absSize, Core::Vector2::k_zero, m_texture, m_UVs, in_absColour, Rendering::AlignmentAnchor::k_middleCentre);
        }
    }
}