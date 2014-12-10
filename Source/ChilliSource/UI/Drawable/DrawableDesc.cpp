//
//  DrawableDesc.cpp
//  Chilli Source
//  Created by Ian Copland on 21/11/2014.
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

#include <ChilliSource/UI/Drawable/DrawableDesc.h>

#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <json/json.h>

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
        
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        DrawableDesc::DrawableDesc(const Json::Value& in_json, Core::StorageLocation in_relStorageLocation, const std::string& in_relDirectoryPath)
        {
            const char k_typeKey[] = "Type";
            const char k_textureLocationKey[] = "TextureLocation";
            const char k_textureFilePathKey[] = "TexturePath";
            const char k_atlasLocationKey[] = "AtlasLocation";
            const char k_atlasFilePathKey[] = "AtlasPath";
            const char k_atlasIdKey[] = "AtlasId";
            const char k_uvsKey[] = "UVs";
            const char k_ninePatchInsetsKey[] = "Insets";
            const char k_threePatchInsetsKey[] = "Insets";
            const char k_threePatchDirectionKey[] = "Direction";
            
            CS_ASSERT(in_json.isObject() == true, "Drawable description must be created from a json value of type Object.");
            
            const auto& typeJson = in_json.get(k_typeKey, Json::nullValue);
            CS_ASSERT(typeJson != Json::nullValue, "'" + std::string(k_typeKey) + "' must be specified in a Drawable Description.");
            
            m_type = ParseDrawableType(typeJson.asString());
            
            for(auto it = in_json.begin(); it != in_json.end(); ++it)
            {
                CS_ASSERT((*it).isString() == true, "All properties in a Drawable Description must be a string: " + std::string(it.memberName()));
                
                std::string key = it.memberName();
                std::string value = (*it).asString();
                
                if (key == k_textureLocationKey)
                {
                    m_textureLocation = Core::ParseStorageLocation(value);
                }
                else if (key == k_textureFilePathKey)
                {
                    m_texturePath = value;
                }
                else if (key == k_atlasLocationKey)
                {
                    m_atlasLocation = Core::ParseStorageLocation(value);
                }
                else if (key == k_atlasFilePathKey)
                {
                    m_atlasPath = value;
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
                else if (key == k_ninePatchInsetsKey && m_type == DrawableType::k_ninePatch)
                {
                    m_ninePatchInsets = Core::ParseVector4(value);
                }
                else if (key == k_threePatchInsetsKey && m_type == DrawableType::k_threePatch)
                {
                    m_threePatchInsets = Core::ParseVector2(value);
                }
                else if (key == k_threePatchDirectionKey && m_type == DrawableType::k_threePatch)
                {
                    m_threePatchDirection = ParseThreePatchDirection(value);
                }
                else if (key == k_typeKey)
                {
                    //ignore
                }
                else
                {
                    CS_LOG_FATAL("Invalid property found in a " + ToString(m_type) + " layout description: " + key);
                }
            }
            
            CS_ASSERT(m_texturePath.empty() == false, "Texture must be supplied in a drawable.");
            CS_ASSERT((m_atlasPath == "" && m_atlasId == "") || (m_atlasPath != "" && m_atlasId != ""), "Both the atlas id and atlas path must be specified when using a texture atlas in a widget drawable.");
            
            //We need to support relative paths. When no storage location is specified in Json a path relative to the
            //parent resources directory will be used instead.
            if (in_json.isMember(k_textureLocationKey) == false)
            {
                m_textureLocation = in_relStorageLocation;
                m_texturePath = Core::StringUtils::StandardiseDirectoryPath(in_relDirectoryPath) + m_texturePath;
            }
            
            if (m_atlasPath.empty() == false && in_json.isMember(k_atlasLocationKey) == false)
            {
                m_atlasLocation = in_relStorageLocation;
                m_atlasPath = Core::StringUtils::StandardiseDirectoryPath(in_relDirectoryPath) + m_atlasPath;
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        DrawableType DrawableDesc::GetType() const
        {
            return m_type;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        Core::StorageLocation DrawableDesc::GetTextureLocation() const
        {
            return m_textureLocation;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const std::string& DrawableDesc::GetTexturePath() const
        {
            return m_texturePath;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        Core::StorageLocation DrawableDesc::GetAtlasLocation() const
        {
            return m_atlasLocation;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const std::string& DrawableDesc::GetAtlasPath() const
        {
            return m_atlasPath;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const std::string& DrawableDesc::GetAtlasId() const
        {
            return m_atlasId;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Rendering::UVs& DrawableDesc::GetUVs() const
        {
            return m_uvs;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Core::Vector4& DrawableDesc::GetNinePatchInsets() const
        {
            return m_ninePatchInsets;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        const Core::Vector2& DrawableDesc::GetThreePatchInsets() const
        {
            return m_threePatchInsets;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        ThreePatchDrawable::Direction DrawableDesc::GetThreePatchDirection() const
        {
            return m_threePatchDirection;
        }
    }
}
