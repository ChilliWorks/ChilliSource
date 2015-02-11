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

#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/Math/Matrix3.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace PropertyTypes
        {
            CS_DEFINE_PROPERTYTYPE(bool, Bool, false, &ParseBool);
            CS_DEFINE_PROPERTYTYPE(s32, Int, 0, &ParseS32);
            CS_DEFINE_PROPERTYTYPE(u32, UInt, 0, &ParseU32);
            CS_DEFINE_PROPERTYTYPE(f32, Float, 0.0f, &ParseF32);
            CS_DEFINE_PROPERTYTYPE(Core::Vector2, Vector2, Core::Vector2(), &ParseVector2);
            CS_DEFINE_PROPERTYTYPE(Core::Vector3, Vector3, Core::Vector3(), &ParseVector3);
            CS_DEFINE_PROPERTYTYPE(Core::Vector4, Vector4, Core::Vector4(), &ParseVector4);
            CS_DEFINE_PROPERTYTYPE(Core::Matrix3, Matrix3, Core::Matrix3(), &ParseMatrix3);
            CS_DEFINE_PROPERTYTYPE(Core::Matrix4, Matrix4, Core::Matrix4(), &ParseMatrix4);
            CS_DEFINE_PROPERTYTYPE(Core::Quaternion, Quaternion, Core::Quaternion(), &ParseQuaternion);
            CS_DEFINE_PROPERTYTYPE(Core::Colour, Colour, Core::Colour(), &ParseColour);
            CS_DEFINE_PROPERTYTYPE(Core::StorageLocation, StorageLocation, Core::StorageLocation::k_package, &ParseStorageLocation);
            CS_DEFINE_PROPERTYTYPE(std::string, String, "", [](const std::string& in_string){return in_string;});
        }
    }
}
