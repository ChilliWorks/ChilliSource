//
//  AESEncrypt.h
//  Chilli Source
//
//  Created by Scott Downie on 25/09/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_CRYPTOGRAPHIC_AESENCRYPT_H_
#define _CHILLISOURCE_CORE_CRYPTOGRAPHIC_AESENCRYPT_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace AESEncrypt
        {
            u32 CalculateAlignedSize(u32 inudwDataSize);
            void Encrypt(const u8* inpData, u32 inudwDataLength, const std::string& instrPrivateKey, u8* outpData);
            void Decrypt(const u8* inpData, u32 inudwDataLength, const std::string& instrPrivateKey, u8* outpData);
        };
    }
}

#endif

