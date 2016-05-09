//
//  VirtualFileStream.cpp
//  Chilli Source
//  Created by Ian Copland on 25/03/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <CSBackend/Platform/Android/Main/JNI/Core/File/VirtualFileStream.h>

namespace CSBackend
{
	namespace Android 
	{
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		VirtualFileStream::VirtualFileStream(std::unique_ptr<u8[]> in_buffer, u32 in_bufferSize, ChilliSource::FileMode in_fileMode)
		{
			CS_ASSERT(in_fileMode == ChilliSource::FileMode::k_read || in_fileMode == ChilliSource::FileMode::k_readBinary, "Virtual file stream only supports read file modes.");
            CS_ASSERT(in_buffer != nullptr, "Cannot create a virtual file stream with a null buffer.");
            CS_ASSERT(in_bufferSize > 0, "Invalid buffer size.");

			m_buffer = std::move(in_buffer);
			m_stream.rdbuf()->pubsetbuf(reinterpret_cast<s8*>(m_buffer.get()), in_bufferSize);

			m_isValid = true;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool VirtualFileStream::IsValid() const
		{
			return m_isValid;
		}
		//--------------------------------------------------------------------------------------------------
		/// End Of File
		//--------------------------------------------------------------------------------------------------
		bool VirtualFileStream::EndOfFile()
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			return m_stream.eof();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		s32 VirtualFileStream::Get()
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			return m_stream.get();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Get(s8 & outbyChar)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.get(outbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Get(s8 * outpbyString, s32 indwStreamSize)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.get(outpbyString, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Get(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.get(outpbyString, indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::GetLine(std::string &outstrString)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream >> outstrString;
		}
		//--------------------------------------------------------------------------------------------------
		/// Get All
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::GetAll(std::string &outstrString)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			outstrString = m_stream.str().substr(m_stream.tellg());
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Get(std::stringstream &outstrStringStream)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			outstrStringStream << m_stream.rdbuf();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::GetLine(s8 * outpbyString, s32 indwStreamSize)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.getline(outpbyString, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::GetLine(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.getline(outpbyString, indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Ignore
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Ignore(s32 indwStreamSize, s8 inbyDelim)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.ignore(indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Peek
		//--------------------------------------------------------------------------------------------------
		s32 VirtualFileStream::Peek()
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			return m_stream.peek();
		}
		//--------------------------------------------------------------------------------------------------
		/// Read
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Read(s8* inpbyBuffer, s32 indwStreamSize)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.read(inpbyBuffer, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Put Back
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::PutBack(s8 inbyChar)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.putback(inbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Unget
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Unget()
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.unget();
		}
		//--------------------------------------------------------------------------------------------------
		/// TellG
		//--------------------------------------------------------------------------------------------------
		s32 VirtualFileStream::TellG()
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			return m_stream.tellg();
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekG
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::SeekG(s32 indwPosition)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			m_stream.seekg(indwPosition, std::ios_base::beg);
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekG
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::SeekG(s32 indwPosition, ChilliSource::SeekDir ineDir)
		{
        	CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

			std::ios_base::seekdir dir;
			switch (ineDir)
			{
				default:
				case ChilliSource::SeekDir::k_beginning:
					dir = std::ios_base::beg;
					break;
				case ChilliSource::SeekDir::k_current:
					dir = std::ios_base::cur;
					break;
				case ChilliSource::SeekDir::k_end:
					dir = std::ios_base::end;
					break;
			}
			m_stream.seekg(indwPosition, dir);
		}
		//--------------------------------------------------------------------------------------------------
		/// Sync
		//--------------------------------------------------------------------------------------------------
		s32 VirtualFileStream::Sync()
		{
        	CS_LOG_FATAL("Cannot sync a virtual file stream.");

        	return 0;
		}
		//--------------------------------------------------------------------------------------------------
		/// Put
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Put(s8 inbyChar)
		{
        	CS_LOG_FATAL("Cannot write to a virtual file stream.");
		}
		//--------------------------------------------------------------------------------------------------
		/// Write
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Write(const s8* inpbyChar, s32 indwStreamSize)
		{
        	CS_LOG_FATAL("Cannot write to a virtual file stream.");
		}
		//--------------------------------------------------------------------------------------------------
		/// Write
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Write(const std::string& _instrString)
		{
        	CS_LOG_FATAL("Cannot write to a virtual file stream.");
		}
		//--------------------------------------------------------------------------------------------------
		/// TellP
		//--------------------------------------------------------------------------------------------------
		s32 VirtualFileStream::TellP()
		{
			CS_LOG_FATAL("A virtual file stream does not have a 'put' position to get.");

			return 0;
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekP
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::SeekP(s32 indwPosition)
		{
        	CS_LOG_FATAL("A virtual file stream does not have a 'put' position to seek.");
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekP
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::SeekP(s32 indwPosition, ChilliSource::SeekDir ineDir)
		{
        	CS_LOG_FATAL("A virtual file stream does not have a 'put' position to seek.");
		}
		//--------------------------------------------------------------------------------------------------
		/// Flush
		//--------------------------------------------------------------------------------------------------
		void VirtualFileStream::Flush()
		{
        	CS_LOG_FATAL("Cannot flush a virtual file stream.");
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		VirtualFileStream::~VirtualFileStream()
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
