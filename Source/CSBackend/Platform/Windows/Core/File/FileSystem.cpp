//
//  FileSystem.cpp
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

#include <CSBackend/Platform/Windows/Core/File/FileSystem.h>

#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <iostream>
#include <shlobj.h>
#include <stack>

//This includes windows so needs to come last, else it might cause problems with other includes
#include <CSBackend/Platform/Windows/Core/File/WindowsFileUtils.h>

//Undefine the windows file system functions that share names with ours.
#undef CopyFile
#undef DeleteFile
#undef CreateDirectory

namespace CSBackend
{
	namespace Windows 
	{
		namespace
		{
			const std::string k_saveDataPath = "SaveData/";
			const std::string k_cachePath = "Cache/";
			const std::string k_dlcPath = "DLC/";

			//--------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return whether or not the given file mode is a write mode
			//--------------------------------------------------------------
			bool IsWriteMode(ChilliSource::FileMode in_fileMode)
			{
				switch (in_fileMode)
				{
				case ChilliSource::FileMode::k_write:
				case ChilliSource::FileMode::k_writeAppend:
				case ChilliSource::FileMode::k_writeAtEnd:
				case ChilliSource::FileMode::k_writeBinary:
				case ChilliSource::FileMode::k_writeBinaryAppend:
				case ChilliSource::FileMode::k_writeBinaryAtEnd:
				case ChilliSource::FileMode::k_writeBinaryTruncate:
				case ChilliSource::FileMode::k_writeTruncate:
					return true;
				default:
					return false;

				}
			}
			//--------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The file path.
			///
			/// @return Whether or not the given file path exists.
			//--------------------------------------------------------------
			bool DoesFileExist(const std::string& in_filePath)
			{
				std::wstring filePath = WindowsStringUtils::ConvertStandardPathToWindows(in_filePath);
				DWORD attributes = GetFileAttributes(filePath.c_str());

				return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
			}
			//--------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The directory path.
			///
			/// @return Whether or not the given directory path exists.
			//--------------------------------------------------------------
			bool DoesDirectoryExist(const std::string& in_directoryPath)
			{
				std::wstring directoryPath = WindowsStringUtils::ConvertStandardPathToWindows(in_directoryPath);
				DWORD attributes = GetFileAttributes(directoryPath.c_str());

				return (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY));
			}
			//--------------------------------------------------------------
			/// Creates a new directory at the given path. This will not
			/// create intermediate directories.
			///
			/// @author Ian Copland
			///
			/// @param The directory path.
			///
			/// @return Whether or not the directory was created. Returns
			/// false if the directory already existed.
			//--------------------------------------------------------------
			bool CreateDirectory(const std::string& in_directoryPath)
			{
				auto windowsDirectoryPath = WindowsStringUtils::ConvertStandardPathToWindows(in_directoryPath);
				return (WindowsFileUtils::WindowsCreateDirectory(windowsDirectoryPath.c_str(), NULL) == TRUE);
			}
			//--------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The directory path.
			///
			/// @return Whether or not the given directory path exists.
			//--------------------------------------------------------------
			bool DeleteDirectory(const std::string& in_directoryPath)
			{
				std::wstring directoryQuery = WindowsStringUtils::ConvertStandardPathToWindows(in_directoryPath) + L"\\*";

				WIN32_FIND_DATA fileData;
				HANDLE fileHandle = WindowsFileUtils::WindowsFindFirstFile(directoryQuery.c_str(), &fileData);
				if (fileHandle == INVALID_HANDLE_VALUE)
				{
					return false;
				}

				do
				{
					if (wcscmp(fileData.cFileName, L".") != 0 && wcscmp(fileData.cFileName, L"..") != 0)
					{
						if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							std::string directoryName = WindowsStringUtils::UTF16ToUTF8(fileData.cFileName);
							
							if (DeleteDirectory(ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath + directoryName)) == false)
							{
								return false;
							}
						}
						else
						{
							std::string fileName = WindowsStringUtils::UTF16ToUTF8(fileData.cFileName);
							std::string filePath = ChilliSource::StringUtils::StandardiseFilePath(in_directoryPath + fileName);
							if (WindowsFileUtils::WindowsDeleteFile(WindowsStringUtils::ConvertStandardPathToWindows(filePath).c_str()) == FALSE)
							{
								return false;
							}
						}
					}
				} while (WindowsFileUtils::WindowsFindNextFile(fileHandle, &fileData) == TRUE);

				bool success = (GetLastError() == ERROR_NO_MORE_FILES);
				FindClose(fileHandle);

				if (success == false)
				{
					return false;
				}

				if (WindowsFileUtils::WindowsRemoveDirectory(WindowsStringUtils::ConvertStandardPathToWindows(in_directoryPath).c_str()) == FALSE)
				{
					return false;
				}

				return true;
			}
			//--------------------------------------------------------------
			/// Lists all files and sub-directories inside the given directory.
			/// All paths will be relative to the given directory.
			///
			/// @author Ian Copland
			///
			/// @param The directory.
			/// @param Whether or not to recurse into sub directories.
			/// @param [Out] The sub directories.
			/// @param [Out] The files.
			/// @param [Optional] The relative directory path. This is used
			/// in recursion and shouldn't be set outside of this function.
			/// @return Whether or not this succeeded.
			//--------------------------------------------------------------
			bool ListDirectoryContents(const std::string& in_directoryPath, bool in_recursive, std::vector<std::string>& out_directoryPaths, std::vector<std::string>& out_filePaths, 
				const std::string& in_relativeDirectoryPath = "")
			{
				std::wstring directoryQuery = WindowsStringUtils::ConvertStandardPathToWindows(in_directoryPath) + L"\\*";

				WIN32_FIND_DATA fileData;
				HANDLE fileHandle = WindowsFileUtils::WindowsFindFirstFile(directoryQuery.c_str(), &fileData);
				if (fileHandle == INVALID_HANDLE_VALUE)
				{
					return false;
				}

				do
				{
					if (wcscmp(fileData.cFileName, L".") != 0 && wcscmp(fileData.cFileName, L"..") != 0)
					{
						if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							std::string directoryName = WindowsStringUtils::UTF16ToUTF8(fileData.cFileName);
							std::string relativeDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_relativeDirectoryPath + directoryName);
							out_directoryPaths.push_back(relativeDirectoryPath);

							if (in_recursive == true)
							{
								std::string absoluteDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath + directoryName);
								if (ListDirectoryContents(absoluteDirectoryPath, true, out_directoryPaths, out_filePaths, relativeDirectoryPath) == false)
								{
									return false;
								}
							}
						}
						else
						{
							std::string fileName = WindowsStringUtils::UTF16ToUTF8(fileData.cFileName);
							std::string relativeFilePath = ChilliSource::StringUtils::StandardiseFilePath(in_relativeDirectoryPath + fileName);
							out_filePaths.push_back(relativeFilePath);
						}
					}
				} 
				while (WindowsFileUtils::WindowsFindNextFile(fileHandle, &fileData) == TRUE);

				bool success = (GetLastError() == ERROR_NO_MORE_FILES);
				FindClose(fileHandle);

				return success;
			}
		}
		CS_DEFINE_NAMEDTYPE(FileSystem);
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		FileSystem::FileSystem()
		{
			wchar_t pathChars[MAX_PATH];
			GetModuleFileName(nullptr, pathChars, MAX_PATH);
			std::string path = WindowsStringUtils::ConvertWindowsFilePathToStandard(std::wstring(pathChars));
			std::string::size_type pos = path.find_last_of("/");
			std::string strWorkingDir = ChilliSource::StringUtils::StandardiseDirectoryPath(path.substr(0, pos));

			m_packagePath = strWorkingDir + "assets/";
			m_documentsPath = strWorkingDir + "Documents/";

			CSBackend::Windows::CreateDirectory(m_documentsPath);
			CS_ASSERT(CSBackend::Windows::DoesDirectoryExist(m_documentsPath), "Could not create Documents directory.");

			CSBackend::Windows::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_saveData));
			CS_ASSERT(CSBackend::Windows::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_saveData)), "Could not create SaveData storage location.");

			CSBackend::Windows::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_cache));
			CS_ASSERT(CSBackend::Windows::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_cache)), "Could not create Cache storage location.");

			CSBackend::Windows::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC));
			CS_ASSERT(CSBackend::Windows::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC)), "Could not create DLC storage location.");
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		bool FileSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (ChilliSource::FileSystem::InterfaceID == in_interfaceId || FileSystem::InterfaceID == in_interfaceId);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		ChilliSource::FileStreamUPtr FileSystem::CreateFileStream(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, ChilliSource::FileMode in_fileMode) const
		{
			if (IsWriteMode(in_fileMode) == true)
			{
				CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");
			}

			if (in_storageLocation == ChilliSource::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_filePath) == false && IsWriteMode(in_fileMode) == false)
			{
				auto absFilePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + in_filePath;
				ChilliSource::FileStreamUPtr output(new ChilliSource::FileStream(absFilePath, in_fileMode));
				if (output->IsValid() == true)
				{
					return output;
				}
			}
			else
			{
				auto absFilePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
				ChilliSource::FileStreamUPtr output(new ChilliSource::FileStream(absFilePath, in_fileMode));
				if (output->IsValid() == true)
				{
					return output;
				}
			}

			return nullptr;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CreateDirectoryPath(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");

			//get each level of the new directory separately and then iterate though each creating it if it does not already exist.
			auto currentDirectoryPath = GetAbsolutePathToStorageLocation(in_storageLocation);
			auto relativePathSections = ChilliSource::StringUtils::Split(ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath), "/");

			for (const auto& relativePathSection : relativePathSections)
			{
				currentDirectoryPath += ChilliSource::StringUtils::StandardiseDirectoryPath(relativePathSection);
				if (!CSBackend::Windows::DoesDirectoryExist(currentDirectoryPath))
				{
					if (!CSBackend::Windows::CreateDirectory(currentDirectoryPath))
					{
						CS_LOG_ERROR("File System: Failed to create directory '" + in_directoryPath + "'");
						return false;
					}
				}
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyFile(ChilliSource::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath, 
			ChilliSource::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");
            
			std::string sourceFilePath;
			if (in_sourceStorageLocation == ChilliSource::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_sourceFilePath) == false)
			{
				sourceFilePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + ChilliSource::StringUtils::StandardiseFilePath(in_sourceFilePath);
			}
			else
			{
				sourceFilePath = GetAbsolutePathToStorageLocation(in_sourceStorageLocation) + ChilliSource::StringUtils::StandardiseFilePath(in_sourceFilePath);
			}

			if (CSBackend::Windows::DoesFileExist(sourceFilePath) == false)
			{
				CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
				return false;
			}

            //get the path to the file
			std::string destinationFileName, destinationDirectoryPath;
			ChilliSource::StringUtils::SplitFilename(in_destinationFilePath, destinationFileName, destinationDirectoryPath);
            
            //create the output directory
			CreateDirectoryPath(in_destinationStorageLocation, destinationDirectoryPath);
            
            //try and copy the files
			std::wstring sourceWPath = WindowsStringUtils::ConvertStandardPathToWindows(sourceFilePath);
			std::wstring destWPath = WindowsStringUtils::ConvertStandardPathToWindows(GetAbsolutePathToStorageLocation(in_destinationStorageLocation) + in_destinationFilePath);
			if (WindowsFileUtils::WindowsCopyFile(sourceWPath.c_str(), destWPath.c_str(), FALSE) == FALSE)
			{
				CS_LOG_ERROR("File System: Failed to copy file '" + in_sourceFilePath + "'");
				return false;
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyDirectory(ChilliSource::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath, 
			ChilliSource::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");

			if (DoesDirectoryExist(in_sourceStorageLocation, in_sourceDirectoryPath) == false)
			{
				CS_LOG_ERROR("File System: Trying to copy directory '" + in_sourceDirectoryPath + "' but it doesn't exist.");
				return false;
			}

			std::vector<std::string> filePaths = GetFilePaths(in_sourceStorageLocation, in_sourceDirectoryPath, true);

			//if the source directory is empty, just create the equivelent directory in the destination
			if (filePaths.size() == 0)
			{
				CreateDirectoryPath(in_destinationStorageLocation, in_destinationDirectoryPath);
			}
			else
			{
				std::string sourceDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_sourceDirectoryPath);
				std::string destinationDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_destinationDirectoryPath);
				for (const std::string& filePath : filePaths)
				{
					if (CopyFile(in_sourceStorageLocation, sourceDirectoryPath + filePath, in_destinationStorageLocation, destinationDirectoryPath + filePath) == false)
					{
						CS_LOG_ERROR("File System: Failed to copy directory '" + in_sourceDirectoryPath + "'");
						return false;
					}
				}
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DeleteFile(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");

			std::wstring filePath = WindowsStringUtils::ConvertStandardPathToWindows(GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath);
			if (WindowsFileUtils::WindowsDeleteFile(filePath.c_str()) == FALSE)
			{
				return false;
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DeleteDirectory(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");

			std::string directoryPath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
			if (CSBackend::Windows::DeleteDirectory(directoryPath) == false)
			{
				return false;
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetFilePaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
		{
			std::vector<std::string> possibleDirectories = GetPossibleAbsoluteDirectoryPaths(in_storageLocation, in_directoryPath);
            
			std::vector<std::string> output;
			std::vector<std::string> filePaths;
			std::vector<std::string> directoryPaths;
			for (const std::string& possibleDirectory : possibleDirectories)
			{
				filePaths.clear();
				directoryPaths.clear();

				ListDirectoryContents(possibleDirectory, in_recursive, directoryPaths, filePaths);
				output.insert(output.end(), filePaths.begin(), filePaths.end());
			}

			std::sort(output.begin(), output.end());
			std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
			output.resize(it - output.begin());
			return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetDirectoryPaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
		{
			std::vector<std::string> possibleDirectories = GetPossibleAbsoluteDirectoryPaths(in_storageLocation, in_directoryPath);

			std::vector<std::string> output;
			std::vector<std::string> filePaths;
			std::vector<std::string> directoryPaths;
			for (const std::string& possibleDirectory : possibleDirectories)
			{
				filePaths.clear();
				directoryPaths.clear();

				ListDirectoryContents(possibleDirectory, in_recursive, directoryPaths, filePaths);
				output.insert(output.end(), directoryPaths.begin(), directoryPaths.end());
			}

			std::sort(output.begin(), output.end());
			std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
			output.resize(it - output.begin());
			return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExist(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			switch (in_storageLocation)
			{
				case ChilliSource::StorageLocation::k_DLC:
				{
					if (DoesItemExistInDLCCache(in_filePath, false) == true)
					{
						return true;
					}

					return DoesFileExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
				}
				default:
				{
					std::string path = ChilliSource::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath);
					return CSBackend::Windows::DoesFileExist(path);
				}
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInCachedDLC(const std::string& in_filePath) const
		{
			return DoesItemExistInDLCCache(in_filePath, false);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInPackageDLC(const std::string& in_filePath) const
		{
			return DoesFileExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExist(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			switch (in_storageLocation)
			{
				case ChilliSource::StorageLocation::k_DLC:
				{
					if (DoesItemExistInDLCCache(in_directoryPath, true) == true)
					{
						return true;
					}

					return DoesDirectoryExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
				}
				default:
				{
					std::string path = ChilliSource::StringUtils::StandardiseDirectoryPath(GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath);
					return CSBackend::Windows::DoesDirectoryExist(path);
				}
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInCachedDLC(const std::string& in_directoryPath) const
		{
			return DoesItemExistInDLCCache(in_directoryPath, true);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInPackageDLC(const std::string& in_directoryPath) const
		{
			return DoesDirectoryExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation in_storageLocation) const
		{
			switch (in_storageLocation)
			{
			case ChilliSource::StorageLocation::k_package:
				return m_packagePath + "AppResources/";
			case ChilliSource::StorageLocation::k_chilliSource:
				return m_packagePath + "CSResources/";
			case ChilliSource::StorageLocation::k_saveData:
				return m_documentsPath + k_saveDataPath;
			case ChilliSource::StorageLocation::k_cache:
				return m_documentsPath + k_cachePath;
			case ChilliSource::StorageLocation::k_DLC:
				return m_documentsPath + k_dlcPath;
			case ChilliSource::StorageLocation::k_root:
				return "";
				break;
			default:
				CS_LOG_ERROR("Storage Location not available on this platform!");
				return "";
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesItemExistInDLCCache(const std::string& in_path, bool in_isDirectory) const
		{
			std::string path = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + in_path;
			if (in_isDirectory == true)
			{
				return CSBackend::Windows::DoesDirectoryExist(ChilliSource::StringUtils::StandardiseDirectoryPath(path));
			}
			else
			{
				return CSBackend::Windows::DoesFileExist(ChilliSource::StringUtils::StandardiseFilePath(path));
			}
		}
		//------------------------------------------------------------
		//------------------------------------------------------------
		std::vector<std::string> FileSystem::GetPossibleAbsoluteDirectoryPaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_path) const
		{
			std::vector<std::string> output;

			switch (in_storageLocation)
			{
				case ChilliSource::StorageLocation::k_DLC:
				{
					output.push_back(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + in_path);
					output.push_back(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + in_path);
					break;
				}
				default:
				{
					output.push_back(GetAbsolutePathToStorageLocation(in_storageLocation) + in_path);
					break;
				}
			}

			return output;
		}
	}
}

#endif
