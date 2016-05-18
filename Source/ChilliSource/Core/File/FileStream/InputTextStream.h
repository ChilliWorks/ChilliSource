//
//  InputTextStream.h
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


#ifndef _CHILLISOURCE_CORE_FILE_FILESTREAM_INPUTTEXTSTREAM_H_
#define _CHILLISOURCE_CORE_FILE_FILESTREAM_INPUTTEXTSTREAM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileStream/IInputTextStream.h>

#include <fstream>

namespace ChilliSource
{
    /// Class to provide textual read functionality for a file
    ///
    /// InputTextStream is thread agnostic, but not thread-safe.
    /// i.e. Instances can used by different threads as long as its
    /// creation and destruction happens on the same thread.
    ///
    class InputTextStream : public IInputTextStream
    {
        CS_DECLARE_NOCOPY(InputTextStream);
        
    public:
        
        /// @param filepath
        ///     Path to the file to open
        ///
        InputTextStream(const std::string& filepath);
        
        /// Destructor
        ///
        ~InputTextStream();
        
        /// Checks if this is a valid stream
        ///
        /// @return If Valid
        ///
        bool IsValid() const noexcept override;
        
        /// @return Length of stream
        ///
        u64 GetLength() const noexcept override;
        
        /// @return The current position through the filestream
        ///
        u64 GetReadPosition() noexcept override;
        
        /// @param readPosition
        ///     Sets the position to begin the next readLine/read from (ReadAll unaffected)
        ///
        void SetReadPosition(u64 readPosition) noexcept override;
        
        /// @return String containing all of the stream, including newlines
        ///
        std::string ReadAll() noexcept override;
        
        /// @param line
        ///     String that the next line will populate, this will not contain newline characters
        ///
        /// @return If a line was read successfully
        ///
        bool ReadLine(std::string& line) noexcept override;
        
        /// @param length
        ///     Number of characters to read
        ///
        /// @param readChars
        ///     String containing the next number of characters, including newlines
        ///
        /// @return If the read was successfull
        ///
        bool Read(u64 length, std::string& readChars) noexcept override;
        
    private:
        
        u64 m_length = 0;
        bool m_isValid = false;
        std::string m_filename;
        std::ifstream m_fileStream;
    };
}

#endif
