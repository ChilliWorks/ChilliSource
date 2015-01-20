//
//  DrawableDef.cpp
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

#include <ChilliSource/UI/Drawable/DrawableDef.h>

#include <ChilliSource/UI/Drawable/NinePatchDrawableDef.h>
#include <ChilliSource/UI/Drawable/StandardDrawableDef.h>
#include <ChilliSource/UI/Drawable/ThreePatchDrawableDef.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            //--------------------------------------------------------------
            /// Identifiers for the different drawable definition types.
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            enum class DrawableType
            {
                k_none,
                k_standard,
                k_ninePatch,
                k_threePatch
            };
            //--------------------------------------------------------------
            /// Converts a string to a drawable type
            ///
            /// @author S Downie
            ///
            /// @param String
            ///
            /// @return Drawable type
            //--------------------------------------------------------------
            DrawableType ParseDrawableType(const std::string& in_type)
            {
                std::string lowerCase = in_type;
                Core::StringUtils::ToLowerCase(lowerCase);
                
                if(lowerCase == "none") return DrawableType::k_none;
                if(lowerCase == "standard") return DrawableType::k_standard;
                if(lowerCase == "ninepatch") return DrawableType::k_ninePatch;
                if(lowerCase == "threepatch") return DrawableType::k_threePatch;
                
                CS_LOG_FATAL("Cannot parse drawable type: " + in_type);
                return DrawableType::k_none;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(DrawableDef);
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        DrawableDefCUPtr DrawableDef::Create(const Json::Value& in_json, Core::StorageLocation in_defaultLocation, const std::string& in_defaultPath)
        {
            const char k_typeKey[] = "Type";
            
            CS_ASSERT(in_json.isObject() == true, "Drawable Def must be created from a json value of type Object.");
            
            const auto& typeJson = in_json.get(k_typeKey, Json::nullValue);
            CS_ASSERT(typeJson != Json::nullValue, "'" + std::string(k_typeKey) + "' must be specified in a Drawable Def.");
            
            DrawableType type = ParseDrawableType(typeJson.asString());
            
            switch (type)
            {
                case DrawableType::k_standard:
                    return DrawableDefCUPtr(new StandardDrawableDef(in_json, in_defaultLocation, in_defaultPath));
                case DrawableType::k_threePatch:
                    return DrawableDefCUPtr(new ThreePatchDrawableDef(in_json, in_defaultLocation, in_defaultPath));
                case DrawableType::k_ninePatch:
                    return DrawableDefCUPtr(new NinePatchDrawableDef(in_json, in_defaultLocation, in_defaultPath));
                case DrawableType::k_none:
                    return nullptr;
                default:
                    CS_LOG_FATAL("Invalid drawable def type.");
                    return nullptr;
            }
        }
        
    }
}
