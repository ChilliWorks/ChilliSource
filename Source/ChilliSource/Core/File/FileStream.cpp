//
//  FileStream.cpp
//  Chilli Source
//  Created by Scott Downie on 16/05/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#include <ChilliSource/Core/File/FileStream.h>

#include <md5/md5.h>
#include <SHA1/SHA1.h>

#include <sstream>

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        //--------------------------------------------------------------
        /// Get MD5 Checksum 
        ///
        /// Calculate the MD5 checksum of the file 
        ///
        /// @return MD5 checksum
        //--------------------------------------------------------------
        std::string FileStream::GetMD5Checksum()
        {
            s32 dwCurrentPos = TellG();
            
            // Get file size
            SeekG(0, SeekDir::k_end);
            u32 udwLength = TellG();
            
            SeekG(0);
            
            const u32 kudwChunkSize = 256;
            s8 byData[kudwChunkSize];
            
            MD5 Hash;
            
            while(udwLength >= kudwChunkSize)
            {
                Read(byData, kudwChunkSize);
                Hash.update(byData, kudwChunkSize);
                udwLength -= kudwChunkSize;
            }
            
            // Last chunk
            if(udwLength > 0)
            {
                Read(byData, udwLength);
                Hash.update(byData, udwLength);
            }
            
            SeekG(dwCurrentPos);
            
            Hash.finalize();
            return Hash.binarydigest();
        }
        //--------------------------------------------------------------
        /// Get SHA1 Checksum
        //--------------------------------------------------------------
        std::string FileStream::GetSHA1Checksum(CSHA1::REPORT_TYPE ineReportType)
        {
            s32 dwCurrentPos = TellG();
            
            // Get file size
            SeekG(0, SeekDir::k_end);
            u32 udwLength = TellG();
            
            SeekG(0);
            
            const u32 kudwChunkSize = 256;
            s8 byData[kudwChunkSize];
            
            CSHA1 Hash;
            Hash.Reset();
            
            while(udwLength >= kudwChunkSize)
            {
                Read(byData, kudwChunkSize);
                
                Hash.Update(reinterpret_cast<u8*>(byData), kudwChunkSize);
                udwLength -= kudwChunkSize;
            }
            
            // Last chunk
            if(udwLength > 0)
            {
                Read(byData, udwLength);
                Hash.Update(reinterpret_cast<u8*>(byData), udwLength);
            }
            
            SeekG(dwCurrentPos);
            
            Hash.Final();
            
            const u32 kudwMaxSHA1Length = 80;

#ifdef CS_TARGETPLATFORM_WINDOWS
            TCHAR cHash[kudwMaxSHA1Length];
            memset(cHash, 0, kudwMaxSHA1Length);
            Hash.ReportHash(cHash, ineReportType);
            return CSBackend::Windows::WindowsStringUtils::UTF16ToUTF8(std::wstring(cHash));
#else
			char cHash[kudwMaxSHA1Length];
			memset(cHash, 0, kudwMaxSHA1Length);
			Hash.ReportHash(cHash, ineReportType);
			return std::string(cHash);
#endif
        }
        //--------------------------------------------------------------------------------------------------
		/// Open
		//--------------------------------------------------------------------------------------------------			
		void FileStream::Open(const std::string& instrFilename, Core::FileMode ineMode)
		{
			mstrFilename = instrFilename;
			meFileMode = ineMode;
			
			std::ios_base::openmode mode = GetFileMode();
			
			mFileStream.open(instrFilename.c_str(), mode);
		}
		//--------------------------------------------------------------------------------------------------
		/// Is Open
		//--------------------------------------------------------------------------------------------------
		bool FileStream::IsOpen()
		{
			return mFileStream.is_open();
		}
		//--------------------------------------------------------------------------------------------------
		/// Is Bad
		//--------------------------------------------------------------------------------------------------
		bool FileStream::IsBad()
		{
			return (mFileStream.bad() || mFileStream.fail());
		}
		//--------------------------------------------------------------------------------------------------
		/// End Of File
		//--------------------------------------------------------------------------------------------------
		bool FileStream::EndOfFile()
		{
			return mFileStream.eof();
		}
		//--------------------------------------------------------------------------------------------------
		/// Close
		//--------------------------------------------------------------------------------------------------
		void FileStream::Close()
		{
			mFileStream.close();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		s32 FileStream::Get()
		{
			return mFileStream.get();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void FileStream::Get(s8 & outbyChar)
		{
			mFileStream.get(outbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void FileStream::Get(s8 * outpbyString, s32 indwStreamSize)
		{
			mFileStream.get(outpbyString, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void FileStream::Get(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
		{
			mFileStream.get(outpbyString, indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void FileStream::GetLine(std::string &outstrString)
		{
			mFileStream >> outstrString;
		}
		//--------------------------------------------------------------------------------------------------
		/// Get All
		//--------------------------------------------------------------------------------------------------
		void FileStream::GetAll(std::string &outstrString)
		{
			std::stringstream cStream;
			cStream << mFileStream.rdbuf();
			
			outstrString = cStream.str();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get
		//--------------------------------------------------------------------------------------------------
		void FileStream::Get(std::stringstream &outstrStringStream)
		{
			outstrStringStream << mFileStream.rdbuf();
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void FileStream::GetLine(s8 * outpbyString, s32 indwStreamSize)
		{
			mFileStream.getline(outpbyString, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Line
		//--------------------------------------------------------------------------------------------------
		void FileStream::GetLine(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
		{
			mFileStream.getline(outpbyString, indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Ignore
		//--------------------------------------------------------------------------------------------------
		void FileStream::Ignore(s32 indwStreamSize, s8 inbyDelim)
		{
			mFileStream.ignore(indwStreamSize, inbyDelim);
		}
		//--------------------------------------------------------------------------------------------------
		/// Peek
		//--------------------------------------------------------------------------------------------------
		s32 FileStream::Peek()
		{
			return mFileStream.peek();
		}
		//--------------------------------------------------------------------------------------------------
		/// Read
		//--------------------------------------------------------------------------------------------------
		void FileStream::Read(s8* inpbyBuffer, s32 indwStreamSize)
		{
			mFileStream.read(inpbyBuffer, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Put Back
		//--------------------------------------------------------------------------------------------------
		void FileStream::PutBack(s8 inbyChar)
		{
			mFileStream.putback(inbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Unget
		//--------------------------------------------------------------------------------------------------
		void FileStream::Unget()
		{
			mFileStream.unget();
		}
		//--------------------------------------------------------------------------------------------------
		/// TellG
		//--------------------------------------------------------------------------------------------------
		s32 FileStream::TellG()
		{
			return (s32)mFileStream.tellg();
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekG
		//--------------------------------------------------------------------------------------------------
		void FileStream::SeekG(s32 indwPosition)
		{
			mFileStream.seekg(indwPosition);
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekG
		//--------------------------------------------------------------------------------------------------
		void FileStream::SeekG(s32 indwPosition, Core::SeekDir ineDir)
		{
			std::ios_base::seekdir dir;
			switch (ineDir)
			{
				default:
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
			mFileStream.seekg(indwPosition,dir);
		}
		//--------------------------------------------------------------------------------------------------
		/// Sync
		//--------------------------------------------------------------------------------------------------
		s32 FileStream::Sync()
		{
			return mFileStream.sync();
		}
		//--------------------------------------------------------------------------------------------------
		/// Put
		//--------------------------------------------------------------------------------------------------
		void FileStream::Put(s8 inbyChar)
		{
			mFileStream.put(inbyChar);
		}
		//--------------------------------------------------------------------------------------------------
		/// Write
		//--------------------------------------------------------------------------------------------------
		void FileStream::Write(const s8* inpbyChar, s32 indwStreamSize)
		{
			mFileStream.write(inpbyChar, indwStreamSize);
		}
		//--------------------------------------------------------------------------------------------------
		/// Write
		//--------------------------------------------------------------------------------------------------
		void FileStream::Write(const std::string& _instrString)
		{
			mFileStream.write(_instrString.c_str(), _instrString.length());
		}
		//--------------------------------------------------------------------------------------------------
		/// TellP
		//--------------------------------------------------------------------------------------------------
		s32 FileStream::TellP()
		{
			return (s32)mFileStream.tellp();
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekP
		//--------------------------------------------------------------------------------------------------
		void FileStream::SeekP(s32 indwPosition)
		{
			mFileStream.seekp(indwPosition);
		}
		//--------------------------------------------------------------------------------------------------
		/// SeekP
		//--------------------------------------------------------------------------------------------------
		void FileStream::SeekP(s32 indwPosition, Core::SeekDir ineDir)
		{
			std::ios_base::seekdir dir;
			switch (ineDir)
			{
				default:
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
			mFileStream.seekp(indwPosition, dir);
		}
		//--------------------------------------------------------------------------------------------------
		/// Flush
		//--------------------------------------------------------------------------------------------------
		void FileStream::Flush()
		{
			mFileStream.flush();
		}
        //--------------------------------------------------------------------------------------------------
		/// Get File Mode
		//--------------------------------------------------------------------------------------------------
		std::ios_base::openmode FileStream::GetFileMode() const
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
