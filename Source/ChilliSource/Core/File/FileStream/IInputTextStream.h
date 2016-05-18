//
//  IInputTextStream.h
//  ChilliSource
//
//  The MIT License (MIT)
//
//  Copyright © 2016 Tag Games. All rights reserved.
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


#ifndef _CHILLISOURCE_CORE_FILE_FILESTREAM_IINPUTTEXTSTREAM_H_
#define _CHILLISOURCE_CORE_FILE_FILESTREAM_IINPUTTEXTSTREAM_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    /// Interface for an input text stream
    ///
    class IInputTextStream
    {
    public:
        
        /// Checks if this is a valid stream
        ///
        /// @return If Valid
        ///
        virtual bool IsValid() const noexcept = 0;
        
        /// @return Length of stream
        ///
        virtual u64 GetLength() const noexcept = 0;
        
        /// @return The current position through the filestream
        ///
        virtual u64 GetReadPosition() noexcept = 0;
        
        /// @param readPosition
        ///     Sets the position to begin the next readLine/read from (ReadAll unaffected)
        ///
        virtual void SetReadPosition(u64 readPosition) noexcept = 0;
        
        /// @return String containing all of the text file, including newlines
        ///
        virtual std::string ReadAll() noexcept = 0;
        
        /// @param line
        ///     String that the next line will populate, this will not contain newline characters
        ///
        /// @return If a line was read successfully
        ///
        virtual bool ReadLine(std::string& line) noexcept = 0;
        
        /// @param length
        ///     Number of characters to read
        ///
        /// @param readLine
        ///     String containing the next number of characters, including newlines
        ///
        /// @return If the read was successfull
        ///
        virtual bool Read(u64 length, std::string& readLine) noexcept = 0;
    };
}

#endif