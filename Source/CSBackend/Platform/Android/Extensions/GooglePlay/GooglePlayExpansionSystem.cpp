//
//  GooglePlayExpansionSystem.cpp
//  Chilli Source
//  Created by Hugh McLaughlin on 25/06/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY

#include <CSBackend/Platform/Android/Extensions/GooglePlay/GooglePlayExpansionSystem.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Extensions/GooglePlay/GooglePlayExpansionJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <json/json.h>
#include <minizip/unzip.h>

namespace CSBackend
{
    namespace Android
    {
    	namespace
    	{
			//----------------------------------------------------------------
    		/// @author S Downie
    		///
    		/// @param The path.
    		///
    		/// @return Whether or not this given path contains the directory
    		/// portion of a path.
			//----------------------------------------------------------------
			bool ContainsDirectoryPath(const std::string& in_path)
			{
				return (in_path.find_first_of("/") != std::string::npos);
			}
			//----------------------------------------------------------------
    		/// @author S Downie
    		///
    		/// @param The path.
    		///
    		/// @return Whether or not this given path points to a file.
			//----------------------------------------------------------------
			bool IsFile(const std::string& in_path)
			{
				return (in_path.find_first_of(".") != std::string::npos);
			}
			//----------------------------------------------------------------
    		/// @author S Downie
    		///
    		/// @param The path.
    		///
    		/// @return The given path minus the filename portion.
			//----------------------------------------------------------------
			std::string GetPathExcludingFileName(const std::string& in_path)
			{
				u32 udwOffset = in_path.find_last_of("/");
				if(udwOffset != std::string::npos)
				{
					return in_path.substr(0, udwOffset);
				}

				return "";
			}
			//----------------------------------------------------------------
    		/// @author S Downie
    		///
    		/// @param The path.
    		///
    		/// @return Just the file name part of the path.
			//----------------------------------------------------------------
			std::string GetFileNameExcludingPath(const std::string& in_path)
			{
				u32 udwOffset = in_path.find_last_of("/");
				if(udwOffset != std::string::npos)
				{
					if(udwOffset + 1 < in_path.size())
					{
						return in_path.substr(udwOffset+1, in_path.size());
					}
				}

				return in_path;
			}
			//----------------------------------------------------------------
    		/// @author S Downie
    		///
    		/// @param The path.
    		///
    		/// @return Just the root folder of the path.
			//----------------------------------------------------------------
			std::string GetRootFolderExcludingPath(const std::string& in_path)
			{
				u32 udwOffset = in_path.find_first_of("/", 1);
				if(udwOffset != std::string::npos)
				{
					return in_path.substr(0, udwOffset);
				}

				return "";
			}
    	}

    	CS_DEFINE_NAMEDTYPE(GooglePlayExpansionSystem);
    	//----------------------------------------------------------------
    	//----------------------------------------------------------------
    	GooglePlayExpansionSystemUPtr GooglePlayExpansionSystem::Create()
    	{
    		return GooglePlayExpansionSystemUPtr(new GooglePlayExpansionSystem());
    	}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
    	GooglePlayExpansionSystem::GooglePlayExpansionSystem()
    		: m_installProgress(0.0f), m_numExpansions(0), m_downloadStatusDelegate(nullptr)
    	{
    	}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        bool GooglePlayExpansionSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == GooglePlayExpansionSystem::InterfaceID;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        bool GooglePlayExpansionSystem::DoExpansionDownloadFilesExist() const
        {
        	for(u32 i=0; i<m_numExpansions; ++i)
        	{
        		if(!CSCore::Application::Get()->GetFileSystem()->DoesFileExist(CSCore::StorageLocation::k_root, m_javaInterface->GetExpansionPath(i)))
        		{
        			return false;
        		}
        	}

        	return true;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        bool GooglePlayExpansionSystem::HasEnoughStorageSpace(u64 in_required)
        {
			u64 uddwAvailableStorage = m_javaInterface->GetExternalFreeStorageInBytes();

			//I made this intentionally not >= do give some wiggle room
			return uddwAvailableStorage > in_required;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        bool GooglePlayExpansionSystem::IsDownloadRequired()
        {
        	//Check if the download files exist
			bool bZippedFilesExist = DoExpansionDownloadFilesExist();

			if(!bZippedFilesExist)
			{
				return true;
			}

			//Check if the installed files exist
			bool bInstalledFilesExist = DoInstalledFilesExist();

			if(!bInstalledFilesExist)
			{
				return true;
			}

			//Check if they are the latest version
			bool bLatestVersion = false;

			Json::Value jDesc;
			if(CSCore::JsonUtils::ReadJson(CSCore::StorageLocation::k_cache, "GoogleExpansionDownloader.cache", jDesc) == true)
			{
				bLatestVersion = true;

				for(u32 i=0; i<jDesc.size(); ++i)
				{
					Json::Value jExpansion = jDesc[i];
					if(jExpansion["VersionCode"].asUInt() < m_javaInterface->GetExpansionVersionCode(i))
					{
						bLatestVersion = false;
						break;
					}
				}
			}

			if(!bLatestVersion)
			{
				return true;
			}

			return false;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::Download(const DownloadStatusDelegate& in_delegate)
        {
        	m_downloadStatusDelegate = in_delegate;

        	if(!IsDownloadRequired())
        	{
        		if(m_downloadStatusDelegate)
        		{
        			m_downloadStatusDelegate(DownloadStatus::k_complete);
        		}
        		return;
        	}
        	// If Downloaded obb exists (i.e. is downloaded or copied locally)
        	// but not installed, install them
			else if(DoExpansionDownloadFilesExist())
			{
				OnDownloadStatusChanged(DownloadStatus::k_complete);
				return;
			}

        	CSCore::Application::Get()->GetFileSystem()->DeleteFile(CSCore::StorageLocation::k_cache, "GoogleExpansionDownloader.cache");
        	m_javaInterface->Download();
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::Pause()
        {
        	m_javaInterface->PauseDownload();
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::Resume()
        {
        	m_javaInterface->ResumeDownload();
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        f32 GooglePlayExpansionSystem::GetDownloadProgress()
        {
			return m_javaInterface->GetDownloadProgress();
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        f32 GooglePlayExpansionSystem::GetInstallProgress() const
        {
			return m_installProgress;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        bool GooglePlayExpansionSystem::DoInstalledFilesExist() const
        {
        	return CSCore::Application::Get()->GetFileSystem()->DoesFileExist(CSCore::StorageLocation::k_cache, "GoogleExpansionDownloader.cache");
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        u64 GooglePlayExpansionSystem::GetUncompressedZipSize(const std::string& in_filePath) const
        {
        	unzFile ZippedFile = unzOpen(in_filePath.c_str());
        	s32 dwStatus = unzGoToFirstFile(ZippedFile);

			//Go to the first file in the zip
			const u64 uddwFilenameLength = 256;
			s8 byaFileName[uddwFilenameLength];

			u64 uddwTotalSize = 0;

			while(dwStatus == UNZ_OK)
			{
				//Open the next file
				if (unzOpenCurrentFile(ZippedFile) != UNZ_OK)
					break;

				//Get file information
				unz_file_info FileInfo;
				unzGetCurrentFileInfo(ZippedFile, &FileInfo, byaFileName, uddwFilenameLength, nullptr, 0, nullptr, 0);
				uddwTotalSize += FileInfo.uncompressed_size;

				//Close current file and jump to the next
				unzCloseCurrentFile(ZippedFile);
				dwStatus = unzGoToNextFile(ZippedFile);
			}

			//Close the zip
			unzClose(ZippedFile);

			return uddwTotalSize;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        u64 GooglePlayExpansionSystem::GetRequiredDownloadSpaceInBytes()
        {
        	u32 kudwSafetyPaddingInBytes = 256;
        	u64 uddwFileSize = kudwSafetyPaddingInBytes;
        	for(u32 i=0; i<m_numExpansions; ++i)
        	{
        		uddwFileSize += m_javaInterface->GetExpansionFileSizeInBytes(i);
        	}

        	return uddwFileSize;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        u64 GooglePlayExpansionSystem::GetRequiredInstallSpaceInBytes()
        {
        	u32 kudwSafetyPaddingInBytes = 256;
        	u64 uddwFileSize = kudwSafetyPaddingInBytes;

        	for(u32 i=0; i<m_numExpansions; ++i)
        	{
        		uddwFileSize += GetUncompressedZipSize(m_javaInterface->GetExpansionPath(i));
        	}

        	return uddwFileSize;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        u64 GooglePlayExpansionSystem::GetRequiredStorageSpaceInBytes()
        {
        	return GetRequiredInstallSpaceInBytes() + GetRequiredDownloadSpaceInBytes();
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::OnInit()
        {
        	m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<GooglePlayExpansionJavaInterface>();
        	if (m_javaInterface == nullptr)
        	{
        		m_javaInterface = std::make_shared<GooglePlayExpansionJavaInterface>();
        		JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
        	}

        	m_javaInterface->SetGooglePlayExpansionSystem(this);
        	m_javaInterface->Init();
        	m_numExpansions = m_javaInterface->GetNumExpansions();
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::OnResume()
        {
        	m_javaInterface->ResumeDownload();
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::OnSuspend()
        {
        	m_javaInterface->PauseDownload();
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::Install()
        {
        	u64 uddwRequiredSpace = GetRequiredInstallSpaceInBytes();

        	if(!HasEnoughStorageSpace(uddwRequiredSpace))
        	{
        		if(m_downloadStatusDelegate)
        		{
        			m_downloadStatusDelegate(DownloadStatus::k_failedInsufficientStorage);
        			return;
        		}
        	}

        	CSCore::Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(&GooglePlayExpansionSystem::UnzipTask, this));
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::UnzipTask()
        {
        	m_javaInterface->KeepAppAwake();

        	RemoveInstalledFiles();

        	Json::Value jManifest(Json::arrayValue);
			for(u32 i=0; i<m_numExpansions; ++i)
			{
				Unzip(m_javaInterface->GetExpansionPath(i), jManifest);
			}

			CSCore::Application::Get()->GetFileSystem()->WriteFile(CSCore::StorageLocation::k_cache, "AndroidExpansion.manifest", (s8*)jManifest.toStyledString().data(), jManifest.toStyledString().size());

			CachePackageDescriptions();

			m_javaInterface->AllowAppToSleep();

			CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&GooglePlayExpansionSystem::UnzipCompleteTask, this, DownloadStatus::k_complete));
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::RemoveInstalledFiles()
        {
        	//Load the manifest
        	Json::Value jManifest;
        	if(CSCore::JsonUtils::ReadJson(CSCore::StorageLocation::k_cache, "AndroidExpansion.manifest", jManifest))
        	{
        		for(u32 i=0; i<jManifest.size(); ++i)
        		{
        			if(jManifest[i]["IsDirectory"].asBool())
        			{
        				CSCore::Application::Get()->GetFileSystem()->DeleteDirectory(CSCore::StorageLocation::k_DLC, jManifest[i]["Path"].asString());
        			}
        			else
        			{
        				CSCore::Application::Get()->GetFileSystem()->DeleteFile(CSCore::StorageLocation::k_DLC, jManifest[i]["Path"].asString());
        			}
        		}

        		CSCore::Application::Get()->GetFileSystem()->DeleteFile(CSCore::StorageLocation::k_DLC, "AndroidExpansion.manifest");
        	}
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::Unzip(const std::string& instrZipPath, Json::Value& outjManifest)
        {
        	CSCore::Application::Get()->GetFileSystem()->CreateDirectoryPath(CSCore::StorageLocation::k_DLC, "");

        	unzFile ZippedFile = unzOpen(instrZipPath.c_str());

        	unz_global_info pInfo;
        	unzGetGlobalInfo(ZippedFile, &pInfo);
        	s32 dwTotalNumEntries = pInfo.number_entry;
        	s32 dwCurrentEntry = 1;

        	if(ZippedFile)
        	{
        		s32 dwStatus = unzGoToFirstFile(ZippedFile);
        		u32 udwBufferSize = 0;
        		s8* pbyDataBuffer = nullptr;

        		//Go to the first file in the zip
        		const u64 uddwFilenameLength = 256;
        		s8 byaFileName[uddwFilenameLength];

        		while(dwStatus == UNZ_OK)
        		{
        			//Open the next file
        			if (unzOpenCurrentFile(ZippedFile) != UNZ_OK)
        				break;

        			//Get file information
        			unz_file_info FileInfo;
        			unzGetCurrentFileInfo(ZippedFile, &FileInfo, byaFileName, uddwFilenameLength, 0, 0, 0, 0);

        			//Load the file into memory and then save it out to the directory
        			if(FileInfo.uncompressed_size > udwBufferSize)
        			{
        				udwBufferSize = FileInfo.uncompressed_size;
        				CS_SAFEDELETE_ARRAY(pbyDataBuffer);
        				pbyDataBuffer = new s8[udwBufferSize];
        			}

        			unzReadCurrentFile(ZippedFile, pbyDataBuffer, FileInfo.uncompressed_size);

					std::string strFilePath = std::string(byaFileName);

					Json::Value jManifestEntry;
					if(ContainsDirectoryPath(strFilePath))
					{
						//There is a nested folder so we need to create the directory structure
						jManifestEntry["IsDirectory"] = true;
						jManifestEntry["Path"] = GetRootFolderExcludingPath(strFilePath);

						std::string strPath = GetPathExcludingFileName(strFilePath);
						CSCore::Application::Get()->GetFileSystem()->CreateDirectoryPath(CSCore::StorageLocation::k_DLC, "/" + strPath);
					}
					else
					{
						//This is just a file
						jManifestEntry["IsDirectory"] = false;
						jManifestEntry["Path"] = GetFileNameExcludingPath(strFilePath);
					}

					if(IsFile(strFilePath))
					{
						CSCore::Application::Get()->GetFileSystem()->WriteFile(CSCore::StorageLocation::k_DLC, "/" + strFilePath, pbyDataBuffer, FileInfo.uncompressed_size);
					}

					outjManifest.append(jManifestEntry);

        			unzCloseCurrentFile(ZippedFile);
        			dwStatus = unzGoToNextFile(ZippedFile);

        			m_installProgress = (f32)dwCurrentEntry / (f32)dwTotalNumEntries;
        			dwCurrentEntry++;
        		}

        		CS_SAFEDELETE_ARRAY(pbyDataBuffer);
        	}
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::CachePackageDescriptions()
        {
			Json::Value jDesc(Json::arrayValue);

			for(u32 i=0; i<m_numExpansions; ++i)
			{
				Json::Value jExpansion;
				jExpansion["VersionCode"] = m_javaInterface->GetExpansionVersionCode(i);
				jExpansion["FileSize"] = (u32)m_javaInterface->GetExpansionFileSizeInBytes(i);
				jDesc.append(jExpansion);
			}

			CSCore::Application::Get()->GetFileSystem()->WriteFile(CSCore::StorageLocation::k_cache, "GoogleExpansionDownloader.cache", (s8*)jDesc.toStyledString().data(), jDesc.toStyledString().size());
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		void GooglePlayExpansionSystem::UnzipCompleteTask(DownloadStatus ineStatus)
		{
			if (m_downloadStatusDelegate)
			{
				m_downloadStatusDelegate(ineStatus);
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void GooglePlayExpansionSystem::OnDownloadStatusChanged(DownloadStatus ineStatus)
        {
        	//Forward any issues onto the listener but keep the complete back for
        	//ourself so we can install the downloads before telling the listener we are done
        	if(ineStatus != DownloadStatus::k_complete)
        	{
        		if(m_downloadStatusDelegate)
        		{
        			m_downloadStatusDelegate(ineStatus);
        		}
        	}
        	else
        	{
        		if(m_downloadStatusDelegate)
        		{
        			m_downloadStatusDelegate(DownloadStatus::k_installing);
        		}

        		Install();
        	}
        }
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::OnDestroy()
        {
        	m_javaInterface->SetGooglePlayExpansionSystem(nullptr);
        	m_javaInterface.reset();
        }
    }
}

#endif

#endif
