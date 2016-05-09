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

#ifdef CS_TARGETPLATFORM_WINDOWS

#ifndef _CSBACKEND_WINDOWS_CORE_FILE_FILESYSTEM_H_
#define _CSBACKEND_WINDOWS_CORE_FILE_FILESYSTEM_H_

#include <CSBackend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/File/FileSystem.h>

#include <string>

namespace CSBackend
{
	namespace Windows 
	{
		//-----------------------------------------------------------------
		/// The windows backend for the File System.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------
		class FileSystem : public ChilliSource::FileSystem
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
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
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
			ChilliSource::FileStreamUPtr CreateFileStream(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, ChilliSource::FileMode in_fileMode) const override;
			//--------------------------------------------------------------
			/// Creates the given directory. The full directory hierarchy
			/// will be created.
			///
			/// @author Ian Copland
			///
			/// @param The Storage Location
			/// @param The directory path.
			///
            /// @return Whether or not this was successful. Failure to
            /// create the directory because it already exists is considered
            /// a success.
			//--------------------------------------------------------------
			bool CreateDirectoryPath(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
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
			bool CopyFile(ChilliSource::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
				ChilliSource::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const override;
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
			bool CopyDirectory(ChilliSource::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
				ChilliSource::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const override;
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
			bool DeleteFile(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filepath) const override;
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
			bool DeleteDirectory(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
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
			std::vector<std::string> GetFilePaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const override;
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
			std::vector<std::string> GetDirectoryPaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const override;
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
			bool DoesFileExist(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath) const override;
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
			bool DoesDirectoryExist(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const override;
			//--------------------------------------------------------------
			/// Returns whether or not the directory exists in the Cached
			/// DLC directory.
			///
			/// @author Ian Copland
			///
			/// @param in_directoryPath - The directory path.
			///
			/// @return Whether or not it is in the cached DLC.
			//--------------------------------------------------------------
			bool DoesDirectoryExistInCachedDLC(const std::string& in_directoryPath) const override;
			//--------------------------------------------------------------
			/// Returns whether or not the directory exists in the package
			/// DLC directory.
			///
			/// @author Ian Copland
			///
			/// @param in_directoryPath - The directory path.
			///
			/// @return Whether or not it is in the package DLC.
			//--------------------------------------------------------------
			bool DoesDirectoryExistInPackageDLC(const std::string& in_directoryPath) const override;
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
			std::string GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation in_storageLocation) const override;
            
		private:
			friend ChilliSource::FileSystemUPtr ChilliSource::FileSystem::Create();
			//--------------------------------------------------------------
			/// Private constructor to force use of the factory
			/// method.
			///
			/// @author Ian Copland
			//--------------------------------------------------------------
			FileSystem();
			//--------------------------------------------------------------
			/// Returns whether or not a file or directory exists specifically
			/// in the DCL cache.
			///
			/// @author Ian Copland
			///
			/// @param the filepath.
			///
			/// @return whether or not it exists.
			//--------------------------------------------------------------
			bool DoesItemExistInDLCCache(const std::string& in_path, bool in_isDirectory) const;
			//------------------------------------------------------------
			/// Builds a list of the paths that the given path might refer
			/// to in the given storage location. For example, a path in
			/// DLC might refer to the DLC cache or the Package DLC.
			///
			/// @author Ian Copland
			///
			/// @param Storage location
			/// @param File name to append
			///
			/// @return All the paths for the given location
			//------------------------------------------------------------
			std::vector<std::string> GetPossibleAbsoluteDirectoryPaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const;

			std::string m_packagePath;
			std::string m_documentsPath;
		};
	}
}

#endif

#endif
