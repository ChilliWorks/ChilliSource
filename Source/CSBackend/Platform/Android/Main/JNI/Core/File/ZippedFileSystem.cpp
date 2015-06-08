//
//  ZippedFileSystem.cpp
//  ChilliSource
//  Created by Ian Copland on 08/06/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <CSBackend/Platform/Android/Main/JNI/Core/File/ZippedFileSystem.h>

namespace CSBackend
{
    namespace Android
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        ZippedFileSystem::ZippedFileSystem(const std::string& in_zipFilePath, const std::string& in_rootDirectoryPath)
        {
            CS_LOG_ERROR(">>> Created ZippedFileSystem with file '" + in_zipFilePath + "' and root '" + in_rootDirectoryPath + "'.");

            //TODO: !?
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSCore::FileStreamUPtr ZippedFileSystem::CreateFileStream(const std::string& in_filePath, CSCore::FileMode in_fileMode)
        {
            //TODO: !?
            return nullptr;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::CopyFile(const std::string& in_sourceFilePath, const std::string& in_destinationFilePath)
        {
            //TODO: !?
            return false;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::CopyFiles(const std::vector<std::pair<std::string, std::string>>& in_filePaths)
        {
            //TODO: !?
            return false;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::DoesFileExist(const std::string& in_filePath)
        {
            //TODO: !?
            return false;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::DoesDirectoryExist(const std::string& in_directoryPath)
        {
            //TODO: !?
            return false;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        std::vector<std::string> ZippedFileSystem::GetFilePaths(const std::string& in_directoryPath, bool in_recursive)
        {
            //TODO: !?
            return std::vector<std::string>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        std::vector<std::string> ZippedFileSystem::GetDirectoryPaths(const std::string& in_directoryPath, bool in_recursive)
        {
            //TODO: !?
            return std::vector<std::string>();
        }
    }
}