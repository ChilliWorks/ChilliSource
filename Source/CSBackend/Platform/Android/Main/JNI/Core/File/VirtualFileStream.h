//
//  VirtualFileStream.h
//  ChilliSource
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_CORE_FILE_VIRTUALFILESTREAM_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_CORE_FILE_VIRTUALFILESTREAM_H_

#include <ChilliSource/Core/File/FileStream.h>

#include <fstream>
#include <ios>
#include <sstream>

namespace CSBackend
{
	namespace Android
	{
		//--------------------------------------------------------------------------------------------------
		/// A filestream is used for all reading and writing of files. This will emulate the functionality
		/// of fstream and allows for cross platform filereading.
		//--------------------------------------------------------------------------------------------------
		class VirtualFileStream final : public ChilliSource::FileStream
		{
		public:
			//------------------------------------------------------------------------------
			/// Constructor. Creates a new file stream into the given memory blob.
			///
			/// @author Ian Copland
			///
			/// @param in_buffer - The memory buffer which will be used as a "virtual"
			/// file.
			/// @param in_bufferSize - The size of the memory buffer.
			/// @param in_fileMode - The file mode with which the "file" should be opened.
			//------------------------------------------------------------------------------
			VirtualFileStream(std::unique_ptr<u8[]> in_buffer, u32 in_bufferSize, ChilliSource::FileMode in_fileMode);
			//------------------------------------------------------------------------------
			/// This should be called immediately after construction, before calling any
			/// other file stream methods. If this returns false the FileStream should be
			/// discarded, no other methods should be called.
			///
			/// @author Ian Copland
			///
			/// @return Whether or not the file was successfully opened on construction and
			/// is now ready to be read from.
			//------------------------------------------------------------------------------
			bool IsValid() const override;
			//--------------------------------------------------------------------------------------------------
			/// End Of File
			///
			/// @return Whether the end of the file has been reached.
			//--------------------------------------------------------------------------------------------------
			bool EndOfFile() override;
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// @return the next character in the file stream as an integer
			//--------------------------------------------------------------------------------------------------
			s32 Get() override;
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// Reads the next character into outbyChar
			///
			/// @param Container for the output value.
			//--------------------------------------------------------------------------------------------------
			void Get(s8 & outbyChar) override;
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// Reads indwStreamSize bytes from the stream unless an EOF is encountered.
			///
			/// @param buffer for the output string.
			/// @param The amount of bytes to read into the buffer.
			//--------------------------------------------------------------------------------------------------
			void Get(s8 * outpbyString, s32 indwStreamSize) override;
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// Reads indwStreamSize bytes from the stream unless the character specifed by inbyDelim is 
			/// encountered
			///
			/// @param buffer for the output string.
			/// @param The amount of bytes to read into the buffer.
			/// @param The delimiting character.
			//--------------------------------------------------------------------------------------------------
			void Get(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim) override;
			//--------------------------------------------------------------------------------------------------
			/// Get Line
			///
			/// Reads a single line into a std string.
			///
			/// @param OUT: output string.
			//--------------------------------------------------------------------------------------------------
			void GetLine(std::string &outstrString) override;
			//--------------------------------------------------------------------------------------------------
			/// Get All
			///
			/// Reads the entire file into a std string.
			///
			/// @param OUT: output string.
			//--------------------------------------------------------------------------------------------------
			void GetAll(std::string &outstrString) override;
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// Reads into a std string stream.
			///
			/// @param OUT: output string.
			//--------------------------------------------------------------------------------------------------
			void Get(std::stringstream &outstrStringStream) override;
			//--------------------------------------------------------------------------------------------------
			/// Get Line
			///
			/// Reads indwStreamSize bytes from the stream unless a return character is encountered.
			///
			/// @param buffer for the output string.
			/// @param The amount of bytes to read into the buffer.
			//--------------------------------------------------------------------------------------------------
			void GetLine(s8 * outpbyString, s32 indwStreamSize) override;
			//--------------------------------------------------------------------------------------------------
			/// Get Line
			///
			/// Reads indwStreamSize bytes from the stream unless the character specifed by inbyDelim is 
			/// encountered
			///
			/// @param buffer for the output string.
			/// @param The amount of bytes to read into the buffer.
			/// @param The delimiting character.
			//--------------------------------------------------------------------------------------------------
			void GetLine(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim) override;
			//--------------------------------------------------------------------------------------------------
			/// Ignore
			///
			/// Reads indwStreamSize bytes from the stream unless the character specifed by inbyDelim is 
			/// encountered and the discards them.
			///
			/// @param The amount of bytes to read into the buffer.
			/// @param The delimiting character.
			//--------------------------------------------------------------------------------------------------
			void Ignore(s32 indwStreamSize = 1, s8 inbyDelim = EOF) override;
			//--------------------------------------------------------------------------------------------------
			/// Peek
			///
			/// Reads the next byte in the stream and returns it as an integer, but doesn't seek to the next
			/// byte, leaving it remaining as the next byte.
			///
			/// @return the next byte as an integer.
			//--------------------------------------------------------------------------------------------------
			s32 Peek() override;
			//--------------------------------------------------------------------------------------------------
			/// Read
			///
			/// Reads a block of data into the buffer on size specified by indwStreamSize
			///
			/// @param The buffer.
			/// @param the stream size.
			//--------------------------------------------------------------------------------------------------
			void Read(s8* inpbyBuffer, s32 indwStreamSize) override;
			//--------------------------------------------------------------------------------------------------
			/// Put Back
			///
			/// Decriments the get pointer by one, and sets inbyChar as the next char to be read.
			///
			/// @param the next char.
			//--------------------------------------------------------------------------------------------------
			void PutBack(s8 inbyChar) override;
			//--------------------------------------------------------------------------------------------------
			/// Unget
			///
			/// Decriments the get pointer by one.
			//--------------------------------------------------------------------------------------------------
			void Unget() override;
			//--------------------------------------------------------------------------------------------------
			/// TellG
			///
			/// @return the absolute position of the get pointer.
			//--------------------------------------------------------------------------------------------------
			s32 TellG() override;
			//--------------------------------------------------------------------------------------------------
			/// SeekG
			///
			/// Sets the position of the get pointer
			///
			/// @param the new position.
			//--------------------------------------------------------------------------------------------------
			void SeekG(s32 indwPosition) override;
			//--------------------------------------------------------------------------------------------------
			/// SeekG
			///
			/// Sets the position of the get pointer, using the specified SeekDir
			///
			/// @param the new position.
			/// @param the direction from which to seek.
			//--------------------------------------------------------------------------------------------------
			void SeekG(s32 indwPosition, ChilliSource::SeekDir ineDir) override;
			//--------------------------------------------------------------------------------------------------
			/// Sync
			///
			/// Syncronises with the associated buffer. This effectively means that unread bytes are discarded.
			//--------------------------------------------------------------------------------------------------
			s32 Sync() override;
			//--------------------------------------------------------------------------------------------------
			/// Put
			///
			/// Writes the specified character to the output.
			///
			/// @param the char to write.
			//--------------------------------------------------------------------------------------------------
			void Put(s8 inbyChar) override;
			//--------------------------------------------------------------------------------------------------
			/// Write
			///
			/// Writes a block of data to the output.
			///
			/// @param the data buffer.
			/// @param the stream size.
			//--------------------------------------------------------------------------------------------------
			void Write(const s8* inpbyChar, s32 indwStreamSize) override;
			//--------------------------------------------------------------------------------------------------
			/// Write
			///
			/// Writes using a std:string
			///
			/// @param the string.
			//--------------------------------------------------------------------------------------------------
			void Write(const std::string& _instrString) override;
			//--------------------------------------------------------------------------------------------------
			/// TellP
			///
			/// @return the currently position of the put pointer
			//--------------------------------------------------------------------------------------------------
			s32 TellP() override;
			//--------------------------------------------------------------------------------------------------
			/// SeekP
			///
			/// Sets the position of the put pointer
			///
			/// @param the new position.
			//--------------------------------------------------------------------------------------------------
			void SeekP(s32 indwPosition) override;
			//--------------------------------------------------------------------------------------------------
			/// SeekP
			///
			/// Sets the position of the put pointer, using the specified SeekDir
			///
			/// @param the new position.
			/// @param the direction from which to seek.
			//--------------------------------------------------------------------------------------------------
			void SeekP(s32 indwPosition, ChilliSource::SeekDir ineDir) override;
			//--------------------------------------------------------------------------------------------------
			/// Flush
			///
			/// Synchronises the associated buffer with the stream.
			//--------------------------------------------------------------------------------------------------
			void Flush() override;
			//------------------------------------------------------------------------------
			/// Destructor. Closes the file stream if it was successfully opened.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			virtual ~VirtualFileStream();

		private:

			std::unique_ptr<u8[]> m_buffer;
			std::stringstream m_stream;
		};
	}
}


#endif

#endif
