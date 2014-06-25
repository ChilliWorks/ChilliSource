//
//  Property.cpp
//  Chilli Source
//  Created by Scott Downie on 29/11/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Core/Reflection/Property.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource 
{
    namespace Core
    {
        namespace Reflect
        {
            //--------------------------------------------------------
            /// Get Name Hash
            ///
            /// @return The hashed name identifier of this property
            //--------------------------------------------------------
            u32 CProperty::GetNameHash() const
            {
                return mudwNameHash;
            }
            //--------------------------------------------------------
            /// Get Name
            ///
            /// @return The name identifier of this property
            //--------------------------------------------------------
            const std::string& CProperty::GetName() const
            {
                return mstrName;
            }
            //--------------------------------------------------------
            /// Get Type Hash
            ///
            /// @return The hashed type identifier of this property
            //--------------------------------------------------------
            u32 CProperty::GetTypeHash() const
            {
                return mudwTypeHash;
            }
            //--------------------------------------------------------
            /// Get Type
            ///
            /// @return The name identifier of this property type
            //--------------------------------------------------------
            const std::string& CProperty::GetType() const
            {
                return mstrType;
            }
            //--------------------------------------------------------
            /// Get Size
            ///
            /// @return Sizeof the property type
            //--------------------------------------------------------
            u32 CProperty::GetSize() const
            {
                return mudwSize;
            }
            //--------------------------------------------------------
            /// Get Class
            ///
            /// @return The meta class for this instance
            //--------------------------------------------------------
            CMetaClass* CProperty::GetClass() const
            {
                return mpClass;
            }
        }
    }
}

