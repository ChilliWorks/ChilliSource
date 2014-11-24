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
            /// string. If the given string is not a valid direction this
            /// will fatal log.
            ///
            /// @author Ian Copland
            ///
            /// @param The string to parse.
            ///
            /// @return The output three-patch drawable direction.
            //--------------------------------------------------------------
            ThreePatchDrawable::Direction ParseThreePatchDirection(const std::string& in_directionString)
            {
                std::string directionString = in_directionString;
                Core::StringUtils::ToLowerCase(directionString);
                
                if (directionString == "horizontal")
                {
                    return ThreePatchDrawable::Direction::k_horizontal;
                }
                else if (directionString == "vertical")
                {
                    return ThreePatchDrawable::Direction::k_vertical;
                }
                
                CS_LOG_FATAL("Cannot parse invalid three-patch direction.");
                return ThreePatchDrawable::Direction::k_horizontal;
            }
        }
        
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        DrawableDesc::DrawableDesc(const Json::Value& in_json)
        {
            CS_ASSERT(in_json.isObject() == true, "Drawable description must be created from a json value of type Object.");
            
            auto typeJson = in_json.get("Type", Json::nullValue);
            CS_ASSERT(typeJson != Json::nullValue, "Type must be specified in a Drawable Description.");
            
            m_type = ParseDrawableType(typeJson.asString());
            
            for(auto it = in_json.begin(); it != in_json.end(); ++it)
            {
                CS_ASSERT((*it).isString() == true, "All properties in a Drawable Description must be a string: " + std::string(it.memberName()));
                
                std::string key = it.memberName();
                std::string value = (*it).asString();
                
                if (key == "TextureLocation")
                {
                    m_textureLocation = Core::ParseStorageLocation(value);
                }
                else if (key == "TexturePath")
                {
                    m_texturePath = value;
                }
                else if (key == "AtlasLocation")
                {
                    m_atlasLocation = Core::ParseStorageLocation(value);
                }
                else if (key == "AtlasPath")
                {
                    m_atlasPath = value;
                }
                else if (key == "AtlasId")
                {
                    m_atlasId = value;
                }
                else if (key == "UVs")
                {
                    auto vec =  Core::ParseVector4(value);;
                    m_uvs = Rendering::UVs(vec.x, vec.y, vec.z, vec.w);
                }
                else if (key == "Insets" && m_type == DrawableType::k_ninePatch)
                {
                    m_ninePatchInsets = Core::ParseVector4(value);
                }
                else if (key == "Insets" && m_type == DrawableType::k_threePatch)
                {
                    m_threePatchInsets = Core::ParseVector2(value);
                }
                else if (key == "Direction" && m_type == DrawableType::k_threePatch)
                {
                    m_threePatchDirection = ParseThreePatchDirection(value);
                }
                else if (key == "Type")
                {
                    //ignore
                }
                else
                {
                    CS_LOG_FATAL("Invalid property found in a " + ToString(m_type) + " layout description: " + key);
                }
                
                CS_ASSERT((m_atlasPath == "" && m_atlasId == "") || (m_atlasPath != "" && m_atlasId != ""), "Both the atlas id and atlas path must be specified when using a texture atlas in a widget drawable.");
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
