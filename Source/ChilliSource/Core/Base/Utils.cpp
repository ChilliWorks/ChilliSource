//
//  Utils.cpp
//  ChilliSource
//  Created by Andrew Glass on 09/08/2012.
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

#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Base/Application.h>

#include <json/json.h>
#include <zlib.h>

namespace ChilliSource
{
    namespace Utils
    {
        bool FileToString(StorageLocation ineStorageLocation, const std::string& instrPath, std::string& outstrFileContent)
        {
            auto pFile = Application::Get()->GetFileSystem()->CreateTextInputStream(ineStorageLocation, instrPath);
            
            if(pFile == nullptr)
            {
                CS_LOG_WARNING("Utils::FileToString: Could not open file: " + instrPath);
                return false;
            }
            
            outstrFileContent = pFile->ReadAll();
            
            return true;
        }
        
        TextOutputStreamUPtr StringToFile(StorageLocation ineStorageLocation, const std::string& instrPath, const std::string& instrFileOut)
        {
            auto pFile = Application::Get()->GetFileSystem()->CreateTextOutputStream(ineStorageLocation, instrPath, FileWriteMode::k_overwrite);
            
            if(pFile == nullptr)
            {
                CS_LOG_WARNING("Utils::StringToFile: Could not open file: " + instrPath);
                return nullptr;
            }
            
            pFile->Write(instrFileOut);
            
            return std::move(pFile);
        }
        
        bool ZlibCompressString(const std::string &instrUncompressed, std::string &outstrCompressed)
        {
            uLongf maxCompSize = (uLongf)(instrUncompressed.size() * 1.1f + 12);
            
            outstrCompressed.clear();
            outstrCompressed.resize(maxCompSize);
            
            s32 zlibResult = compress2(
                                      (Bytef*)(outstrCompressed.data()),
                                      &maxCompSize,
                                      (Bytef*)instrUncompressed.data(),
                                      uLong(instrUncompressed.size()),
                                       Z_BEST_COMPRESSION);
            
            
            if(zlibResult == Z_OK)
            {
                outstrCompressed.resize(maxCompSize);
                return true;
            }
            
            outstrCompressed.clear();
            return false;
        }

        bool ZlibDecompressString(const std::string &instrCompressed, std::string& outstrUncompress,  u32 inOutputSizeGuess)
        {
            outstrUncompress.clear();
            outstrUncompress.resize(inOutputSizeGuess);
            
            uLongf outBufferSize = inOutputSizeGuess;
            s32 zlibResult = uncompress(
                                        (Bytef*)(outstrUncompress.data()) ,
                                        &outBufferSize,
                                        (const Bytef*)instrCompressed.data(),
                                        uLong(instrCompressed.size()));
            
            if(zlibResult == Z_OK)
            {
                outstrUncompress.resize(outBufferSize);
                return true;
            }
            
            outstrUncompress.clear();
            return false;
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        u8 HexToDec(const u8* inpHex)
        {
            if(*inpHex >= '0' && *inpHex <= '9')
            {
                return *inpHex - '0';
            }
            else
                if(*inpHex >= 'A' && *inpHex <= 'F')
                {
                    return 10 + (*inpHex - 'A');
                }
                else
                {
                    CS_LOG_ERROR("Invalid hex value of \""+ToString(*inpHex)+"\"");
                }
            
            return -1;
        }
    }
}
