//
//  StandardDrawableDef.cpp
//  Chilli Source
//  Created by Ian Copland on 15/12/2014.
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

#include <ChilliSource/UI/Drawable/StandardDrawableDef.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/UI/Drawable/StandardDrawable.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        CS_DEFINE_NAMEDTYPE(StandardDrawableDef);
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        StandardDrawableDef::StandardDrawableDef(const Json::Value& in_json, Core::StorageLocation in_defaultLocation, const std::string& in_defaultPath)
        {
            const char k_typeKey[] = "Type";
            const char k_textureLocationKey[] = "TextureLocation";
            const char k_textureFilePathKey[] = "TexturePath";
            const char k_atlasLocationKey[] = "AtlasLocation";
            const char k_atlasFilePathKey[] = "AtlasPath";
            const char k_atlasIdKey[] = "AtlasId";
            const char k_uvsKey[] = "UVs";
            const char k_colourKey[] = "Colour";
            
            CS_ASSERT(in_json.isObject() == true, "Drawable Def must be created from a json value of type Object.");
            
            //read all the properties from JSON.
            Core::StorageLocation textureLocation = Core::StorageLocation::k_none;
            Core::StorageLocation atlasLocation = Core::StorageLocation::k_none;
            std::string texturePath;
            std::string atlasPath;
            for(auto it = in_json.begin(); it != in_json.end(); ++it)
            {
                CS_ASSERT((*it).isString() == true, "All properties in a Drawable Def must be a string: " + std::string(it.memberName()));
                
                std::string key = it.memberName();
                std::string value = (*it).asString();
                
                if (key == k_textureLocationKey)
                {
                    textureLocation = Core::ParseStorageLocation(value);
                }
                else if (key == k_textureFilePathKey)
                {
                    texturePath = value;
                }
                else if (key == k_atlasLocationKey)
                {
                    atlasLocation = Core::ParseStorageLocation(value);
                }
                else if (key == k_atlasFilePathKey)
                {
                    atlasPath = value;
                }
                else if (key == k_atlasIdKey)
                {
                    m_atlasId = value;
                }
                else if (key == k_uvsKey)
                {
                    auto vec =  Core::ParseVector4(value);;
                    m_uvs = Rendering::UVs(vec.x, vec.y, vec.z, vec.w);
                }
                else if (key == k_colourKey)
                {
                    m_colour = Core::ParseColour(value);
                }
                else if (key == k_typeKey)
                {
                    //ignore
                }
                else
                {
                    CS_LOG_FATAL("Invalid property found in a Standard Drawable Def: " + key);
                }
            }
            
            //load the texture.
            CS_ASSERT(texturePath.empty() == false, "A texture must be supplied in a Standard Drawable Def.")
            
            auto resPool = Core::Application::Get()->GetResourcePool();
            if (textureLocation == Core::StorageLocation::k_none)
            {
                textureLocation = in_defaultLocation;
                texturePath = Core::StringUtils::StandardiseDirectoryPath(in_defaultPath) + texturePath;
            }
            
            m_texture = resPool->LoadResource<Rendering::Texture>(textureLocation, texturePath);
            CS_ASSERT(m_texture != nullptr, "Invalid texture supplied in a Standard Drawable Def.");
            
            //try and load the atlas
            if (atlasPath.empty() == false)
            {
                if (atlasLocation == Core::StorageLocation::k_none)
                {
                    atlasLocation = in_defaultLocation;
                    atlasPath = Core::StringUtils::StandardiseDirectoryPath(in_defaultPath) + atlasPath;
                }
                
                m_atlas = resPool->LoadResource<Rendering::TextureAtlas>(atlasLocation, atlasPath);
                CS_ASSERT(m_texture, "Invalid texture atlas supplied in a Standard Drawable Def.");
                CS_ASSERT(m_atlasId.empty() == false, "A texture atlas Id must be specified when using a texture atlas in a Standard Drawable Def.");
            }
            else
            {
                CS_ASSERT(m_atlasId.empty() == true, "Cannot specify a texture atlas Id without a texture atlas in a Standard Drawable Def.");
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        StandardDrawableDef::StandardDrawableDef(const Rendering::TextureCSPtr& in_texture, const Rendering::UVs& in_uvs, const Core::Colour& in_colour)
            : m_texture(in_texture), m_uvs(in_uvs), m_colour(in_colour)
        {
            CS_ASSERT(m_texture != nullptr, "The texture cannot be null in a Standard Drawable Def.");
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        StandardDrawableDef::StandardDrawableDef(const Rendering::TextureCSPtr& in_texture, const Rendering::TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId, const Rendering::UVs& in_uvs, const Core::Colour& in_colour)
            : m_texture(in_texture), m_atlas(in_atlas), m_atlasId(in_atlasId), m_uvs(in_uvs), m_colour(in_colour)
        {
            CS_ASSERT(m_texture != nullptr, "The texture cannot be null in a Standard Drawable Def.");
            CS_ASSERT(m_atlas != nullptr, "Cannot specify a null texture atlas in a Standard Drawable Def. Use the texture only constructor instead.");
            CS_ASSERT(m_atlas->HasFrameWithId(m_atlasId) == true, "Invalid texture atlas Id provided in a Standard Drawable Def.");
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool StandardDrawableDef::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (DrawableDef::InterfaceID == in_interfaceId || StandardDrawableDef::InterfaceID == in_interfaceId);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Rendering::TextureCSPtr& StandardDrawableDef::GetTexture() const
        {
            return m_texture;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Rendering::TextureAtlasCSPtr& StandardDrawableDef::GetAtlas() const
        {
            return m_atlas;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const std::string& StandardDrawableDef::GetAtlasId() const
        {
            return m_atlasId;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Rendering::UVs& StandardDrawableDef::GetUVs() const
        {
            return m_uvs;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Core::Colour& StandardDrawableDef::GetColour() const
        {
            return m_colour;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        DrawableUPtr StandardDrawableDef::CreateDrawable() const
        {
            StandardDrawableUPtr drawable;
            
            if (m_atlas != nullptr)
            {
                drawable = StandardDrawableUPtr(new StandardDrawable(m_texture, m_atlas, m_atlasId));
            }
            else
            {
                drawable = StandardDrawableUPtr(new StandardDrawable(m_texture));
            }
            
            drawable->SetUVs(m_uvs);
            drawable->SetColour(m_colour);

            DrawableUPtr output = std::move(drawable);
            return output;
        }
    }
}
