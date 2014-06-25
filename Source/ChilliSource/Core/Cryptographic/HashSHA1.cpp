//
//  HashSHA1.cpp
//  Chilli Source
//  Created by Ian Copland on 09/06/2014.
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


#include <ChilliSource/Core/Cryptographic/HashSHA1.h>

#include <SHA1/SHA1.h>

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        namespace HashSHA1
        {
            //------------------------------------------------
            //------------------------------------------------
            std::string GenerateHexHashCode(const s8* in_data, u32 in_size)
            {
                const u32 k_sha1Length = 80;
                
                CSHA1 hash;
                hash.Reset();
                hash.Update(reinterpret_cast<const u8*>(in_data), in_size);
                hash.Final();
                
#ifdef CS_TARGETPLATFORM_WINDOWS
                TCHAR cHash[k_sha1Length];
                memset(cHash, 0, k_sha1Length);
                hash.ReportHash(cHash, CSHA1::REPORT_HEX_SHORT);
				return CSBackend::Windows::WindowsStringUtils::UTF16ToUTF8(std::wstring(cHash));
#else
				s8 cHash[k_sha1Length];
				memset(cHash, 0, k_sha1Length);
				hash.ReportHash(cHash, CSHA1::REPORT_HEX_SHORT);
				return std::string(cHash);
#endif
            }
        }
    }
}
