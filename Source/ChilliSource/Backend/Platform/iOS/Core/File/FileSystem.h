//
//  FileSystem.h
//  Chilli Source
//
//  Created by I Copland on 25/03/2011.
//  Copyright 2011 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_CORE_FILE_FILESYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_CORE_FILE_FILESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>

#include <string>

namespace ChilliSource
{
	namespace iOS 
	{
		//-------------------------------------------------------------------
		/// The iOS specific implementation for the File System.
        ///
        /// @author I Copland
		//-------------------------------------------------------------------
		class FileSystem final : public Core::FileSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(FileSystem);
            //----------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
            ///
            /// @author I Copland
			///
			/// @param The interface Id.
            ///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------------------
            /// Creates a new file stream to the given file in the given
            /// storage location.
            ///
            /// @author I Copland
            ///
            /// @param The storage location.
            /// @param The file path.
            /// @param The file mode.
            ///
            /// @return The new file stream.
            //--------------------------------------------------------------
            Core::FileStreamUPtr CreateFileStream(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::FileMode in_fileMode) const override;
            //--------------------------------------------------------------
            /// Creates the given directory. The full directory hierarchy will
            /// be created.
            ///
            /// @author I Copland
            ///
            /// @param The Storage Location
            /// @param The directory path.
            ///
            /// @return Returns whether or not this was successful. Failure to
            /// create the directory becuase it already exists is considered
            /// a success.
            //--------------------------------------------------------------
            bool CreateDirectory(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
            //--------------------------------------------------------------
            /// Copies a file from one location to another.
            ///
            /// @author I Copland
            ///
            /// @param The source storage location.
            /// @param The source directory.
            /// @param The destination storage location.
            /// @param The destination directory.
            ///
            /// @return Whether or not the file was successfully copied.
            //--------------------------------------------------------------
            bool CopyFile(Core::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
                          Core::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const override;
            //--------------------------------------------------------------
            /// Copies a directory from one location to another. If the
            /// destination directory does not exist, it will be created.
            ///
            /// @author I Copland
            ///
            /// @param The source storage location.
            /// @param The source directory.
            /// @param The destination storage location.
            /// @param The destination directory.
            ///
            /// @return Whether or not the files were successfully copied.
            //--------------------------------------------------------------
            bool CopyDirectory(Core::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
                               Core::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const override;
            //--------------------------------------------------------------
            /// Deletes the specified file.
            ///
            /// @author I Copland
            ///
            /// @param The storage location.
            /// @param The filepath.
            ///
            /// @return Whether or not the file was successfully deleted.
            //--------------------------------------------------------------
            bool DeleteFile(Core::StorageLocation in_storageLocation, const std::string& in_filepath) const override;
            //--------------------------------------------------------------
            /// Deletes a directory and all its contents.
            ///
            /// @author I Copland
            ///
            /// @param The storage location.
            /// @param The directory.
            ///
            /// @return Whether or not the directory was successfully deleted.
            //--------------------------------------------------------------
            bool DeleteDirectory(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
            //--------------------------------------------------------------
            /// creates a dynamic array containing the filenames of each
            /// file that has the provided extension in the given directory.
            /// File paths will be returned relative to the input directory.
            ///
            /// @author S Downie
            ///
            /// @param The Storage Location
            /// @param The directory path.
            /// @param Flag to determine whether or not to recurse into sub
            /// directories
            /// @param The extension
            /// @param [Out] Dynamic array containing the filenames.
            //--------------------------------------------------------------
            void GetFilePathsWithExtension(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, const std::string& in_extension,
                                           std::vector<std::string>& out_filePaths) const override;
            //--------------------------------------------------------------
            /// Creates a dynamic array containing the filenames of each of
            /// each file with the given name in the given directory. File
            /// paths will be relative to the input directory.
            ///
            /// @author S Downie
            ///
            /// @param The Storage Location.
            /// @param The directory.
            /// @param Flag to determine whether or not to recurse into sub
            /// directories.
            /// @param The filename.
            /// @param [Out] Dynamic array containing the filenames.
            //--------------------------------------------------------------
            void GetFilePathsWithFileName(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, const std::string& in_fileName,
                                          std::vector<std::string>& out_filePaths) const override;
            //--------------------------------------------------------------
            /// Creates a dynamic array containing the filenames of each file
            /// in the given directory. File paths will be relative to the
            /// input directory.
            ///
            /// @author I Copland
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub
            /// directories
            /// @param [Out] dynamic array containing the filenames.
            //--------------------------------------------------------------
            void GetFilePaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, std::vector<std::string>& out_filePaths) const override;
            //--------------------------------------------------------------
            /// Creates a dynamic array containing the names of each directory
            /// in the given directory. Directory paths will be relative to
            /// the input directory.
            ///
            /// @author I Copland
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub directories
            /// @param Output dynamic array containing the dir names.
            //--------------------------------------------------------------
            void GetDirectoryPaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, std::vector<std::string> &out_directoryPaths) const override;
            //--------------------------------------------------------------
            /// returns whether or not the given file exists.
            ///
            /// @author I Copland
            ///
            /// @param The Storage Location
            /// @param The file path
            ///
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            bool DoesFileExist(Core::StorageLocation in_storageLocation, const std::string& in_filePath) const override;
			//--------------------------------------------------------------
			/// Returns whether or not the file exists in the Cached DLC
            /// directory.
            ///
            /// @author I Copland
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
            /// @author I Copland
			///
			/// @param The file path.
            ///
			/// @return Whether or not it is in the package DLC.
			//--------------------------------------------------------------
			bool DoesFileExistInPackageDLC(const std::string& in_filePath) const override;
            //--------------------------------------------------------------
            /// Returns whether or not the given directory exists.
            ///
            /// @author I Copland
            ///
            /// @param The Storage Location
            /// @param The directory path
            ///
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            bool DoesDirectoryExist(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
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
			std::string GetAbsolutePathToStorageLocation(Core::StorageLocation in_storageLocation) const override;
            //--------------------------------------------------------------
			/// Returns the full absolute path to the given file in the
            /// given storage location. The value returned from this will
            /// be platform specfic so it should be used with care in
            /// cross platform projects.
            ///
            /// @author S Downie
			///
            /// @param The storage location for the file.
            /// @param The file path of the file relative to the storage
            /// location.
            ///
			/// @return The full path to the file.
			//--------------------------------------------------------------
			std::string GetAbsolutePathToFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath) const override;

		private:
            friend Core::FileSystemUPtr Core::FileSystem::Create();
            //--------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            FileSystem();
            //--------------------------------------------------------------
			/// Returns whether or not a file or directory exists specifically
            /// in the DLC cache.
            ///
            /// @author I Copland
            ///
            /// @param The filepath.
            ///
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesItemExistInDLCCache(const std::string& in_path, bool in_folder) const;
            //--------------------------------------------------------------
			/// returns whether the path exists in the stored hashed manifest
            ///
            /// @author S Downie
            ///
            /// @param the filepath.
            ///
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesFileExistInHashedStore(const std::string& in_path) const;
            //------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param File name to append
            /// @param [Out] All the paths for the given location
            //------------------------------------------------------------
            void GetPathsForStorageLocation(Core::StorageLocation in_storageLocation, const std::string& in_path, std::vector<std::string>& out_paths) const;
            //--------------------------------------------------------------
            /// Creates the hashed bundle file list.
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            void CreateHashedBundleFileList();
            
            
			std::string m_bundlePath;
			std::string m_documentsPath;
            std::string m_libraryPath;
            
            std::vector<u32> m_hashedPackageFilePaths;
		};
	}
}

#endif
