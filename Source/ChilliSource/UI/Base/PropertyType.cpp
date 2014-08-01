//
//  PropertyType.cpp
//  Chilli Source
//  Created by Scott Downie on 25/07/2014.
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

#include <ChilliSource/UI/Base/PropertyType.h>

#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyType ParsePropertyType(const std::string& in_type)
        {
            std::string lowerCase = in_type;
            Core::StringUtils::ToLowerCase(lowerCase);
            
            if(lowerCase == "int") return PropertyType::k_int;
            if(lowerCase == "bool") return PropertyType::k_bool;
            if(lowerCase == "float") return PropertyType::k_float;
            if(lowerCase == "string") return PropertyType::k_string;
            if(lowerCase == "vec2") return PropertyType::k_vec2;
            if(lowerCase == "vec3") return PropertyType::k_vec3;
            if(lowerCase == "vec4") return PropertyType::k_vec4;
            if(lowerCase == "sizepolicy") return PropertyType::k_sizePolicy;
            if(lowerCase == "alignmentanchor") return PropertyType::k_alignmentAnchor;
            if(lowerCase == "colour") return PropertyType::k_colour;
            if(lowerCase == "propertymap") return PropertyType::k_propertyMap;
            
            CS_LOG_FATAL("Cannot parse property type: " + in_type);
            return PropertyType::k_unknown;
        }
    }
}