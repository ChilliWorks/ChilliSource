//
//  AESEncrypt.cpp
//  MoFlow
//
//  Created by Scott Downie on 25/09/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Cryptographic/AESEncrypt.h>
#include <ChilliSource/Core/Cryptographic/aes.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace AESEncrypt
        {
            namespace
            {
                const u32 kudwAESBlockSize = 16;
            }
            u32 CalculateAlignedSize(u32 inudwDataSize)
            {
                u32 udwMod (inudwDataSize % kudwAESBlockSize);
                
                if(udwMod == 0)
                {
                    //already aligned, no need to add
                    return inudwDataSize;
                }
                
                u32 udwEncryptedSize = inudwDataSize + (kudwAESBlockSize - udwMod); // otherwise add the difference to the next boundary
                return udwEncryptedSize;
            }

            void Encrypt(const u8* inpData, u32 inudwDataLength, const std::string& instrPrivateKey, u8* outpData)
            {
                CS_ASSERT(instrPrivateKey.size() == 16, "AES Encryption keys must be 16 bytes");
                
                AES_KEY sKey;
                if(AES_set_encrypt_key((const u8*)instrPrivateKey.data(), 128, &sKey) != 0)
                {
                    CS_LOG_FATAL("AES: Cannot set decryption key");
                }
                
                s32 dwBytesRemaining = (s32)inudwDataLength;
                
                const u8* pReadData = inpData;
                u8* pWriteData = outpData;
                
                while(dwBytesRemaining > 0)
                {
                    AES_encrypt(pReadData, pWriteData, &sKey);
                    
                    pReadData += kudwAESBlockSize;
                    pWriteData += kudwAESBlockSize;
                    dwBytesRemaining -= kudwAESBlockSize;
                }
            }
            
            void Decrypt(const u8* inpData, u32 inudwDataLength, const std::string& instrPrivateKey, u8* outpData)
            {
                CS_ASSERT(instrPrivateKey.size() == 16, "AES Encryption keys must be 16 bytes");
                
                AES_KEY sKey;
                if(AES_set_decrypt_key((const u8*)instrPrivateKey.data(), 128, &sKey) != 0)
                {
                    CS_LOG_FATAL("AES: Cannot set decryption key");
                }

                s32 dwBytesRemaining = (s32)inudwDataLength;
                
                const u8* pReadData = inpData;
                u8* pWriteData = outpData;
                
                while(dwBytesRemaining > 0)
                {
                    AES_decrypt(pReadData, pWriteData, &sKey);
                    pReadData += kudwAESBlockSize;
                    pWriteData += kudwAESBlockSize;
                    dwBytesRemaining -= kudwAESBlockSize;
                }
            }
        }
    }
}
