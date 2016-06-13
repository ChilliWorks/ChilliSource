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

#ifndef _CHILLISOURCE_CORE_FILE_FILESTREAM_OUTPUTFILEMODE_H_
#define _CHILLISOURCE_CORE_FILE_FILESTREAM_OUTPUTFILEMODE_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    /// Specifies the modes an TextOutputStream file can be opened with.
    ///
    enum class TextOutputFileMode
    {
        k_write,
        k_writeAppend
    };
    
    /// Specifies the modes an BinaryOutputStream file can be opened with.
    ///
    enum class BinaryOutputFileMode
    {
        k_write,
        k_writeAppend
    };
    
    /// Converts an TextOutputFileMode to a std openmode for use in
    /// file opening.
    ///
    /// @param outputFileMode
    ///     The TextOutputFileMode to convert.
    ///
    /// @return The std open mode.
    ///
    std::ios_base::openmode ToFileMode(TextOutputFileMode outputFileMode);

    /// Converts an BinaryOutputFileMode to a std openmode for use in
    /// file opening.
    ///
    /// @param outputFileMode
    ///     The BinaryOutputFileMode to convert.
    ///
    /// @return The std open mode.
    ///
    std::ios_base::openmode ToFileMode(BinaryOutputFileMode outputFileMode);
}

#endif
