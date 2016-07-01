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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Main/JNI/Core/File/VirtualBinaryInputStream.h>

namespace CSBackend
{
	namespace Android
	{
        //------------------------------------------------------------------------------
        VirtualBinaryInputStream::VirtualBinaryInputStream(std::unique_ptr<u8[]> buffer, u32 bufferSize) noexcept
        {
            CS_ASSERT(buffer != nullptr, "Cannot create a virtual file stream with a null buffer.");
            CS_ASSERT(bufferSize > 0, "Invalid buffer size.");

            m_buffer = std::move(buffer);
            m_stream.rdbuf()->pubsetbuf(reinterpret_cast<s8*>(m_buffer.get()), bufferSize);

            m_isValid = true;

            m_length = bufferSize;
        }
        //------------------------------------------------------------------------------
        bool VirtualBinaryInputStream::IsValid() const noexcept
        {
            return m_isValid;
        }
        //------------------------------------------------------------------------------
        u64 VirtualBinaryInputStream::GetLength() const noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");

            return m_length;
        }
        //------------------------------------------------------------------------------
        u64 VirtualBinaryInputStream::GetReadPosition() noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
            return m_stream.tellg();
        }
        //------------------------------------------------------------------------------
        void VirtualBinaryInputStream::SetReadPosition(u64 readPosition) noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
            CS_ASSERT(readPosition <= GetLength(), "Position out of bounds!");

            m_stream.seekg(readPosition);
        }
        //------------------------------------------------------------------------------
        ChilliSource::ByteBufferUPtr VirtualBinaryInputStream::ReadAll() noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");

            //Reset the read position to the beginning
            SetReadPosition(0);

            return Read(m_length);
        }
        //------------------------------------------------------------------------------
        bool VirtualBinaryInputStream::Read(u8* buffer, u64 length) noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");

            if(m_stream.eof())
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

            m_stream.read(reinterpret_cast<s8*>(buffer), maxValidLength);

            CS_ASSERT(!m_stream.fail(), "Unexpected error occured in filestream");

            return true;
        }
        //------------------------------------------------------------------------------
        ChilliSource::ByteBufferUPtr VirtualBinaryInputStream::Read(u64 length) noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");

            if(m_stream.eof())
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
            m_stream.read(data, maxValidLength);

            CS_ASSERT(!m_stream.fail(), "Unexpected error occured in filestream");

            std::unique_ptr<const u8[]> uniqueData(reinterpret_cast<u8*>(data));

            return ChilliSource::ByteBufferUPtr(new ChilliSource::ByteBuffer(std::move(uniqueData), maxValidLength));
        }
        //------------------------------------------------------------------------------
        VirtualBinaryInputStream::~VirtualBinaryInputStream() noexcept
        {
            if(IsValid() == true)
            {
                m_stream.str(std::string());
                m_buffer.reset();
            }
        }
	}
}

#endif