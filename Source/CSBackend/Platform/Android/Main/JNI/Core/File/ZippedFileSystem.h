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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileStream.h>

#include <minizip/unzip.h>

#include <mutex>
#include <unordered_map>
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
        /// This is thread-safe and many operations can be performed without locking.
        /// This isn't possible for some operations, however, such as creating a file
        /// stream or copying files. In these cases care needs to be taken to avoid
        /// negatively hurting performance.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class ZippedFileSystem final
        {
        public:
            CS_DECLARE_NOCOPY(ZippedFileSystem);
            //------------------------------------------------------------------------------
            /// A struct containing information about the location of a single file within
            /// the zip. This can be used to manually access the zip data.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct FileInfo final
            {
                u32 m_offset = 0;
                u32 m_size = 0;
                u32 m_uncompressedSize = 0;
                bool m_isCompressed = false;
            };
            //------------------------------------------------------------------------------
            /// A delegate called by the ExtractFiles() method as each file is read from
            /// the zip.
            ///
            /// @author Ian Copland
            ///
            /// @param in_fileName - The path to the file which this callback refers to.
            /// @param in_fileContents - The entire contents of the file.
            /// @param in_fileSize - The size of the file.
            ///
            /// @return False should be returned if an error has occurred and ExtractFiles()
            /// should no longer continue.
            //------------------------------------------------------------------------------
            using FileReadDelegate = std::function<bool(const std::string& in_filePath, std::unique_ptr<const u8[]> in_fileContents, u32 in_fileSize)>;
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
            /// This should be checked prior to using any of the other methods. If this
            /// returns false the ZippedFileSystem should be discarded.
            ///
            /// @author Ian Copland
            ///
            /// @return Whether or not the zip has been successfully opened and is ready
            /// for use.
            //------------------------------------------------------------------------------
            bool IsValid() const;
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
            /// directory path".
            /// @param in_fileMode - The file mode. The file mode has to be a "read" file
            /// mode or this will assert.
            ///
            /// @return The file stream.
            //------------------------------------------------------------------------------
            ChilliSource::FileStreamUPtr CreateFileStream(const std::string& in_filePath, ChilliSource::FileMode in_fileMode) const;
            //------------------------------------------------------------------------------
            /// Opens a series of files one by one without closing the zip. The contents of
            /// each file are returned via the FileReadDelegate.
            ///
            /// While each file could be loaded individually using CreateFileStream(), it
            /// would involve repeatedly opening and closing the zip file. For bulk
            /// operations - i.e copying a directory - that approach would be fairly
            /// inefficient. This method doesn't close the zip after each file stream is
            /// created and acts as a more efficient alternate.
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
            /// @param in_delegate - The file read delegate. Called as each file is read.
            ///
            /// @return Whether or not this was successful. If any file fails to extract
            /// this will return null.
            //------------------------------------------------------------------------------
            bool ExtractFiles(const std::vector<std::string>& in_filePaths, const FileReadDelegate& in_delegate) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_filePath - The file path inside the zip, relative to the "root
            /// directory path".
            ///
            /// @return Whether or not the file path exists.
            //------------------------------------------------------------------------------
            bool DoesFileExist(const std::string& in_filePath) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_directoryPath - The directory path inside the zip, relative to the
            /// "root directory path".
            ///
            /// @return Whether or not the directory path exists.
            //------------------------------------------------------------------------------
            bool DoesDirectoryExist(const std::string& in_directoryPath) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_directoryPath - The directory path inside the zip, relative to the
            /// "root directory path".
            /// @param in_recursive - Whether or not to recurse into sub-directories.
            ///
            /// @return A list of all files within the given directory path.
            //------------------------------------------------------------------------------
            std::vector<std::string> GetFilePaths(const std::string& in_directoryPath, bool in_recursive) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_directoryPath - The directory path inside the zip, relative to the
            /// "root directory path".
            /// @param in_recursive - Whether or not to recurse into sub-directories.
            ///
            /// @return A list of all directories within the given directory path.
            //------------------------------------------------------------------------------
            std::vector<std::string> GetDirectoryPaths(const std::string& in_directoryPath, bool in_recursive) const;
            //------------------------------------------------------------------------------
            /// Gets information on a single file within the zip. This information requires
            /// the zip file to be opened and therefore requires a lock to remain thread
            /// safe.
            ///
            /// @author Ian Copland
            ///
            /// @param in_filePath - The path to the file within the zip.
            /// @param out_fileInfo - [Out] The output file info. This will only be set if
            /// the request was successful.
            ///
            /// @return Whether or not this was successful.
            //------------------------------------------------------------------------------
            bool TryGetFileInfo(const std::string& in_filePath, FileInfo& out_fileInfo) const;

        private:
            //------------------------------------------------------------------------------
            /// A container for the information on a item within the zip file.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct ManifestItem
            {
                u32 m_pathHash = 0;
                std::string m_path;
                bool m_isFile = false;
                unz_file_pos m_zipPosition;
            };
            //------------------------------------------------------------------------------
            /// Builds a manifest of the contents of the given root directory within the
            /// zip file. If the entire zip is required then an empty root directory path
            /// should be given.
            ///
            /// This *must* only be called from the constructor. The class needs to be
            /// immutable after construction to allow lockless thread-safety on many of
            /// the methods. Building the manifest after construction would violate this.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void BuildManifest(const std::string& in_rootDirectoryPath);
            //------------------------------------------------------------------------------
            /// Adds a new item to the manifest. If the directory path the manifest is
            /// located in (or any parent directories) doesn't exist in the manifest then
            /// they will be added as well.
            ///
            /// This *must* only be called during construction. The class needs to be
            /// immutable after construction to allow lockless thread-safety on many of
            /// the methods. Building the manifest after construction would violate this.
            ///
            /// @author Ian Copland
            ///
            /// @param in_filePath - The file path which should be added to the manifest.
            /// @param in_zipPosition - The position of the file inside the zip.
            /// @param in_isFile - Whether or not the entry is a file.
            //------------------------------------------------------------------------------
            void AddItemToManifest(const std::string& in_filePath, unz_file_pos in_zipPosition, bool in_isFile);
            //------------------------------------------------------------------------------
            /// Searches the manifest item list for the manifest item with the given path.
            /// If it is found true is returned and the output manifest item is set. If it
            /// is not found false is returned and no value is set for the output manifest
            /// item.
            ///
            /// @author Ian Copland
            ///
            /// @param in_path - The file or directory path to look up.
            /// @param out_manifestItem - [Out] The manifest item if successful.
            ///
            /// @return Whether or not the look up was successful.
            //------------------------------------------------------------------------------
            bool TryGetManifestItem(const std::string& in_path, ManifestItem& out_manifestItem) const;

            std::string m_filePath;
            bool m_isValid = false;
            std::vector<ManifestItem> m_manifestItems;

            mutable std::mutex m_mutex;
        };
    }
}

#endif