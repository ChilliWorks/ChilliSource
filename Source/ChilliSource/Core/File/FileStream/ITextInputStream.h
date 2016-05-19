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


#ifndef _CHILLISOURCE_CORE_FILE_FILESTREAM_ITEXTINPUTSTREAM_H_
#define _CHILLISOURCE_CORE_FILE_FILESTREAM_ITEXTINPUTSTREAM_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    /// Interface for classes to provide textual read functionality for a file
    ///
    /// Implementations shoud be considered thread agnostic, but not thread-safe.
    /// i.e. Instances can be used by one thread at a time. It doesn't matter
    /// which thread as long as any previous threads are no longer accessing it
    ///
    class ITextInputStream
    {
    public:
        
        /// Checks the status of the stream, if this returns false then the stream
        /// can no longer be accessed.
        ///
        /// @return If the stream is valid and available for use.
        ///
        virtual bool IsValid() const noexcept = 0;
        
        /// @return Length of stream in bytes.
        ///
        virtual u64 GetLength() const noexcept = 0;
        
        /// Gets the position from which the next read operation will begin. The position
        /// is always specified relative to the start of the file
        ///
        /// @return The position from the start of the stream.
        ///
        virtual u64 GetReadPosition() noexcept = 0;
        
        /// Sets the position through the stream from which the next read operation will
        /// begin. The position is always specified relative to the start of the file. This does
        /// not affect the output of ReadAll().
        ///
        /// @param readPosition
        ///     The position from the start of the stream.
        ///
        virtual void SetReadPosition(u64 readPosition) noexcept = 0;
        
        /// @return String containing the contents of the text file, including newlines
        ///
        virtual std::string ReadAll() noexcept = 0;
        
        /// Reads from the current read position until the newline character ('\n') or EOF
        /// is reached. If the current read position is at the end of the file on entrance
        /// then this will return false
        ///
        /// @param line
        ///     String buffer that will be populated, this will not contain newline characters
        ///
        /// @return If a line was read successfully
        ///
        virtual bool ReadLine(std::string& line) noexcept = 0;
        
        /// Reads in a number of characters from the current read position and puts them
        /// into the passed in string. If the length of the stream is overrun, readChars
        /// will contain everything up to that point.
        ///
        /// If the current read position is at the end of the file, this function will return
        /// false.
        ///
        /// @param length
        ///     The number of characters to read
        ///
        /// @param readChars
        ///     A string to hold the read characters, including newlines
        ///
        /// @return If the read was successfull
        ///
        virtual bool Read(u64 length, std::string& readLine) noexcept = 0;
        
        /// Destructor
        ///
        virtual ~ITextInputStream() noexcept {};
    };
}

#endif