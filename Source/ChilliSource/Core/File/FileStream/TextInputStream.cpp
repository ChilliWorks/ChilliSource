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

#include <ChilliSource/Core/File/FileStream/TextInputStream.h>

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    TextInputStream::TextInputStream(const std::string& filePath) noexcept
    {
        m_filename = filePath;
        
        //Opening the file in binary mode as tellg does not function correctly on all platforms in
        //::in mode after a read is carried out on the stream
        m_fileStream.open(m_filename.c_str(), std::ios_base::binary);
        
        m_isValid = m_fileStream.is_open() && !m_fileStream.bad() && !m_fileStream.fail();
        
        if(m_isValid)
        {
            m_fileStream.seekg(0, m_fileStream.end);
            m_length = m_fileStream.tellg();
            m_fileStream.seekg(0, m_fileStream.beg);
        }
    }
    //------------------------------------------------------------------------------
    TextInputStream::~TextInputStream() noexcept
    {
        if(m_fileStream.is_open())
        {
            m_fileStream.close();
        }
    }
    //------------------------------------------------------------------------------
    bool TextInputStream::IsValid() const noexcept
    {
        return m_isValid;
    }
    //------------------------------------------------------------------------------
    u64 TextInputStream::GetLength() const noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");

        return m_length;
    }
    //------------------------------------------------------------------------------
    u64 TextInputStream::GetReadPosition() noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        return m_fileStream.tellg();
    }
    //------------------------------------------------------------------------------
    void TextInputStream::SetReadPosition(u64 readPosition) noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        CS_ASSERT(readPosition <= GetLength(), "Position out of bounds!");

        m_fileStream.seekg(readPosition);
    }
    //------------------------------------------------------------------------------
    std::string TextInputStream::ReadAll() noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        
        //Reset the read position to the beginning
        SetReadPosition(0);
        
        std::stringstream stringStream;
        stringStream << m_fileStream.rdbuf();
        
        std::string fileContents = stringStream.str();
        
        return fileContents;
    }
    //------------------------------------------------------------------------------
    bool TextInputStream::ReadLine(std::string& line) noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        
        if(m_fileStream.eof())
        {
            return false;
        }
        
        std::getline(m_fileStream, line);
        
        //Shouldn't check fail here as the fail bit will be set on a read overrun, which we handle
        CS_ASSERT(!m_fileStream.bad(), "Unexpected error occured in filestream");
        
        //Need to carry out another eof check here, as the check at the start of the function may
        //not catch allcases. The EOF bit is only set when a read operation is attempted, not a seekg,
        //so it may only be set after the above getline.
        if(m_fileStream.eof())
        {
            return !line.empty();
        }
        
        return true;
    }
    //------------------------------------------------------------------------------
    bool TextInputStream::Read(u64 length, std::string& readChars) noexcept
    {
        CS_ASSERT(IsValid(), "Trying to use an invalid FileStream.");
        
        if(m_fileStream.eof())
        {
            return false;
        }
        
        readChars.resize((std::string::size_type)length);
        
        //A string is guaranteed to be in contiguous memory in the c++11 standard, but not in c++03.
        //this allows the below overwrite to work
        m_fileStream.read(&readChars[0], (std::streamsize)length);
        
        //Shouldn't check fail here as the fail bit will be set on a read overrun, which we handle
        CS_ASSERT(!m_fileStream.bad(), "Unexpected error occured in filestream");
        
        readChars.resize(m_fileStream.gcount());
        
        //Need to carry out another eof check here, as the check at the start of the function may
        //not catch allcases. The EOF bit is only set when a read operation is attempted, not a seekg,
        //so it may only be set after the above read.
        if(m_fileStream.eof())
        {
            return m_fileStream.gcount() != 0;
        }
        
        return true;
    }
}
