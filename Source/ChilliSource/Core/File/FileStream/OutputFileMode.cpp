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

#include <ChilliSource/Core/File/FileStream/OutputFileMode.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    std::ios_base::openmode ToFileMode(TextOutputFileMode outputFileMode)
    {
        //We use the binary mode for text to ensure no platform specific formatting is appended
        switch (outputFileMode)
        {
            case TextOutputFileMode::k_write:
                return std::ios_base::out | std::ios_base::binary;
            case TextOutputFileMode::k_writeAppend:
                return std::ios_base::out | std::ios_base::binary | std::ios_base::app;
            default:
                CS_LOG_FATAL("Unhandled TextOutputFileMode case.");
                break;
        }
        
        return std::ios_base::out | std::ios_base::binary;
    }
    //------------------------------------------------------------------------------
    std::ios_base::openmode ToFileMode(BinaryOutputFileMode outputFileMode)
    {
        switch (outputFileMode)
        {
            case BinaryOutputFileMode::k_write:
                return std::ios_base::out | std::ios_base::binary;
            case BinaryOutputFileMode::k_writeAppend:
                return std::ios_base::out | std::ios_base::binary | std::ios_base::app;
            default:
                CS_LOG_FATAL("Unhandled BinaryOutputFileMode case.");
                break;
        }
        
        return std::ios_base::out | std::ios_base::binary;
    }
}