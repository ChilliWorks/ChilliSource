//
//  HashMD5.h
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

#ifndef _CHILLISOURCE_CORE_CRYPTOGRAPHIC_HASHMD5_H_
#define _CHILLISOURCE_CORE_CRYPTOGRAPHIC_HASHMD5_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace HashMD5
        {
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param String to hash
            ///
            /// @return MD5 hex hash code
            //-------------------------------------------------------------
            std::string GenerateHexHashCode(const std::string &in_value);
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Null terminated string to hash
            ///
            /// @return MD5 hex hash code
            //-------------------------------------------------------------
            std::string GenerateHexHashCode(const s8* in_value);
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Block of binary data
            /// @param Size of data
            ///
            /// @return MD5 hex hash code
            //-------------------------------------------------------------
            std::string GenerateHexHashCode(const s8* in_value, u32 in_size);
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param String to hash
            ///
            /// @return MD5 binary hash code
            //-------------------------------------------------------------
            std::string GenerateBinaryHashCode(const std::string &in_value);
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Null terminated string to hash
            ///
            /// @return MD5 binary hash code
            //-------------------------------------------------------------
            std::string GenerateBinaryHashCode(const s8* in_value);
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Block of binary data
            /// @param Size of data
            ///
            /// @return MD5 binary hash code
            //-------------------------------------------------------------
            std::string GenerateBinaryHashCode(const s8* in_value, u32 in_size);
        }
    }
}

#endif