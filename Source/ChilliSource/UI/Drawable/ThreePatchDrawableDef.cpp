//
//  ThreePatchDrawableDef.cpp
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

#include <ChilliSource/UI/Drawable/ThreePatchDrawableDef.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            //--------------------------------------------------------------
            /// Parses a three-patch drawable direction from the given
            /// string. If the given string is not a valid direction the app
            /// is considered to be in an irrecoverable state and will
            /// terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The string to parse.
            ///
            /// @return The output three-patch drawable direction.
            //--------------------------------------------------------------
            ThreePatchDrawable::Direction ParseThreePatchDirection(const std::string& in_directionString)
            {
                const char k_horizontalType[] = "horizontal";
                const char k_verticalType[] = "vertical";
                
                std::string directionString = in_directionString;
                Core::StringUtils::ToLowerCase(directionString);
                
                if (directionString == k_horizontalType)
                {
                    return ThreePatchDrawable::Direction::k_horizontal;
                }
                else if (directionString == k_verticalType)
                {
                    return ThreePatchDrawable::Direction::k_vertical;
                }
                
                CS_LOG_FATAL("Cannot parse invalid three-patch direction.");
                return ThreePatchDrawable::Direction::k_horizontal;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(ThreePatchDrawableDef);
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        ThreePatchDrawableDef::ThreePatchDrawableDef(const Json::Value& in_json, Core::StorageLocation in_defaultLocation, const std::string& in_defaultPath)
        {
            const char k_typeKey[] = "Type";
            const char k_textureLocationKey[] = "TextureLocation";
            const char k_textureFilePathKey[] = "TexturePath";
            const char k_atlasLocationKey[] = "AtlasLocation";
            const char k_atlasFilePathKey[] = "AtlasPath";
            const char k_atlasIdKey[] = "AtlasId";
            const char k_uvsKey[] = "UVs";
            const char k_colourKey[] = "Colour";
            const char k_insetsKey[] = "Insets";
            const char k_directionKey[] = "Direction";
            
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
                else if (key == k_insetsKey)
                {
                    m_insets = Core::ParseVector2(value);
                }
                else if (key == k_directionKey)
                {
                    m_direction = ParseThreePatchDirection(value);
                }
                else if (key == k_typeKey)
                {
                    //ignore
                }
                else
                {
                    CS_LOG_FATAL("Invalid property found in a Three-Patch Drawable Def: " + key);
                }
            }
            
            //load the texture.
            CS_ASSERT(texturePath.empty() == false, "A texture must be supplied in a Three-Patch Drawable Def.")
            
            auto resPool = Core::Application::Get()->GetResourcePool();
            if (textureLocation == Core::StorageLocation::k_none)
            {
                textureLocation = in_defaultLocation;
                texturePath = Core::StringUtils::StandardiseDirectoryPath(in_defaultPath) + texturePath;
            }
            
            m_texture = resPool->LoadResource<Rendering::Texture>(textureLocation, texturePath);
            CS_ASSERT(m_texture != nullptr, "Invalid texture supplied in a Three-Patch Drawable Def.");
            
            //try and load the atlas
            if (atlasPath.empty() == false)
            {
                if (atlasLocation == Core::StorageLocation::k_none)
                {
                    atlasLocation = in_defaultLocation;
                    atlasPath = Core::StringUtils::StandardiseDirectoryPath(in_defaultPath) + atlasPath;
                }
                
                m_atlas = resPool->LoadResource<Rendering::TextureAtlas>(atlasLocation, atlasPath);
                CS_ASSERT(m_texture, "Invalid texture atlas supplied in a Three-Patch Drawable Def.");
                CS_ASSERT(m_atlasId.empty() == false, "A texture atlas Id must be specified when using a texture atlas in a Three-Patch Drawable Def.");
            }
            else
            {
                CS_ASSERT(m_atlasId.empty() == true, "Cannot specify a texture atlas Id without a texture atlas in a Three-Patch Drawable Def.");
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        ThreePatchDrawableDef::ThreePatchDrawableDef(const Rendering::TextureCSPtr& in_texture, const Rendering::UVs& in_uvs, const Core::Colour& in_colour, const Core::Vector2& in_insets, ThreePatchDrawable::Direction in_direction)
            : m_texture(in_texture), m_uvs(in_uvs), m_colour(in_colour), m_insets(in_insets), m_direction(in_direction)
        {
            CS_ASSERT(m_texture != nullptr, "The texture cannot be null in a Three-Patch Drawable Def.");
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        ThreePatchDrawableDef::ThreePatchDrawableDef(const Rendering::TextureCSPtr& in_texture, const Rendering::TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId, const Rendering::UVs& in_uvs,
                                                     const Core::Colour& in_colour, const Core::Vector2& in_insets, ThreePatchDrawable::Direction in_direction)
            : m_texture(in_texture), m_atlas(in_atlas), m_atlasId(in_atlasId), m_uvs(in_uvs), m_colour(in_colour), m_insets(in_insets), m_direction(in_direction)
        {
            CS_ASSERT(m_texture != nullptr, "The texture cannot be null in a Three-Patch Drawable Def.");
            CS_ASSERT(m_atlas != nullptr, "Cannot specify a null texture atlas in a Three-Patch Drawable Def. Use the texture only constructor instead.");
            CS_ASSERT(m_atlas->HasFrameWithId(m_atlasId) == true, "Invalid texture atlas Id provided in a Three-Patch Drawable Def.");
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool ThreePatchDrawableDef::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (DrawableDef::InterfaceID == in_interfaceId || ThreePatchDrawableDef::InterfaceID == in_interfaceId);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Rendering::TextureCSPtr& ThreePatchDrawableDef::GetTexture() const
        {
            return m_texture;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Rendering::TextureAtlasCSPtr& ThreePatchDrawableDef::GetAtlas() const
        {
            return m_atlas;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const std::string& ThreePatchDrawableDef::GetAtlasId() const
        {
            return m_atlasId;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Rendering::UVs& ThreePatchDrawableDef::GetUVs() const
        {
            return m_uvs;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Core::Colour& ThreePatchDrawableDef::GetColour() const
        {
            return m_colour;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Core::Vector2& ThreePatchDrawableDef::GetInsets() const
        {
            return m_insets;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        ThreePatchDrawable::Direction ThreePatchDrawableDef::GetDirection() const
        {
            return m_direction;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        DrawableUPtr ThreePatchDrawableDef::CreateDrawable() const
        {
            ThreePatchDrawableUPtr drawable;
            
            if (m_atlas != nullptr)
            {
                drawable = ThreePatchDrawableUPtr(new ThreePatchDrawable(m_texture, m_atlas, m_atlasId, m_direction, m_insets.x, m_insets.y));
            }
            else
            {
                drawable = ThreePatchDrawableUPtr(new ThreePatchDrawable(m_texture, m_direction, m_insets.x, m_insets.y));
            }
            
            drawable->SetUVs(m_uvs);
            drawable->SetColour(m_colour);
            
            DrawableUPtr output = std::move(drawable);
            return output;
        }
    }
}
