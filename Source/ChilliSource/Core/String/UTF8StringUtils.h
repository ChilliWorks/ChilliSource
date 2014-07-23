//
//  UTF8StringUtils.h
//  Chilli Source
//  Created by Scott Downie on 26/05/2014.
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

#ifndef _CHILLISOURCE_CORE_STRING_UTF8STRINGUTILS_H_
#define _CHILLISOURCE_CORE_STRING_UTF8STRINGUTILS_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        using UTF8Char = u32;
        
        //---------------------------------------------------------
        /// Utility methods for accessing a list of code points as
        /// a UTF8 encoded string (often a std::string).
        /// UTF8 characters range in size from 1 byte to 4 bytes
        /// depending on the number of code points.
        /// ASCII and UTF8 have a direct mapping.
        ///
        /// @author S Downie
        //---------------------------------------------------------
        namespace UTF8StringUtils
        {
            //-----------------------------------------------------
            /// Return the next UTF-8 character in the sequence
            /// from the current iterator and advance the iterator
            /// by a character.
            ///
            /// NOTE: This will not perform any bounds checking on
            /// the container.
            ///
            /// @author S Downie
            ///
            /// @param Current iterator which is advanced
            ///
            /// @return UTF8 character pointed to by iterator prior
            /// to advancing
            //-----------------------------------------------------
            template<typename TCodePointIter> UTF8Char Next(TCodePointIter& inout_currentIt)
            {
                UTF8Char result = 0;
                
                s8 firstCodePoint = (*inout_currentIt);
                
                //Single byte code (ASCII)
                if(firstCodePoint > 0)
                {
                    //Just return the code point
                    result = (UTF8Char)firstCodePoint;
                    ++inout_currentIt;
                    return result;
                }
                
                switch(0xF0 & firstCodePoint)
                {
                    case 0xE0:
                        result =  (*inout_currentIt & 0x0F) << 12;
                        ++inout_currentIt;
                        result |= (*inout_currentIt & 0x3F) << 6;
                        ++inout_currentIt;
                        result |= (*inout_currentIt & 0x3F);
                        ++inout_currentIt;
                        break;
                    case 0xF0:
                        result =  (*inout_currentIt & 0x07) << 18;
                        ++inout_currentIt;
                        result |= (*inout_currentIt & 0x3F) << 12;
                        ++inout_currentIt;
                        result |= (*inout_currentIt & 0x3F) << 6;
                        ++inout_currentIt;
                        result |= (*inout_currentIt & 0x3F);
                        ++inout_currentIt;
                        break;
                    default:
                        result =  (*inout_currentIt & 0x1F) << 6;
                        ++inout_currentIt;
                        result |= (*inout_currentIt & 0x3F);
                        ++inout_currentIt;
                        break;
                }
                
                return result;
            }
            //-----------------------------------------------------
            /// Calculate the number of UTF-8 characters based on
            /// list of codepoints between the start and end
            /// iteration points
            ///
            /// @author S Downie
            ///
            /// @param Begin iterator
            /// @param End iterator
            ///
            /// @return Number of characters between begin and end
            //-----------------------------------------------------
            template<typename TCodePointIter> u32 CalcLength(TCodePointIter in_begin, TCodePointIter in_end)
            {
                //UTF-8 is a multi-byte standard which means the characters
                //are not fixed length but each character contains the 'trail' bytes bit pattern of 10xxxxxx.
                //We can calculate the length by counting the number of 'trail' bytes
                u32 length = 0;
                while(in_begin < in_end)
                {
                    length += (*in_begin & 0xc0) != 0x80;
                    ++in_begin;
                }
                
                return length;
            }
            //-----------------------------------------------------
            /// Copy and append the given UTF8 character to the list of code
            /// points held in the std::string and return the result
            ///
            /// @author S Downie
            ///
            /// @param UTF-8 character
            /// @param String of code points
            ///
            /// @return String of code points with character appended
            //-----------------------------------------------------
            std::string AppendCopy(UTF8Char in_char, const std::string& in_string);
            //-----------------------------------------------------
            /// Append the given UTF8 character to the list of code
            /// points held in the std::string
            ///
            /// @author S Downie
            ///
            /// @param UTF-8 character
            /// @param [Out] String of code points
            //-----------------------------------------------------
            void Append(UTF8Char in_char, std::string& out_appendedResult);
            //-----------------------------------------------------
            /// Return a sub-string copy going from begin for
            /// the given number of characters
            ///
            /// Note: Length should not exceed end of string
            ///
            /// @author S Downie
            ///
            /// @param UTF8 String
            /// @param Start character index (UTF-8)
            /// @param Num UTF-8 characters from start to copy
            ///
            /// @return Sub-string of code points
            //-----------------------------------------------------
            std::string SubString(const std::string& in_string, u32 in_start, u32 in_length);
        }
    }
}

#endif
