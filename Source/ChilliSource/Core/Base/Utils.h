//
//  Utils.h
//  Chilli Source
//
//  Created by Scott Downie on 25/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_BASE_UTILS_H_
#define _CHILLISOURCE_CORE_BASE_UTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/JSON/forwards.h>

#include <vector>

#define BYTE_SWAP_2(src) ChilliSource::Core::Utils::Endian2ByteSwap(reinterpret_cast<u16*>(src))
#define BYTE_SWAP_4(src) ChilliSource::Core::Utils::Endian4ByteSwap(reinterpret_cast<u32*>(src))

namespace ChilliSource
{
	namespace Core
	{
		namespace Utils
		{
			//------------------------------------------------------
			/// Endian Byte Swap
			///
			/// Converts the endianness of the given bytes
			//------------------------------------------------------
			inline s16 Endian2ByteSwap(const u16* pSrc)
			{
				u16 Result = *pSrc;
				// Use bit manipulations instead of accessing individual bytes from memory, much faster.
				return (Result >> 8) | (Result << 8);
			}
			
            inline s16 Endian2ByteSwap(const s16* pSrc)
			{
				u16 Result = *pSrc;
				// Use bit manipulations instead of accessing individual bytes from memory, much faster.
				return (Result >> 8) | (Result << 8);
			}
            
			// Specialization for 4-byte types.
			inline s32 Endian4ByteSwap(const u32* pSrc)
			{
				u32 Result = *pSrc;
				// Use bit manipulations instead of accessing individual bytes from memory, much faster.
				return (Result >> 24) | ((Result & 0x00ff0000) >> 8) | ((Result & 0x0000ff00) << 8) | (Result << 24);
			}
            
            inline s32 Endian4ByteSwap(const s32* pSrc)
			{
				u32 Result = *pSrc;
				// Use bit manipulations instead of accessing individual bytes from memory, much faster.
				return (Result >> 24) | ((Result & 0x00ff0000) >> 8) | ((Result & 0x0000ff00) << 8) | (Result << 24);
			}
            
            template<typename T>
            inline T BitwiseRotateRight(T inValue)
            {
                return (inValue >> 1) | (inValue << (sizeof(T)*8 -1));
            }
            
            template<typename T>
            inline T BitwiseRotateLeft(T inValue)
            {
                return (inValue << 1) | (inValue >> (sizeof(T)*8 -1));
            }
            
            bool ReadJson(StorageLocation ineStorageLocation, const std::string & instrPath, Json::Value * outpJson);
            
            bool          FileToString(StorageLocation ineStorageLocation, const std::string & instrPath, std::string & outstrFileContent);
            FileStreamSPtr StringToFile(StorageLocation ineStorageLocation, const std::string & instrPath, const std::string& instrFileOut);
            
            bool ZlibCompressString(const std::string &instrUncompressed, std::string& outstrCompressed);

            bool ZlibDecompressString(const std::string &instrCompressed, std::string& outstrUncompress, u32 inOutputSizeGuess);
            
            // Optimised removal of many elements from a vector while preserving the order of the vector
            template<typename ELEMENT>
            u32 VectorRemoveMany(std::vector<ELEMENT> & inaElements, std::vector<ELEMENT>& inaRemovables)
            {
                if(inaRemovables.empty())
                    return 0;
                
                std::sort(inaRemovables.begin(), inaRemovables.end());
                    
                u32 uCurrent = 0;
                u32 uRemoved = 0;
                const u32 uSize = inaElements.size();
                
                // First run through array till we reach first item to remove
                while(uCurrent + uRemoved < uSize && uRemoved == 0)
                {
                    if(std::binary_search(inaRemovables.begin(), inaRemovables.end(), inaElements[uCurrent+uRemoved]))
                        uRemoved++;
                    
                    if(uRemoved == 0)
                        uCurrent += 1;
                }
                
                // Next copy ahead of current position in array by amount of items removed
                // skipping the items to be removed
                while(uCurrent + uRemoved < uSize)
                {
                    if(std::binary_search(inaRemovables.begin(), inaRemovables.end(), inaElements[uCurrent+uRemoved]))
                    {
                        uRemoved++;
                        continue;
                    }
                    
                    if(uRemoved != 0)
                        inaElements[uCurrent] = inaElements[uCurrent + uRemoved];
                    
                    uCurrent++;
                }
                inaElements.resize(uCurrent);
                return uRemoved;
            }
            
            Vector2 ScaleMaintainingAspectRatio(const Vector2& invCurrentSize, const Vector2& invTargetSize, bool inbFitInside = false);
		};
	}
}

#endif