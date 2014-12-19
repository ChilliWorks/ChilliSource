//
//  Utils.h
//  Chilli Source
//  Created by Scott Downie on 25/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_BASE_UTILS_H_
#define _CHILLISOURCE_CORE_BASE_UTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileStream.h>

#include <json/forwards.h>

#include <algorithm>
#include <vector>

namespace ChilliSource
{
	namespace Core
	{
		namespace Utils
		{
            //------------------------------------------------------
			/// performs a bitmask set on a with b.
			///
            /// @author Ian Copland
            ///
            /// @param The bitmask to set.
            /// @param The bitmask to apply.
			//------------------------------------------------------
            template <typename TType> inline void BitmapSet(TType& in_a, TType in_b)
            {
                in_a |= in_b;
            }
            //------------------------------------------------------
			/// performs a bitmask clear on a with b.
			///
            /// @author Ian Copland
            ///
            /// @param The bitmask to set.
            /// @param The bitmask to apply.
			//------------------------------------------------------
            template <typename TType> inline void BitmapClear(TType& in_a, TType in_b)
            {
                in_a &= (~in_b);
            }
            //------------------------------------------------------
			/// performs a bitmask flip on a with b.
			///
            /// @author Ian Copland
            ///
            /// @param The bitmask to set.
            /// @param The bitmask to apply.
			//------------------------------------------------------
            template <typename TType> inline void BitmapFlip(TType& in_a, TType in_b)
            {
                in_a ^= in_b;
            }
            //------------------------------------------------------
			/// performs a bitmask check on a with b.
			///
            /// @author Ian Copland
            ///
            /// @param The bitmask to set.
            /// @param The bitmask to apply.
			//------------------------------------------------------
            template <typename TType> inline bool BitmapCheck(TType in_a, TType in_b)
            {
                return (in_a & in_b) != 0;
            }
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
            //------------------------------------------------------
            /// @author S Downie
            ///
            /// @param A single hexidecimal value to convert
            ///
            /// @return Base ten representation of hex value or -1
            //------------------------------------------------------
            u8 HexToDec(const u8* in_hex);
		};
	}
}

#endif
