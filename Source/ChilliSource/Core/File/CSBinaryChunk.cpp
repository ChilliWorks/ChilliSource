//
//  CSBinaryChunk.cpp
//  Chilli Source
//  Created by Ian Copland on 28/10/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Core/File/CSBinaryChunk.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        CSBinaryChunk::CSBinaryChunk(std::unique_ptr<u8[]> in_dataBlob, u32 in_dataSize)
        : m_dataBlob(std::move(in_dataBlob)), m_dataSize(in_dataSize)
        {
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        u32 CSBinaryChunk::GetSize() const
        {
            return m_dataSize;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        const u8* CSBinaryChunk::GetData() const
        {
            return m_dataBlob.get();
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        u32 CSBinaryChunk::GetReadPosition() const
        {
            return m_readPosition;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        const u8* CSBinaryChunk::Read(u32 in_dataSize)
        {
            if (m_readPosition + in_dataSize > m_dataSize)
            {
                CS_LOG_FATAL("Trying to read beyond the end of a Binary Chunk.");
            }
            
            u8* output = m_dataBlob.get() + m_readPosition;
            m_readPosition += in_dataSize;
            return output;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        std::unique_ptr<u8[]> CSBinaryChunk::ClaimData()
        {
            m_dataSize = 0;
            m_readPosition = 0;
            return std::move(m_dataBlob);
        }
    }
}