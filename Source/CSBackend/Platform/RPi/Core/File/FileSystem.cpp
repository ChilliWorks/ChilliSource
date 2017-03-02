//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#include <CSBackend/Platform/RPi/Core/File/FileSystem.h>

#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <iostream>
#include <shlobj.h>
#include <stack>

namespace CSBackend
{
	namespace RPi
	{
		namespace
		{
			const std::string k_saveDataPath = "SaveData/";
			const std::string k_cachePath = "Cache/";
			const std::string k_dlcPath = "DLC/";
		}

		CS_DEFINE_NAMEDTYPE(FileSystem);

		//---------------------------------------------------------------------------------
		FileSystem::FileSystem()
		{
			// wchar_t pathChars[MAX_PATH];
			// GetModuleFileName(nullptr, pathChars, MAX_PATH);
			// std::string path = WindowsStringUtils::ConvertWindowsFilePathToStandard(std::wstring(pathChars));
			// std::string::size_type pos = path.find_last_of("/");
			// std::string strWorkingDir = ChilliSource::StringUtils::StandardiseDirectoryPath(path.substr(0, pos));
			//
			// m_packagePath = strWorkingDir + "assets/";
			// m_documentsPath = strWorkingDir + "Documents/";
			//
			// CSBackend::Windows::CreateDirectory(m_documentsPath);
			// CS_ASSERT(CSBackend::Windows::DoesDirectoryExist(m_documentsPath), "Could not create Documents directory.");
			//
			// CSBackend::Windows::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_saveData));
			// CS_ASSERT(CSBackend::Windows::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_saveData)), "Could not create SaveData storage location.");
			//
			// CSBackend::Windows::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_cache));
			// CS_ASSERT(CSBackend::Windows::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_cache)), "Could not create Cache storage location.");
			//
			// CSBackend::Windows::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC));
			// CS_ASSERT(CSBackend::Windows::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC)), "Could not create DLC storage location.");
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::IsA(ChilliSource::InterfaceIDType interfaceId) const
		{
			return (ChilliSource::FileSystem::InterfaceID == interfaceId || FileSystem::InterfaceID == interfaceId);
		}

		//---------------------------------------------------------------------------------
        ChilliSource::ITextInputStreamUPtr FileSystem::CreateTextInputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const
        {
        	// std::string absFilePath = "";
			// if (in_storageLocation == ChilliSource::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_filePath) == false)
			// {
			// 	absFilePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + in_filePath;
			// }
			// else
			// {
			// 	absFilePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
			// }
			//
			// ChilliSource::ITextInputStreamUPtr output(new ChilliSource::TextInputStream(absFilePath));
            // if (output->IsValid() == true)
            // {
            //     return output;
            // }
            // else
            // {
                 return nullptr;
            // }
        }

		//---------------------------------------------------------------------------------
		ChilliSource::IBinaryInputStreamUPtr FileSystem::CreateBinaryInputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const
		{
			// std::string absFilePath = "";
			// if (in_storageLocation == ChilliSource::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_filePath) == false)
			// {
			// 	absFilePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + in_filePath;
			// }
			// else
			// {
			// 	absFilePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
			// }
			//
			// ChilliSource::IBinaryInputStreamUPtr output(new ChilliSource::BinaryInputStream(absFilePath));
			// if (output->IsValid() == true)
			// {
			// 	return output;
			// }
			// else
			// {
			 	return nullptr;
			// }
		}

		//---------------------------------------------------------------------------------
		ChilliSource::TextOutputStreamUPtr FileSystem::CreateTextOutputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath, ChilliSource::FileWriteMode fileMode) const
		{
			// CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");
			//
			// if (IsStorageLocationWritable(in_storageLocation))
			// {
			// 	std::string absFilePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
			//
			// 	ChilliSource::TextOutputStreamUPtr output(new ChilliSource::TextOutputStream(absFilePath, in_fileMode));
			// 	if (output->IsValid() == true)
			// 	{
			// 		return output;
			// 	}
			// }

			return nullptr;
		}

		//---------------------------------------------------------------------------------
		ChilliSource::BinaryOutputStreamUPtr FileSystem::CreateBinaryOutputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath, ChilliSource::FileWriteMode fileMode) const
		{
			// CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");
			//
			// if (IsStorageLocationWritable(in_storageLocation))
			// {
			// 	std::string absFilePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
			//
			// 	ChilliSource::BinaryOutputStreamUPtr output(new ChilliSource::BinaryOutputStream(absFilePath, in_fileMode));
			// 	if (output->IsValid() == true)
			// 	{
			// 		return output;
			// 	}
			// }

			return nullptr;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::CreateDirectoryPath(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath) const
		{
			// CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");
			//
			// //get each level of the new directory separately and then iterate though each creating it if it does not already exist.
			// auto currentDirectoryPath = GetAbsolutePathToStorageLocation(in_storageLocation);
			// auto relativePathSections = ChilliSource::StringUtils::Split(ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath), "/");
			//
			// for (const auto& relativePathSection : relativePathSections)
			// {
			// 	currentDirectoryPath += ChilliSource::StringUtils::StandardiseDirectoryPath(relativePathSection);
			// 	if (!CSBackend::Windows::DoesDirectoryExist(currentDirectoryPath))
			// 	{
			// 		if (!CSBackend::Windows::CreateDirectory(currentDirectoryPath))
			// 		{
			// 			CS_LOG_ERROR("File System: Failed to create directory '" + in_directoryPath + "'");
			// 			return false;
			// 		}
			// 	}
			// }

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::CopyFile(ChilliSource::StorageLocation sourceLocation, const std::string& sourceFilePath, ChilliSource::StorageLocation destLocation, const std::string& destFilePath) const
		{
			// CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");
            //
			// std::string sourceFilePath;
			// if (in_sourceStorageLocation == ChilliSource::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_sourceFilePath) == false)
			// {
			// 	sourceFilePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + ChilliSource::StringUtils::StandardiseFilePath(in_sourceFilePath);
			// }
			// else
			// {
			// 	sourceFilePath = GetAbsolutePathToStorageLocation(in_sourceStorageLocation) + ChilliSource::StringUtils::StandardiseFilePath(in_sourceFilePath);
			// }
			//
			// if (CSBackend::Windows::DoesFileExist(sourceFilePath) == false)
			// {
			// 	CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
			// 	return false;
			// }
			//
            // //get the path to the file
			// std::string destinationFileName, destinationDirectoryPath;
			// ChilliSource::StringUtils::SplitFilename(in_destinationFilePath, destinationFileName, destinationDirectoryPath);
            //
            // //create the output directory
			// CreateDirectoryPath(in_destinationStorageLocation, destinationDirectoryPath);
            //
            // //try and copy the files
			// std::wstring sourceWPath = WindowsStringUtils::ConvertStandardPathToWindows(sourceFilePath);
			// std::wstring destWPath = WindowsStringUtils::ConvertStandardPathToWindows(GetAbsolutePathToStorageLocation(in_destinationStorageLocation) + in_destinationFilePath);
			// if (WindowsFileUtils::WindowsCopyFile(sourceWPath.c_str(), destWPath.c_str(), FALSE) == FALSE)
			// {
			// 	CS_LOG_ERROR("File System: Failed to copy file '" + in_sourceFilePath + "'");
			// 	return false;
			// }

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::CopyDirectory(ChilliSource::StorageLocation sourceLocation, const std::string& sourceDirPath, ChilliSource::StorageLocation destLocation, const std::string& destDirPath) const
		{
			// CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");
			//
			// if (DoesDirectoryExist(in_sourceStorageLocation, in_sourceDirectoryPath) == false)
			// {
			// 	CS_LOG_ERROR("File System: Trying to copy directory '" + in_sourceDirectoryPath + "' but it doesn't exist.");
			// 	return false;
			// }
			//
			// std::vector<std::string> filePaths = GetFilePaths(in_sourceStorageLocation, in_sourceDirectoryPath, true);
			//
			// //if the source directory is empty, just create the equivelent directory in the destination
			// if (filePaths.size() == 0)
			// {
			// 	CreateDirectoryPath(in_destinationStorageLocation, in_destinationDirectoryPath);
			// }
			// else
			// {
			// 	std::string sourceDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_sourceDirectoryPath);
			// 	std::string destinationDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_destinationDirectoryPath);
			// 	for (const std::string& filePath : filePaths)
			// 	{
			// 		if (CopyFile(in_sourceStorageLocation, sourceDirectoryPath + filePath, in_destinationStorageLocation, destinationDirectoryPath + filePath) == false)
			// 		{
			// 			CS_LOG_ERROR("File System: Failed to copy directory '" + in_sourceDirectoryPath + "'");
			// 			return false;
			// 		}
			// 	}
			// }

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DeleteFile(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const
		{
			// CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");
			//
			// std::wstring filePath = WindowsStringUtils::ConvertStandardPathToWindows(GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath);
			// if (WindowsFileUtils::WindowsDeleteFile(filePath.c_str()) == FALSE)
			// {
			// 	return false;
			// }

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DeleteDirectory(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath) const
		{
			// CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");
			//
			// std::string directoryPath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
			// if (CSBackend::Windows::DeleteDirectory(directoryPath) == false)
			// {
			// 	return false;
			// }

			return true;
		}

		//---------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetFilePaths(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath, bool recursive) const
		{
			//std::vector<std::string> possibleDirectories = GetPossibleAbsoluteDirectoryPaths(in_storageLocation, in_directoryPath);

			std::vector<std::string> output;
			// std::vector<std::string> filePaths;
			// std::vector<std::string> directoryPaths;
			// for (const std::string& possibleDirectory : possibleDirectories)
			// {
			// 	filePaths.clear();
			// 	directoryPaths.clear();
			//
			// 	ListDirectoryContents(possibleDirectory, in_recursive, directoryPaths, filePaths);
			// 	output.insert(output.end(), filePaths.begin(), filePaths.end());
			// }
			//
			// std::sort(output.begin(), output.end());
			// std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
			// output.resize(it - output.begin());
			return output;
		}

		//---------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetDirectoryPaths(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath, bool recursive) const
		{
			//std::vector<std::string> possibleDirectories = GetPossibleAbsoluteDirectoryPaths(in_storageLocation, in_directoryPath);

			std::vector<std::string> output;
			// std::vector<std::string> filePaths;
			// std::vector<std::string> directoryPaths;
			// for (const std::string& possibleDirectory : possibleDirectories)
			// {
			// 	filePaths.clear();
			// 	directoryPaths.clear();
			//
			// 	ListDirectoryContents(possibleDirectory, in_recursive, directoryPaths, filePaths);
			// 	output.insert(output.end(), directoryPaths.begin(), directoryPaths.end());
			// }
			//
			// std::sort(output.begin(), output.end());
			// std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
			// output.resize(it - output.begin());
			return output;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesFileExist(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const
		{
			// switch (in_storageLocation)
			// {
			// 	case ChilliSource::StorageLocation::k_DLC:
			// 	{
			// 		if (DoesItemExistInDLCCache(in_filePath, false) == true)
			// 		{
			// 			return true;
			// 		}
			//
			// 		return DoesFileExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
			// 	}
			// 	default:
			// 	{
			// 		std::string path = ChilliSource::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath);
			// 		return CSBackend::Windows::DoesFileExist(path);
			// 	}
			// }

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesFileExistInCachedDLC(const std::string& filePath) const
		{
			return true;//DoesItemExistInDLCCache(in_filePath, false);
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesFileExistInPackageDLC(const std::string& filePath) const
		{
			return true;//DoesFileExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExist(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath) const
		{
			// switch (in_storageLocation)
			// {
			// 	case ChilliSource::StorageLocation::k_DLC:
			// 	{
			// 		if (DoesItemExistInDLCCache(in_directoryPath, true) == true)
			// 		{
			// 			return true;
			// 		}
			//
			// 		return DoesDirectoryExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
			// 	}
			// 	default:
			// 	{
			// 		std::string path = ChilliSource::StringUtils::StandardiseDirectoryPath(GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath);
			// 		return CSBackend::Windows::DoesDirectoryExist(path);
			// 	}
			// }

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInCachedDLC(const std::string& directoryPath) const
		{
			return true;//DoesItemExistInDLCCache(in_directoryPath, true);
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInPackageDLC(const std::string& directoryPath) const
		{
			return true;//DoesDirectoryExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
		}

		//---------------------------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation storageLocation) const
		{
			switch (storageLocation)
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

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesItemExistInDLCCache(const std::string& path, bool isDirectory) const
		{
			// std::string path = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + in_path;
			// if (in_isDirectory == true)
			// {
			// 	return CSBackend::Windows::DoesDirectoryExist(ChilliSource::StringUtils::StandardiseDirectoryPath(path));
			// }
			// else
			// {
			// 	return CSBackend::Windows::DoesFileExist(ChilliSource::StringUtils::StandardiseFilePath(path));
			// }
			return true;
		}

		//---------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetPossibleAbsoluteDirectoryPaths(ChilliSource::StorageLocation storageLocation, const std::string& path) const
		{
			std::vector<std::string> output;

			// switch (in_storageLocation)
			// {
			// 	case ChilliSource::StorageLocation::k_DLC:
			// 	{
			// 		output.push_back(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + in_path);
			// 		output.push_back(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + in_path);
			// 		break;
			// 	}
			// 	default:
			// 	{
			// 		output.push_back(GetAbsolutePathToStorageLocation(in_storageLocation) + in_path);
			// 		break;
			// 	}
			// }

			return output;
		}
	}
}

#endif
