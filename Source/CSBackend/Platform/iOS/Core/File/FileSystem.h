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

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CSBACKEND_PLATFORM_IOS_CORE_FILE_FILESYSTEM_H_
#define _CSBACKEND_PLATFORM_IOS_CORE_FILE_FILESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/File/FileSystem.h>

#include <string>

namespace CSBackend
{
	namespace iOS 
	{
		//-------------------------------------------------------------------
		/// The iOS specific implementation for the File System.
        ///
        /// @author Ian Copland
		//-------------------------------------------------------------------
		class FileSystem final : public CSCore::FileSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(FileSystem);
            //----------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
            ///
            /// @author Ian Copland
			///
			/// @param The interface Id.
            ///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------------------
            /// Creates a new file stream to the given file in the given
            /// storage location.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location.
            /// @param The file path.
            /// @param The file mode.
            ///
            /// @return The new file stream.
            //--------------------------------------------------------------
            CSCore::FileStreamUPtr CreateFileStream(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, CSCore::FileMode in_fileMode) const override;
            //--------------------------------------------------------------
            /// Creates the given directory. The full directory hierarchy will
            /// be created.
            ///
            /// @author Ian Copland
            ///
            /// @param The Storage Location
            /// @param The directory path.
            ///
            /// @return Returns whether or not this was successful. Failure to
            /// create the directory because it already exists is considered
            /// a success.
            //--------------------------------------------------------------
            bool CreateDirectoryPath(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
            //--------------------------------------------------------------
            /// Copies a file from one location to another.
            ///
            /// @author Ian Copland
            ///
            /// @param The source storage location.
            /// @param The source directory.
            /// @param The destination storage location.
            /// @param The destination directory.
            ///
            /// @return Whether or not the file was successfully copied.
            //--------------------------------------------------------------
            bool CopyFile(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
                          CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const override;
            //--------------------------------------------------------------
            /// Copies a directory from one location to another. If the
            /// destination directory does not exist, it will be created.
            ///
            /// @author Ian Copland
            ///
            /// @param The source storage location.
            /// @param The source directory.
            /// @param The destination storage location.
            /// @param The destination directory.
            ///
            /// @return Whether or not the files were successfully copied.
            //--------------------------------------------------------------
            bool CopyDirectory(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
                               CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const override;
            //--------------------------------------------------------------
            /// Deletes the specified file.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location.
            /// @param The filepath.
            ///
            /// @return Whether or not the file was successfully deleted.
            //--------------------------------------------------------------
            bool DeleteFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filepath) const override;
            //--------------------------------------------------------------
            /// Deletes a directory and all its contents.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location.
            /// @param The directory.
            ///
            /// @return Whether or not the directory was successfully deleted.
            //--------------------------------------------------------------
            bool DeleteDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
            //--------------------------------------------------------------
            /// Creates a dynamic array containing the filenames of each file
            /// in the given directory. File paths will be relative to the
            /// input directory.
            ///
            /// @author Ian Copland
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub
            /// directories
            ///
            /// @return dynamic array containing the filenames.
            //--------------------------------------------------------------
            std::vector<std::string> GetFilePaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const override;
            //--------------------------------------------------------------
            /// Creates a dynamic array containing the names of each directory
            /// in the given directory. Directory paths will be relative to
            /// the input directory.
            ///
            /// @author Ian Copland
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub
            /// directories
            ///
            /// @return Output dynamic array containing the dir names.
            //--------------------------------------------------------------
            std::vector<std::string> GetDirectoryPaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const override;
            //--------------------------------------------------------------
            /// returns whether or not the given file exists.
            ///
            /// @author Ian Copland
            ///
            /// @param The Storage Location
            /// @param The file path
            ///
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            bool DoesFileExist(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const override;
			//--------------------------------------------------------------
			/// Returns whether or not the file exists in the Cached DLC
            /// directory.
            ///
            /// @author Ian Copland
			///
			/// @param The file path.
            ///
			/// @return Whether or not it is in the cached DLC.
			//--------------------------------------------------------------
			bool DoesFileExistInCachedDLC(const std::string& in_filePath) const override;
			//--------------------------------------------------------------
			/// Returns whether or not the file exists in the package DLC
            /// directory.
            ///
            /// @author Ian Copland
			///
			/// @param The file path.
            ///
			/// @return Whether or not it is in the package DLC.
			//--------------------------------------------------------------
			bool DoesFileExistInPackageDLC(const std::string& in_filePath) const override;
            //--------------------------------------------------------------
            /// Returns whether or not the given directory exists.
            ///
            /// @author Ian Copland
            ///
            /// @param The Storage Location
            /// @param The directory path
            ///
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            bool DoesDirectoryExist(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
			//--------------------------------------------------------------
			/// Returns the absolute path to the given storage location. The
            /// value this returns is platform specific and use of this
            /// should be kept to a minimum in cross platform projects.
            ///
            /// @author S Downie
            ///
			/// @param The source storage location.
            ///
			/// @return The directory. returns an empty string if the location
			/// is not available.
			//--------------------------------------------------------------
			std::string GetAbsolutePathToStorageLocation(CSCore::StorageLocation in_storageLocation) const override;
            //--------------------------------------------------------------
			/// Returns the absolute path to the file in the given storage
            /// location. The file must exist otherwise an empty string
            /// will be returned. The result of this is platform specific
            /// so care should be taken when using this in cross platform
            /// projects.
            ///
            /// @author S Downie
			///
            /// @param The storage location for the file.
            /// @param The file path relative to the storage location.
            ///
			/// @return The full path to the file.
			//--------------------------------------------------------------
			std::string GetAbsolutePathToFile(CSCore::StorageLocation in_storageLocation, const std::string& in_path) const override;
            //--------------------------------------------------------------
			/// Returns the absolute path to the directory in the given storage
            /// location. The directory must exist otherwise an empty string
            /// will be returned. The result of this is platform specific
            /// so care should be taken when using this in cross platform
            /// projects.
            ///
            /// @author Ian Copland
			///
            /// @param The storage location for the directory.
            /// @param The directory path relative to the storage location.
            ///
			/// @return The full path to the directory.
			//--------------------------------------------------------------
			std::string GetAbsolutePathToDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_path) const override;

		private:
            friend CSCore::FileSystemUPtr CSCore::FileSystem::Create();
            //--------------------------------------------------------------
            /// A container for information on a single item in the package
            /// manifest.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------
            struct PackageManifestItem
            {
                u32 m_pathHash;
                bool m_isFile;
            };
            //--------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            FileSystem();
            //--------------------------------------------------------------
            /// Creates the manifest of all files and directories in the
            /// package.
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            void CreatePackageManifest();
            //--------------------------------------------------------------
	        /// Tries to get an item from the package manifest for the given
            /// path.
	        ///
            /// @author Ian Copland
            ///
            /// @param The path to look up.
	        /// @param [Out] The manifest item if successful.
	        ///
            /// @return Whether or not the look up was successful.
            //--------------------------------------------------------------
            bool TryGetPackageManifestItem(const std::string& in_path, PackageManifestItem& out_manifestItem) const;
            //--------------------------------------------------------------
			/// Returns whether or not this given file path exist in the
            /// package. This looks up the package manifest rather than
            /// reading from disk.
            ///
            /// @author S Downie
            ///
            /// @param the filepath.
            ///
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesFileExistInPackage(const std::string& in_filePath) const;
            //--------------------------------------------------------------
			/// Returns whether or not this given directory path exist in the
            /// package. This looks up the package manifest rather than
            /// reading from disk.
            ///
            /// @author Ian Copland
            ///
            /// @param the directory path.
            ///
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesDirectoryExistInPackage(const std::string& in_directoryPath) const;
            //--------------------------------------------------------------
			/// Returns whether or not a file or directory exists specifically
            /// in the DLC cache.
            ///
            /// @author Ian Copland
            ///
            /// @param The filepath.
            ///
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesItemExistInDLCCache(const std::string& in_path, bool in_isDirectory) const;
            //------------------------------------------------------------
            /// Builds a list of the paths that the given path might refer
			/// to in the given storage location. For example, a path in
			/// DLC might refer to the DLC cache or the Package DLC.
            ///
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param File name to append
            ///
            /// @return All the paths for the given location
            //------------------------------------------------------------
            std::vector<std::string> GetPossibleAbsoluteDirectoryPaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const;
            
            
			std::string m_bundlePath;
			std::string m_documentsPath;
            std::string m_libraryPath;
            
            std::vector<PackageManifestItem> m_packageManifestItems;
		};
	}
}

#endif

#endif