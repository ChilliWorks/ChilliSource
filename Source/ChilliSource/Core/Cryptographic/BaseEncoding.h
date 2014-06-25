//
//  BaseEncoding.h
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

#ifndef _CHILLISOURCE_CORE_CRYPTOGRAPHIC_BASEENCODING_H_
#define _CHILLISOURCE_CORE_CRYPTOGRAPHIC_BASEENCODING_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        //-------------------------------------------------------------
        /// A collection of methods for working with Base 64 encoding.
        ///
        /// @author S Downie
        //-------------------------------------------------------------
        namespace BaseEncoding
        {
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param String to encode
            ///
            /// @return Base 64 encoded string
            //-------------------------------------------------------
            std::string Base64Encode(const std::string& in_value);
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Data to encode
            /// @param Size of data in bytes
            ///
            /// @return Base 64 encoded string
            //-------------------------------------------------------
            std::string Base64Encode(const s8* in_data, u32 in_size);
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Base 64 encoded string
            ///
            /// @return Decoded string
            //-------------------------------------------------------
            std::string Base64Decode(const std::string& in_value);

        }
    }
}

#endif