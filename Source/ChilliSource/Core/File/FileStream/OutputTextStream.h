//
//  OutputTextStream.h
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


#ifndef _CHILLISOURCE_CORE_FILE_FILESTREAM_OUTPUTTEXTSTREAM_H_
#define _CHILLISOURCE_CORE_FILE_FILESTREAM_OUTPUTTEXTSTREAM_H_

#include <ChilliSource/ChilliSource.h>

#include <fstream>

namespace ChilliSource
{
    /// Class to provide textual write functionality for a file
    ///
    /// OutputTextStream is thread agnostic, but not thread-safe.
    /// i.e. Instances can used by one thread at a time. It
    /// doesn't matter which thread as long as any previous threads
    /// are no longer accessing it
    ///
    class OutputTextStream final
    {
    public:
        
        /// This will create the filestream from the path passed in and evaluate if
        /// the stream is valid. After construction, IsValid() should be called to
        /// ensure the stream was created without errors before proceeding to call
        /// further functionality.
        ///
        /// @param filepath
        ///     The absolute path to a file
        ///
        OutputTextStream(const std::string& filePath) noexcept;
        
        /// Checks the status of the stream, if this returns false then the stream
        /// can no longer be accessed.
        ///
        /// @return If the stream is valid and available for use
        ///
        bool IsValid() const noexcept;
        
        /// Writes some string data to the stream
        ///
        /// @return If the stream is valid and available for use
        ///
        void Write(const std::string& data) noexcept;
        
    private:
        
        bool m_isValid = false;
        std::string m_filename;
        std::ofstream m_fileStream;
    };
}

#endif