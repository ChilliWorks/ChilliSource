//
//  PropertyTypes.h
//  Chilli Source
//  Created by Ian Copland on 04/12/2014.
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_PROPERTY_DEFAULTPROPERTYTYPES_H_
#define _CHILLISOURCE_CORE_CONTAINER_PROPERTY_DEFAULTPROPERTYTYPES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChillISource/Core/Base/Colour.h>
#include <ChillISource/Core/Container/Property/PropertyType.h>
#include <ChillISource/Core/File/StorageLocation.h>
#include <ChillISource/Core/Math/Matrix3.h>
#include <ChillISource/Core/Math/Matrix4.h>
#include <ChillISource/Core/Math/Quaternion.h>
#include <ChillISource/Core/Math/Vector2.h>
#include <ChillISource/Core/Math/Vector3.h>
#include <ChillISource/Core/Math/Vector4.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// The list of default property types. Other property types can be added
        /// in addition to these.
        ///
        /// @param Ian Copland
        //------------------------------------------------------------------------
        namespace PropertyTypes
        {
            extern const PropertyType<bool> k_bool;
            extern const PropertyType<s32> k_int;
            extern const PropertyType<u32> k_uint;
            extern const PropertyType<f32> k_float;
            extern const PropertyType<Vector2> k_vec2;
            extern const PropertyType<Vector3> k_vec3;
            extern const PropertyType<Vector4> k_vec4;
            extern const PropertyType<Matrix3> k_mat3;
            extern const PropertyType<Matrix4> k_mat4;
            extern const PropertyType<Quaternion> k_quaternion;
            extern const PropertyType<Colour> k_colour;
            extern const PropertyType<StorageLocation> k_storageLocation;
            extern const PropertyType<std::string> k_string;
        }
    }
}

#endif
