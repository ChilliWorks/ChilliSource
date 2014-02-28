/*
 *  FileSystem.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_WINDOWS_FILEIO_FILESYSTEM_
#define _MOFLO_PLATFORM_WINDOWS_FILEIO_FILESYSTEM_

#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <string>


namespace ChilliSource
{
	namespace Windows 
	{
		//--------------------------------------------------------------------------------------------------
		/// Description:
		///
		/// All file input and output is handled through the manager. From this a
		/// FileStream to a file in the app bundle or documents folder can be created.
		/// This stream can then be used to read data directly from the file.
		/// This class should be used over fstream in order to insure cross platform
		/// compatability.
		//--------------------------------------------------------------------------------------------------
		class CFileSystem : public Core::IFileSystem
		{
		public:
			CFileSystem();
			//--------------------------------------------------------------------------------------------------
			/// Description:
			///
			/// Initialise the File Manager for use on the current file system.
			//--------------------------------------------------------------------------------------------------
			void Initialise();
			//--------------------------------------------------------------------------------------------------
			/// Description:
			///
			/// Creates a fileStream for the file at the specified location.
			///
			/// @param The storage location 
			/// @param The filepath relative to the storage location
			/// @param FileModeL ineMode The file mode. The file can be opened as read or write.
			//--------------------------------------------------------------------------------------------------
			Core::FileStreamSPtr CreateFileStream(Core::StorageLocation ineStorageLocation, const std::string& instrFilename, Core::FileMode ineMode) const;
			//--------------------------------------------------------------
            /// Create File
            ///
            /// Creates a new file at the given location with the provided
			/// data
            ///
			/// @param Storage location
            /// @param The filepath relative to the storage location
            /// @param Data to write to file
            /// @param Data size
			/// @return Success
            //--------------------------------------------------------------
			bool CreateFile(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, s8* inpbyData, u32 inudwDataSize) const;
			//--------------------------------------------------------------
            /// Create Directory
            ///
            /// Creates a new folder at the given location 
            ///
			/// @param Storage location
            /// @param The filepath relative to the storage location
			/// @return Success
            //--------------------------------------------------------------
			bool CreateDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const;
			//--------------------------------------------------------------
			/// Copy File
			///
			/// Copies a single file from one location to another
			///
			/// @param The source storage location
			/// @param The source filepath
			/// @param The destination storage location
			/// @param The destination filepath
			/// @return Whether or not the file was copied
			//--------------------------------------------------------------
			bool CopyFile(Core::StorageLocation ineSourceLocation, const std::string& instrSourcePath, 
							Core::StorageLocation ineDestLocation, const std::string& instrDestPath) const;
			//--------------------------------------------------------------
			/// Copy Directory
			///
			/// Copies a directory file from one location to another
			///
			/// @param The source storage location
			/// @param The source filepath
			/// @param The destination storage location
			/// @param The destination filepath
			/// @return Whether or not the file was copied
			//--------------------------------------------------------------
			bool CopyDirectory(Core::StorageLocation ineSourceLocation, const std::string& instrSourcePath, 
							Core::StorageLocation ineDestLocation, const std::string& instrDestPath) const;
			//--------------------------------------------------------------
			/// Delete File
			///
			/// Deletes the specified file
			///
			/// @param Storage location
			/// @param File path
			/// @return Whether successfully deleted
			//--------------------------------------------------------------
			bool DeleteFile(Core::StorageLocation ineLocation, const std::string& instrFilePath) const;
			//--------------------------------------------------------------
			/// Delete Directory
			///
			/// Deletes the specified directory and its contents
			///
			/// @param Storage location
			/// @param File path
			/// @return Whether successfully deleted
			//--------------------------------------------------------------
			bool DeleteDirectory(Core::StorageLocation ineLocation, const std::string& instrFilePath) const;
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
			void GetPathForFilesWithNameInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
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
            /// Does File Exist
            //--------------------------------------------------------------
            bool DoesFileExist(const std::string& instrFilepath) const;
            //--------------------------------------------------------------
            /// Does Directory Exist
            //--------------------------------------------------------------
            bool DoesDirectoryExist(const std::string& instrDirectory) const;

		private:

			std::string mstrBundlePath;
			std::string mstrDocumentsPath;
            std::string mstrLibraryPath;
		};
	}
}

#endif
