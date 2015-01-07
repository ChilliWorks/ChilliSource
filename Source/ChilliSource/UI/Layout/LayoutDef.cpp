//
//  LayoutDef.cpp
//  Chilli Source
//  Created by Ian Copland on 07/01/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <ChilliSource/UI/Layout/LayoutDef.h>

#include <ChilliSource/UI/Layout/GridLayoutDef.h>
#include <ChilliSource/UI/Layout/HListLayoutDef.h>
#include <ChilliSource/UI/Layout/VListLayoutDef.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// Identifiers for classes that are responsible for laying out the child content
            /// of a widget
            ///
            /// @author S Downie
            //------------------------------------------------------------------------------
            enum class LayoutType
            {
                k_none,
                k_grid,
                k_hList,
                k_vList
            };
            //------------------------------------------------------------------------------
            /// Converts a string to a drawable type
            ///
            /// @author S Downie
            ///
            /// @param String
            ///
            /// @return Drawable type
            //------------------------------------------------------------------------------
            LayoutType ParseLayoutType(const std::string& in_type)
            {
                std::string lowerCase = in_type;
                Core::StringUtils::ToLowerCase(lowerCase);

                if(lowerCase == "none") return LayoutType::k_none;
                if(lowerCase == "grid") return LayoutType::k_grid;
                if(lowerCase == "hlist") return LayoutType::k_hList;
                if(lowerCase == "vlist") return LayoutType::k_vList;

                CS_LOG_FATAL("Cannot parse layout type: " + in_type);
                return LayoutType::k_none;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(LayoutDef);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        LayoutDefCUPtr LayoutDef::Create(const Json::Value& in_json)
        {
            const char k_typeKey[] = "Type";
            
            CS_ASSERT(in_json.isObject() == true, "Layout Def must be created from a json value of type Object.");
            
            const auto& typeJson = in_json.get(k_typeKey, Json::nullValue);
            CS_ASSERT(typeJson != Json::nullValue, "'" + std::string(k_typeKey) + "' must be specified in a Layout Def.");
            
            LayoutType type = ParseLayoutType(typeJson.asString());
            
            switch (type)
            {
                case LayoutType::k_grid:
                    return LayoutDefCUPtr(new GridLayoutDef(in_json));
                case LayoutType::k_hList:
                    return LayoutDefCUPtr(new HListLayoutDef(in_json));
                case LayoutType::k_vList:
                    return LayoutDefCUPtr(new VListLayoutDef(in_json));
                default:
                    CS_LOG_FATAL("Invalid layout def type.");
                    return nullptr;
            }
        }
        
    }
}
