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
            ///
            /// @return dynamic array containing the filenames.
            //--------------------------------------------------------------
            std::vector<std::string> GetFilePaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const override;
            //--------------------------------------------------------------
            /// Creates a dynamic array containing the names of each directory
            /// in the given directory. Directory paths will be relative to
            /// the input directory.
            ///
            /// @author I Copland
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub
            /// directories
            ///
            /// @return Output dynamic array containing the dir names.
            //--------------------------------------------------------------
            std::vector<std::string> GetDirectoryPaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const override;
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
			std::string GetAbsolutePathToFile(Core::StorageLocation in_storageLocation, const std::string& in_path) const override;
            //--------------------------------------------------------------
			/// Returns the absolute path to the directory in the given storage
            /// location. The directory must exist otherwise an empty string
            /// will be returned. The result of this is platform specific
            /// so care should be taken when using this in cross platform
            /// projects.
            ///
            /// @author I Copland
			///
            /// @param The storage location for the directory.
            /// @param The directory path relative to the storage location.
            ///
			/// @return The full path to the directory.
			//--------------------------------------------------------------
			std::string GetAbsolutePathToDirectory(Core::StorageLocation in_storageLocation, const std::string& in_path) const override;

		private:
            friend Core::FileSystemUPtr Core::FileSystem::Create();
            //--------------------------------------------------------------
            /// A container for information on a single item in the package
            /// manifest.
            ///
            /// @author I Copland
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
            /// @author I Copland
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
            /// @author I Copland
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
            /// @author I Copland
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
            std::vector<std::string> GetPossibleAbsoluteDirectoryPaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const;
            
            
			std::string m_bundlePath;
			std::string m_documentsPath;
            std::string m_libraryPath;
            
            std::vector<PackageManifestItem> m_packageManifestItems;
		};
	}
}

#endif
