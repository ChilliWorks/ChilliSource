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
#include <ChilliSource/Core/Container/Property/PropertyType.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// The list of default property types. Other property types can be added
        /// in addition to these.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        namespace PropertyTypes
        {
            CS_DECLARE_PROPERTYTYPE(bool, Bool);
            CS_DECLARE_PROPERTYTYPE(s32, Int);
            CS_DECLARE_PROPERTYTYPE(u32, UInt);
            CS_DECLARE_PROPERTYTYPE(f32, Float);
            CS_DECLARE_PROPERTYTYPE(Vector2, Vector2);
            CS_DECLARE_PROPERTYTYPE(Vector3, Vector3);
            CS_DECLARE_PROPERTYTYPE(Vector4, Vector4);
            CS_DECLARE_PROPERTYTYPE(Matrix3, Matrix3);
            CS_DECLARE_PROPERTYTYPE(Matrix4, Matrix4);
            CS_DECLARE_PROPERTYTYPE(Quaternion, Quaternion);
            CS_DECLARE_PROPERTYTYPE(Colour, Colour);
            CS_DECLARE_PROPERTYTYPE(StorageLocation, StorageLocation);
            CS_DECLARE_PROPERTYTYPE(std::string, String);
        }
    }
}

#endif
