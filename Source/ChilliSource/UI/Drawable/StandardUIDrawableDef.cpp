//
//  StandardUIDrawableDef.cpp
//  ChilliSource
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

#include <ChilliSource/UI/Drawable/StandardUIDrawableDef.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/CanvasDrawMode.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/UI/Drawable/StandardUIDrawable.h>

#include <json/json.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(StandardUIDrawableDef);
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    StandardUIDrawableDef::StandardUIDrawableDef(const Json::Value& in_json, StorageLocation in_defaultLocation, const std::string& in_defaultPath)
    : m_drawMode(CanvasDrawMode::k_standard)
    {
        const char k_typeKey[] = "Type";
        const char k_textureLocationKey[] = "TextureLocation";
        const char k_textureFilePathKey[] = "TexturePath";
        const char k_atlasLocationKey[] = "AtlasLocation";
        const char k_atlasFilePathKey[] = "AtlasPath";
        const char k_atlasIdKey[] = "AtlasId";
        const char k_uvsKey[] = "UVs";
        const char k_colourKey[] = "Colour";
        const char k_drawModeKey[] = "DrawMode";
        
        CS_ASSERT(in_json.isObject() == true, "UIDrawable Def must be created from a json value of type Object.");
        
        //read all the properties from JSON.
        StorageLocation textureLocation = StorageLocation::k_none;
        StorageLocation atlasLocation = StorageLocation::k_none;
        std::string texturePath;
        std::string atlasPath;
        for(auto it = in_json.begin(); it != in_json.end(); ++it)
        {
            CS_ASSERT((*it).isString() == true, "All properties in a UIDrawable Def must be a string: " + std::string(it.memberName()));
            
            std::string key = it.memberName();
            std::string value = (*it).asString();
            
            if (key == k_textureLocationKey)
            {
                textureLocation = ParseStorageLocation(value);
            }
            else if (key == k_textureFilePathKey)
            {
                texturePath = value;
            }
            else if (key == k_atlasLocationKey)
            {
                atlasLocation = ParseStorageLocation(value);
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
                auto vec =  ParseVector4(value);;
                m_uvs = UVs(vec.x, vec.y, vec.z, vec.w);
            }
            else if (key == k_colourKey)
            {
                m_colour = ParseColour(value);
            }
            else if (key == k_drawModeKey)
            {
                m_drawMode = ParseCanvasDrawMode(value);
            }
            else if (key == k_typeKey)
            {
                //ignore
            }
            else
            {
                CS_LOG_FATAL("Invalid property found in a Standard UIDrawable Def: " + key);
            }
        }
        
        //load the texture.
        CS_ASSERT(texturePath.empty() == false, "A texture must be supplied in a Standard UIDrawable Def.")
        
        auto resPool = Application::Get()->GetResourcePool();
        if (textureLocation == StorageLocation::k_none)
        {
            textureLocation = in_defaultLocation;
            texturePath = StringUtils::StandardiseDirectoryPath(in_defaultPath) + texturePath;
        }
        
        m_texture = resPool->LoadResource<Texture>(textureLocation, texturePath);
        CS_ASSERT(m_texture != nullptr, "Invalid texture supplied in a Standard UIDrawable Def.");
        
        //try and load the atlas
        if (atlasPath.empty() == false)
        {
            if (atlasLocation == StorageLocation::k_none)
            {
                atlasLocation = in_defaultLocation;
                atlasPath = StringUtils::StandardiseDirectoryPath(in_defaultPath) + atlasPath;
            }
            
            m_atlas = resPool->LoadResource<TextureAtlas>(atlasLocation, atlasPath);
            CS_ASSERT(m_texture, "Invalid texture atlas supplied in a Standard UIDrawable Def.");
            CS_ASSERT(m_atlasId.empty() == false, "A texture atlas Id must be specified when using a texture atlas in a Standard UIDrawable Def.");
        }
        else
        {
            CS_ASSERT(m_atlasId.empty() == true, "Cannot specify a texture atlas Id without a texture atlas in a Standard UIDrawable Def.");
        }
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    StandardUIDrawableDef::StandardUIDrawableDef(const TextureCSPtr& in_texture, const Colour& in_colour, const UVs& in_uvs)
    : m_texture(in_texture), m_colour(in_colour), m_uvs(in_uvs), m_drawMode(CanvasDrawMode::k_standard)
    {
        CS_ASSERT(m_texture != nullptr, "The texture cannot be null in a Standard UIDrawable Def.");
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    StandardUIDrawableDef::StandardUIDrawableDef(const TextureCSPtr& in_texture, const TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId, const Colour& in_colour, const UVs& in_uvs)
    : m_texture(in_texture), m_atlas(in_atlas), m_atlasId(in_atlasId), m_colour(in_colour), m_uvs(in_uvs), m_drawMode(CanvasDrawMode::k_standard)
    {
        CS_ASSERT(m_texture != nullptr, "The texture cannot be null in a Standard UIDrawable Def.");
        CS_ASSERT(m_atlas != nullptr, "Cannot specify a null texture atlas in a Standard UIDrawable Def. Use the texture only constructor instead.");
        CS_ASSERT(m_atlas->HasFrameWithId(m_atlasId) == true, "Invalid texture atlas Id provided in a Standard UIDrawable Def.");
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    bool StandardUIDrawableDef::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIDrawableDef::InterfaceID == in_interfaceId || StandardUIDrawableDef::InterfaceID == in_interfaceId);
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    const TextureCSPtr& StandardUIDrawableDef::GetTexture() const
    {
        return m_texture;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    const TextureAtlasCSPtr& StandardUIDrawableDef::GetAtlas() const
    {
        return m_atlas;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    const std::string& StandardUIDrawableDef::GetAtlasId() const
    {
        return m_atlasId;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    const UVs& StandardUIDrawableDef::GetUVs() const
    {
        return m_uvs;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    const Colour& StandardUIDrawableDef::GetColour() const
    {
        return m_colour;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    UIDrawableUPtr StandardUIDrawableDef::CreateDrawable() const
    {
        StandardUIDrawableUPtr drawable;
        
        if (m_atlas != nullptr)
        {
            drawable = StandardUIDrawableUPtr(new StandardUIDrawable(m_texture, m_atlas, m_atlasId));
        }
        else
        {
            drawable = StandardUIDrawableUPtr(new StandardUIDrawable(m_texture));
        }
        
        drawable->SetUVs(m_uvs);
        drawable->SetColour(m_colour);

        UIDrawableUPtr output = std::move(drawable);
        return output;
    }
}
