//
//  PropertyTypes.cpp
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

#include <ChilliSource/Core/Container/Property/PropertyTypes.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace PropertyTypes
        {
            const PropertyType<bool> k_bool("Bool", false, &ParseBool);
            const PropertyType<s32> k_int("Int", 0, &ParseS32);
            const PropertyType<u32> k_uint("Uint", 0, &ParseU32);
            const PropertyType<f32> k_float("Float", 0.0f, &ParseF32);
            const PropertyType<Vector2> k_vec2("Vec2", Vector2(), &ParseVector2);
            const PropertyType<Vector3> k_vec3("Vec3", Vector3(), &ParseVector3);
            const PropertyType<Vector4> k_vec4("Vec4", Vector4(), &ParseVector4);
            const PropertyType<Matrix3> k_mat3("Mat3", Matrix3(), &ParseMatrix3);
            const PropertyType<Matrix4> k_mat4("Mat4", Matrix4(), &ParseMatrix4);
            const PropertyType<Quaternion> k_quaternion("Quaternion", Quaternion(), &ParseQuaternion);
            const PropertyType<Colour> k_colour("Colour", Colour(), &ParseColour);
            const PropertyType<StorageLocation> k_storageLocation("Quaternion", Core::StorageLocation::k_package, &ParseStorageLocation);
            const PropertyType<std::string> k_string("String", "", [](const std::string& in_string){return in_string;});
        }
    }
}
