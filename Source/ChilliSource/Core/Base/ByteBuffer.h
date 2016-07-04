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

#ifndef _CHILLISOURCE_CORE_FILE_FILESTREAM_BYTEBUFFER_H_
#define _CHILLISOURCE_CORE_FILE_FILESTREAM_BYTEBUFFER_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    /// Class to store a data pointer and size
    ///
    /// Class will take ownership of any data passed and data will
    /// be destroyed with this instance
    ///
    class ByteBuffer final
    {
    public:
		CS_DECLARE_NOCOPY(ByteBuffer);
        
        ByteBuffer(ByteBuffer&&) = default;
        ByteBuffer& operator=(ByteBuffer&&) = default;
        
        /// Constructor
        ///
        /// @param data
        ///     The unique pointer to the start of the data
        /// @param length
        ///     The length of the data
        ///
        ByteBuffer(std::unique_ptr<const u8[]> data, u32 length) noexcept;
        
        /// Gets the pointer to the start of the data.
        /// This should never be modified directly.
        ///
        /// @return The pointer to the start of the data
        ///
        const u8* GetData() const noexcept;
        
        /// @return The length of the data stored
        ///
        u32 GetLength() const noexcept;
        
    private:
        std::unique_ptr<const u8[]> m_data;
        u32 m_length;
    };
}

#endif
