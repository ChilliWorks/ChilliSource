//
//  AESEncrypt.cpp
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

#include <ChilliSource/Core/Cryptographic/AESEncrypt.h>

#include <aes/aes.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace AESEncrypt
        {
            namespace
            {
                const u32 k_aesBlockSize = 16;
            }
            
			//---------------------------------------------------------
			//---------------------------------------------------------
			Data::Data(Data&& in_moveable)
				: m_data(std::move(in_moveable.m_data)), m_size(in_moveable.m_size)
			{
			}
			//---------------------------------------------------------
			//---------------------------------------------------------
			Data& Data::operator=(Data&& in_moveable)
			{
				m_data = std::move(in_moveable.m_data);
				m_size = in_moveable.m_size;
				return *this;
			}
            //---------------------------------------------------
            //---------------------------------------------------
            u32 CalculateAlignedSize(u32 in_dataSize)
            {
                u32 udwMod (in_dataSize % k_aesBlockSize);
                
                if(udwMod == 0)
                {
                    //already aligned, no need to add
                    return in_dataSize;
                }
                
                u32 udwEncryptedSize = in_dataSize + (k_aesBlockSize - udwMod); // otherwise add the difference to the next boundary
                return udwEncryptedSize;
            }
            //---------------------------------------------------
            //---------------------------------------------------
            Data EncryptBinary(const u8* in_data, u32 in_dataLength, const std::string& in_privateKey)
            {
                CS_ASSERT(in_privateKey.size() == 16, "AES Encryption keys must be 16 bytes");
                
                AES_KEY sKey;
                if(AES_set_encrypt_key((const u8*)in_privateKey.data(), 128, &sKey) != 0)
                {
                    CS_LOG_FATAL("AES: Cannot set decryption key");
                }
                
                Data output;
                output.m_size = in_dataLength;
                output.m_data = std::unique_ptr<u8[]>(new u8[output.m_size]);
                
                s32 dwBytesRemaining = (s32)in_dataLength;
                const u8* pReadData = in_data;
                u8* pWriteData = output.m_data.get();
                
                while(dwBytesRemaining > 0)
                {
                    AES_encrypt(pReadData, pWriteData, &sKey);
                    
                    pReadData += k_aesBlockSize;
                    pWriteData += k_aesBlockSize;
                    dwBytesRemaining -= k_aesBlockSize;
                }
                
                return std::move(output);
            }
            //---------------------------------------------------
            //---------------------------------------------------
            Data DecryptBinary(const u8* in_data, u32 in_dataLength, const std::string& in_privateKey)
            {
                CS_ASSERT(in_privateKey.size() == 16, "AES Encryption keys must be 16 bytes");
                
                AES_KEY sKey;
                if(AES_set_decrypt_key((const u8*)in_privateKey.data(), 128, &sKey) != 0)
                {
                    CS_LOG_FATAL("AES: Cannot set decryption key");
                }
                
                Data output;
                output.m_size = in_dataLength;
                output.m_data = std::unique_ptr<u8[]>(new u8[output.m_size]);
                
                s32 dwBytesRemaining = (s32)in_dataLength;
                const u8* pReadData = in_data;
                u8* pWriteData = output.m_data.get();
                
                while(dwBytesRemaining > 0)
                {
                    AES_decrypt(pReadData, pWriteData, &sKey);
                    pReadData += k_aesBlockSize;
                    pWriteData += k_aesBlockSize;
                    dwBytesRemaining -= k_aesBlockSize;
                }
                
                return std::move(output);
            }
            //---------------------------------------------------
            //---------------------------------------------------
            Data EncryptString(const std::string& in_string, const std::string& in_privateKey)
            {
                CS_ASSERT(in_privateKey.size() == 16, "AES Encryption keys must be 16 bytes");
                
                AES_KEY privateKey;
                if(AES_set_encrypt_key((const u8*)in_privateKey.data(), 128, &privateKey) != 0)
                {
                    CS_LOG_FATAL("AES: Cannot set decryption key");
                }
                
                const u32 terminatedStringSize = in_string.length() + 1;
                const u32 encryptedDataSize = CalculateAlignedSize(terminatedStringSize);
                const u32 numBlocks = encryptedDataSize / k_aesBlockSize;
                
                Data output;
                output.m_size = encryptedDataSize;
                output.m_data = std::unique_ptr<u8[]>(new u8[output.m_size]);
                
                const u8* readData = reinterpret_cast<const u8*>(in_string.c_str());
                u8* writeData = output.m_data.get();
                
                for (u32 blockIndex = 0; blockIndex < numBlocks - 1; ++blockIndex)
                {
                    AES_encrypt(readData, writeData, &privateKey);
                    readData += k_aesBlockSize;
                    writeData += k_aesBlockSize;
                }
                
                //encrypt the final block.
                std::unique_ptr<u8[]> finalBlock(new u8[k_aesBlockSize]);
                memset(finalBlock.get(), '\0', k_aesBlockSize);
                u32 remainingStringSize = in_string.length() % k_aesBlockSize;
                if (remainingStringSize > 0)
                {
                    memcpy(finalBlock.get(), readData, remainingStringSize);
                }
                AES_encrypt(finalBlock.get(), writeData, &privateKey);
                
                return std::move(output);
            }
            //---------------------------------------------------
            //---------------------------------------------------
            std::string DecryptString(const u8* in_data, u32 in_dataLength, const std::string& in_privateKey)
            {
                CS_ASSERT(in_privateKey.size() == 16, "AES Encryption keys must be 16 bytes");
                
                AES_KEY sKey;
                if(AES_set_decrypt_key((const u8*)in_privateKey.data(), 128, &sKey) != 0)
                {
                    CS_LOG_FATAL("AES: Cannot set decryption key");
                }
                
                Data decryptedData;
                decryptedData.m_size = in_dataLength;
                decryptedData.m_data = std::unique_ptr<u8[]>(new u8[decryptedData.m_size]);
                
                s32 dwBytesRemaining = (s32)in_dataLength;
                const u8* pReadData = in_data;
                u8* pWriteData = decryptedData.m_data.get();
                
                while(dwBytesRemaining > 0)
                {
                    AES_decrypt(pReadData, pWriteData, &sKey);
                    pReadData += k_aesBlockSize;
                    pWriteData += k_aesBlockSize;
                    dwBytesRemaining -= k_aesBlockSize;
                }
                
                const s8* data = reinterpret_cast<const s8*>(decryptedData.m_data.get());
                std::string output(data);
                
                return output;
            }
        }
    }
}
