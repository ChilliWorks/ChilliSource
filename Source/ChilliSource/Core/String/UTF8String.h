//
//  UTF8String.h
//  moFlow
//
//  Created by Scott Downie on 24/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_CONTAINERS_UTF8_STRING_H_
#define _MO_FLO_CORE_CONTAINERS_UTF8_STRING_H_

namespace ChilliSource
{
    namespace Core
    {
        class CUTF8String
        {
        public:
            
            typedef u8  CodePoint;
            typedef u32 Char;
            
            typedef const u8* const_iterator;
            typedef u8* iterator;
            
            CUTF8String();
            CUTF8String(const std::string& inCodePoints);
            CUTF8String(const s8* instrCString);
            CUTF8String(const CodePoint* inpCodePoints, u32 inudwNumCodePoints);
            
            //---------------------------------------------------------
            /// Length (Alias of size)
            ///
            /// @returns The number of UTF-8 characters in the string
            //---------------------------------------------------------
            u32 length() const;
            //---------------------------------------------------------
            /// Size
            ///
            /// @returns The number of UTF-8 characters in the string
            //---------------------------------------------------------
            u32 size() const;
            //---------------------------------------------------------
            /// At ([] operator)
            ///
            /// @param Index into string
            /// @return UTF8 character at that index
            //---------------------------------------------------------
            Char at(u32 inudwIndex) const;
            //---------------------------------------------------------
            /// Begin
            ///
            /// @return Iterator pointing to the first character 
            //---------------------------------------------------------
            iterator begin();
            //---------------------------------------------------------
            /// End
            ///
            /// @return Iterator pointing to the last character 
            //---------------------------------------------------------
            iterator end();
            //---------------------------------------------------------
            /// Begin (Const)
            ///
            /// @return Iterator pointing to the first character 
            //---------------------------------------------------------
            const const_iterator begin() const;
            //---------------------------------------------------------
            /// End (Const)
            ///
            /// @return Iterator pointing to the last character 
            //---------------------------------------------------------
            const const_iterator end() const;
            //---------------------------------------------------------
            /// Next
            ///
            /// Returns the character given by the iterator and
            /// increments the iterator to the next character
            ///
            /// @param Iterator pointing to lead codepoint
            /// @return Character starting at that codepoint
            //---------------------------------------------------------
            Char next(iterator& inpIt) const;
            //---------------------------------------------------------
            /// Front
            ///
            /// @return The first character 
            //---------------------------------------------------------
            Char front() const;
            //---------------------------------------------------------
            /// Back
            ///
            /// @return The last character 
            //---------------------------------------------------------
            Char back() const;
            //---------------------------------------------------------
            /// Append (+= operator)
            ///
            /// Append the given UTF-8 string to this one
            ///
            /// @param String to append
            //---------------------------------------------------------
            void append(const CUTF8String& instrUTF8String);
            //---------------------------------------------------------
            /// Append (+= operator)
            ///
            /// Append a given decoded UTF-8 character to this string
            ///
            /// @param Character to append
            //---------------------------------------------------------
            void appendChar(CUTF8String::Char inChar);
            //---------------------------------------------------------
            /// Append (+= operator)
            ///
            /// Append the given UTF-8 code points to this string
            ///
            /// @param Codepoints to append
            //---------------------------------------------------------
            void appendCodePoints(CUTF8String::Char inChar);
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
            CUTF8String substr(u32 inudwStartPosition = 0, u32 inudwLength = npos);
            //---------------------------------------------------------
            /// Data
            ///
            /// @return Pointer to the code points
            //---------------------------------------------------------
            const CodePoint* data() const;
            //---------------------------------------------------------
            /// Data Size
            ///
            /// @return Size of the code points in bytes
            //---------------------------------------------------------
            u32 data_size() const;
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
            void erase(iterator inpIt);
            //---------------------------------------------------------
            /// Clear
            ///
            /// Clear the string but does not free any memory
            //---------------------------------------------------------
            void clear();
            //---------------------------------------------------------
            /// To ASCII
            ///
            /// Assumes the string is within the ascii range
            /// 
            /// @return std string
            //---------------------------------------------------------
            std::string ToASCII() const;
            
            //---Operators
            Char operator[](u32 inudwIndex) const
            {
                return at(inudwIndex);
            }
            void operator+=(const CUTF8String& instrUTF8String)
            {
                this->append(instrUTF8String);
            }
            void operator+=(CUTF8String::Char inChar)
            {
                this->appendChar(inChar);
            }
            bool operator==(const CUTF8String& instrUTF8String) const
            {
                if(mCodePoints.size() != instrUTF8String.mCodePoints.size())
                {
                    return false;
                }
                
                for(u32 i=0; i<mCodePoints.size(); ++i)
                {
                    if(mCodePoints[i] != instrUTF8String.mCodePoints[i])
                    {
                        return false;
                    }
                }
                
                return true;
            }
            bool operator!=(const CUTF8String& instrUTF8String) const
            {
                if(mCodePoints.size() != instrUTF8String.mCodePoints.size())
                {
                    return true;
                }
                
                for(u32 i=0; i<mCodePoints.size(); ++i)
                {
                    if(mCodePoints[i] != instrUTF8String.mCodePoints[i])
                    {
                        return true;
                    }
                }
                
                return false;
            }
            CUTF8String operator=(const std::string& inCodePoints)
            {
                return CUTF8String(inCodePoints);
            }
            
        private:
            //----------------------------------------------------------
            /// Get Character
            ///
            /// @param Leading codepoint 
            /// @param Out: The number of code points for that char
            /// @return Multibyte character
            //----------------------------------------------------------
            Char GetCharacter(const_iterator inpIt, u32& outudwNumCodePointsInCharacter) const;
            
        public:
            static const u32 npos = -1;

        private:
            
            DYNAMIC_ARRAY<CodePoint> mCodePoints;
            
            mutable u32 mudwLength;
            
            mutable bool mbIsLengthCacheValid;
        };
        
        CUTF8String operator+(const CUTF8String& instrUTF8String, const CUTF8String& instrUTF8String2);
        
        typedef CUTF8String UTF8String;
    }
}

#endif
