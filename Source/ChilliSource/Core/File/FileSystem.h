//
//  FileSystem.h
//  Chilli Source
//  Created by Ian Copland on 25/03/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_FILE_FILESYSTEM_H_
#define _CHILLISOURCE_CORE_FILE_FILESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <mutex>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// A system for handling cross platform access to the file system. This
    /// provides sandboxed access to specific locations on disk, such as access to
    /// application assets, or save data. It is safe to use the File System during
    /// the OnInit and OnDestroy lifecycle events.
    ///
    /// This is thread-safe though care still needs to be taken when dealing with
    /// file streams as they are not. A file stream should be read and destroyed
    /// on the same thread it was created.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    class FileSystem : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(FileSystem);
        //------------------------------------------------------------------------------
        /// Reads the contents of a file from disc if the file exists.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The Storage Location.
        /// @param in_filePath - The file path.
        /// @param out_contents - [Out] The contents of the file.
        ///
        /// @return Whether or not the file read was successful.
        //------------------------------------------------------------------------------
        bool ReadFile(StorageLocation in_storageLocation, const std::string& in_filePath, std::string& out_contents) const;
        //------------------------------------------------------------------------------
        /// Write a file to disc with the given data.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The Storage Location.
        /// @param in_filePath - The file path.
        /// @param in_contents - The contents of the file.
        ///
        /// @return Whether or not the file was successfully written.
        //------------------------------------------------------------------------------
        bool WriteFile(StorageLocation in_storageLocation, const std::string& in_filePath, const std::string& in_contents) const;
        //------------------------------------------------------------------------------
        /// Write a file to disc with the given data.
        ///
        /// This is thread-safe.
        ///
        /// @author S Downie
        ///
        /// @param in_storageLocation - The Storage Location.
        /// @param in_filePath - The file path.
        /// @param in_data - The data.
        /// @param in_dataSize - The size of the data.
        ///
        /// @return Whether or not the file was successfully written.
        //------------------------------------------------------------------------------
        bool WriteFile(StorageLocation in_storageLocation, const std::string& in_filePath, const s8* in_data, u32 in_dataSize) const;
        //------------------------------------------------------------------------------
        /// Creates a new file stream to the given file in the given storage location.
        ///
        /// This is thread-safe, though the created file stream is not. Make sure it
        /// is used and destroyed on the same thread it was created.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The storage location.
        /// @param in_filePath - The file path.
        /// @param in_fileMode - The file mode.
        ///
        /// @return The new file stream. If the stream cannot be created or is invalid,
        /// null be returned.
        //------------------------------------------------------------------------------
        virtual FileStreamUPtr CreateFileStream(StorageLocation in_storageLocation, const std::string& in_filePath, FileMode in_fileMode) const = 0;
        //------------------------------------------------------------------------------
        /// Creates the given directory. The full directory hierarchy will be created.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The Storage Location
        /// @param in_directoryPath - The directory path.
        ///
        /// @return Whether or not this was successful. Failure to create the directory
        /// because it already exists is considered a success.
        //------------------------------------------------------------------------------
        virtual bool CreateDirectoryPath(StorageLocation in_storageLocation, const std::string& in_directoryPath) const = 0;
        //------------------------------------------------------------------------------
        /// Copies a file from one location to another.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_sourceStorageLocation - The source storage location.
        /// @param in_sourceFilePath - The source directory.
        /// @param in_destinationStorageLocation - The destination storage location.
        /// @param in_destinationFilePath - The destination directory.
        ///
        /// @return Whether or not the file was successfully copied.
        //------------------------------------------------------------------------------
        virtual bool CopyFile(StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
                              StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const = 0;
        //------------------------------------------------------------------------------
        /// Copies a directory from one location to another. If the destination
        /// directory does not exist, it will be created.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_sourceStorageLocation - The source storage location.
        /// @param in_sourceDirectoryPath - The source directory.
        /// @param in_destinationStorageLocation - The destination storage location.
        /// @param in_destinationDirectoryPath - The destination directory.
        ///
        /// @return Whether or not the files were successfully copied.
        //------------------------------------------------------------------------------
        virtual bool CopyDirectory(StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
                                   StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const = 0;
        //------------------------------------------------------------------------------
        /// Deletes the specified file.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The storage location.
        /// @param in_filePath - The filepath.
        ///
        /// @return Whether or not the file was successfully deleted.
        //------------------------------------------------------------------------------
        virtual bool DeleteFile(StorageLocation in_storageLocation, const std::string& in_filePath) const = 0;
        //------------------------------------------------------------------------------
        /// Deletes a directory and all its contents.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The storage location.
        /// @param in_directoryPath - The directory.
        ///
        /// @return Whether or not the directory was successfully deleted.
        //------------------------------------------------------------------------------
        virtual bool DeleteDirectory(StorageLocation in_storageLocation, const std::string& in_directoryPath) const = 0;
        //------------------------------------------------------------------------------
        /// Creates a dynamic array containing the file names of each file in the given
        /// directory. File paths will be relative to the input directory.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The Storage Location
        /// @param in_directoryPath - The directory
        /// @param in_recursive - Flag to determine whether or not to recurse into sub
        /// directories
        ///
        /// @return dynamic array containing the file names.
        //------------------------------------------------------------------------------
        virtual std::vector<std::string> GetFilePaths(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const = 0;
        //------------------------------------------------------------------------------
        /// Creates a dynamic array containing the file names of each file that has the
        /// provided extension in the given directory. File paths will be returned
        /// relative to the input directory.
        ///
        /// This is thread-safe.
        ///
        /// @author S Downie
        ///
        /// @param in_storageLocation - The Storage Location
        /// @param in_directoryPath - The directory path.
        /// @param in_recursive - Flag to determine whether or not to recurse into sub
        /// directories
        /// @param in_extension - The extension
        ///
        /// @return Dynamic array containing the file names.
        //------------------------------------------------------------------------------
        std::vector<std::string> GetFilePathsWithExtension(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, const std::string& in_extension) const;
        //------------------------------------------------------------------------------
        /// Creates a dynamic array containing the file names of each of each file with
        /// the given name in the given directory. File paths will be relative to the
        /// input directory.
        ///
        /// This is thread-safe.
        ///
        /// @author S Downie
        ///
        /// @param in_storageLocation - The Storage Location.
        /// @param in_directoryPath - The directory.
        /// @param in_recursive - Flag to determine whether or not to recurse into sub
        /// directories.
        /// @param in_fileName - The file name.
        ///
        /// @return Dynamic array containing the file names.
        //------------------------------------------------------------------------------
        std::vector<std::string> GetFilePathsWithFileName(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, const std::string& in_fileName) const;
        //------------------------------------------------------------------------------
        /// Creates a dynamic array containing the names of each directory in the given
        /// directory. Directory paths will be relative to the input directory.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The Storage Location
        /// @param in_directoryPath - The directory
        /// @param in_recursive - Flag to determine whether or not to recurse into sub
        /// directories
        ///
        /// @return Output dynamic array containing the dir names.
        //------------------------------------------------------------------------------
        virtual std::vector<std::string> GetDirectoryPaths(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const = 0;
        //------------------------------------------------------------------------------
        /// returns whether or not the given file exists.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        /// 
        /// @param in_storageLocation - The Storage Location
        /// @param in_filePath - The file path
        ///
        /// @return Whether or not it exists.
        //------------------------------------------------------------------------------
        virtual bool DoesFileExist(StorageLocation in_storageLocation, const std::string& in_filePath) const = 0;
        //------------------------------------------------------------------------------
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_filePath - The file path.
        ///
        /// @return Whether or not the file exists in the Cached DLC directory.
        //------------------------------------------------------------------------------
        virtual bool DoesFileExistInCachedDLC(const std::string& in_filePath) const = 0;
        //------------------------------------------------------------------------------
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_filePath - The file path.
        ///
        /// @return Whether or not the file exists in the package DLC directory.
        //------------------------------------------------------------------------------
        virtual bool DoesFileExistInPackageDLC(const std::string& in_filePath) const = 0;
        //------------------------------------------------------------------------------
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        /// 
        /// @param in_storageLocation - The Storage Location
        /// @param in_directoryPath - The directory path
        ///
        /// @return Whether or not the given directory exists.
        //------------------------------------------------------------------------------
        virtual bool DoesDirectoryExist(StorageLocation in_storageLocation, const std::string& in_directoryPath) const = 0;
        //------------------------------------------------------------------------------
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_directoryPath - The directory path.
        ///
        /// @return Whether or not the directory exists in the Cached DLC directory.
        //------------------------------------------------------------------------------
        virtual bool DoesDirectoryExistInCachedDLC(const std::string& in_directoryPath) const = 0;
        //------------------------------------------------------------------------------
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_directoryPath - The directory path.
        ///
        /// @return Whether or not the directory exists in the package DLC directory.
        //------------------------------------------------------------------------------
        virtual bool DoesDirectoryExistInPackageDLC(const std::string& in_directoryPath) const = 0;
        //------------------------------------------------------------------------------
        /// Returns the absolute path to the given storage location. The value this
        /// returns is platform specific and use of this should be kept to a minimum in
        /// cross platform projects.
        ///
        /// This is thread-safe.
        ///
        /// @author S Downie
        ///
        /// @param in_storageLocation - The source storage location.
        ///
        /// @return The directory. returns an empty string if the location is not
        /// available.
        //------------------------------------------------------------------------------
        virtual std::string GetAbsolutePathToStorageLocation(StorageLocation in_storageLocation) const = 0;
        //------------------------------------------------------------------------------
        /// Sets the directory used by the DLC system to fall back on if it is not in
        /// the DLC cache directory.
        ///
        /// This is thread safe.
        ///
        /// @author S Downie
        ///
        /// @param in_directoryPath - The package DLC directory.
        //------------------------------------------------------------------------------
        void SetPackageDLCPath(const std::string& in_directoryPath);
        //------------------------------------------------------------------------------
        /// This is thread-safe.
        ///
        /// @author S Downie
        ///
        /// @return The package DLC directory.
        //------------------------------------------------------------------------------
        const std::string& GetPackageDLCPath() const;
        //------------------------------------------------------------------------------
        /// Calculate the SHA1 checksum of the file at the given directory
        ///
        /// This is thread-safe.
        ///
        /// @author N Tanda
        ///
        /// @param in_storageLocation - Storage location
        /// @param in_filePath - File path
        ///
        /// @return SHA1 checksum
        //------------------------------------------------------------------------------
        std::string GetFileChecksumSHA1(StorageLocation in_storageLocation, const std::string& in_filePath) const;
        //------------------------------------------------------------------------------
        /// Calculate the MD5 checksum of the file at the given directory
        ///
        /// This is thread-safe.
        ///
        /// @author S Downie
        ///
        /// @param in_storageLocation - Storage location
        /// @param in_filePath - File path
        ///
        /// @return MD5 checksum
        //------------------------------------------------------------------------------
        std::string GetFileChecksumMD5(StorageLocation in_storageLocation, const std::string& in_filePath) const;
        //------------------------------------------------------------------------------
        /// Calculate the MD5 checksum of the given directory
        ///
        /// This is thread-safe.
        ///
        /// @author S Downie
        ///
        /// @param in_storageLocation - Storage location
        /// @param in_directoryPath - File path
        ///
        /// @return MD5 checksum
        //------------------------------------------------------------------------------
        std::string GetDirectoryChecksumMD5(StorageLocation in_storageLocation, const std::string& in_directoryPath) const;
        //------------------------------------------------------------------------------
        /// Gets the CRC32 Checksum for the given file.
        ///
        /// This is thread-safe.
        ///
        /// @author S Downie
        ///
        /// @param in_storageLocation - The storage location.
        /// @param in_filePath - The filepath.
        ///
        /// @return the checksum.
        //------------------------------------------------------------------------------
        u32 GetFileChecksumCRC32(StorageLocation in_storageLocation, const std::string& in_filePath) const;
        //------------------------------------------------------------------------------
        /// Gets the CRC32 Checksum for the given directory.
        ///
        /// This is thread-safe.
        ///
        /// @param in_storageLocation - The storage location.
        /// @param in_directoryPath - the filepath.
        ///
        /// @return the checksum.
        //------------------------------------------------------------------------------
        u32 GetDirectoryChecksumCRC32(StorageLocation in_storageLocation, const std::string& in_directoryPath) const;
        //------------------------------------------------------------------------------
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The storage location.
        /// @param in_filePath - the file path.
        ///
        /// @return The size in bytes of the given file.
        //------------------------------------------------------------------------------
        u32 GetFileSize(StorageLocation in_storageLocation, const std::string& in_filePath) const;
        //------------------------------------------------------------------------------
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The storage location.
        /// @param in_directoryPath - the Directory path.
        ///
        /// @return The size in bytes of the contents of the given directory.
        //------------------------------------------------------------------------------
        u32 GetDirectorySize(StorageLocation in_storageLocation, const std::string& in_directoryPath) const;
        //------------------------------------------------------------------------------
        /// Returns whether or not the given storage location can be written to.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_storageLocation - The storage location.
        ///
        /// @return Whether or not it can be written to.
        //------------------------------------------------------------------------------
        bool IsStorageLocationWritable(StorageLocation in_storageLocation) const;
        //------------------------------------------------------------------------------
        /// Destructor
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        virtual ~FileSystem() {}

    protected:
        friend class Application;
        //------------------------------------------------------------------------------
        /// Create the platform dependent backend
        ///
        /// @author S Downie
        ///
        /// @return New backend instance
        //------------------------------------------------------------------------------
        static FileSystemUPtr Create();
        //------------------------------------------------------------------------------
        /// Private constructor to force use of factory method
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        FileSystem();
        //------------------------------------------------------------------------------
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        ///
        /// @param in_fileMode - The file mode.
        ///
        /// @return whether or not the given file mode is a write mode
        //------------------------------------------------------------------------------
        bool IsWriteMode(FileMode in_fileMode) const;

    private:
        std::string m_packageDLCPath;
        mutable std::mutex m_packageDLCPathMutex;
    };
}

#endif
