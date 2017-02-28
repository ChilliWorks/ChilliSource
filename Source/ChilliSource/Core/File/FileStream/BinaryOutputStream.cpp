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

#include <ChilliSource/Core/File/FileStream/BinaryOutputStream.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    BinaryOutputStream::BinaryOutputStream(const std::string& filePath, FileWriteMode fileMode) noexcept
    {
        m_fileStream.open(filePath.c_str(), ToOpenMode(fileMode));
        m_isValid = m_fileStream.is_open() && !m_fileStream.bad() && !m_fileStream.fail();
    }
    //------------------------------------------------------------------------------
    bool BinaryOutputStream::IsValid() const noexcept
    {
        return m_isValid;
    }
    //------------------------------------------------------------------------------
    void BinaryOutputStream::Write(const u8* data, u64 length) noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        m_fileStream.write(reinterpret_cast<const s8*>(data), (std::streamsize)length);
        CS_ASSERT(!m_fileStream.fail(), "Unexpected error occured writing to the stream.");
    }
    //------------------------------------------------------------------------------
    void BinaryOutputStream::Write(const ByteBufferUPtr& byteBuffer) noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        m_fileStream.write(reinterpret_cast<const s8*>(byteBuffer->GetData()), byteBuffer->GetLength());
        CS_ASSERT(!m_fileStream.fail(), "Unexpected error occured writing to the stream.");
    }
    //------------------------------------------------------------------------------
    BinaryOutputStream::~BinaryOutputStream() noexcept
    {
        if(m_fileStream.is_open())
        {
            //Close also flushes the changes to disk
            m_fileStream.close();
        }
    }
}
