/*
 *  FileSystem.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_ANDROID_FILEIO_FILESYSTEM_
#define _MOFLO_PLATFORM_ANDROID_FILEIO_FILESYSTEM_

#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/Minizip/unzip.h>

#include <mutex>
#include <string>

namespace ChilliSource
{
	namespace Android 
	{
		struct DirInfo
		{
			std::string strPath;
			Core::StorageLocation eLocation;
		};

        struct APKFileInfo
        {
        	u32 udwFileNameHash;
        	unz_file_pos sZipPos;
        };

		//=========================================================================================
		/// File System
		///
		/// The Android Backend for the file system
		//=========================================================================================
		class FileSystem : public Core::FileSystem
		{
		public:
			FileSystem();
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
            Core::FileStreamSPtr CreateFileStream(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath, Core::FileMode ineFileMode) const override;
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
            bool CreateFile(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, s8* inpbyData, u32 inudwDataSize) const override;
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
            bool CreateDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const override;
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
            			  Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationFilepath) const override;
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
            				   Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationDirectory) const override;
            //--------------------------------------------------------------
            /// Delete File
            ///
            /// Deletes the specified file.
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @return Whether or not the file was successfully deleted.
            //--------------------------------------------------------------
            bool DeleteFile(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath) const override;
            //--------------------------------------------------------------
            /// Delete Directory
            ///
            /// Deletes a directory and all its contents.
            ///
            /// @param The storage location.
            /// @param The directory.
            /// @return Whether or not the directory was successfully deleted.
            //--------------------------------------------------------------
            bool DeleteDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const override;
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
            void GetFileNamesWithExtensionInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                      const std::string& instrExtension, std::vector<std::string> &outstrFileNames, bool inbAppendFullPath = false) const override;
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
                                                            const std::string& instrName, std::vector<std::string> &outstrFileNames, bool inbAppendFullPath = false) const override;
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
            void GetFileNamesInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                 std::vector<std::string> &outstrFileNames, bool inbAppendFullPath = false) const override;
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
            void GetDirectoriesInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                   std::vector<std::string> &outstrDirectories, bool inbAppendFullPath = false) const override;
            //--------------------------------------------------------------
            /// Does File Exist
            ///
            /// returns whether or not the given file exists.
            ///
            /// @param The Storage Location
            /// @param The filepath
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            bool DoesFileExist(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath) const override;
			//--------------------------------------------------------------
			/// Does File Exist In Cached DLC
			///
			/// @param The filepath.
			/// @return Whether or not it is in the cached DLC.
			//--------------------------------------------------------------
			bool DoesFileExistInCachedDLC(const std::string& instrFilepath) const override;
			//--------------------------------------------------------------
			/// Does File Exist In Package DLC
			///
			/// @param The filepath.
			/// @return Whether or not it is in the package DLC.
			//--------------------------------------------------------------
			bool DoesFileExistInPackageDLC(const std::string& instrFilepath) const override;
            //--------------------------------------------------------------
            /// Does Directory Exist
            ///
            /// returns whether or not the given directory exists.
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            bool DoesDirectoryExist(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const override;
            //--------------------------------------------------------------
			/// Is Storage Location Available
			///
            /// @param The source storage location.
			/// @return whether or not the storage location is available on
            ///         this device.
			//--------------------------------------------------------------
			bool IsStorageLocationAvailable(Core::StorageLocation ineStorageLocation) const override;
            //--------------------------------------------------------------
			/// Get Storage Location Directory
			///
            /// @param The source storage location.
			/// @return The directory. returns an empty string if the location
            ///         is not available.
			//--------------------------------------------------------------
			std::string GetStorageLocationDirectory(Core::StorageLocation ineStorageLocation) const override;
            //--------------------------------------------------------------
            /// Get Directory For DLC File
            ///
            /// @param The filename of the DLC asset.
            /// @return The directory to either the package DLC or cache DLC.
            //--------------------------------------------------------------
            std::string GetDirectoryForDLCFile(const std::string& instrFilePath) const override;
            //--------------------------------------------------------------
			/// Get Directory For Package File
			///
            /// @param The filename of the package asset.
			/// @return The directory to either the correct device package directory.
			//--------------------------------------------------------------
            std::string GetDirectoryForPackageFile(const std::string& instrFilePath) const override;
            //--------------------------------------------------------------
            /// Get Zip Pos For Package File
            ///
            /// @param The filename of the package asset.
            /// @return The zip pos to the package file
            //--------------------------------------------------------------
            unz_file_pos GetZipPosForPackageFile(const std::string& instrFilePath) const;
            //--------------------------------------------------------------
			/// Is Storage Available
			///
            /// @return whether or not android storage is available
			//--------------------------------------------------------------
			bool IsStorageAvailable() const;

		private:
			//--------------------------------------------------------------
			/// Create APK File Stream
			///
			/// Creates a file stream to a file within the APK (zip) file.
			/// This provides a uniform api for handling files within
			/// the apk and normal filestreams.
			///
			/// @param the file pos.
			/// @param the file mode.
			/// @return the APK filestream.
			//--------------------------------------------------------------
			Core::FileStreamSPtr CreateAPKFileStream(const unz_file_pos& inPos, Core::FileMode ineFileMode) const;
			//--------------------------------------------------------------
			/// Copy File From APK
			///
			/// Copies a file from the APK to one of the writable storage
			/// locations.
			///
			/// @param the source file.
			/// @param the destination storage location
			/// @param the destination file.
			/// @return whether or not this has succeeded.
			//--------------------------------------------------------------
			bool CopyFileFromAPK(const std::string& instrPath, Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationFilepath) const;
			//--------------------------------------------------------------
			/// Get All Items In Directory In APK
			///
			/// returns all files and directories in a directory of the APK.
			/// Note: Unlike other similar methods, empty directories will
			/// not be listed.
			///
			/// @param the directory.
			/// @param whether or not to recurse
			/// @param out parameter containing all the found items.
			//--------------------------------------------------------------
			void GetAllItemsInDirectoryInAPK(const std::string& instrPath, bool inbRecurse, std::vector<std::string>& outItems, std::vector<unz_file_pos>& outPos) const;
			//--------------------------------------------------------------
			/// Does File Exist In APK
			///
			/// returns whether or not a file exists in the APK
			///
			/// @param the filepath.
			/// @param Out: The zip pos
			/// @return whether or not it exists.
			//--------------------------------------------------------------
			bool DoesFileExistInAPKHashedStore(const std::string& instrPath, unz_file_pos& outZipPos) const;
			//--------------------------------------------------------------
			/// Does Directory Exist In APK
			///
			/// returns whether or not a directory exists in the APK.
			/// Note: Unlike other similar methods, empty directories will be
			/// considered to be not existing.
			///
			/// @param the directory path.
			/// @return whether or not it exists.
			//--------------------------------------------------------------
			bool DoesDirectoryExistInAPK(const std::string& instrPath) const;
			//--------------------------------------------------------------
			/// Create Base Directory
			///
			/// Creates a base directory.
			///
			/// @param The directory
			/// @return Returns whether or not this was successful. Inability
			///         to create the directory due to it already existing
			///         is still considered successful.
			//--------------------------------------------------------------
			bool CreateBaseDirectory(const std::string& instrDirectory) const;
			//--------------------------------------------------------------
			/// Copy File
			///
			/// Copies a file from one location to another.
			///
			/// @param The source directory.
			/// @param The destination directory.
			/// @return Whether or not the file was successfully copied.
			//--------------------------------------------------------------
			bool CopyFile(const std::string& instrSourceFilepath, const std::string& instrDestinationFilepath) const;
			//--------------------------------------------------------------
			/// Delete Directory
			///
			/// Deletes a directory and all its contents.
			///
			/// @param The directory.
			/// @return Whether or not the directory was successfully deleted.
			//--------------------------------------------------------------
			bool DeleteDirectory(const std::string& instrDirectory) const;
			//--------------------------------------------------------------
			/// Get All Items In Directory
			///
			/// returns all files and directories in a directory
			///
			/// @param the directory.
			/// @param whether or not to recurse
			/// @param the path to add to the items
			/// @param out parameter containing all the found items.
			//--------------------------------------------------------------
			void GetAllItemsInDirectory(const std::string& instrPath, bool inbRecurse, const std::string& instrPathSoFar, std::vector<std::string>& outItems) const;
			//--------------------------------------------------------------
			/// Does File Exist
			///
			/// returns whether or not a file exists
			///
			/// @param the filepath.
			/// @return whether or not it exists.
			//--------------------------------------------------------------
			bool DoesFileExist(const std::string& instrPath) const;
			//--------------------------------------------------------------
			/// Does Directory Exist
			///
			/// returns whether or not a directory exists
			///
			/// @param the directory path.
			/// @return whether or not it exists.
			//--------------------------------------------------------------
			bool DoesDirectoryExist(const std::string& instrPath) const;
			//--------------------------------------------------------------
			/// Does File Exist In DLC Cache
			///
			/// returns whether or not a file exists specifically
			/// in the DCL cache.
			///
			/// @param the filepath.
			/// @return whether or not it exists.
			//--------------------------------------------------------------
			bool DoesFileExistInDLCCache(const std::string& instrPath) const;
			//--------------------------------------------------------------
			/// Does Directory Exist In DLC Cache
			///
			/// returns whether or not a directory exists specifically
			/// in the DCL cache.
			///
			/// @param the directory path.
			/// @return whether or not it exists.
			//--------------------------------------------------------------
			bool DoesDirectoryExistInDLCCache(const std::string& instrPath) const;
	        //------------------------------------------------------------
	        /// Get Paths For Storage Location
	        ///
	        /// @param Storage location
	        /// @param File name to append
			/// @param Out: All the paths for the given location
			//------------------------------------------------------------
			void GetPathsForStorageLocation(Core::StorageLocation ineStorageLocation, const std::string& instrFileName, std::vector<DirInfo>& outaPaths) const;
	        //------------------------------------------------------------
	        /// Get Directory Contents
	        ///
	        /// @param Directories
	        /// @param Recurse into sub directories
	        /// @param Out: Content file names
	        //------------------------------------------------------------
	        void GetDirectoryContents(const std::vector<DirInfo>& inaDirs, bool inbRecursive, std::vector<std::string>& outstrContents) const;
	        //--------------------------------------------------------------
	        /// Create Hashed APK File List
	        ///
	        //--------------------------------------------------------------
	        void CreateHashedAPKFileList();

		private:

			mutable std::mutex mMinizipMutex;
			std::string mstrPathToAPK;
			std::string mstrStorageDir;
			bool mbStorageAvailable;

			std::vector<APKFileInfo> mAPKFileInfo;
		};
	}
}

#endif
