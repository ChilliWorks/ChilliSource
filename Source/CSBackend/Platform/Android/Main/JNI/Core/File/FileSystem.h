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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_FILE_FILESYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_FILE_FILESYSTEM_H_

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/File/ZippedFileSystem.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace CSBackend
{
	namespace Android 
	{
		//------------------------------------------------------------------------------
		/// The Android implementation of the file system.
		///
		/// The literal locations of the Package and ChilliSource storage locations
		/// differ depending on the Android Flavour. In both cases Package and
		/// ChilliSource are "virtual" storage locations, which are contained within
		/// zip files. Google Play builds use the main Apk Expansion file, and Amazon
		/// builds use the Apk.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class FileSystem final : public CSCore::FileSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(FileSystem);
			//------------------------------------------------------------------------------
			/// Queries whether or not this system implements the interface with the given
			/// Id.
			///
			/// @author Ian Copland
			///
			/// @param in_interfaceId - The interface Id.
			///
			/// @return Whether or not the interface is implemented.
			//------------------------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
			//------------------------------------------------------------------------------
			/// Creates a new file stream to the given file in the givenstorage location.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The storage location.
			/// @param in_filePath - The file path.
			/// @param in_fileMode - The file mode.
			///
			/// @return The new file stream. If the stream cannot be created or is invalid,
			/// null be be returned.
			//------------------------------------------------------------------------------
			CSCore::FileStreamUPtr CreateFileStream(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, CSCore::FileMode in_fileMode) const override;
			//------------------------------------------------------------------------------
			/// Creates the given directory. The full directory hierarchy will be created.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The Storage Location
			/// @param in_directoryPath - The directory path.
			///
			/// @return Whether or not this was successful. Failure to create the directory
			/// because it already exists is considered a success.
			//------------------------------------------------------------------------------
			bool CreateDirectoryPath(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
			//------------------------------------------------------------------------------
			/// Copies a file from one location to another.
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
			bool CopyFile(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
						  CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const override;
			//------------------------------------------------------------------------------
			/// Copies a directory from one location to another. If the destination
			/// directory does not exist, it will be created.
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
			bool CopyDirectory(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
							   CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const override;
			//------------------------------------------------------------------------------
			/// Deletes the specified file.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The storage location.
			/// @param in_filePath - The file path.
			///
			/// @return Whether or not the file was successfully deleted.
			//------------------------------------------------------------------------------
			bool DeleteFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const override;
			//------------------------------------------------------------------------------
			/// Deletes a directory and all its contents.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The storage location.
			/// @param in_directoryPath - The directory.
			///
			/// @return Whether or not the directory was successfully deleted.
			//------------------------------------------------------------------------------
			bool DeleteDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
			//------------------------------------------------------------------------------
			/// Creates a dynamic array containing the filenames of each file in the given
			/// directory. File paths will be relative to the input directory.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The Storage Location
			/// @param in_directoryPath - The directory
			/// @param in_recursive - Flag to determine whether or not to recurse into sub
			/// directories
			///
			/// @return dynamic array containing the filenames.
			//------------------------------------------------------------------------------
			std::vector<std::string> GetFilePaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const override;
			//------------------------------------------------------------------------------
			/// Creates a dynamic array containing the names of each directory in the given
			/// directory. Directory paths will be relative to the input directory.
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
			std::vector<std::string> GetDirectoryPaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const override;
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The Storage Location
			/// @param in_filePath - The file path
			///
			/// @return Whether or not the described file exists.
			//------------------------------------------------------------------------------
			bool DoesFileExist(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const override;
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param in_filePath - The file path.
			///
			/// @return Whether or not the given file path exists in the Cache DLC directory.
			//------------------------------------------------------------------------------
			bool DoesFileExistInCachedDLC(const std::string& in_filePath) const override;
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param in_filePath - The file path.
			///
			/// @return Whether or not the given dile path exists in the Package DLC
			/// directory.
			//------------------------------------------------------------------------------
			bool DoesFileExistInPackageDLC(const std::string& in_filePath) const override;
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The Storage Location
			/// @param in_directoryPath - The directory path
			///
			/// @return Whether or not the described directory exists.
			//------------------------------------------------------------------------------
			bool DoesDirectoryExist(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param in_directoryPath - The directory path.
			///
			/// @return Whether or not the directory exists in the Cache DLC directory.
			//------------------------------------------------------------------------------
			bool DoesDirectoryExistInCachedDLC(const std::string& in_directoryPath) const override;
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param in_directoryPath - The directory path.
			///
			/// @return Whether or not the directory exists in the Package DLC directory.
			//------------------------------------------------------------------------------
			bool DoesDirectoryExistInPackageDLC(const std::string& in_directoryPath) const override;
			//------------------------------------------------------------------------------
			/// Returns the absolute path to the given storage location. The value this
			/// returns is platform specific and use of this should be kept to a minimum in
			/// cross platform projects.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The source storage location.
			///
			/// @return The directory. returns an empty string if the location is not
			/// available.
			//------------------------------------------------------------------------------
			std::string GetAbsolutePathToStorageLocation(CSCore::StorageLocation in_storageLocation) const override;
            //------------------------------------------------------------------------------
			/// Calculates the full path to the file by appending the path to the storage
			/// location path. If the path is in the DLC storage location the Cached DLC
			/// will first be checked to see if the file exists. If it does this path is
			/// returned, otherwise the Package DLC path is given. The file at the returned
			/// path doesn't necessarily exist.
			///
			/// The result of this is platform specific so care should be taken when using
			/// it in cross platform projects.
            ///
            /// @author Ian Copland
			///
            /// @param in_storageLocation - The storage location for the file.
            /// @param in_path - The file path relative to the storage location.
            ///
			/// @return The full path to the file.
			//------------------------------------------------------------------------------
			std::string GetAbsolutePathToFile(CSCore::StorageLocation in_storageLocation, const std::string& in_path) const override;
            //------------------------------------------------------------------------------
			/// Calculates the full path to the directory by appending the path to the
			/// storage location path. If the path is in the DLC storage location the Cached
			/// DLC will first be checked to see if the directory exists. If it does this
			/// path is returned, otherwise the Package DLC path is given. The directory at
			/// the returned path doesn't necessarily exist.
			///
			/// The result of this is platform specific so care should be taken when using
			/// it in cross platform projects.
            ///
            /// @author Ian Copland
			///
            /// @param in_storageLocation - The storage location for the directory.
            /// @param in_path - The directory path relative to the storage location.
            ///
			/// @return The full path to the directory.
			//------------------------------------------------------------------------------
			std::string GetAbsolutePathToDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_path) const override;

		private:
			friend CSCore::FileSystemUPtr CSCore::FileSystem::Create();
            //------------------------------------------------------------------------------
            /// Private constructor to force use of factory method.
            ///
            /// @author S Downie
            //------------------------------------------------------------------------------
            FileSystem();

			std::string m_storagePath;
			ZippedFileSystemUPtr m_zippedFileSystem;
		};
	}
}

#endif

#endif
