//
//  InputTextStream.cpp
//  ChilliSource
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

#include <ChilliSource/Core/File/FileStream/InputTextStream.h>

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    InputTextStream::InputTextStream(const std::string& filePath)
    {
        m_filename = filePath;
        
        //Opening the file in binary mode as tellg does not function correctly on all platforms in ::in mode
        //after a read is carried out on the stream
        m_fileStream.open(m_filename.c_str(), std::ios_base::binary);
        
        m_isValid = m_fileStream.is_open() == true && m_fileStream.bad() == false && m_fileStream.fail() == false;
        
        if(m_isValid == true)
        {
            // get length of file:
            m_fileStream.seekg(0, m_fileStream.end);
            m_length = m_fileStream.tellg();
            m_fileStream.seekg(0, m_fileStream.beg);
        }
    }
    //------------------------------------------------------------------------------
    InputTextStream::~InputTextStream()
    {
        if(m_fileStream.is_open())
        {
            m_fileStream.close();
        }
    }
    //------------------------------------------------------------------------------
    bool InputTextStream::IsValid() const noexcept
    {
        return m_isValid;
    }
    //------------------------------------------------------------------------------
    u64 InputTextStream::GetLength() const noexcept
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");

        return m_length;
    }
    //------------------------------------------------------------------------------
    u64 InputTextStream::GetReadPosition() noexcept
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");
        return m_fileStream.tellg();
    }
    //------------------------------------------------------------------------------
    void InputTextStream::SetReadPosition(u64 readPosition) noexcept
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");
        CS_ASSERT(readPosition <= GetLength(), "Position out of bounds!");

        m_fileStream.seekg(readPosition);
    }
    //------------------------------------------------------------------------------
    std::string InputTextStream::ReadAll() noexcept
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");
        
        //Reset the read position to the beginning
        SetReadPosition(0);
        
        std::stringstream stringStream;
        stringStream << m_fileStream.rdbuf();
        
        std::string fileContents = stringStream.str();
        
        return fileContents;
    }
    //------------------------------------------------------------------------------
    bool InputTextStream::ReadLine(std::string& line) noexcept
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");
        
        bool success = false;
        
        if(!m_fileStream.eof())
        {
            std::getline(m_fileStream, line);
            
            if(m_fileStream.bad())
            {
                //If the badbit is set the stream is most likely in an invalid state
                m_isValid = false;
                
                //if getline fails we check if the bad bit has been set, then print the error with perror
                perror(("ReadLine failed on file " + m_filename).c_str());
            }
            else
            {
                success = true;
            }
        }
        
        return success;
    }
    //------------------------------------------------------------------------------
    bool InputTextStream::Read(u64 length, std::string& readChars) noexcept
    {
        CS_ASSERT(IsValid() == true, "Trying to use an invalid FileStream.");
        
        bool success = false;
        
        if(!m_fileStream.eof())
        {
            //Ensure the outstring is big enough to store the result
            if(readChars.size() < length)
            {
                readChars.resize(length, ' ');
            }
            
            //If the length of the stream is overrun, readChars will contain everything up to that point and the EOF
            //bit will be set
            //Note::A string is guaranteed to be in contiguous memory in the c++11 standard, but not in c++03.
            //this allows the below overwrite to work
            m_fileStream.read(&readChars[0], length);
    
            if(m_fileStream.eof())
            {
                //If we prematurely reached the eof then we need to resize the string buffer to contain only
                //the read characters, retrieved through gcount
                u32 readCount = m_fileStream.gcount();
                
                if(readCount != readChars.size())
                {
                    readChars.resize(readCount);
                }
                
                success = readChars.size() != 0;
            }
            else if(m_fileStream.bad())
            {
                //If the badbit is set the stream is most likely in an invalid state
                m_isValid = false;
                
                //if getline fails we check if the bad bit has been set, then print the error with perror
                perror(("ReadLine failed on file " + m_filename).c_str());
            }
            else
            {
                success = true;
            }
        }
        
        return success;
    }
}