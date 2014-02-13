/*
 *  FileStream.cpp
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Platform/Android/FileIO/FileStreamAndroidAPK.h>
#include <ChilliSource/Platform/Android/FileIO/FileSystem.h>

namespace moFlo
{
	namespace AndroidPlatform 
	{
		//--------------------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------------------
		CFileStreamAPK::CFileStreamAPK(CThread::Mutex* inpMinizipMutex)
			: mbOpen(false), mbError(false), mUnzipper(NULL), mpMinizipMutex(inpMinizipMutex), mpDataBuffer(NULL)
		{
		}
		//--------------------------------------------------------------------------------------------------
		/// Destructor
		//--------------------------------------------------------------------------------------------------
		CFileStreamAPK::~CFileStreamAPK()
		{
			if(IsOpen())
			{
				Close();
			}
		}
		//--------------------------------------------------------------------------------------------------
		/// Open APK
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::OpenFromAPK(const std::string& instrApkPath, const unz_file_pos& inFilePos, Core::FILE_MODE ineMode)
		{
			CThread::ScopedLock lock(*mpMinizipMutex);
			mUnzipper = unzOpen(instrApkPath.c_str());

			if (mUnzipper == NULL)
			{
				FATAL_LOG("Failed to open APK");
			}

			Open(inFilePos, ineMode);
		}
		//--------------------------------------------------------------------------------------------------
		/// Open
		//--------------------------------------------------------------------------------------------------			
		void CFileStreamAPK::Open(const unz_file_pos& inFilePos, Core::FILE_MODE ineMode)
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
					unzGetCurrentFileInfo(mUnzipper, &info, filename, dwFilenameLength, NULL, 0, NULL, 0);

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
		bool CFileStreamAPK::IsOpen()
		{
			return mbOpen;
		}
		//--------------------------------------------------------------------------------------------------
		/// Is Bad
		//--------------------------------------------------------------------------------------------------
		bool CFileStreamAPK::IsBad()
		{
			return mbError;
		}
		//--------------------------------------------------------------------------------------------------
		/// End Of File
		//--------------------------------------------------------------------------------------------------
		bool CFileStreamAPK::EndOfFile()
		{
			return mStringStream.eof();
		}
		//--------------------------------------------------------------------------------------------------
		/// Close
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Close()
		{
			if(mpDataBuffer)
			{
				delete[] mpDataBuffer;
				mpDataBuffer = NULL;
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
		s32 CFileStreamAPK::Get()
		{
			return mStringStream.get();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Get(s8 & outbyChar)
		{
			mStringStream.get(outbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Get(s8 * outpbyString, s32 indwStreamSize)
		{
			mStringStream.get(outpbyString, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Get(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
		{
			mStringStream.get(outpbyString, indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::GetLine(std::string &outstrString)
		{
			mStringStream >> outstrString;
		}
		//--------------------------------------------------------------------------------------------------
		/// Get All
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::GetAll(std::string &outstrString)
		{
			outstrString = mStringStream.str().substr(mStringStream.tellg());
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Get(std::stringstream &outstrStringStream)
		{
			outstrStringStream << mStringStream.rdbuf();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::GetLine(s8 * outpbyString, s32 indwStreamSize)
		{
			mStringStream.getline(outpbyString, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::GetLine(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
		{
			mStringStream.getline(outpbyString, indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Ignore
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Ignore(s32 indwStreamSize, s8 inbyDelim)
		{
			mStringStream.ignore(indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Peek
		//--------------------------------------------------------------------------------------------------
		s32 CFileStreamAPK::Peek()
		{
			return mStringStream.peek();
		}
		//--------------------------------------------------------------------------------------------------
		/// Read
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Read(s8* inpbyBuffer, s32 indwStreamSize)
		{
			mStringStream.read(inpbyBuffer, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Read Some
		//--------------------------------------------------------------------------------------------------
		s32 CFileStreamAPK::ReadSome(s8* inpbyBuffer, s32 indwStreamSize)
		{
			return mStringStream.readsome(inpbyBuffer, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Put Back
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::PutBack(s8 inbyChar)
		{
			mStringStream.putback(inbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Unget
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Unget()
		{
			mStringStream.unget();
		}
		//--------------------------------------------------------------------------------------------------
		/// TellG
		//--------------------------------------------------------------------------------------------------
		s32 CFileStreamAPK::TellG()
		{
			return mStringStream.tellg();
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekG
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::SeekG(s32 indwPosition)
		{
			mStringStream.seekg(indwPosition, std::ios_base::beg);
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekG
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::SeekG(s32 indwPosition, Core::SEEK_DIR ineDir)
		{
			std::ios_base::seekdir dir;
			switch (ineDir)
			{
				case Core::SD_BEGINNING:
					dir = std::ios_base::beg;
					break;
				case Core::SD_CURRENT:
					dir = std::ios_base::cur;
					break;
				case Core::SD_END:
					dir = std::ios_base::end;
					break;
			}
			mStringStream.seekg(indwPosition,dir);
		}
		//--------------------------------------------------------------------------------------------------
		/// Sync
		//--------------------------------------------------------------------------------------------------
		s32 CFileStreamAPK::Sync()
		{
			return mStringStream.sync();
		}
		//--------------------------------------------------------------------------------------------------
		/// Put
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Put(s8 inbyChar)
		{
			mStringStream.put(inbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Write
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Write(s8* inpbyChar, s32 indwStreamSize)
		{
			ERROR_LOG("Cannot write to Android APK");
		}
		//--------------------------------------------------------------------------------------------------
		/// Write
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Write(const std::string& _instrString)
		{
			ERROR_LOG("Cannot write to Android APK");
		}
		//--------------------------------------------------------------------------------------------------
		/// TellP
		//--------------------------------------------------------------------------------------------------
		s32 CFileStreamAPK::TellP()
		{
			return mStringStream.tellp();
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekP
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::SeekP(s32 indwPosition)
		{
			mStringStream.seekp(indwPosition, std::ios_base::beg);
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekP
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::SeekP(s32 indwPosition, Core::SEEK_DIR ineDir)
		{
			std::ios_base::seekdir dir;
			switch (ineDir)
			{
				case Core::SD_BEGINNING:
					dir = std::ios_base::beg;
					break;
				case Core::SD_CURRENT:
					dir = std::ios_base::cur;
					break;
				case Core::SD_END:
					dir = std::ios_base::end;
					break;
			}
			mStringStream.seekp(indwPosition, dir);
		}
		//--------------------------------------------------------------------------------------------------
		/// Flush
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Flush()
		{
			mStringStream.flush();
		}
							 
		//--------------------------------------------------------------------------------------------------
		/// Get File Mode
		//--------------------------------------------------------------------------------------------------
		std::ios_base::openmode CFileStreamAPK::GetFileMode()
		{
			switch (meFileMode)
			{
				case Core::FM_READ:
					return (std::ios_base::in);
				case Core::FM_READ_BINARY:
					return (std::ios_base::in | std::ios_base::binary);
				case Core::FM_WRITE:
					return (std::ios_base::out);
				case Core::FM_WRITE_APPEND:
					return (std::ios_base::out | std::ios_base::app);
				case Core::FM_WRITE_AT_END:
					return (std::ios_base::out | std::ios_base::ate);
				case Core::FM_WRITE_TRUNCATE:
					return (std::ios_base::out | std::ios_base::trunc);
				case Core::FM_WRITE_BINARY:
					return (std::ios_base::out | std::ios_base::binary);
				case Core::FM_WRITE_BINARY_APPEND:
					return (std::ios_base::out | std::ios_base::binary | std::ios_base::app);
				case Core::FM_WRITE_BINARY_AT_END:
					return (std::ios_base::out | std::ios_base::binary | std::ios_base::ate);
				case Core::FM_WRITE_BINARY_TRUNCATE:
					return (std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
				default:
					return (std::ios_base::in);
			}
		}
	}
}
