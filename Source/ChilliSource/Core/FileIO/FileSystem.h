/*
 *  FileSystem.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_FILEIO_FILE_SYSTEM_
#define _MOFLO_CORE_FILEIO_FILE_SYSTEM_

#include <ChilliSource/Core/FileIO/FileStream.h>
#include <ChilliSource/Core/System.h>
#include <string>

namespace moFlo
{
	namespace Core
	{
        //============================================
		/// Storage Location
		///
		/// enum stating where a file should be loaded
        /// from.
		//============================================
        enum STORAGE_LOCATION
        {
        	SL_NONE,
            SL_PACKAGE,
            SL_SAVEDATA,
            SL_CACHE,
            SL_DLC,
            SL_ROOT
        };
        //============================================
		/// Storage Location From String
		///
		/// Parses a string and returns a storage
        /// location.
		//============================================
        moFlo::Core::STORAGE_LOCATION GetStorageLocationFromString(const std::string & instrStorage);
        //============================================
		/// Storage Location And Filename
		///
		/// struct which states the storage location
        /// and filename of a file.
		//============================================
        struct StorageLocationAndFilename
        {
            std::string mstrFilename;
            STORAGE_LOCATION meStorageLocation;
        };
        typedef SHARED_PTR<StorageLocationAndFilename> StorageLocationAndFilenamePtr;
		//=========================================================================================
		/// IFileSystem
        ///
        /// Frontend for MoFlows file system. All file input and output in MoFlow should be handled
        /// through this to maintain cross platform support.
		//=========================================================================================
		class IFileSystem : public ISystem
		{
			DECLARE_NAMED_INTERFACE(IFileSystem);
		public:
            IFileSystem();
			virtual ~IFileSystem(){}
			//-------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //--------------------------------------------
            /// Get Device Resource Directory
            ///
            /// @return Directory to load device dependent assets from
            //--------------------------------------------
            static const std::string& GetDeviceResourceDirectory();
            //--------------------------------------------
            /// Get Default Device Resource Directory
            ///
            /// @return Directory to load device dependent assets from
            //--------------------------------------------
            static const std::string& GetDefaultDeviceResourceDirectory();
            //--------------------------------------------
            /// Get Default Resource Directory
            ///
            /// @return Directory to load shared assets from
            //--------------------------------------------
            static const std::string& GetDefaultResourceDirectory();
            //--------------------------------------------
            /// Get Resources Density
            ///
            /// @return Density of assets in device
            /// dependent folder
            //--------------------------------------------
            static f32 GetDeviceResourcesDensity();
            //--------------------------------------------
            /// Set Resource Directories
            ///
            /// @param Directory to load device dependent assets from
            /// @param Fallback to load device depenedent assets from
            /// @param Directory to load shared assets from
            /// @param Density of assets in the device dependent folder
            //--------------------------------------------
            static void SetResourceDirectories(const std::string& instrDeviceDirectory, const std::string& instrDefaultDeviceDirectory, const std::string& instrDefaultDirectory, f32 infAssetsDensity);
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
            virtual FileStreamPtr CreateFileStream(STORAGE_LOCATION ineStorageLocation, const std::string& instrFilepath, FILE_MODE ineFileMode) const = 0;
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
            virtual bool CreateFile(STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory, s8* inpbyData, u32 inudwDataSize) const = 0;
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
            virtual bool CreateDirectory(STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory) const = 0;
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
            virtual bool CopyFile(STORAGE_LOCATION ineSourceStorageLocation, const std::string& instrSourceFilepath, 
                                  STORAGE_LOCATION ineDestinationStorageLocation, const std::string& instrDestinationFilepath) const = 0;
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
            virtual bool CopyDirectory(STORAGE_LOCATION ineSourceStorageLocation, const std::string& instrSourceDirectory, 
                                       STORAGE_LOCATION ineDestinationStorageLocation, const std::string& instrDestinationDirectory) const = 0;
            //--------------------------------------------------------------
            /// Delete File
            ///
            /// Deletes the specified file.
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @return Whether or not the file was successfully deleted.
            //--------------------------------------------------------------
            virtual bool DeleteFile(STORAGE_LOCATION ineStorageLocation, const std::string& instrFilepath) const = 0;
            //--------------------------------------------------------------
            /// Delete Directory
            ///
            /// Deletes a directory and all its contents.
            ///
            /// @param The storage location.
            /// @param The directory.
            /// @return Whether or not the directory was successfully deleted.
            //--------------------------------------------------------------
            virtual bool DeleteDirectory(STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory) const = 0;
            //--------------------------------------------------------------
            /// Get File Names With Extension In Directory
            ///
            /// creates a dynamic array containing the filenames of each 
            /// file that has the provided extension in the given
            /// directory.
            ///
            /// @param The Storage Location
            /// @param The directory
            /// @param Flag to determine whether or not to recurse into sub directories
            /// @param The extension
            /// @param Output dynamic array containing the filenames.
            //--------------------------------------------------------------
            virtual void GetFileNamesWithExtensionInDirectory(STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                              const std::string& instrExtension, DYNAMIC_ARRAY<std::string> &outstrFileNames, bool inbAppendFullPath = false) const = 0;
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
            virtual void GetPathForFilesWithNameInDirectory(STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                              const std::string& instrName, DYNAMIC_ARRAY<std::string> &outstrFileNames, bool inbAppendFullPath = false) const = 0;
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
            virtual void GetFileNamesInDirectory(STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                 DYNAMIC_ARRAY<std::string> &outstrFileNames, bool inbAppendFullPath = false) const = 0;
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
            virtual void GetDirectoriesInDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                   DYNAMIC_ARRAY<std::string> &outstrDirectories, bool inbAppendFullPath = false) const = 0;
            //--------------------------------------------------------------
            /// Does File Exist
            ///
            /// returns whether or not the given file exists.
            /// 
            /// @param The Storage Location
            /// @param The filepath
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            virtual bool DoesFileExist(STORAGE_LOCATION ineStorageLocation, const std::string& instrFilepath) const = 0;
			//--------------------------------------------------------------
			/// Does File Exist In Cached DLC
			///
			/// @param The filepath.
			/// @return Whether or not it is in the cached DLC.
			//--------------------------------------------------------------
			virtual bool DoesFileExistInCachedDLC(const std::string& instrFilepath) const = 0;
			//--------------------------------------------------------------
			/// Does File Exist In Package DLC
			///
			/// @param The filepath.
			/// @return Whether or not it is in the local DLC.
			//--------------------------------------------------------------
			virtual bool DoesFileExistInPackageDLC(const std::string& instrFilepath) const = 0;
            //--------------------------------------------------------------
            /// Does Directory Exist
            ///
            /// returns whether or not the given directory exists.
            /// 
            /// @param The Storage Location
            /// @param The directory
            /// @return Whether or not it exists.
            //--------------------------------------------------------------
            virtual bool DoesDirectoryExist(STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory) const = 0;
            //--------------------------------------------------------------
			/// Is Storage Location Available
			///
            /// @param The source storage location.
			/// @return whether or not the storage location is available on
            ///         this device.
			//--------------------------------------------------------------
			virtual bool IsStorageLocationAvailable(STORAGE_LOCATION ineStorageLocation) const = 0;
			//--------------------------------------------------------------
			/// Get Storage Location Directory
			///
			/// @param The source storage location.
			/// @return The directory. returns an empty string if the location
			///         is not available.
			//--------------------------------------------------------------
			virtual std::string GetStorageLocationDirectory(STORAGE_LOCATION ineStorageLocation) const = 0;
            //--------------------------------------------------------------
			/// Get Directory For DLC File
			///
            /// @param The filename of the DLC asset.
			/// @return The directory to either the package DLC or cache DLC.
			//--------------------------------------------------------------
			virtual std::string GetDirectoryForDLCFile(const std::string& instrFilePath) const = 0;
            //--------------------------------------------------------------
			/// Get Directory For Package File
			///
            /// @param The filename of the package asset.
			/// @return The directory to either the correct device package directory.
			//--------------------------------------------------------------
			virtual std::string GetDirectoryForPackageFile(const std::string& instrFilePath) const = 0;
            //--------------------------------------------------------------
			/// Is Storage Location Writeable
			///
            /// @param The source storage location.
			/// @return whether or not the storage location can be written to
			//--------------------------------------------------------------
			bool IsStorageLocationWritable(STORAGE_LOCATION ineSourceStorageLocation) const;
            //--------------------------------------------------------------
			/// Set Package DLC Directory
			///
            /// Sets the directory used by the DLC system to fall back on
            /// if it is not in the DLC cache directory.
            ///
            /// @param The package DLC directory.
			//--------------------------------------------------------------
			void SetPackageDLCDirectory(const std::string&  instrDirectory);
            //--------------------------------------------------------------
            /// Get File MD5 Checksum 
            ///
            /// Calculate the MD5 checksum of the file at the
            /// give directory
            ///
            /// @param Storage location
            /// @param File path
            /// @return MD5 checksum
            //--------------------------------------------------------------
            std::string GetFileMD5Checksum(STORAGE_LOCATION ineLocation, const std::string& instrFilePath) const;
            //--------------------------------------------------------------
            /// Get Directory MD5 Checksum 
            ///
            /// Calculate the MD5 checksum of the given directory
            ///
            /// @param Storage location
            /// @param File path
            /// @return MD5 checksum
            //--------------------------------------------------------------
            std::string GetDirectoryMD5Checksum(STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory) const;
            //--------------------------------------------------------------
			/// Get Package DLC Directory
			///
            /// @return The package DLC directory.
			//--------------------------------------------------------------
			const std::string& GetPackageDLCDirectory() const;
            //--------------------------------------------------------------
			/// Get File CRC32 Checksum
			///
            /// Gets the CRC32 Checksum for the given file.
            ///
            /// @param The storage location.
			/// @param the filepath.
			/// @return the checksum.
			//--------------------------------------------------------------
			u32 GetFileCRC32Checksum(STORAGE_LOCATION ineStorageLocation, const std::string&  instrFilepath) const;
            //--------------------------------------------------------------
			/// Get Directory CRC32 Checksum
			///
            /// Gets the CRC32 Checksum for the given directory.
            ///
            /// @param The storage location.
			/// @param the filepath.
			/// @return the checksum.
			//--------------------------------------------------------------
			u32 GetDirectoryCRC32Checksum(STORAGE_LOCATION ineStorageLocation, const std::string&  instrDirectory) const;
	        //--------------------------------------------------------------
			/// Get File length
            /// @param The storage location.
			/// @param the filepath.
			//--------------------------------------------------------------
			u32 GetFileSize(STORAGE_LOCATION ineStorageLocation, const std::string&  instrFilepath) const;
	        //--------------------------------------------------------------
			/// Get Total File size of all Files in Directory
            /// @param The storage location.
			/// @param the Directory path.
			//--------------------------------------------------------------
			u32 GetDirectorySize(STORAGE_LOCATION ineStorageLocation, const std::string&  instrDirectory) const;
            //--------------------------------------------------------------
            /// Get Best Path To File
            ///
            /// @param Storage location
            /// @param File name
            /// @param Out: The path to the most up to date file with the
            /// given name. This argument is unchanged if file is not found
            //--------------------------------------------------------------
            void GetBestPathToFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFileName, std::string& outFilePath) const;

        protected:
            
            std::string mstrPackageDLCPath;
            
            static f32 mfAssetsDensity;
            static std::string mastrResourceDirectory[3];
		};
	}
		
}

#endif
