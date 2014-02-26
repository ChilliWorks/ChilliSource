/*
 *  FileSystem.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_IOS_FILEIO_FILESYSTEM_
#define _MOFLO_PLATFORM_IOS_FILEIO_FILESYSTEM_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>

#include <string>


namespace ChilliSource
{
	namespace iOS 
	{
		//=========================================================================================
		/// File System
        ///
        /// The iOS Backend for the file system
		//=========================================================================================
		class CFileSystem : public Core::IFileSystem
		{
		public:
			CFileSystem();
            //--------------------------------------------------------------
            /// Create File Stream
            ///
            /// This will create a filestream to the file at the specified
            /// filepath.
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param The file mode.
            //--------------------------------------------------------------
            Core::FileStreamPtr CreateFileStream(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath, Core::FileMode ineFileMode) const;
            //--------------------------------------------------------------
            /// Create File
            ///
            /// Creates a new file at the given location with the provided
            /// data.
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param The data.
            /// @param The size of the data.
            /// @return Whether or not the file was successfully created.
            //--------------------------------------------------------------
            bool CreateFile(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, s8* inpbyData, u32 inudwDataSize) const;
            //--------------------------------------------------------------
            /// Create Directory
            ///
            /// Creates the given directory. The full directory hierarchy will
            /// be created.
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @return Returns whether or not this was successful. Inability
            ///         to create the directory due to it already existing
            ///         is still considered successful.
            //--------------------------------------------------------------
            bool CreateDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const;
            //--------------------------------------------------------------
            /// Copy File
            ///
            /// Copies a file from one location to another.
            ///
            /// @param The source storage location.
            /// @param The source directory.
            /// @param The destination storage location.
            /// @param The destination directory.
            /// @return Whether or not the file was successfully copied.
            //--------------------------------------------------------------
            bool CopyFile(Core::StorageLocation ineSourceStorageLocation, const std::string& instrSourceFilepath,
                          Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationFilepath) const;
            //--------------------------------------------------------------
            /// Copy Directory
            ///
            /// Copies a directory from one location to another. If the 
            /// destination directory does not exist, it will be created.
            ///
            /// @param The source storage location.
            /// @param The source directory.
            /// @param The destination storage location.
            /// @param The destination directory.
            /// @return Whether or not the files were successfully copied.
            //--------------------------------------------------------------
            bool CopyDirectory(Core::StorageLocation ineSourceStorageLocation, const std::string& instrSourceDirectory,
                               Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationDirectory) const;
            //--------------------------------------------------------------
            /// Delete File
            ///
            /// Deletes the specified file.
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @return Whether or not the file was successfully deleted.
            //--------------------------------------------------------------
            bool DeleteFile(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath) const;
            //--------------------------------------------------------------
            /// Delete Directory
            ///
            /// Deletes a directory and all its contents.
            ///
            /// @param The storage location.
            /// @param The directory.
            /// @return Whether or not the directory was successfully deleted.
            //--------------------------------------------------------------
            bool DeleteDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const;
            //--------------------------------------------------------------
            /// Get File Names With Extension In Directory
            ///
            /// creates a dynamic array containing the filenames of each of
            /// each file that has the provided extension in the given
            /// directory.
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub directories
            /// @param The extension
            /// @param Output dynamic array containing the filenames.
            //--------------------------------------------------------------
            void GetFileNamesWithExtensionInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
                                                      const std::string& instrExtension, std::vector<std::string> &outstrFileNames, bool inbAppendFullPath = false) const;
            //--------------------------------------------------------------
            /// Get Path For Files With Name In Directory
            ///
            /// Creates a dynamic array containing the filenames of each of
            /// each file with the given name in the given
            /// directory.
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub directories
            /// @param The name
            /// @param Output dynamic array containing the filenames.
            //--------------------------------------------------------------
            void GetPathForFilesWithNameInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                                 const std::string& instrName, std::vector<std::string> &outstrFileNames, bool inbAppendFullPath = false) const;
            //--------------------------------------------------------------
            /// Get File Names In Directory
            ///
            /// creates a dynamic array containing the filenames of each file
            /// in the given directory.
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub directories
            /// @param Output dynamic array containing the filenames.
            //--------------------------------------------------------------
            void GetFileNamesInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
                                         std::vector<std::string> &outstrFileNames, bool inbAppendFullPath = false) const;
            //--------------------------------------------------------------
            /// Get Directories In Directory
            ///
            /// creates a dynamic array containing the names of each directory
            /// in the given directory.
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub directories
            /// @param Output dynamic array containing the dir names.
            //--------------------------------------------------------------
            void GetDirectoriesInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
                                           std::vector<std::string> &outstrDirectories, bool inbAppendFullPath = false) const;
            //--------------------------------------------------------------
            /// Does File Exist
            ///
            /// returns whether or not the given file exists.
            /// 
            /// @param The Storage Location
            /// @param The filepath
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            bool DoesFileExist(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath) const;
            //--------------------------------------------------------------
			/// Does File Exist In Cached DLC
			///
			/// @param The filepath.
			/// @return Whether or not it is in the cached DLC.
			//--------------------------------------------------------------
			bool DoesFileExistInCachedDLC(const std::string& instrFilepath) const;
			//--------------------------------------------------------------
			/// Does File Exist In Package DLC
			///
			/// @param The filepath.
			/// @return Whether or not it is in the local DLC.
			//--------------------------------------------------------------
			bool DoesFileExistInPackageDLC(const std::string& instrFilepath) const;
            //--------------------------------------------------------------
            /// Does Directory Exist
            ///
            /// returns whether or not the given directory exists.
            /// 
            /// @param The Storage Location
            /// @param The directory
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            bool DoesDirectoryExist(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const;
            //--------------------------------------------------------------
			/// Get Directory For DLC File
			///
            /// @param The filename of the DLC asset.
			/// @return The directory to either the package DLC or cache DLC.
			//--------------------------------------------------------------
            std::string GetDirectoryForDLCFile(const std::string& instrFilePath) const;
            //--------------------------------------------------------------
			/// Get Directory For Package File
			///
            /// @param The filename of the package asset.
			/// @return The directory to either the correct device package directory.
			//--------------------------------------------------------------
            std::string GetDirectoryForPackageFile(const std::string& instrFilePath) const;
            //--------------------------------------------------------------
			/// Is Storage Location Available
			///
            /// @param The source storage location.
			/// @return whether or not the storage location is available on
            ///         this device.
			//--------------------------------------------------------------
			bool IsStorageLocationAvailable(Core::StorageLocation ineStorageLocation) const;
            //--------------------------------------------------------------
			/// Get Storage Location Directory
			///
            /// @param The source storage location.
			/// @return The directory. returns an empty string if the location
            ///         is not available.
			//--------------------------------------------------------------
			std::string GetStorageLocationDirectory(Core::StorageLocation ineStorageLocation) const;

		private:
            //--------------------------------------------------------------
			/// Does Item Exist In DLC Cache
			///
			/// returns whether or not a file or directory exists specifically
            ///  in the DCL cache.
            ///
            /// @param the filepath.
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesItemExistInDLCCache(const std::string& instrPath, bool inbFolder) const;
            //--------------------------------------------------------------
			/// Does File Exist In Hashed Store
			///
			/// returns whether the path exists in the stored hashed manifest
            ///
            /// @param the filepath.
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesFileExistInHashedStore(const std::string& instrPath) const;
            //--------------------------------------------------------------
			/// Does File Exist
			///
			/// returns whether the path exists on the filesystem
            ///
            /// @param the filepath.
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesFileExist(const std::string& instrPath) const;
            //--------------------------------------------------------------
			/// Does Folder Exist
			///
			/// returns whether the path exists on the filesystem
            ///
            /// @param the filepath.
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesFolderExist(const std::string& instrPath) const;
            //------------------------------------------------------------
            /// Get Paths For Storage Location
            ///
            /// @param Storage location
            /// @param File name to append
            /// @param Out: All the paths for the given location
            //------------------------------------------------------------
            void GetPathsForStorageLocation(Core::StorageLocation ineStorageLocation, const std::string& instrFileName, std::vector<std::string>& outaPaths) const;
            //--------------------------------------------------------------
            /// Create Hashed Bundle File List
            ///
            //--------------------------------------------------------------
            void CreateHashedBundleFileList();
            
            
			std::string mstrBundlePath;
			std::string mstrDocumentsPath;
            std::string mstrLibraryPath;
            
            std::vector<u32> mHashedPackageFileNames;
		};
	}
}

#endif
