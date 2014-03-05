//
//  CGooglePlayExpansionSystem.cpp
//  MoFlow
//
//  Created by Hugh McLaughlin on 25/06/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayExpansionSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Minizip/unzip.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

ChilliSource::Android::GooglePlayExpansionSystem* gpActiveExpansionSystem = nullptr;

extern "C"
{
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedInProgress(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedComplete(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailed(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailedNoStorage(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPaused(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPausedNoWiFi(JNIEnv* inpEnv, jobject inpThis);
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedInProgress(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(ChilliSource::Android::DownloadStatus::k_downloading);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedComplete(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(ChilliSource::Android::DownloadStatus::k_complete);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailed(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(ChilliSource::Android::DownloadStatus::k_failed);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailedNoStorage(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(ChilliSource::Android::DownloadStatus::k_failedInsufficientStorage);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPaused(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(ChilliSource::Android::DownloadStatus::k_paused);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPausedNoWiFi(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(ChilliSource::Android::DownloadStatus::k_pausedNoWiFi);
	}
}

namespace ChilliSource
{
    namespace Android
    {
    	CS_DEFINE_NAMEDTYPE(GooglePlayExpansionSystem);
		//--------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------
    	GooglePlayExpansionSystem::GooglePlayExpansionSystem() : mfInstallProgress(0.0f), mDownloadStatusDelegate(nullptr)
    	{
    		gpActiveExpansionSystem = this;

    		mpJavaInterface = new GooglePlayExpansionJavaInterface();
	        ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(IJavaInterfaceSPtr(mpJavaInterface));

	        m_appResumeConnection = Core::ApplicationEvents::GetResumeEvent().OpenConnection(Core::MakeDelegate(this, &GooglePlayExpansionSystem::OnApplicationResume));
	        m_appSuspendConnection = Core::ApplicationEvents::GetLateSuspendEvent().OpenConnection(Core::MakeDelegate(this, &GooglePlayExpansionSystem::OnApplicationSuspend));
    	}
        //-------------------------------------------------------------
        /// Is A
        //-------------------------------------------------------------
        bool GooglePlayExpansionSystem::IsA(Core::InterfaceIDType inID) const
        {
            return inID == GooglePlayExpansionSystem::InterfaceID;
        }
        //-------------------------------------------------------------
        /// Init
    	//-------------------------------------------------------------
        void GooglePlayExpansionSystem::Init()
        {
        	mpJavaInterface->Init();
        	mudwNumExpansions = mpJavaInterface->GetNumExpansions();
        }
        //-------------------------------------------------------------
        /// On Application Resume
        ///
        /// Resume listening for the current download
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::OnApplicationResume()
        {
        	if(mpJavaInterface)
        	{
        		mpJavaInterface->ResumeDownload();
        	}
        }
        //-------------------------------------------------------------
        /// On Application Suspend
        ///
        /// Stop listening for the current download
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::OnApplicationSuspend()
        {
        	if(mpJavaInterface)
        	{
        		mpJavaInterface->PauseDownload();
        	}
        }
        //-------------------------------------------------------------
        /// Do Expansion Download Files Exist
        ///
        /// @param Whether the zip file exists on device
        //-------------------------------------------------------------
        bool GooglePlayExpansionSystem::DoExpansionDownloadFilesExist() const
        {
        	for(u32 i=0; i<mudwNumExpansions; ++i)
        	{
        		if(!Core::Application::GetFileSystemPtr()->DoesFileExist(Core::StorageLocation::k_root, mpJavaInterface->GetExpansionPath(i)))
        		{
        			return false;
        		}
        	}

        	return true;
        }
        //-------------------------------------------------------------
        /// Has Enough Storage Space
        ///
        /// @param Required space in bytes
        /// @return Whether enough space is available
        //-------------------------------------------------------------
        bool GooglePlayExpansionSystem::HasEnoughStorageSpace(u64 uddwRequired)
        {
			u64 uddwAvailableStorage = mpJavaInterface->GetExternalFreeStorageInBytes();

			//I made this intentionally not >= do give some wiggle room
			return uddwAvailableStorage > uddwRequired;
        }
        //-------------------------------------------------------------
        /// Is Download Required
        ///
        /// Check to see if the files have already been downloaded
        /// and if they are complete
        ///
        /// @return Whether a download is required
        //-------------------------------------------------------------
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
			if(Core::Utils::ReadJson(Core::StorageLocation::k_cache, "GoogleExpansionDownloader.cache", &jDesc))
			{
				bLatestVersion = true;

				for(u32 i=0; i<jDesc.size(); ++i)
				{
					Json::Value jExpansion = jDesc[i];
					if(jExpansion["VersionCode"].asUInt() < mpJavaInterface->GetExpansionVersionCode(i))
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
        //-------------------------------------------------------------
        /// Download
        ///
        /// Attempts to download the package and install to device
        /// from the google store.
        ///
        /// @param Delegate gets called whenever the download status
        /// changes - such as complete, paused, failed, etc
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::Download(const DownloadStatusDelegate& inDelegate)
        {
        	mDownloadStatusDelegate = inDelegate;

        	if(!IsDownloadRequired())
        	{
        		if(mDownloadStatusDelegate)
        		{
        			mDownloadStatusDelegate(DownloadStatus::k_complete);
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

        	Core::Application::GetFileSystemPtr()->DeleteFile(Core::StorageLocation::k_cache, "GoogleExpansionDownloader.cache");
        	mpJavaInterface->Download();
        }
        //-------------------------------------------------------------
        /// Pause
        ///
        /// Suspend the currently in-progress download
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::Pause()
        {
        	mpJavaInterface->PauseDownload();
        }
        //-------------------------------------------------------------
        /// Resume
        ///
        /// Resume the currently in-progress download
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::Resume()
        {
        	mpJavaInterface->ResumeDownload();
        }
        //-------------------------------------------------------------
        /// Get Download Progress
        ///
        /// @return Get the percentage progress of the current download
        //-------------------------------------------------------------
        f32 GooglePlayExpansionSystem::GetDownloadProgress()
        {
			return mpJavaInterface->GetDownloadProgress();
        }
        //-------------------------------------------------------------
        /// Get Install Progress
        ///
        /// @return Get the percentage progress of the current install
        //-------------------------------------------------------------
        f32 GooglePlayExpansionSystem::GetInstallProgress() const
        {
			return mfInstallProgress;
        }
        //-------------------------------------------------------------
        /// Do Installed Files Exist
        ///
        /// @return Whether the downloaded files require unzipping
        //-------------------------------------------------------------
        bool GooglePlayExpansionSystem::DoInstalledFilesExist() const
        {
        	return Core::Application::GetFileSystemPtr()->DoesFileExist(Core::StorageLocation::k_cache, "GoogleExpansionDownloader.cache");
        }
        //-------------------------------------------------------------
		/// Get Uncompressed Zip Size
		//-------------------------------------------------------------
        u64 GooglePlayExpansionSystem::GetUncompressedZipSize(const std::string& instrFilePath) const
        {
        	unzFile ZippedFile = unzOpen(instrFilePath.c_str());
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
        //-------------------------------------------------------------
        /// Get Required Download Space in Bytes
        ///
        /// @return Storage requirements for download
        //-------------------------------------------------------------
        u64 GooglePlayExpansionSystem::GetRequiredDownloadSpaceInBytes()
        {
        	u32 kudwSafetyPaddingInBytes = 256;
        	u64 uddwFileSize = kudwSafetyPaddingInBytes;
        	for(u32 i=0; i<mudwNumExpansions; ++i)
        	{
        		uddwFileSize += mpJavaInterface->GetExpansionFileSizeInBytes(i);
        	}

        	return uddwFileSize;
        }
        //-------------------------------------------------------------
        /// Get Required Install Space in Bytes
        ///
        /// @return Storage requirements for installation
        //-------------------------------------------------------------
        u64 GooglePlayExpansionSystem::GetRequiredInstallSpaceInBytes()
        {
        	u32 kudwSafetyPaddingInBytes = 256;
        	u64 uddwFileSize = kudwSafetyPaddingInBytes;

        	for(u32 i=0; i<mudwNumExpansions; ++i)
        	{
        		uddwFileSize += GetUncompressedZipSize(mpJavaInterface->GetExpansionPath(i));
        	}

        	return uddwFileSize;
        }
        //-------------------------------------------------------------
        /// Get Required Storage Space in Bytes
        ///
        /// @return Storage requirements
        //-------------------------------------------------------------
        u64 GooglePlayExpansionSystem::GetRequiredStorageSpaceInBytes()
        {
        	return GetRequiredInstallSpaceInBytes() + GetRequiredDownloadSpaceInBytes();
        }
        //-------------------------------------------------------------
        /// Install
        ///
        /// Unzip the expansions into the DLC directory ready for
        /// use by the file system
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::Install()
        {
        	u64 uddwRequiredSpace = GetRequiredInstallSpaceInBytes();

        	if(!HasEnoughStorageSpace(uddwRequiredSpace))
        	{
        		if(mDownloadStatusDelegate)
        		{
        			mDownloadStatusDelegate(DownloadStatus::k_failedInsufficientStorage);
        			return;
        		}
        	}

        	Core::Task<> UnzipTask(this, &GooglePlayExpansionSystem::UnzipTask);
        	Core::TaskScheduler::ScheduleTask(UnzipTask);
        }
        //-------------------------------------------------------------
        /// Unzip Task
        ///
        /// A task for unzipping the expansions
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::UnzipTask()
        {
        	mpJavaInterface->KeepAppAwake();

        	RemoveInstalledFiles();

        	Json::Value jManifest(Json::arrayValue);
			for(u32 i=0; i<mudwNumExpansions; ++i)
			{
				Unzip(mpJavaInterface->GetExpansionPath(i), jManifest);
			}

			Core::Application::GetFileSystemPtr()->CreateFile(Core::StorageLocation::k_cache, "AndroidExpansion.manifest", (s8*)jManifest.toUnformattedString().data(), jManifest.toUnformattedString().size());

			CachePackageDescriptions();

			mpJavaInterface->AllowAppToSleep();

        	Core::Task<DownloadStatus> CompleteTask(this, &GooglePlayExpansionSystem::UnzipCompleteTask, DownloadStatus::k_complete);
        	Core::TaskScheduler::ScheduleMainThreadTask(CompleteTask);
        }
        //-------------------------------------------------------------
        /// Remove Installed Files
        ///
        /// Attempt to load the manifest and remove all the files
        /// it contains from the installed location
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::RemoveInstalledFiles()
        {
        	//Load the manifest
        	Json::Value jManifest;
        	if(Core::Utils::ReadJson(Core::StorageLocation::k_cache, "AndroidExpansion.manifest", &jManifest))
        	{
        		for(u32 i=0; i<jManifest.size(); ++i)
        		{
        			if(jManifest[i]["IsDirectory"].asBool())
        			{
        				Core::Application::GetFileSystemPtr()->DeleteDirectory(Core::StorageLocation::k_DLC, jManifest[i]["Path"].asString());
        			}
        			else
        			{
        				Core::Application::GetFileSystemPtr()->DeleteFile(Core::StorageLocation::k_DLC, jManifest[i]["Path"].asString());
        			}
        		}

        		Core::Application::GetFileSystemPtr()->DeleteFile(Core::StorageLocation::k_DLC, "AndroidExpansion.manifest");
        	}
        }

        //---Path Helpers
        bool ContainsDirectoryPath(const std::string& instrPath)
        {
        	return (instrPath.find_first_of("/") != std::string::npos);
        }

        bool IsFile(const std::string& instrPath)
        {
        	return (instrPath.find_first_of(".") != std::string::npos);
        }

        std::string GetPathExcludingFileName(const std::string& instrPath)
        {
			u32 udwOffset = instrPath.find_last_of("/");
			if(udwOffset != std::string::npos)
			{
				return instrPath.substr(0, udwOffset);
			}

			return "";
        }

        std::string GetFileNameExcludingPath(const std::string& instrPath)
        {
			u32 udwOffset = instrPath.find_last_of("/");
			if(udwOffset != std::string::npos)
			{
				if(udwOffset + 1 < instrPath.size())
				{
					return instrPath.substr(udwOffset+1, instrPath.size());
				}
			}

			return instrPath;
        }

        std::string GetRootFolderExcludingPath(const std::string& instrPath)
        {
			u32 udwOffset = instrPath.find_first_of("/", 1);
			if(udwOffset != std::string::npos)
			{
				return instrPath.substr(0, udwOffset);
			}

			return "";
        }
        //-------------------------------------------------------------
        /// Unzip
        ///
        /// Unzipping the expansion to a directory.
        //-------------------------------------------------------------
        void GooglePlayExpansionSystem::Unzip(const std::string& instrZipPath, Json::Value& outjManifest)
        {
        	Core::Application::GetFileSystemPtr()->CreateDirectory(Core::StorageLocation::k_DLC, "");

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
						Core::Application::GetFileSystemPtr()->CreateDirectory(Core::StorageLocation::k_DLC, "/" + strPath);
					}
					else
					{
						//This is just a file
						jManifestEntry["IsDirectory"] = false;
						jManifestEntry["Path"] = GetFileNameExcludingPath(strFilePath);
					}

					if(IsFile(strFilePath))
					{
						Core::Application::GetFileSystemPtr()->CreateFile(Core::StorageLocation::k_DLC, "/" + strFilePath, pbyDataBuffer, FileInfo.uncompressed_size);
					}

					outjManifest.append(jManifestEntry);

        			unzCloseCurrentFile(ZippedFile);
        			dwStatus = unzGoToNextFile(ZippedFile);

        			mfInstallProgress = (f32)dwCurrentEntry / (f32)dwTotalNumEntries;
        			dwCurrentEntry++;
        		}

        		CS_SAFEDELETE_ARRAY(pbyDataBuffer);
        	}
        }
		//-------------------------------------------------------------
		/// Cache Package Descriptions
        ///
        /// Save the package version that will allow us to check
        /// whether we have the latest package
		//-------------------------------------------------------------
        void GooglePlayExpansionSystem::CachePackageDescriptions()
        {
			Json::Value jDesc(Json::arrayValue);

			for(u32 i=0; i<mudwNumExpansions; ++i)
			{
				Json::Value jExpansion;
				jExpansion["VersionCode"] = mpJavaInterface->GetExpansionVersionCode(i);
				jExpansion["FileSize"] = (u32)mpJavaInterface->GetExpansionFileSizeInBytes(i);
				jDesc.append(jExpansion);
			}

			Core::Application::GetFileSystemPtr()->CreateFile(Core::StorageLocation::k_cache, "GoogleExpansionDownloader.cache", (s8*)jDesc.toUnformattedString().data(), jDesc.toUnformattedString().size());
        }
		//-------------------------------------------------------------
		/// Unzip Finished Task
        ///
        /// @param Status
		//-------------------------------------------------------------
		void GooglePlayExpansionSystem::UnzipCompleteTask(DownloadStatus ineStatus)
		{
			if (mDownloadStatusDelegate)
			{
				mDownloadStatusDelegate(ineStatus);
			}
		}

        //----Called from Java
        void GooglePlayExpansionSystem::OnDownloadStatusChanged(DownloadStatus ineStatus)
        {
        	//Forward any issues onto the listener but keep the complete back for
        	//ourself so we can install the downloads before telling the listener we are done
        	if(ineStatus != DownloadStatus::k_complete)
        	{
        		if(mDownloadStatusDelegate)
        		{
        			mDownloadStatusDelegate(ineStatus);
        		}
        	}
        	else
        	{
        		if(mDownloadStatusDelegate)
        		{
        			mDownloadStatusDelegate(DownloadStatus::k_installing);
        		}

        		Install();
        	}
        }
		//--------------------------------------------------------------------
		/// Destructor
		//--------------------------------------------------------------------
    	GooglePlayExpansionSystem::~GooglePlayExpansionSystem()
    	{
    		if(gpActiveExpansionSystem == this)
    		{
    			gpActiveExpansionSystem = nullptr;
    		}
    	}

		//--------------------------------------------------------------------
		/// Java Interface
		//--------------------------------------------------------------------
    	CS_DEFINE_NAMEDTYPE(GooglePlayExpansionJavaInterface);

    	bool GooglePlayExpansionJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
    	{
    		return GooglePlayExpansionJavaInterface::InterfaceID == inInterfaceID;
    	}

    	GooglePlayExpansionJavaInterface::GooglePlayExpansionJavaInterface()
    	{
    		CreateNativeInterface("com/chillisource/googleplay/ExpansionDownloaderNativeInterface");

    		CreateMethodReference("Init", "()V");
    		CreateMethodReference("Download", "()V");
    		CreateMethodReference("PauseDownload", "()V");
    		CreateMethodReference("ResumeDownload", "()V");
    		CreateMethodReference("GetDownloadProgress", "()F");
    		CreateMethodReference("GetExternalFreeStorageInBytes", "()J");
    		CreateMethodReference("KeepAppAwake", "()V");
    		CreateMethodReference("AllowAppToSleep", "()V");
    		CreateMethodReference("GetNumExpansions", "()I");
    		CreateMethodReference("GetExpansionVersionCode", "(I)I");
    		CreateMethodReference("GetExpansionFileSize", "(I)J");
    		CreateMethodReference("GetExpansionPath", "(I)Ljava/lang/String;");
    	}

    	void GooglePlayExpansionJavaInterface::Init()
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("Init"));
    	}
    	void GooglePlayExpansionJavaInterface::Download()
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("Download"));
    	}
    	void GooglePlayExpansionJavaInterface::PauseDownload()
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("PauseDownload"));
    	}
    	void GooglePlayExpansionJavaInterface::ResumeDownload()
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("ResumeDownload"));
    	}
    	f32 GooglePlayExpansionJavaInterface::GetDownloadProgress()
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallFloatMethod(GetJavaObject(), GetMethodID("GetDownloadProgress"));
    	}
    	u64 GooglePlayExpansionJavaInterface::GetExternalFreeStorageInBytes()
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallLongMethod(GetJavaObject(), GetMethodID("GetExternalFreeStorageInBytes"));
    	}
    	void GooglePlayExpansionJavaInterface::KeepAppAwake()
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("KeepAppAwake"));
    	}
    	void GooglePlayExpansionJavaInterface::AllowAppToSleep()
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("AllowAppToSleep"));
    	}
    	u32 GooglePlayExpansionJavaInterface::GetNumExpansions()
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallIntMethod(GetJavaObject(), GetMethodID("GetNumExpansions"));
    	}
    	u32 GooglePlayExpansionJavaInterface::GetExpansionVersionCode(u32 inudwIndex)
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallIntMethod(GetJavaObject(), GetMethodID("GetExpansionVersionCode"), inudwIndex);
    	}
    	u64 GooglePlayExpansionJavaInterface::GetExpansionFileSizeInBytes(u32 inudwIndex)
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallLongMethod(GetJavaObject(), GetMethodID("GetExpansionFileSize"), inudwIndex);
    	}
    	std::string GooglePlayExpansionJavaInterface::GetExpansionPath(u32 inudwIndex)
    	{
    		JNIEnv* env = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		jstring jstrZipPath = (jstring) env->CallObjectMethod(GetJavaObject(), GetMethodID("GetExpansionPath"), inudwIndex);
    		std::string strZipPath = JavaInterfaceUtils::CreateSTDStringFromJString(jstrZipPath);
    		return strZipPath;
    	}
    }
}
