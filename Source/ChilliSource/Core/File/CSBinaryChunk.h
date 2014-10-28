//
//  CSBinaryChunk.h
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

#ifndef _CHILLISOURCE_CORE_FILE_CSBINARYCHUNK_H_
#define _CHILLISOURCE_CORE_FILE_CSBINARYCHUNK_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------------
        /// A single chunk within a Chilli Source binary "Chunked" file. The
        /// contents of the chunk are loaded directly into memory on creation
        /// meaning each call to Read() avoids further disc reads.
        ///
        /// CSBinaryChunk is thread agnostic and can be used and accessed on any
        /// thread but it is not thread-safe, so a single instance should not
        /// be shared accross different threads at the same time.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------------------
        class CSBinaryChunk final
        {
        public:
            CS_DECLARE_NOCOPY(CSBinaryChunk);
            //----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The size of the chunk in bytes.
            //----------------------------------------------------------------
            u32 GetSize() const;
            //----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The raw underlying chunk data. This returns the entire
            /// chunk, disregarding the read position.
            //----------------------------------------------------------------
            const u8* GetData() const;
            //----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The current read position.
            //----------------------------------------------------------------
            u32 GetReadPosition() const;
            //----------------------------------------------------------------
            /// Reads a value from the given chunk at the read position. After
            /// the read, the read position is incremented by the size of the
            /// requested type.
            ///
            /// If this attempts to read beyond the end of the chunk a fatal
            /// error will be logged.
            ///
            /// @author Ian Copland
            ///
            /// @return The value.
            //----------------------------------------------------------------
            template <typename TType> TType Read();
            //----------------------------------------------------------------
            /// Reads the requested size of data from the chunk at the current
            /// read position. After the read, the read position is incremented
            /// by the given data size. The returned data is not a copy and
            /// should not be altered.
            ///
            /// If this attempts to read beyond the end of the chunk a fatal
            /// error will be logged.
            ///
            /// @author Ian Copland
            ///
            /// @return The value.
            //----------------------------------------------------------------
            const u8* Read(u32 in_dataSize);
            //----------------------------------------------------------------
            /// Claims the chunks memory. This is used in cases where the
            /// entire contents of the chunk is desired without having to
            /// copy the data, for example image data. After this has been
            /// called the chunk can no longer be used.
            ///
            /// @author Ian Copland
            ///
            /// @return The size of the chunk in bytes.
            //----------------------------------------------------------------
            std::unique_ptr<u8[]> ClaimData();
        private:
            friend class CSBinaryInputStream;
            //----------------------------------------------------------------
            /// Constructor. Creates the chunk with a pointer to the chunk
            /// data blob. This is private to ensure only the CS Binary Input
            /// Stream can instantiate it.
            ///
            /// @author Ian Copland
            ///
            /// @param The chunk data blob.
            /// @param The chunk data size.
            //----------------------------------------------------------------
            CSBinaryChunk(std::unique_ptr<u8[]> in_dataBlob, u32 in_dataSize);
            
            std::unique_ptr<u8[]> m_dataBlob;
            u32 m_dataSize = 0;
            u32 m_readPosition = 0;
        };
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        template <typename TType> TType CSBinaryChunk::Read()
        {
            const u8* data = Read(sizeof(TType));
            const TType output = *reinterpret_cast<const TType*>(data);
            return output;
        }
    }
}

#endif
