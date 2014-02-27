//
//  UTF8String.cpp
//  moFlow
//
//  Created by Scott Downie on 24/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/String/UTF8String.h>

namespace ChilliSource
{
    namespace Core
    {
        //---------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //---------------------------------------------------------
        UTF8String::UTF8String() : mudwLength(0), mbIsLengthCacheValid(false)
        {

        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// From code points contained in STL string
        //---------------------------------------------------------
        UTF8String::UTF8String(const std::string& inCodePoints) : mudwLength(0), mbIsLengthCacheValid(false)
        {
            mCodePoints.assign(inCodePoints.begin(), inCodePoints.end());
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// From code points contained in c string
        //---------------------------------------------------------
        UTF8String::UTF8String(const s8* instrCString) : mudwLength(0), mbIsLengthCacheValid(false)
        {
            u32 udwLength = strlen(instrCString);
            
            mCodePoints.assign(instrCString, instrCString + udwLength);
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// From code points in CString
        //---------------------------------------------------------
        UTF8String::UTF8String(const CodePoint* inpCodePoints, u32 inudwNumCodePoints) : mudwLength(0), mbIsLengthCacheValid(false)
        {
            mCodePoints.assign(inpCodePoints, inpCodePoints + inudwNumCodePoints);
        }
        //---------------------------------------------------------
        /// Length (Alias of size)
        ///
        /// @returns The number of UTF-8 characters in the string
        //---------------------------------------------------------
        u32 UTF8String::length() const
        {
            return size();
        }
        //---------------------------------------------------------
        /// Size
        ///
        /// @returns The number of UTF-8 characters in the string
        //---------------------------------------------------------
        u32 UTF8String::size() const
        {
            //UTF-8 is a multi-byte standard which means the characters
            //are not fixed length but each character contains the 'trail' bytes bit pattern of 10xxxxxx. 
            //We can calculate the length by counting the number of 'trail' bytes
            if(!mbIsLengthCacheValid)
            {
                //Lets cache the result and only compute again if required (i.e. on append or erase)
                mudwLength = 0;
                for(std::vector<CodePoint>::const_iterator it = mCodePoints.begin(); it != mCodePoints.end(); ++it)
                {
                    mudwLength += (*it & 0xc0) != 0x80;

                    mbIsLengthCacheValid = true;
                }
            }

            return mudwLength;
        }
        //---------------------------------------------------------
        /// At ([] operator)
        ///
        /// @param Index into string
        /// @return UTF8 character at that index
        //---------------------------------------------------------
        UTF8String::Char UTF8String::at(u32 inudwIndex) const
        {
            Char Result = 0;
            u32 udwOffset = 0;
            u32 udwCodePoint = 0;

            for(u32 i=0; i<=inudwIndex; ++i)
            {
                Result = GetCharacter(&mCodePoints[udwCodePoint], udwOffset);
                udwCodePoint += udwOffset;
            }

            return Result;
        }
        //---------------------------------------------------------
        /// Begin (Const)
        ///
        /// @return Iterator pointing to the first character 
        //---------------------------------------------------------
        const UTF8String::const_iterator UTF8String::begin() const
        {
            if(mCodePoints.size() > 0)
            {
                return &mCodePoints[0];
            }

            return nullptr;
        }
        //---------------------------------------------------------
        /// End (Const)
        ///
        /// @return Iterator pointing to the last character 
        //---------------------------------------------------------
        const UTF8String::const_iterator UTF8String::end() const
        {
            if(mCodePoints.size() > 0)
            {
                return &mCodePoints[0] + mCodePoints.size();
            }

            return nullptr;
        }
        //---------------------------------------------------------
        /// Begin
        ///
        /// @return Iterator pointing to the first character 
        //---------------------------------------------------------
        UTF8String::iterator UTF8String::begin() 
        {
            if(mCodePoints.size() > 0)
            {
                return &mCodePoints[0];
            }

            return nullptr;
        }
        //---------------------------------------------------------
        /// End
        ///
        /// @return Iterator pointing to the last character 
        //---------------------------------------------------------
        UTF8String::iterator UTF8String::end() 
        {
            if(mCodePoints.size() > 0)
            {
                return &mCodePoints[0] + mCodePoints.size();
            }

            return nullptr;
        }
        //---------------------------------------------------------
        /// Next
        ///
        /// Returns the character given by the iterator and
        /// increments the iterator to the next character
        ///
        /// @param Iterator pointing to lead codepoint
        /// @return Character starting at that codepoint
        //---------------------------------------------------------
        UTF8String::Char UTF8String::next(iterator& inpIt) const
        {
            u32 udwOffset = 0;
            UTF8String::Char Result = GetCharacter(inpIt, udwOffset);

            inpIt += udwOffset;

            return Result;
        }
        //---------------------------------------------------------
        /// Front
        ///
        /// @return The first character 
        //---------------------------------------------------------
        UTF8String::Char UTF8String::front() const
        {
            return *begin();
        }
        //---------------------------------------------------------
        /// Back
        ///
        /// @return The last character 
        //---------------------------------------------------------
        UTF8String::Char UTF8String::back() const
        {
            return *(end() - 1);
        }
        //---------------------------------------------------------
        /// Append (+= operator)
        ///
        /// Append the given UTF-8 string to this one
        ///
        /// @param String to append
        //---------------------------------------------------------
        void UTF8String::append(const UTF8String& instrUTF8String)
        {
            mbIsLengthCacheValid = false;

            mCodePoints.insert(mCodePoints.end(), instrUTF8String.begin(), instrUTF8String.end());
        }
        //---------------------------------------------------------
        /// Append (+= operator)
        ///
        /// Append the given UTF-8 char to this string
        ///
        /// @param Character to append
        //---------------------------------------------------------
        void UTF8String::appendChar(UTF8String::Char inChar)
        {
            mbIsLengthCacheValid = false;
            
            u32 udwNumCodePoints = 0;
            
            CodePoint cp[4];
            
            //Single byte code (ASCII)
            if((inChar) <= 0x7F)
            {
                udwNumCodePoints = 1;
                cp[0]=((inChar>>0) & 0x7F);
            }
            //Two byte code
            else if((inChar) <= 0x7FF)
            {
                udwNumCodePoints = 2;
                cp[0]=0xC0 | ((inChar>>6) & 0x1F);
                cp[1]=0x80 | ((inChar) & 0x3F);
                
            }
            //Three byte code
            else if((inChar) <= 0xFFFF)
            {
                udwNumCodePoints = 3;
                cp[0]=0xE0 | ((inChar>>12) & 0x0F);
                cp[1]=0x80 | ((inChar>>6) & 0x3F);
                cp[2]=0x80 | ((inChar) & 0x3F);
                
            }
            //Four byte code
            else if((inChar) <= 0x1FFFFF)
            {
                udwNumCodePoints = 4;
                cp[0]=0xF0 | ((inChar>>18) & 0x07);
                cp[1]=0x80 | ((inChar>>12) & 0x3F);
                cp[2]=0x80 | ((inChar>>6) & 0x3F);
                cp[3]=0x80 | ((inChar) & 0x3F);
            }
            //Invalid format
            else
            {
                udwNumCodePoints = 0;
                CS_DEBUG_LOG("Invalid UT8 format");
            }
            
            for(u32 i=0; i<udwNumCodePoints; ++i)
            {
                mCodePoints.push_back(cp[i]);
            }
        }
        //---------------------------------------------------------
        /// Append (+= operator)
        ///
        /// Append the given UTF-8 char to this string
        ///
        /// @param Character to append
        //---------------------------------------------------------
        void UTF8String::appendCodePoints(UTF8String::Char inChar)
        {
            mbIsLengthCacheValid = false;
            
            u32 udwNumCodePoints = 0;
            CodePoint* pLeadCodePoint = (CodePoint*)(&inChar);
            
            //Single byte code (ASCII)
            if((*pLeadCodePoint) < 0x80)
            {
                udwNumCodePoints = 1;
            }
            //Two byte code
            else if((*pLeadCodePoint) < 0xE0)
            {
                udwNumCodePoints = 2;
            }
            //Three byte code
            else if((*pLeadCodePoint) < 0xF0)
            {
                udwNumCodePoints = 3;
            }
            //Four byte code
            else if((*pLeadCodePoint)  < 0xF8)
            {
                udwNumCodePoints = 4;
            }
            //Invalid format
            else if((*pLeadCodePoint) < 0xC0)
            {
                udwNumCodePoints = 0;
                CS_DEBUG_LOG("Invalid UT8 format");
            }
            
            for(u32 i=0; i<udwNumCodePoints; ++i)
            {
                mCodePoints.push_back(*pLeadCodePoint++);
            }
        }
        //---------------------------------------------------------
        /// substr
        ///
        /// returns a new string initialised to the value of a
        /// portion of this string.
        ///
        /// @param The start position.
        /// @param The length of the sub string.
        /// @return The sub string.
        //---------------------------------------------------------
        UTF8String UTF8String::substr(u32 inudwStartPosition, u32 inudwLength)
        {
            std::string strCodePoints;
            u32 udwPosition = 0;

            //iterate over the code points of this string, copying the section we are interested in.
            for(std::vector<CodePoint>::const_iterator it = mCodePoints.begin(); it != mCodePoints.end(); ++it)
            {
                if (udwPosition >= inudwStartPosition + inudwLength)
                {
                    break;
                }
                else if (udwPosition >= inudwStartPosition)
                {
                    strCodePoints += *it;
                }

                udwPosition += (*it & 0xc0) != 0x80;
            }

            return UTF8String(strCodePoints);
        }
        //---------------------------------------------------------
        /// Data
        ///
        /// @return Pointer to the code points
        //---------------------------------------------------------
        const UTF8String::CodePoint* UTF8String::data() const
        {
            return mCodePoints.empty()? nullptr : &mCodePoints[0];
        }
        //---------------------------------------------------------
        /// Data Size
        ///
        /// @return Size of the code points in bytes
        //---------------------------------------------------------
        u32 UTF8String::data_size() const
        {
            return mCodePoints.size() * sizeof(CodePoint);
        }
        //----------------------------------------------------------
        /// Get Character
        ///
        /// @param Leading codepoint 
        /// @param Out: The number of code points for that char
        /// @return Multibyte character
        //----------------------------------------------------------
        UTF8String::Char UTF8String::GetCharacter(const_iterator inpIt, u32& outudwNumCodePointsInCharacter) const
        {
            s8	cFirstByte = (*inpIt);
            UTF8String::Char Result = 0;

            //Single byte code (ASCII)
            if(cFirstByte > 0)
            {
                //Just return the code point
                Result = (*inpIt);
                outudwNumCodePointsInCharacter = 1;			
                return Result;
            }
            
            switch( 0xF0 & cFirstByte )
            {
                case 0xE0:
                    Result =  ((*inpIt)  & 0x0F) << 12;   
                    Result |= (*(inpIt+1)  & 0x3F) <<  6;   
                    Result |= (*(inpIt+2)  & 0x3F);
                    outudwNumCodePointsInCharacter = 3;
                    break;
                case 0xF0:
                    Result =  ((*inpIt)  & 0x07) << 18;   
                    Result |= (*(inpIt+1)  & 0x3F) << 12;   
                    Result |= (*(inpIt+2)  & 0x3F) <<  6;   
                    Result |= (*(inpIt+3)  & 0x3F);
                    outudwNumCodePointsInCharacter = 4;
                    break;
                default:
                    Result =  ((*inpIt) & 0x1F) << 6;    
                    Result |= (*(inpIt+1) & 0x3F);
                    outudwNumCodePointsInCharacter = 2;
                    break;
                    
            }
            return Result;
        }
        //---------------------------------------------------------
        /// Erase
        ///
        /// Most erase operators return an iterator pointing to 
        /// the next object but that would allow for people to
        /// erase while iterating which is not an option for
        /// multi-byte unicode
        ///
        /// @param Iterator pointing to character to erase
        //---------------------------------------------------------
        void UTF8String::erase(UTF8String::iterator inpIt)
        {
            u32 udwOffset = 0;
            GetCharacter(inpIt, udwOffset);

            for(std::vector<CodePoint>::iterator it = mCodePoints.begin(); it != mCodePoints.end(); ++it)
            {
                if(&(*it) == inpIt)
                {
                    //We have found our codepoint
                    for(u32 i=0; i<udwOffset; ++i)
                    {
                        mCodePoints.erase(it+i);
                    }

                    break;
                }
            }


            mbIsLengthCacheValid = false;
        }
        //---------------------------------------------------------
        /// Clear
        ///
        /// Clear the string but does not free any memory
        //---------------------------------------------------------
        void UTF8String::clear()
        {
            mCodePoints.clear();
            mudwLength = 0;

            mbIsLengthCacheValid = false;
        }
        //---------------------------------------------------------
        /// To ASCII
        ///
        /// Assumes the string is within the ascii range
        /// 
        /// @return std string
        //---------------------------------------------------------
        std::string UTF8String::ToASCII() const
        {
            std::string strResult(mCodePoints.begin(), mCodePoints.end());
            
            return strResult;
        }
        
        UTF8String operator+(const UTF8String& instrUTF8String, const UTF8String& instrUTF8String2)
        {
            UTF8String strResult = instrUTF8String;
            strResult.append(instrUTF8String2);
            return strResult;
        }
    }
}
