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
		/// This is thread-safe though care still needs to be taken when dealing with
		/// file streams as they are not. A file stream should be used and destroyed
		/// on the same thread it was created.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class FileSystem final : public ChilliSource::FileSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(FileSystem);
			//------------------------------------------------------------------------------
			/// A struct containing information about the location of a single file within
			/// the zip. This can be used to manually access the zip data.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			struct ZippedFileInfo final
			{
				u32 m_offset = 0;
				u32 m_size = 0;
				u32 m_uncompressedSize = 0;
				bool m_isCompressed = false;
			};
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
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
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
			ChilliSource::FileStreamUPtr CreateFileStream(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, ChilliSource::FileMode in_fileMode) const override;
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
			bool CreateDirectoryPath(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
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
			bool CopyFile(ChilliSource::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
						  ChilliSource::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const override;
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
			bool CopyDirectory(ChilliSource::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
							   ChilliSource::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const override;
			//------------------------------------------------------------------------------
			/// Deletes the specified file.
            ///
            /// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The storage location.
			/// @param in_filePath - The file path.
			///
			/// @return Whether or not the file was successfully deleted.
			//------------------------------------------------------------------------------
			bool DeleteFile(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath) const override;
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
			bool DeleteDirectory(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
			//------------------------------------------------------------------------------
			/// Creates a dynamic array containing the filenames of each file in the given
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
			/// @return dynamic array containing the filenames.
			//------------------------------------------------------------------------------
			std::vector<std::string> GetFilePaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const override;
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
			std::vector<std::string> GetDirectoryPaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const override;
			//------------------------------------------------------------------------------
            /// This is thread-safe.
            ///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The Storage Location
			/// @param in_filePath - The file path
			///
			/// @return Whether or not the described file exists.
			//------------------------------------------------------------------------------
			bool DoesFileExist(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath) const override;
			//------------------------------------------------------------------------------
            /// This is thread-safe.
            ///
			/// @author Ian Copland
			///
			/// @param in_filePath - The file path.
			///
			/// @return Whether or not the given file path exists in the Cache DLC directory.
			//------------------------------------------------------------------------------
			bool DoesFileExistInCachedDLC(const std::string& in_filePath) const override;
			//------------------------------------------------------------------------------
            /// This is thread-safe.
            ///
			/// @author Ian Copland
			///
			/// @param in_filePath - The file path.
			///
			/// @return Whether or not the given file path exists in the Package DLC
			/// directory.
			//------------------------------------------------------------------------------
			bool DoesFileExistInPackageDLC(const std::string& in_filePath) const override;
			//------------------------------------------------------------------------------
            /// This is thread-safe.
            ///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The Storage Location
			/// @param in_directoryPath - The directory path
			///
			/// @return Whether or not the described directory exists.
			//------------------------------------------------------------------------------
			bool DoesDirectoryExist(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
			//------------------------------------------------------------------------------
            /// This is thread-safe.
            ///
			/// @author Ian Copland
			///
			/// @param in_directoryPath - The directory path.
			///
			/// @return Whether or not the directory exists in the Cache DLC directory.
			//------------------------------------------------------------------------------
			bool DoesDirectoryExistInCachedDLC(const std::string& in_directoryPath) const override;
			//------------------------------------------------------------------------------
            /// This is thread-safe.
            ///
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
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The source storage location.
			///
			/// @return The directory. returns an empty string if the location is not
			/// available.
			//------------------------------------------------------------------------------
			std::string GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation in_storageLocation) const override;
			//------------------------------------------------------------------------------
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @return The file path to the zip. The location of this is dependent on the
			/// current Android Flavour: Google Play builds will return the OBB file, while
			/// Amazon builds will simply return the APK. There aren't many cases where this
			/// zip file should be accessed directly, instead the cross platform file system
			/// methods should be used.
			//------------------------------------------------------------------------------
			const std::string& GetZipFilePath() const;
			//------------------------------------------------------------------------------
			/// Calculates information on a file within the zipped file system. This applies
			/// to the Package, ChilliSource and DLC storage locations, passing other
			/// storage locations will cause this to assert. If DLC is passed, only files
			/// in the Package DLC directory will be checked.
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The storage location. Must be package,
			/// ChilliSource or DLC.
			/// @param in_filePath - The path to the file.
			/// @param out_zippedFileInfo - [Out] Information on the zipped file. This is
			/// only set if the method is successful.
			///
			/// @return Whether or not this was successful.
			//------------------------------------------------------------------------------
			bool TryGetZippedFileInfo(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, ZippedFileInfo& out_zippedFileInfo) const;
		private:
			friend ChilliSource::FileSystemUPtr ChilliSource::FileSystem::Create();
            //------------------------------------------------------------------------------
            /// Private constructor to force use of factory method.
            ///
            /// @author S Downie
            //------------------------------------------------------------------------------
            FileSystem();

			std::string m_storagePath;
			std::string m_zipFilePath;
			ZippedFileSystemUPtr m_zippedFileSystem;
		};
	}
}

#endif

#endif
