//
//  FileStreamAPK.cpp
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

#include <CSBackend/Platform/Android/Core/File/FileStreamAPK.h>

#include <CSBackend/Platform/Android/Core/File/FileSystem.h>

namespace CSBackend
{
	namespace Android 
	{
		//--------------------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------------------
		FileStreamAPK::FileStreamAPK(std::mutex* inpMinizipMutex)
			: mbOpen(false), mbError(false), mUnzipper(nullptr), mpMinizipMutex(inpMinizipMutex), mpDataBuffer(nullptr)
		{
		}
		//--------------------------------------------------------------------------------------------------
		/// Destructor
		//--------------------------------------------------------------------------------------------------
		FileStreamAPK::~FileStreamAPK()
		{
			if(IsOpen())
			{
				Close();
			}
		}
		//--------------------------------------------------------------------------------------------------
		/// Open APK
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::OpenFromAPK(const std::string& instrApkPath, const unz_file_pos& inFilePos, CSCore::FileMode ineMode)
		{
			std::unique_lock<std::mutex> lock(*mpMinizipMutex);
			mUnzipper = unzOpen(instrApkPath.c_str());

			if (mUnzipper == nullptr)
			{
				CS_LOG_FATAL("Failed to open APK");
			}

			Open(inFilePos, ineMode);
		}
		//--------------------------------------------------------------------------------------------------
		/// Open
		//--------------------------------------------------------------------------------------------------			
		void FileStreamAPK::Open(const unz_file_pos& inFilePos, CSCore::FileMode ineMode)
		{
			meFileMode = ineMode;

			unz_file_pos sPos = inFilePos;
			s32 Result = unzGoToFilePos(mUnzipper, &sPos);

			if (Result == UNZ_OK)
			{
				Result = unzOpenCurrentFile(mUnzipper);

				if (Result == UNZ_OK)
				{
					mbOpen = true;

					//get file information
					unz_file_info info;
					const u32 dwFilenameLength = 256;
					s8 filename[dwFilenameLength];
					unzGetCurrentFileInfo(mUnzipper, &info, filename, dwFilenameLength, nullptr, 0, nullptr, 0);

					//load the file into memory
					mpDataBuffer = new s8[info.uncompressed_size];
					unzReadCurrentFile(mUnzipper, (voidp)mpDataBuffer, info.uncompressed_size);

					//create a string stream for reading from it.
					mStringStream.rdbuf()->pubsetbuf(mpDataBuffer, info.uncompressed_size);
				}
				else
				{
					mbError = true;
				}
			}
			else
			{
				mbError = true;
			}

			//close the apk
			unzCloseCurrentFile(mUnzipper);
			unzClose(mUnzipper);
		}
		//--------------------------------------------------------------------------------------------------
		/// Is Open
		//--------------------------------------------------------------------------------------------------
		bool FileStreamAPK::IsOpen()
		{
			return mbOpen;
		}
		//--------------------------------------------------------------------------------------------------
		/// Is Bad
		//--------------------------------------------------------------------------------------------------
		bool FileStreamAPK::IsBad()
		{
			return mbError;
		}
		//--------------------------------------------------------------------------------------------------
		/// End Of File
		//--------------------------------------------------------------------------------------------------
		bool FileStreamAPK::EndOfFile()
		{
			return mStringStream.eof();
		}
		//--------------------------------------------------------------------------------------------------
		/// Close
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Close()
		{
			if(mpDataBuffer)
			{
				delete[] mpDataBuffer;
				mpDataBuffer = nullptr;
			}

			if (mbOpen == true && mbError == false)
			{
				mStringStream.str(std::string());
				mbOpen = false;
			}
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		s32 FileStreamAPK::Get()
		{
			return mStringStream.get();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Get(s8 & outbyChar)
		{
			mStringStream.get(outbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Get(s8 * outpbyString, s32 indwStreamSize)
		{
			mStringStream.get(outpbyString, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Get(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
		{
			mStringStream.get(outpbyString, indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::GetLine(std::string &outstrString)
		{
			mStringStream >> outstrString;
		}
		//--------------------------------------------------------------------------------------------------
		/// Get All
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::GetAll(std::string &outstrString)
		{
			outstrString = mStringStream.str().substr(mStringStream.tellg());
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Get(std::stringstream &outstrStringStream)
		{
			outstrStringStream << mStringStream.rdbuf();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::GetLine(s8 * outpbyString, s32 indwStreamSize)
		{
			mStringStream.getline(outpbyString, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::GetLine(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
		{
			mStringStream.getline(outpbyString, indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Ignore
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Ignore(s32 indwStreamSize, s8 inbyDelim)
		{
			mStringStream.ignore(indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Peek
		//--------------------------------------------------------------------------------------------------
		s32 FileStreamAPK::Peek()
		{
			return mStringStream.peek();
		}
		//--------------------------------------------------------------------------------------------------
		/// Read
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Read(s8* inpbyBuffer, s32 indwStreamSize)
		{
			mStringStream.read(inpbyBuffer, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Put Back
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::PutBack(s8 inbyChar)
		{
			mStringStream.putback(inbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Unget
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Unget()
		{
			mStringStream.unget();
		}
		//--------------------------------------------------------------------------------------------------
		/// TellG
		//--------------------------------------------------------------------------------------------------
		s32 FileStreamAPK::TellG()
		{
			return mStringStream.tellg();
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekG
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::SeekG(s32 indwPosition)
		{
			mStringStream.seekg(indwPosition, std::ios_base::beg);
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekG
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::SeekG(s32 indwPosition, CSCore::SeekDir ineDir)
		{
			std::ios_base::seekdir dir;
			switch (ineDir)
			{
				default:
				case CSCore::SeekDir::k_beginning:
					dir = std::ios_base::beg;
					break;
				case CSCore::SeekDir::k_current:
					dir = std::ios_base::cur;
					break;
				case CSCore::SeekDir::k_end:
					dir = std::ios_base::end;
					break;
			}
			mStringStream.seekg(indwPosition,dir);
		}
		//--------------------------------------------------------------------------------------------------
		/// Sync
		//--------------------------------------------------------------------------------------------------
		s32 FileStreamAPK::Sync()
		{
			return mStringStream.sync();
		}
		//--------------------------------------------------------------------------------------------------
		/// Put
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Put(s8 inbyChar)
		{
			mStringStream.put(inbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Write
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Write(const s8* inpbyChar, s32 indwStreamSize)
		{
			CS_LOG_ERROR("Cannot write to Android APK");
		}
		//--------------------------------------------------------------------------------------------------
		/// Write
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Write(const std::string& _instrString)
		{
			CS_LOG_ERROR("Cannot write to Android APK");
		}
		//--------------------------------------------------------------------------------------------------
		/// TellP
		//--------------------------------------------------------------------------------------------------
		s32 FileStreamAPK::TellP()
		{
			return mStringStream.tellp();
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekP
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::SeekP(s32 indwPosition)
		{
			mStringStream.seekp(indwPosition, std::ios_base::beg);
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekP
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::SeekP(s32 indwPosition, CSCore::SeekDir ineDir)
		{
			std::ios_base::seekdir dir;
			switch (ineDir)
			{
				default:
				case CSCore::SeekDir::k_beginning:
					dir = std::ios_base::beg;
					break;
				case CSCore::SeekDir::k_current:
					dir = std::ios_base::cur;
					break;
				case CSCore::SeekDir::k_end:
					dir = std::ios_base::end;
					break;
			}
			mStringStream.seekp(indwPosition, dir);
		}
		//--------------------------------------------------------------------------------------------------
		/// Flush
		//--------------------------------------------------------------------------------------------------
		void FileStreamAPK::Flush()
		{
			mStringStream.flush();
		}
							 
		//--------------------------------------------------------------------------------------------------
		/// Get File Mode
		//--------------------------------------------------------------------------------------------------
		std::ios_base::openmode FileStreamAPK::GetFileMode()
		{
			switch (meFileMode)
			{
				case CSCore::FileMode::k_read:
					return (std::ios_base::in);
				case CSCore::FileMode::k_readBinary:
					return (std::ios_base::in | std::ios_base::binary);
				case CSCore::FileMode::k_write:
					return (std::ios_base::out);
				case CSCore::FileMode::k_writeAppend:
					return (std::ios_base::out | std::ios_base::app);
				case CSCore::FileMode::k_writeAtEnd:
					return (std::ios_base::out | std::ios_base::ate);
				case CSCore::FileMode::k_writeTruncate:
					return (std::ios_base::out | std::ios_base::trunc);
				case CSCore::FileMode::k_writeBinary:
					return (std::ios_base::out | std::ios_base::binary);
				case CSCore::FileMode::k_writeBinaryAppend:
					return (std::ios_base::out | std::ios_base::binary | std::ios_base::app);
				case CSCore::FileMode::k_writeBinaryAtEnd:
					return (std::ios_base::out | std::ios_base::binary | std::ios_base::ate);
				case CSCore::FileMode::k_writeBinaryTruncate:
					return (std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
				default:
					return (std::ios_base::in);
			}
		}
	}
}

#endif
