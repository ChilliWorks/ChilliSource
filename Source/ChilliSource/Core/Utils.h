/*
 * File: Utilities.h
 * Date: 25/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_CORE_UTILS_H_
#define _MO_FLO_CORE_UTILS_H_

#include <moFlo/Core/FileIO/FileStream.h>
#include <moFlo/Core/JSON/forwards.h>

#define BYTE_SWAP_2(src) moFlo::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(src))
#define BYTE_SWAP_4(src) moFlo::Core::CUtils::Endian4ByteSwap(reinterpret_cast<u32*>(src))

namespace moFlo
{
	namespace Core
	{
		class CUtils
		{
		public:
			//------------------------------------------------------
			/// Endian Byte Swap
			///
			/// Converts the endianness of the given bytes
			//------------------------------------------------------
			static inline s16 Endian2ByteSwap(const u16* pSrc)
			{
				u16 Result = *pSrc;
				// Use bit manipulations instead of accessing individual bytes from memory, much faster.
				return (Result >> 8) | (Result << 8);
			}
			
            static inline s16 Endian2ByteSwap(const s16* pSrc)
			{
				u16 Result = *pSrc;
				// Use bit manipulations instead of accessing individual bytes from memory, much faster.
				return (Result >> 8) | (Result << 8);
			}
            
			// Specialization for 4-byte types.
			static inline s32 Endian4ByteSwap(const u32* pSrc)
			{
				u32 Result = *pSrc;
				// Use bit manipulations instead of accessing individual bytes from memory, much faster.
				return (Result >> 24) | ((Result & 0x00ff0000) >> 8) | ((Result & 0x0000ff00) << 8) | (Result << 24);
			}
            
            static inline s32 Endian4ByteSwap(const s32* pSrc)
			{
				u32 Result = *pSrc;
				// Use bit manipulations instead of accessing individual bytes from memory, much faster.
				return (Result >> 24) | ((Result & 0x00ff0000) >> 8) | ((Result & 0x0000ff00) << 8) | (Result << 24);
			}
            
            template<typename T>
            static inline T BitwiseRotateRight(T inValue)
            {
                return (inValue >> 1) | (inValue << (sizeof(T)*8 -1));
            }
            
            template<typename T>
            static inline T BitwiseRotateLeft(T inValue)
            {
                return (inValue << 1) | (inValue >> (sizeof(T)*8 -1));
            }
            
            static bool ReadJson(STORAGE_LOCATION ineStorageLocation, const std::string & instrPath, Json::Value * outpJson);
            
            static bool          FileToString(STORAGE_LOCATION ineStorageLocation, const std::string & instrPath, std::string & outstrFileContent);
            static FileStreamPtr StringToFile(STORAGE_LOCATION ineStorageLocation, const std::string & instrPath, const std::string& instrFileOut);
            
            static bool ZlibCompressString(const std::string &instrUncompressed, std::string& outstrCompressed);

            static bool ZlibDecompressString(const std::string &instrCompressed, std::string& outstrUncompress, u32 inOutputSizeGuess);
            
            // Optimised removal of many elements from a vector while preserving the order of the vector
            template<typename ELEMENT>
            static u32 VectorRemoveMany(DYNAMIC_ARRAY<ELEMENT> & inaElements, DYNAMIC_ARRAY<ELEMENT>& inaRemovables)
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
            
            static CVector2 ScaleMaintainingAspectRatio(const CVector2& invCurrentSize, const CVector2& invTargetSize, bool inbFitInside = false);
		};
	}
}

#endif