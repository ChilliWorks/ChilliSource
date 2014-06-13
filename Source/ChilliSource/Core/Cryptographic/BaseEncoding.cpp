//
//  BaseEncoding.cpp
//  Chilli Source
//  Created by Scott Downie on 06/07/2011.
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

#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>

#include <base64/base64.h>

#include <algorithm>

namespace ChilliSource
{
    namespace Core
    {
        namespace BaseEncoding
        {
            //---------------------------------------------------------
            //---------------------------------------------------------
            std::string Base64Encode(const std::string& in_value)
            {
                return Base64Encode(in_value.c_str(), in_value.length());
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            std::string Base64Encode(const s8* in_data, u32 in_size)
            {
                return base64_encode(reinterpret_cast<const u8*>(in_data), in_size);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            std::string Base64Decode(const std::string& in_value)
            {
                return base64_decode(in_value);
            }
        }
    }
}