//
//  HashSHA256.cpp
//  ChilliSource
//  Created by Jordan Brown on 24/02/2017.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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


#include <ChilliSource/Core/Cryptographic/HashSHA256.h>

#include <SHA256/SHA256.h>

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#endif

namespace ChilliSource
{
    namespace HashSHA256
    {
        //------------------------------------------------
        //------------------------------------------------
        std::string GenerateHexHashCode(const s8* in_data, u32 in_size)
        {         
            SHA256 hash;
            hash.reset();
            hash.add(reinterpret_cast<const u8*>(in_data), in_size);
 
            return hash.getHash();
        }
    }
}
