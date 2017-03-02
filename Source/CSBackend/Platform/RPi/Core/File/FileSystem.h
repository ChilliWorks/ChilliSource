//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_RPI_CORE_FILE_FILESYSTEM_H_
#define _CSBACKEND_RPI_CORE_FILE_FILESYSTEM_H_

#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <ChilliSource/Core/File/FileStream/BinaryInputStream.h>
#include <ChilliSource/Core/File/FileStream/BinaryOutputStream.h>
#include <ChilliSource/Core/File/FileStream/TextInputStream.h>
#include <ChilliSource/Core/File/FileStream/TextOutputStream.h>
#include <ChilliSource/Core/File/FileSystem.h>

#include <string>

namespace CSBackend
{
	namespace RPi
	{
		/// The backend for the Raspberry Pi filesystem.
		/// The filesystem is responsible for all file manipulations
		/// including creating, copying, deleting, etc.
		///
		class FileSystem final : public ChilliSource::FileSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(FileSystem);

			/// Queries whether or not this system implements the
			/// interface with the given Id.
			///
			/// @param interfaceId
			///		The id of the interface that we need to check if this system implements
			///
			/// @retrun TRUE if the system 'IsA' interface
			///
			bool IsA(ChilliSource::InterfaceIDType interfaceId) const override;

            /// Creates a new "read text stream to the given file in the given storage location.
            ///
            /// @param storageLocation
			///		Location to stream from
            /// @param filePath
			/// 	File path to stream from
            ///
            /// @return The new file stream. If the stream cannot be created or is invalid null will be returned.
            ///
            ChilliSource::ITextInputStreamUPtr CreateTextInputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const override;

			/// Creates a new "read" binary stream to the given file in the given storage location.
            ///
            /// @param storageLocation
			///		Location to stream from
            /// @param filePath
			/// 	File path to stream from
            ///
            /// @return The new file stream. If the stream cannot be created or is invalid null will be returned.
            ///
			ChilliSource::IBinaryInputStreamUPtr CreateBinaryInputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const override;

			/// Creates a new "write" text stream to the given file in the given storage location.
			///
			/// @param storageLocation
			///		Location to stream to
            /// @param filePath
			/// 	File path to stream to
			/// @param fileMode
			///		Write mode (i.e. whether to replace or append, etc)
			///
			/// @return The new file stream. If the stream cannot be created or is invalid null will be returned.
			///
			ChilliSource::TextOutputStreamUPtr CreateTextOutputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath, ChilliSource::FileWriteMode fileMode) const override;

			/// Creates a new "write" binary stream to the given file in the given storage location.
			///
			/// @param storageLocation
			///		Location to stream to
            /// @param filePath
			/// 	File path to stream to
			/// @param fileMode
			///		Write mode (i.e. whether to replace or append, etc)
			///
			/// @return The new file stream. If the stream cannot be created or is invalid null will be returned.
			///
			ChilliSource::BinaryOutputStreamUPtr CreateBinaryOutputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath, ChilliSource::FileWriteMode fileMode) const override;

			/// Creates the given directory. The full directory hierarchy will be created.
			///
			/// @param storageLocation
			///		Location in which to create the directory
            /// @param directoryPath
			///		The path to new directory
			///
            /// @return Whether or not this was successful. Failure to create the directory because it already exists is considered a success.
			///
			bool CreateDirectoryPath(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath) const override;

			/// Copies a file from one location to another.
			///
			/// @param sourceLocation
			///		Location to copy from
			/// @param sourceFilePath
			///		The path to copy from
			/// @param destLocation
			///		Location to copy to
			/// @param destFilePath
			///		The path to copy to
			///
			/// @return Whether or not the file was successfully copied.
			///
			bool CopyFile(ChilliSource::StorageLocation sourceLocation, const std::string& sourceFilePath, ChilliSource::StorageLocation destLocation, const std::string& destFilePath) const override;

			/// Copies a directory and its contents from one location to another.
			///
			/// @param sourceLocation
			///		Location to copy from
			/// @param sourceDirPath
			///		The path to copy from
			/// @param destLocation
			///		Location to copy to
			/// @param destDirPath
			///		The path to copy to
			///
			/// @return Whether or not the directory was successfully copied.
			///
			bool CopyDirectory(ChilliSource::StorageLocation sourceLocation, const std::string& sourceDirPath, ChilliSource::StorageLocation destLocation, const std::string& destDirPath) const override;

			/// Deletes the specified file.
			///
			/// @param storageLocation
			///		Location of file to delete
			/// @param filePath
			/// 	Path to file to delete
			///
			/// @return Whether or not the file was successfully deleted.
			///
			bool DeleteFile(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const override;

			/// Deletes the specified directory and all its contents.
			///
			/// @param storageLocation
			///		Location of dir to delete
			/// @param directoryPath
			/// 	Path to directory to delete
			///
			/// @return Whether or not the directory was successfully deleted.
			///
			bool DeleteDirectory(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath) const override;

			/// Builds a list of all the files in the given directory. Paths are relative to the given directory
			///
			/// @param storageLocation
			///		Location of the directory to profile
			/// @param directoryPath
			///		Path to the directory to profile
			/// @param recursive
			///		If TRUE all subfolders will be profiled, if FALSE only the given directory.
			///
			/// @return List of all filepaths in the given directory
			///
			std::vector<std::string> GetFilePaths(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath, bool recursive) const override;

			/// Builds a list of all the sub-dirs in the given directory. Paths are relative to the given directory
			///
			/// @param storageLocation
			///		Location of the directory to profile
			/// @param directoryPath
			///		Path to the directory to profile
			/// @param recursive
			///		If TRUE all subfolders will be profiled, if FALSE only the given directory.
			///
			/// @return List of all directory paths in the given directory
			///
			std::vector<std::string> GetDirectoryPaths(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath, bool recursive) const override;

			/// @param storageLocation
			///		Location of file to check
			/// @param filePath
			/// 	Path to file to check
			///
			/// @return Whether or not the given file exists.
			///
			bool DoesFileExist(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const override;

			/// @param filePath
			/// 	Path to file to check (relative to cached DLC directory)
			///
			/// @return Whether or not the given file exists in the cached DLC directory.
			///
			bool DoesFileExistInCachedDLC(const std::string& filePath) const override;

			/// @param filePath
			/// 	Path to file to check (relative to package DLC directory)
			///
			/// @return Whether or not the given file exists in the package DLC directory.
			///
			bool DoesFileExistInPackageDLC(const std::string& filePath) const override;

			/// @param storageLocation
			///		Location of directory to check
			/// @param directoryPath
			/// 	Path to dir to check
			///
			/// @return Whether or not the given directory exists.
			///
			bool DoesDirectoryExist(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath) const override;

			/// @param directoryPath
			/// 	Path to dir to check (relative to cached DLC directory)
			///
			/// @return Whether or not the given directory exists in the cached DLC directory
			///
			bool DoesDirectoryExistInCachedDLC(const std::string& directoryPath) const override;

			/// @param directoryPath
			/// 	Path to dir to check (relative to package DLC directory)
			///
			/// @return Whether or not the given directory exists in the package DLC directory
			///
			bool DoesDirectoryExistInPackageDLC(const std::string& directoryPath) const override;

			/// Returns the absolute path to the given storage location. The
			/// value this returns is platform specific and use of this
			/// should be kept to a minimum in cross platform projects.
			///
			/// @param storageLocation
			///		Location to get the path to
			///
			/// @return Full path to the given storage location
			///
			std::string GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation storageLocation) const override;

		private:
			friend ChilliSource::FileSystemUPtr ChilliSource::FileSystem::Create();

			///
			FileSystem();

			/// Returns whether or not a file or directory exists specifically
			/// in the DLC cache.
			///
			/// @param path
			///		Path to file or directory to check
			/// @param isDirectory
			///		If TRUE path is considered to lead to a dir, otherwise a file.
			///
			/// @return whether or not it exists.
			///
			bool DoesItemExistInDLCCache(const std::string& path, bool isDirectory) const;

			/// Builds a list of all possible full paths that the given relative path might refer to.
			/// For example a path in location dlc might refer to either cached dlc or package dlc.
			///
			/// @param storageLocation
			///		Location to profile
			/// @param directoryPath
			///		Relative directory path for which to build possible absolute paths.
			///
			/// @return All the paths for the given location
			///
			std::vector<std::string> GetPossibleAbsoluteDirectoryPaths(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath) const;

			std::string m_packagePath;
			std::string m_documentsPath;
		};
	}
}

#endif

#endif
