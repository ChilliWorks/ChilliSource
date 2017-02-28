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

#include <ChilliSource/Core/File/FileStream/BinaryInputStream.h>

#include <ChilliSource/Core/Base/ByteBuffer.h>

#include <algorithm>
#include <memory>
#include <sstream>
#include <iostream>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    BinaryInputStream::BinaryInputStream(const std::string& filePath) noexcept
    {
        m_fileStream.open(filePath.c_str(), std::ios_base::in | std::ios_base::binary);
        
        m_isValid = m_fileStream.is_open() && !m_fileStream.bad() && !m_fileStream.fail();
        
        if(m_isValid)
        {
            m_fileStream.seekg(0, m_fileStream.end);
            m_length = m_fileStream.tellg();
            m_fileStream.seekg(0, m_fileStream.beg);
        }
    }
    //------------------------------------------------------------------------------
    bool BinaryInputStream::IsValid() const noexcept
    {
        return m_isValid;
    }
    //------------------------------------------------------------------------------
    u64 BinaryInputStream::GetLength() const noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        
        return m_length;
    }
    //------------------------------------------------------------------------------
    u64 BinaryInputStream::GetReadPosition() noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        return m_fileStream.tellg();
    }
    //------------------------------------------------------------------------------
    void BinaryInputStream::SetReadPosition(u64 readPosition) noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        CS_ASSERT(readPosition <= GetLength(), "Position out of bounds!");
        
        m_fileStream.seekg(readPosition);
    }
    //------------------------------------------------------------------------------
    ByteBufferUPtr BinaryInputStream::ReadAll() noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        
        //Reset the read position to the beginning
        SetReadPosition(0);
        
        return Read(m_length);
    }
    //------------------------------------------------------------------------------
    bool BinaryInputStream::Read(u8* buffer, u64 length) noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        
        if(m_fileStream.eof())
        {
            return false;
        }
        
        //Ensure that we never overrun the file stream
        const auto currentPosition = GetReadPosition();
        const auto maxValidLength = std::min(m_length - currentPosition, length);
        
        if(maxValidLength == 0)
        {
            return true;
        }
        
        m_fileStream.read(reinterpret_cast<s8*>(buffer), (std::streamsize)maxValidLength);
        
        CS_ASSERT(!m_fileStream.fail(), "Unexpected error occured in filestream");
        
        return true;
    }
    //------------------------------------------------------------------------------
    ByteBufferUPtr BinaryInputStream::Read(u64 length) noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        
        if(m_fileStream.eof())
        {
            return nullptr;
        }
        
        //Ensure that we never overrun the file stream
        const auto currentPosition = GetReadPosition();
        const auto maxValidLength = std::min(m_length - currentPosition, length);
        
        if(maxValidLength == 0)
        {
            return nullptr;
        }
        
        s8* data = new s8[maxValidLength];
        m_fileStream.read(data, (std::streamsize)maxValidLength);
        
        CS_ASSERT(!m_fileStream.fail(), "Unexpected error occured in filestream");

        std::unique_ptr<const u8[]> uniqueData(reinterpret_cast<const u8*>(data));
        return ByteBufferUPtr(new ByteBuffer(std::move(uniqueData), u32(maxValidLength)));
    }
    //------------------------------------------------------------------------------
    BinaryInputStream::~BinaryInputStream() noexcept
    {
        if(m_fileStream.is_open())
        {
            m_fileStream.close();
        }
    }
}
