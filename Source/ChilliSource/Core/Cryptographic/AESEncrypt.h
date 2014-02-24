//
//  AESEncrypt.h
//  MoFlow
//
//  Created by Scott Downie on 25/09/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_CORE_AESENCRYPT_H_
#define _MOFLOW_CORE_AESENCRYPT_H_

namespace ChilliSource
{
    class CAESEncrypt
    {
    public:
        
        static u32 CalculateAlignedSize(u32 inudwDataSize);
        static void Encrypt(const u8* inpData, u32 inudwDataLength, const std::string& instrPrivateKey, u8* outpData);
        static void Decrypt(const u8* inpData, u32 inudwDataLength, const std::string& instrPrivateKey, u8* outpData);
        
    private:
    };
}

#endif

