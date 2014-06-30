//
//  HashMD5.cpp
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

#include <ChilliSource/Core/Cryptographic/HashMD5.h>

#include <md5/md5.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace HashMD5
        {
            //-------------------------------------------------------------
            //-------------------------------------------------------------
            std::string GenerateHexHashCode(const std::string &in_value)
            {
                MD5 Hash(in_value);
                return Hash.hexdigest();
            }
            //-------------------------------------------------------------
            //-------------------------------------------------------------
            std::string GenerateHexHashCode(const s8* in_value)
            {
                return GenerateHexHashCode(std::string(in_value));
            }
            //-------------------------------------------------------------
            //-------------------------------------------------------------
            std::string GenerateHexHashCode(const s8* in_value, u32 in_size)
            {
                MD5 Hash;
                Hash.update(in_value, in_size);
                Hash.finalize();
                return Hash.hexdigest();
            }
            //-------------------------------------------------------------
            //-------------------------------------------------------------
            std::string GenerateBinaryHashCode(const std::string &in_value)
            {
                MD5 Hash(in_value);
                return Hash.binarydigest();
            }
            //-------------------------------------------------------------
            //-------------------------------------------------------------
            std::string GenerateBinaryHashCode(const s8* in_value)
            {
                return GenerateBinaryHashCode(std::string(in_value));
            }
            //-------------------------------------------------------------
            //-------------------------------------------------------------
            std::string GenerateBinaryHashCode(const s8* in_value, u32 in_size)
            {
                MD5 Hash;
                Hash.update(in_value, in_size);
                Hash.finalize();
                return Hash.binarydigest();
            }
        }
    }
}
