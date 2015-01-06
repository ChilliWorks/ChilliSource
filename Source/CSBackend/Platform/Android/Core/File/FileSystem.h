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

#ifndef _CSBACKEND_PLATFORM_ANDROID_CORE_FILE_FILESYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_CORE_FILE_FILESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Core/File/FileSystem.h>

#include <minizip/unzip.h>

#include <mutex>
#include <string>

namespace CSBackend
{
	namespace Android 
	{
		//------------------------------------------------------------------
		/// The Android implementation of the file system.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------
		class FileSystem final : public CSCore::FileSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(FileSystem);

			static const std::string k_packageAPKDir;
			static const std::string k_csAPKDir;

			//-----------------------------------------------------------
			/// A container for the information on a file within the APK.
			///
			/// @author S Downie
			//-----------------------------------------------------------
	        struct APKManifestItem
	        {
	        	u32 m_pathHash;
	        	std::string m_path;
	        	bool m_isFile;
	        	unz_file_pos m_apkPosition;
	        };
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
			/// create the directory becuase it already exists is considered
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
			/// @param Flag to determine whether or not to recurse into sub directories
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
			/// @author Ian Copland
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
            /// @author S Downie
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
			/// A container for the storage location and path of a single
			/// file or directory.
			///
			/// @author S Downie
			//--------------------------------------------------------------
			struct PathInfo
			{
				std::string m_path;
				CSCore::StorageLocation m_storageLocation;
			};
            //--------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            FileSystem();
	        //--------------------------------------------------------------
	        /// Creates the APK Manifest. This contains information on each
            /// path in the APK, including it's path, it's hash and it's
            /// position within the APK zip.
	        ///
	        /// @author S Downie
	        //--------------------------------------------------------------
	        void CreateAPKManifest();
            //--------------------------------------------------------------
	        /// Tries to get the manifest item for the given path.
	        ///
            /// @author Ian Copland
            ///
	        /// @param The package or CS storage location
            /// @param The path to loop up.
	        /// @param [Out] The manifest item if successful.
	        ///
            /// @return Whether or not the look up was successful.
            //--------------------------------------------------------------
            bool TryGetManifestItem(CSCore::StorageLocation in_location, const std::string& in_path, APKManifestItem& out_manifestItem) const;
			//--------------------------------------------------------------
			/// Creates a file stream to a file within the APK (zip) file.
			/// This provides a uniform api for handling files within
			/// the apk and normal filestreams.
            ///
            /// @author Ian Copland
			///
            /// @param The package or CS storage location
			/// @param The file path of in the APK.
			/// @param the file mode.
            ///
			/// @return the APK filestream.
			//--------------------------------------------------------------
			CSCore::FileStreamUPtr CreateFileStreamInAPK(CSCore::StorageLocation in_location, const std::string& in_filePath, CSCore::FileMode in_fileMode) const;
			//--------------------------------------------------------------
			/// Copies files from one location to another.
			///
			/// @author S Downie
			///
			/// @param The source storage location.
			/// @param The source directories.
			/// @param The destination storage location.
			/// @param The destination directories.
			///
			/// @return Whether or not the files were successfully copied.
			//--------------------------------------------------------------
			bool CopyFiles(CSCore::StorageLocation in_sourceStorageLocation, const std::vector<std::string>& in_sourceFilePaths,
						  CSCore::StorageLocation in_destinationStorageLocation, const std::vector<std::string>& in_destinationFilePaths) const;
			//--------------------------------------------------------------
			/// Copies files from one location to another for files that
			/// are guaranteed to not be in the APK
			///
			/// @author S Downie
			///
			/// @param The source storage location.
			/// @param The source directories.
			/// @param The destination storage location.
			/// @param The destination directories.
			///
			/// @return Whether or not the files were successfully copied.
			//--------------------------------------------------------------
			bool CopyNonAPKFiles(CSCore::StorageLocation in_sourceStorageLocation, const std::vector<std::string>& in_sourceFilePaths,
					CSCore::StorageLocation in_destinationStorageLocation, const std::vector<std::string>& in_destinationFilePaths) const;
			//--------------------------------------------------------------
			/// Copies files from the APK to one of the writable storage
			/// locations.
			///
			/// @author Ian Copland
			///
			/// @param The package or CS storage location of the source
			/// @param the source file paths in the APK.
			/// @param the destination storage location
			/// @param the destination files.
			///
			/// @return whether or not this has succeeded.
			//--------------------------------------------------------------
			bool CopyFilesFromAPK(CSCore::StorageLocation in_srcLocation, const std::vector<std::string>& in_sourcefilePaths, CSCore::StorageLocation in_destinationStorageLocation, const std::vector<std::string>& in_destinationFilepaths) const;
			//--------------------------------------------------------------
			/// returns all files and directories in a directory of the APK.
			/// Note: Unlike other similar methods, empty directories will
			/// not be listed.
			///
			/// @author Ian Copland
			///
			/// @param The package or CS storage location
			/// @param the directory.
			/// @param whether or not to recurse
			/// @param out parameter containing all the found items.
			//--------------------------------------------------------------
			std::vector<std::string> GetPathsInAPK(CSCore::StorageLocation in_location, const std::string& in_directoryPath, bool in_recursive) const;
			//--------------------------------------------------------------
			/// returns whether or not a file exists in the APK.
			///
			/// @author S Downie
			///
			/// @param The package or CS storage location
			/// @param the file path.
			///
			/// @return whether or not it exists.
			//--------------------------------------------------------------
			bool DoesFileExistInAPK(CSCore::StorageLocation in_location, const std::string& in_filePath) const;
			//--------------------------------------------------------------
			/// returns whether or not a directory exists in the APK.
			/// Note: Unlike other similar methods, empty directories will not
			/// be considered.
			///
			/// @author Ian Copland
			///
			/// @param The package or CS storage location
			/// @param the directory path.
			///
			/// @return whether or not it exists.
			//--------------------------------------------------------------
			bool DoesDirectoryExistInAPK(CSCore::StorageLocation in_location, const std::string& in_directoryPath) const;
			//--------------------------------------------------------------
			/// returns all files and directories in a directory.
			///
			/// @author Ian Copland
			///
			/// @param the directory.
			/// @param Whether or not to recurse into sub directories.
			/// @param The parent directory.
			///
			/// @return The output paths.
			//--------------------------------------------------------------
			std::vector<std::string> GetAllPaths(const std::string& in_directoryPath, bool in_recurse, const std::string& in_parentDirectoryPath = "") const;
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
			std::vector<PathInfo> GetPossibleAbsoluteDirectoryPaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const;
	        //------------------------------------------------------------
	        /// @author S Downie
	        ///
	        /// @param Directories
	        /// @param Recurse into sub directories
			///
	        /// @return Content file names
	        //------------------------------------------------------------
			std::vector<std::string> GetDirectoryContents(const std::vector<PathInfo>& in_directoryInfos, bool in_recursive) const;
			//--------------------------------------------------------------
			/// Add the item at the given filepath to the manifest
			///
			/// @author S Downie
			///
			/// @param Location in package or cs
			/// @param File path
			/// @param Zip file pos
			/// @param [In/Out] Manifest to add them to
			//--------------------------------------------------------------
			void AddItemToManifest(CSCore::StorageLocation in_location, const std::string& in_filePath, unz_file_pos in_zipPos, std::vector<APKManifestItem>& inout_items);

			mutable std::mutex m_minizipMutex;

			std::string m_apkPath;
			std::string m_storagePath;
			std::vector<APKManifestItem> m_apkAppManifestItems;
			std::vector<APKManifestItem> m_apkCSManifestItems;
		};
	}
}

#endif

#endif
