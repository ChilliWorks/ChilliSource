//
//  AESEncrypt.h
//  Chilli Source
//  Created by Scott Downie on 25/09/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_CRYPTOGRAPHIC_AESENCRYPT_H_
#define _CHILLISOURCE_CORE_CRYPTOGRAPHIC_AESENCRYPT_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        //--------------------------------------------------------
        /// A collection of methods for working with AES encrpytion.
        /// These are not threadsafe and should not be called from
        /// multiple threads at the same time.
        ///
        /// @author S Downie
        //--------------------------------------------------------
        namespace AESEncrypt
        {
            //---------------------------------------------------
            /// @author S Downie
            ///
            /// @param The data size.
            ///
            /// @return The smallest encryptable size for the given
            /// data size. Data size must by 16 byte aligned to be
            /// encrypted.
            //---------------------------------------------------
            u32 CalculateAlignedSize(u32 in_dataSize);
            //---------------------------------------------------
            /// Encrypts the data with the given private key.
            ///
            /// @author S Downie
            ///
            /// @param The data.
            /// @param The data size. Data size must by 16 byte
            /// aligned to be encrypted.
            /// @param The private key.
            /// @param The output encrypted data.
            //---------------------------------------------------
            void Encrypt(const u8* in_data, u32 in_dataLength, const std::string& in_privateKey, u8* out_data);
            //---------------------------------------------------
            /// Decrypts the data with the given private key.
            ///
            /// @author S Downie
            ///
            /// @param The encrypted data.
            /// @param The data size. Data size must by 16 byte
            /// aligned to be decrypted.
            /// @param The private key.
            /// @param The output decrypted data.
            //---------------------------------------------------
            void Decrypt(const u8* in_data, u32 in_dataLength, const std::string& in_privateKey, u8* out_data);
        };
    }
}

#endif

