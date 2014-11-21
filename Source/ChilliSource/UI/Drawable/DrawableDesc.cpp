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

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        DrawableDesc::DrawableDesc(const Json::Value& in_json)
        {
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
                    m_relativeMargins = Core::ParseVector4(value);
                }
                else if (key == "TexturePath")
                {
                    m_absoluteMargins = Core::ParseVector4(value);
                }
                else if (key == "AtlasLocation")
                {
                    m_numRows = Core::ParseU32(value);
                }
                else if (key == "AtlasPath")
                {
                    m_numCols = Core::ParseU32(value);
                }
                else if (key == "AtlasId")
                {
                    m_relativeHSpacing = Core::ParseF32(value);
                }
                else if (key == "UVs")
                {
                    m_absoluteHSpacing = Core::ParseF32(value);
                }
            
                else if (key == "Type")
                {
                    //ignore
                }
                else if (key == "Type")
                {
                    //ignore
                }
                else
                {
                    CS_LOG_FATAL("Invalid property found in a " + ToString(m_type) + " layout description: " + key);
                }
                
                /// "NinePatchInsets"
                /// "ThreePatchInsets"
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
    }
}

#endif
