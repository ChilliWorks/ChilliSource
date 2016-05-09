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
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    FileStream::FileStream(const std::string& in_filePath, FileMode in_fileMode)
    {
        mstrFilename = in_filePath;
        meFileMode = in_fileMode;

        mFileStream.open(mstrFilename.c_str(), GetFileMode());

        m_isValid = mFileStream.is_open() == true && mFileStream.bad() == false && mFileStream.fail() == false;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool FileStream::IsValid() const
    {
        return m_isValid;
    }
    //--------------------------------------------------------------
    /// Get MD5 Checksum 
    ///
    /// Calculate the MD5 checksum of the file 
    ///
    /// @return MD5 checksum
    //--------------------------------------------------------------
    std::string FileStream::GetMD5Checksum()
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

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
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

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
    /// End Of File
    //--------------------------------------------------------------------------------------------------
    bool FileStream::EndOfFile()
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        return mFileStream.eof();
    }
    //--------------------------------------------------------------------------------------------------
    /// Get
    //--------------------------------------------------------------------------------------------------
    s32 FileStream::Get()
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        return mFileStream.get();
    }
    //--------------------------------------------------------------------------------------------------
    /// Get
    //--------------------------------------------------------------------------------------------------
    void FileStream::Get(s8 & outbyChar)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.get(outbyChar);
    }
    //--------------------------------------------------------------------------------------------------
    /// Get
    //--------------------------------------------------------------------------------------------------
    void FileStream::Get(s8 * outpbyString, s32 indwStreamSize)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.get(outpbyString, indwStreamSize);
    }
    //--------------------------------------------------------------------------------------------------
    /// Get
    //--------------------------------------------------------------------------------------------------
    void FileStream::Get(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.get(outpbyString, indwStreamSize, inbyDelim);
    }
    //--------------------------------------------------------------------------------------------------
    /// Get Line
    //--------------------------------------------------------------------------------------------------
    void FileStream::GetLine(std::string &outstrString)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream >> outstrString;
    }
    //--------------------------------------------------------------------------------------------------
    /// Get All
    //--------------------------------------------------------------------------------------------------
    void FileStream::GetAll(std::string &outstrString)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        std::stringstream cStream;
        cStream << mFileStream.rdbuf();
        
        outstrString = cStream.str();
    }
    //--------------------------------------------------------------------------------------------------
    /// Get
    //--------------------------------------------------------------------------------------------------
    void FileStream::Get(std::stringstream &outstrStringStream)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        outstrStringStream << mFileStream.rdbuf();
    }
    //--------------------------------------------------------------------------------------------------
    /// Get Line
    //--------------------------------------------------------------------------------------------------
    void FileStream::GetLine(s8 * outpbyString, s32 indwStreamSize)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.getline(outpbyString, indwStreamSize);
    }
    //--------------------------------------------------------------------------------------------------
    /// Get Line
    //--------------------------------------------------------------------------------------------------
    void FileStream::GetLine(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.getline(outpbyString, indwStreamSize, inbyDelim);
    }
    //--------------------------------------------------------------------------------------------------
    /// Ignore
    //--------------------------------------------------------------------------------------------------
    void FileStream::Ignore(s32 indwStreamSize, s8 inbyDelim)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.ignore(indwStreamSize, inbyDelim);
    }
    //--------------------------------------------------------------------------------------------------
    /// Peek
    //--------------------------------------------------------------------------------------------------
    s32 FileStream::Peek()
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        return mFileStream.peek();
    }
    //--------------------------------------------------------------------------------------------------
    /// Read
    //--------------------------------------------------------------------------------------------------
    void FileStream::Read(s8* inpbyBuffer, s32 indwStreamSize)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.read(inpbyBuffer, indwStreamSize);
    }
    //--------------------------------------------------------------------------------------------------
    /// Put Back
    //--------------------------------------------------------------------------------------------------
    void FileStream::PutBack(s8 inbyChar)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.putback(inbyChar);
    }
    //--------------------------------------------------------------------------------------------------
    /// Unget
    //--------------------------------------------------------------------------------------------------
    void FileStream::Unget()
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.unget();
    }
    //--------------------------------------------------------------------------------------------------
    /// TellG
    //--------------------------------------------------------------------------------------------------
    s32 FileStream::TellG()
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        return (s32)mFileStream.tellg();
    }
    //--------------------------------------------------------------------------------------------------
    /// SeekG
    //--------------------------------------------------------------------------------------------------
    void FileStream::SeekG(s32 indwPosition)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.seekg(indwPosition);
    }
    //--------------------------------------------------------------------------------------------------
    /// SeekG
    //--------------------------------------------------------------------------------------------------
    void FileStream::SeekG(s32 indwPosition, SeekDir ineDir)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        std::ios_base::seekdir dir;
        switch (ineDir)
        {
            default:
            case SeekDir::k_beginning:
                dir = std::ios_base::beg;
                break;
            case SeekDir::k_current:
                dir = std::ios_base::cur;
                break;
            case SeekDir::k_end:
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
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        return mFileStream.sync();
    }
    //--------------------------------------------------------------------------------------------------
    /// Put
    //--------------------------------------------------------------------------------------------------
    void FileStream::Put(s8 inbyChar)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.put(inbyChar);
    }
    //--------------------------------------------------------------------------------------------------
    /// Write
    //--------------------------------------------------------------------------------------------------
    void FileStream::Write(const s8* inpbyChar, s32 indwStreamSize)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.write(inpbyChar, indwStreamSize);
    }
    //--------------------------------------------------------------------------------------------------
    /// Write
    //--------------------------------------------------------------------------------------------------
    void FileStream::Write(const std::string& _instrString)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.write(_instrString.c_str(), _instrString.length());
    }
    //--------------------------------------------------------------------------------------------------
    /// TellP
    //--------------------------------------------------------------------------------------------------
    s32 FileStream::TellP()
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        return (s32)mFileStream.tellp();
    }
    //--------------------------------------------------------------------------------------------------
    /// SeekP
    //--------------------------------------------------------------------------------------------------
    void FileStream::SeekP(s32 indwPosition)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.seekp(indwPosition);
    }
    //--------------------------------------------------------------------------------------------------
    /// SeekP
    //--------------------------------------------------------------------------------------------------
    void FileStream::SeekP(s32 indwPosition, SeekDir ineDir)
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        std::ios_base::seekdir dir;
        switch (ineDir)
        {
            default:
            case SeekDir::k_beginning:
                dir = std::ios_base::beg;
                break;
            case SeekDir::k_current:
                dir = std::ios_base::cur;
                break;
            case SeekDir::k_end:
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
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        mFileStream.flush();
    }
    //--------------------------------------------------------------------------------------------------
    /// Get File Mode
    //--------------------------------------------------------------------------------------------------
    std::ios_base::openmode FileStream::GetFileMode() const
    {
        switch (meFileMode)
        {
            case FileMode::k_read:
                return (std::ios_base::in);
            case FileMode::k_readBinary:
                return (std::ios_base::in | std::ios_base::binary);
            case FileMode::k_write:
                return (std::ios_base::out);
            case FileMode::k_writeAppend:
                return (std::ios_base::out | std::ios_base::app);
            case FileMode::k_writeAtEnd:
                return (std::ios_base::out | std::ios_base::ate);
            case FileMode::k_writeTruncate:
                return (std::ios_base::out | std::ios_base::trunc);
            case FileMode::k_writeBinary:
                return (std::ios_base::out | std::ios_base::binary);
            case FileMode::k_writeBinaryAppend:
                return (std::ios_base::out | std::ios_base::binary | std::ios_base::app);
            case FileMode::k_writeBinaryAtEnd:
                return (std::ios_base::out | std::ios_base::binary | std::ios_base::ate);
            case FileMode::k_writeBinaryTruncate:
                return (std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
            default:
                return (std::ios_base::in);
        }
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    FileStream::~FileStream()
    {
        if (IsValid() == true)
        {
            mFileStream.close();
        }
    }
}
