//
//  HashSHA1.h
//  ChilliSource
//  Created by Ian Copland on 09/06/2014.
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

#ifndef _CHILLISOURCE_CORE_CRYTOGRAPHIC_HASHSHA1_H_
#define _CHILLISOURCE_CORE_CRYTOGRAPHIC_HASHSHA1_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    //------------------------------------------------------
    /// A collection of methods for working with the SHA1
    /// hashing algorithm.
    ///
    /// @author Ian Copland
    //------------------------------------------------------
    namespace HashSHA1
    {
        //------------------------------------------------
        /// Hashes the given data using SHA1. The output is
        /// returned as a hex string.
        ///
        /// @author Ian Copland
        ///
        /// @param The input data.
        /// @param The input data size.
        ///
        /// @return The output hex string.
        //------------------------------------------------
        std::string GenerateHexHashCode(const s8* in_data, u32 in_size);
    }
}

#endif
