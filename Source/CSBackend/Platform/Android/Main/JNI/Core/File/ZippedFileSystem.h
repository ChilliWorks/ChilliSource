//
//  ZippedFileSystem.h
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_FILE_ZIPPEDFILESYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_FILE_ZIPPEDFILESYSTEM_H_

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/File/FileStreamApk.h>

#include <ChilliSource/ChilliSource.h>

#include <vector>
#include <utility>

namespace CSBackend
{
    namespace Android
    {
        //------------------------------------------------------------------------------
        /// Performs various file system style operations on the contents of a zip file.
        /// This includes operations such as creation of (virtual) file streams, copying
        /// a file and listing the contents of a directory.
        ///
        /// This is thread-safe, however some operations can only be performed by one
        /// thread at a time causing others to block. Such operations include creating
        /// file streams or copying files. Other operations, such as checking if a file
        /// exists, can occur concurrently.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class ZippedFileSystem final
        {
        public:
            CS_DECLARE_NOCOPY(ZippedFileSystem);
            //------------------------------------------------------------------------------
            /// Opens the zip file and builds a manifest of its contents. This is then used
            /// in all future operations for the sake of performance. This means that the
            /// zip file cannot change while this is in use.
            ///
            /// If only a subset of the zip contents is needed then a root path can be
            /// supplied. All operations are then relative to this root directory path.
            ///
            /// @author Ian Copland
            ///
            /// @param in_zipFilePath - The file path to the zip.
            /// @param in_rootDirectoryPath - [Optional] The root directory inside the zip.
            /// Defaults to the root of the zip file.
            //------------------------------------------------------------------------------
            ZippedFileSystem(const std::string& in_zipFilePath, const std::string& in_rootDirectoryPath = "");
            //------------------------------------------------------------------------------
            /// Creates a new "virtual" file stream to a file within the zip. The zipped
            /// file is inflated in full and stored in memory. The "virtual" file stream
            /// then treats this memory as if it were a file on disk.
            ///
            /// The memory location is read-only and write operations on the file stream
            /// are not possible.
            ///
            /// This can only be called by one thread at a time meaning others will block
            /// until it is finished. This can be a slow operation if accessing a large
            /// file, so care needs to be taken to ensure this doesn't cause visible
            /// stutters on the main thread.
            ///
            /// @author Ian Copland
            ///
            /// @param in_filePath - The file path inside the zip, relative to the "root
            /// diectory path".
            /// @param in_fileMode - The file mode. The file mode has to be a "read" file
            /// mode or this will assert.
            ///
            /// @return The file stream.
            //------------------------------------------------------------------------------
            CSCore::FileStreamUPtr CreateFileStream(const std::string& in_filePath, CSCore::FileMode in_fileMode);
            //------------------------------------------------------------------------------
            /// Copies a single file from inside the zip file to a real file path on disk.
            ///
            /// This can only be called by one thread at a time meaning others will block
            /// until it is finished. This can be a slow operation if copying a large
            /// file, so care needs to be taken to ensure this doesn't cause visible
            /// stutters on the main thread.
            ///
            /// @author Ian Copland
            ///
            /// @param in_sourceFilePath - The source file path inside the zip, relative to
            /// the "root diectory path".
            /// @param in_destinationFilePath - The destination file path on disk.
            ///
            /// @return Whether or not the file was copied successfully.
            //------------------------------------------------------------------------------
            bool CopyFile(const std::string& in_sourceFilePath, const std::string& in_destinationFilePath);
            //------------------------------------------------------------------------------
            /// Copies multiple files inside the zip to real file paths on disk. This is
            /// more efficient that multiple calls to copy file as it avoids repeated
            /// opening and closing of the zip file.
            ///
            /// This can only be called by one thread at a time meaning others will block
            /// until it is finished. This can be a slow operation if copying large files,
            /// so care needs to be taken to ensure this doesn't cause visible stutters on
            /// the main thread.
            ///
            /// @author Ian Copland
            ///
            /// @param in_filePaths - Pairs of file paths, the first being the source path
            /// within the zip, the second being the destination path on disk.
            //------------------------------------------------------------------------------
            bool CopyFiles(const std::vector<std::pair<std::string, std::string>>& in_filePaths);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_filePath - The file path inside the zip, relative to the "root
            /// diectory path".
            ///
            /// @return Whether or not the file path exists.
            //------------------------------------------------------------------------------
            bool DoesFileExist(const std::string& in_filePath);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_directoryPath - The directory path inside the zip, relative to the
            /// "root diectory path".
            ///
            /// @return Whether or not the directory path exists.
            //------------------------------------------------------------------------------
            bool DoesDirectoryExist(const std::string& in_directoryPath);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_directoryPath - The directory path inside the zip, relative to the
            /// "root diectory path".
            /// @param in_recursive - Whether or not to recurse into sub-directories.
            ///
            /// @return A list of all files within the given directory path.
            //------------------------------------------------------------------------------
            std::vector<std::string> GetFilePaths(const std::string& in_directoryPath, bool in_recursive);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_directoryPath - The directory path inside the zip, relative to the
            /// "root diectory path".
            /// @param in_recursive - Whether or not to recurse into sub-directories.
            ///
            /// @return A list of all directories within the given directory path.
            //------------------------------------------------------------------------------
            std::vector<std::string> GetDirectoryPaths(const std::string& in_directoryPath, bool in_recursive);
        };
    }
}

#endif