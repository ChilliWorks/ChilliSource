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

#include <CSBackend/Platform/Android/Main/JNI/Core/File/VirtualTextInputStream.h>

namespace CSBackend
{
	namespace Android
	{
        //------------------------------------------------------------------------------
        VirtualTextInputStream::VirtualTextInputStream(std::unique_ptr<u8[]> buffer, u32 bufferSize) noexcept
        {
            CS_ASSERT(buffer != nullptr, "Cannot create a virtual file stream with a null buffer.");
            CS_ASSERT(bufferSize > 0, "Invalid buffer size.");

            m_buffer = std::move(buffer);
            m_stream.rdbuf()->pubsetbuf(reinterpret_cast<s8*>(m_buffer.get()), bufferSize);

            m_isValid = true;

            m_length = bufferSize;
        }
        //------------------------------------------------------------------------------
        bool VirtualTextInputStream::IsValid() const noexcept
        {
            return m_isValid;
        }
        //------------------------------------------------------------------------------
        u64 VirtualTextInputStream::GetLength() const noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");

            return m_length;
        }
        //------------------------------------------------------------------------------
        u64 VirtualTextInputStream::GetReadPosition() noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
            return m_stream.tellg();
        }
        //------------------------------------------------------------------------------
        void VirtualTextInputStream::SetReadPosition(u64 readPosition) noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
            CS_ASSERT(readPosition <= GetLength(), "Position out of bounds! - " + ChilliSource::ToString(readPosition) + "," + ChilliSource::ToString(GetLength()));

            m_stream.seekg(readPosition);
        }
        //------------------------------------------------------------------------------
        std::string VirtualTextInputStream::ReadAll() noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");

            //Reset the read position to the beginning
            SetReadPosition(0);

            std::string fileContents = m_stream.str();

            return fileContents;
        }
        //------------------------------------------------------------------------------
        bool VirtualTextInputStream::ReadLine(std::string& line) noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");

            if(m_stream.eof())
            {
                return false;
            }

            std::getline(m_stream, line);

            //Shouldn't check fail here as the fail bit will be set on a read overrun, which we handle
            CS_ASSERT(!m_stream.bad(), "Unexpected error occured in filestream");

            //Need to carry out another eof check here, as the check at the start of the function may
            //not catch allcases. The EOF bit is only set when a read operation is attempted, not a seekg,
            //so it may only be set after the above getline.
            if(m_stream.eof())
            {
                return !line.empty();
            }

            return true;
        }
        //------------------------------------------------------------------------------
        bool VirtualTextInputStream::Read(u64 length, std::string& readChars) noexcept
        {
            CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");

            if(m_stream.eof())
            {
                return false;
            }

            readChars.resize(length);

            //A string is guaranteed to be in contiguous memory in the c++11 standard, but not in c++03.
            //this allows the below overwrite to work
            m_stream.read(&readChars[0], length);

            //Shouldn't check fail here as the fail bit will be set on a read overrun, which we handle
            CS_ASSERT(!m_stream.bad(), "Unexpected error occured in filestream");

            readChars.resize(m_stream.gcount());

            //Need to carry out another eof check here, as the check at the start of the function may
            //not catch allcases. The EOF bit is only set when a read operation is attempted, not a seekg,
            //so it may only be set after the above read.
            if(m_stream.eof())
            {
                return m_stream.gcount() != 0;
            }

            return true;
        }
        //------------------------------------------------------------------------------
        VirtualTextInputStream::~VirtualTextInputStream() noexcept
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