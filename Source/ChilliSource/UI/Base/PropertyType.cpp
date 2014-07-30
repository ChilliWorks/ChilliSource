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

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyType ParsePropertyType(const std::string& in_type)
        {
            if(in_type == "Int") return PropertyType::k_int;
            if(in_type == "Bool") return PropertyType::k_bool;
            if(in_type == "Float") return PropertyType::k_float;
            if(in_type == "String") return PropertyType::k_string;
            if(in_type == "Vec2") return PropertyType::k_vec2;
            if(in_type == "Vec3") return PropertyType::k_vec3;
            if(in_type == "Vec4") return PropertyType::k_vec4;
            if(in_type == "SizePolicy") return PropertyType::k_sizePolicy;
            if(in_type == "AlignmentAnchor") return PropertyType::k_alignmentAnchor;
            if(in_type == "Colour") return PropertyType::k_colour;
            if(in_type == "PropertyMap") return PropertyType::k_propertyMap;
            
            CS_LOG_FATAL("Cannot parse property type: " + in_type);
            return PropertyType::k_unknown;
        }
    }
}