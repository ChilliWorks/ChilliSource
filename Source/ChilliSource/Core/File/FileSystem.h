//
//  FileSystem.h
//  Chilli Source
//
//  Created by I Copland on 25/03/2011.
//  Copyright 2011 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_FILE_FILESYSTEM_H_
#define _CHILLISOURCE_CORE_FILE_FILESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <string>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------
		/// A system for handling cross platform access to the file
        /// system. This provides sandboxed access to specific locations
        /// on disk, such as access to application assets, or save
        /// data.
        ///
        /// @author I Copland
		//-----------------------------------------------------------------
		class FileSystem : public AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(FileSystem);
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Directory to load device dependent assets from
            //----------------------------------------------------------
            static const std::string& GetDeviceResourceDirectory();
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Directory to load device dependent assets from
            //----------------------------------------------------------
            static const std::string& GetDefaultDeviceResourceDirectory();
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Directory to load shared assets from
            //----------------------------------------------------------
            static const std::string& GetDefaultResourceDirectory();
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Density of assets in device dependent folder
            //----------------------------------------------------------
            static f32 GetDeviceResourcesDensity();
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Directory to load device dependent assets from
            /// @param Fallback to load device depenedent assets from
            /// @param Directory to load shared assets from
            /// @param Density of assets in the device dependent folder
            //----------------------------------------------------------
            static void SetResourceDirectories(const std::string& in_deviceDirectory, const std::string& in_defaultDeviceDirectory, const std::string& in_defaultDirectory, f32 in_assetsDensity);
            //----------------------------------------------------------
            /// Create the platform dependent backend
            ///
            /// @author S Downie
            ///
            /// @return New backend instance
            //----------------------------------------------------------
            static FileSystemUPtr Create();
            //--------------------------------------------------------------
            /// Reads the contents of a file from disc if the file exists.
            ///
            /// @author I Copland
            ///
            /// @param The Storage Location.
            /// @param The file path.
            /// @param [Out] The contents of the file.
            ///
            /// @return Whether or not the file read was successful.
            //--------------------------------------------------------------
            bool ReadFile(StorageLocation in_storageLocation, const std::string& in_filePath, std::string& out_contents) const;
            //--------------------------------------------------------------
            /// Write a file to disc with the given data.
            ///
            /// @author I Copland
            ///
            /// @param The Storage Location.
            /// @param The file path.
            /// @param The contents of the file.
            ///
            /// @return Whether or not the file was successfully written.
            //--------------------------------------------------------------
            bool WriteFile(StorageLocation in_storageLocation, const std::string& in_filePath, const std::string& in_contents) const;
            //--------------------------------------------------------------
            /// Write a file to disc with the given data.
            ///
            /// @author S Downie
            ///
            /// @param The Storage Location.
            /// @param The file path.
            /// @param The data.
            /// @param The size of the data.
            ///
            /// @return Whether or not the file was successfully written.
            //--------------------------------------------------------------
            bool WriteFile(StorageLocation in_storageLocation, const std::string& in_filePath, const s8* in_data, u32 in_dataSize) const;
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
            virtual FileStreamUPtr CreateFileStream(StorageLocation in_storageLocation, const std::string& in_filePath, FileMode in_fileMode) const = 0;
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
            virtual bool CreateDirectory(StorageLocation in_storageLocation, const std::string& in_directoryPath) const = 0;
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
            virtual bool CopyFile(StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
                                  StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const = 0;
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
            virtual bool CopyDirectory(StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
                                       StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const = 0;
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
            virtual bool DeleteFile(StorageLocation in_storageLocation, const std::string& in_filepath) const = 0;
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
            virtual bool DeleteDirectory(StorageLocation in_storageLocation, const std::string& in_directoryPath) const = 0;
            //--------------------------------------------------------------
            /// Creates a dynamic array containing the file names of each file
            /// in the given directory. File paths will be relative to the
            /// input directory.
            ///
            /// @author I Copland
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub
            /// directories
            /// @param [Out] dynamic array containing the file names.
            //--------------------------------------------------------------
            virtual void GetFilePaths(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, std::vector<std::string>& out_filePaths) const = 0;
            //--------------------------------------------------------------
            /// creates a dynamic array containing the file names of each
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
            /// @param [Out] Dynamic array containing the file names.
            //--------------------------------------------------------------
            virtual void GetFilePathsWithExtension(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, const std::string& in_extension,
                                                   std::vector<std::string>& out_filePaths) const = 0;
            //--------------------------------------------------------------
            /// Creates a dynamic array containing the file names of each of
            /// each file with the given name in the given directory. File
            /// paths will be relative to the input directory.
            ///
            /// @author S Downie
            ///
            /// @param The Storage Location.
            /// @param The directory.
            /// @param Flag to determine whether or not to recurse into sub
            /// directories.
            /// @param The file name.
            /// @param [Out] Dynamic array containing the file names.
            //--------------------------------------------------------------
            virtual void GetFilePathsWithFileName(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, const std::string& in_fileName,
                                                  std::vector<std::string>& out_filePaths) const = 0;
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
            virtual void GetDirectoryPaths(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, std::vector<std::string> &out_directoryPaths) const = 0;
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
            virtual bool DoesFileExist(StorageLocation in_storageLocation, const std::string& in_filePath) const = 0;
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
			virtual bool DoesFileExistInCachedDLC(const std::string& in_filePath) const = 0;
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
			virtual bool DoesFileExistInPackageDLC(const std::string& in_filePath) const = 0;
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
            virtual bool DoesDirectoryExist(StorageLocation in_storageLocation, const std::string& in_directoryPath) const = 0;
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
			virtual std::string GetAbsolutePathToStorageLocation(StorageLocation in_storageLocation) const = 0;
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
			virtual std::string GetAbsolutePathToFile(StorageLocation in_storageLocation, const std::string& in_filePath) const = 0;
            //--------------------------------------------------------------
            /// Sets the directory used by the DLC system to fall back on
            /// if it is not in the DLC cache directory.
            ///
            /// @author S Downie
            ///
            /// @param The package DLC directory.
			//--------------------------------------------------------------
			void SetPackageDLCDirectory(const std::string& in_directory);
            //--------------------------------------------------------------
			/// Get Package DLC Directory
            ///
            /// @author S Downie
			///
            /// @return The package DLC directory.
			//--------------------------------------------------------------
			const std::string& GetPackageDLCDirectory() const;
            //--------------------------------------------------------------
            /// Calculate the MD5 checksum of the file at the
            /// give directory
            ///
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param File path
            ///
            /// @return MD5 checksum
            //--------------------------------------------------------------
            std::string GetFileChecksumMD5(StorageLocation in_storageLocation, const std::string& in_filePath) const;
            //--------------------------------------------------------------
            /// Calculate the MD5 checksum of the given directory
            ///
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param File path
            ///
            /// @return MD5 checksum
            //--------------------------------------------------------------
            std::string GetDirectoryChecksumMD5(StorageLocation in_storageLocation, const std::string& in_directoryPath) const;
            //--------------------------------------------------------------
            /// Gets the CRC32 Checksum for the given file.
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
			/// @param the filepath.
            ///
			/// @return the checksum.
			//--------------------------------------------------------------
			u32 GetFileChecksumCRC32(StorageLocation in_storageLocation, const std::string& in_filePath) const;
            //--------------------------------------------------------------
			/// Get Directory CRC32 Checksum
			///
            /// Gets the CRC32 Checksum for the given directory.
            ///
            /// @param The storage location.
			/// @param the filepath.
			/// @return the checksum.
			//--------------------------------------------------------------
			u32 GetDirectoryChecksumCRC32(StorageLocation in_storageLocation, const std::string& in_directoryPath) const;
	        //--------------------------------------------------------------
			/// @author I Copland
            ///
            /// @param The storage location.
			/// @param the file path.
            ///
            /// @return The size of the given file.
			//--------------------------------------------------------------
			u32 GetFileSize(StorageLocation in_storageLocation, const std::string& in_filePath) const;
	        //--------------------------------------------------------------
			/// @author I Copland
            ///
            /// @param The storage location.
			/// @param the Directory path.
            ///
            /// @return The size of the contents of the given directory.
			//--------------------------------------------------------------
			u32 GetDirectorySize(StorageLocation in_storageLocation, const std::string& in_directoryPath) const;
            //--------------------------------------------------------------
			/// Returns whether or not the given storage location can be
            /// written to.
            ///
            /// @author I Copland
			///
            /// @param The storage location.
            ///
			/// @return Whether or not it can be written to.
			//--------------------------------------------------------------
			bool IsStorageLocationWritable(StorageLocation in_storageLocation) const;
            //--------------------------------------------------------------
            /// Destructor
            ///
            /// @author I Copland
            //--------------------------------------------------------------
            virtual ~FileSystem() {}
        protected:
            
            //--------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            FileSystem();
            //--------------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The resource directories.
            //--------------------------------------------------------------
            const std::string* GetResourceDirectories() const;
        private:
            std::string m_packageDLCPath;
            
            static f32 s_assetsDensity;
            static std::string s_resourceDirectory[3];
		};
	}
		
}

#endif
