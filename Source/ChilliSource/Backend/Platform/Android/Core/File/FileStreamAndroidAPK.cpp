/*
 *  FileStream.cpp
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Core/File/FileStreamAndroidAPK.h>

#include <ChilliSource/Backend/Platform/Android/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Android 
	{
		//--------------------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------------------
		CFileStreamAPK::CFileStreamAPK(std::mutex* inpMinizipMutex)
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
		void CFileStreamAPK::OpenFromAPK(const std::string& instrApkPath, const unz_file_pos& inFilePos, Core::FileMode ineMode)
		{
			std::unique_lock<std::mutex> lock(*mpMinizipMutex);
			mUnzipper = unzOpen(instrApkPath.c_str());

			if (mUnzipper == NULL)
			{
				CS_LOG_FATAL("Failed to open APK");
			}

			Open(inFilePos, ineMode);
		}
		//--------------------------------------------------------------------------------------------------
		/// Open
		//--------------------------------------------------------------------------------------------------			
		void CFileStreamAPK::Open(const unz_file_pos& inFilePos, Core::FileMode ineMode)
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
		void CFileStreamAPK::SeekG(s32 indwPosition, Core::SeekDir ineDir)
		{
			std::ios_base::seekdir dir;
			switch (ineDir)
			{
				case Core::SeekDir::k_beginning:
					dir = std::ios_base::beg;
					break;
				case Core::SeekDir::k_current:
					dir = std::ios_base::cur;
					break;
				case Core::SeekDir::k_end:
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
			CS_LOG_ERROR("Cannot write to Android APK");
		}
		//--------------------------------------------------------------------------------------------------
		/// Write
		//--------------------------------------------------------------------------------------------------
		void CFileStreamAPK::Write(const std::string& _instrString)
		{
			CS_LOG_ERROR("Cannot write to Android APK");
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
		void CFileStreamAPK::SeekP(s32 indwPosition, Core::SeekDir ineDir)
		{
			std::ios_base::seekdir dir;
			switch (ineDir)
			{
				case Core::SeekDir::k_beginning:
					dir = std::ios_base::beg;
					break;
				case Core::SeekDir::k_current:
					dir = std::ios_base::cur;
					break;
				case Core::SeekDir::k_end:
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
				case Core::FileMode::k_read:
					return (std::ios_base::in);
				case Core::FileMode::k_readBinary:
					return (std::ios_base::in | std::ios_base::binary);
				case Core::FileMode::k_write:
					return (std::ios_base::out);
				case Core::FileMode::k_writeAppend:
					return (std::ios_base::out | std::ios_base::app);
				case Core::FileMode::k_writeAtEnd:
					return (std::ios_base::out | std::ios_base::ate);
				case Core::FileMode::k_writeTruncate:
					return (std::ios_base::out | std::ios_base::trunc);
				case Core::FileMode::k_writeBinary:
					return (std::ios_base::out | std::ios_base::binary);
				case Core::FileMode::k_writeBinaryAppend:
					return (std::ios_base::out | std::ios_base::binary | std::ios_base::app);
				case Core::FileMode::k_writeBinaryAtEnd:
					return (std::ios_base::out | std::ios_base::binary | std::ios_base::ate);
				case Core::FileMode::k_writeBinaryTruncate:
					return (std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
				default:
					return (std::ios_base::in);
			}
		}
	}
}
