//
//  CGooglePlayExpansionSystem.cpp
//  MoFlow
//
//  Created by Hugh McLaughlin on 25/06/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Platform/Android/GooglePlayExpansion/GooglePlayExpansionSystem.h>

#include <ChilliSource/Core/Application.h>
#include <ChilliSource/Core/TaskScheduler.h>
#include <ChilliSource/Core/Utils.h>
#include <ChilliSource/Core/ApplicationEvents.h>
#include <ChilliSource/Core/Minizip/unzip.h>

#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceUtils.h>

#include <jni.h>

moFlo::AndroidPlatform::CGooglePlayExpansionSystem* gpActiveExpansionSystem = NULL;

extern "C"
{
	void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedInProgress(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedComplete(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedFailed(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedFailedNoStorage(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedPaused(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedPausedNoWiFi(JNIEnv* inpEnv, jobject inpThis);
}

void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedInProgress(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(moFlo::AndroidPlatform::DownloadStatus::DOWNLOADING);
	}
}

void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedComplete(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(moFlo::AndroidPlatform::DownloadStatus::COMPLETE);
	}
}

void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedFailed(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(moFlo::AndroidPlatform::DownloadStatus::FAILED);
	}
}

void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedFailedNoStorage(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(moFlo::AndroidPlatform::DownloadStatus::FAILED_INSUFFICIENT_STORAGE);
	}
}

void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedPaused(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(moFlo::AndroidPlatform::DownloadStatus::PAUSED);
	}
}

void Java_com_taggames_moflow_nativeinterface_CExpansionDownloaderNativeInterface_OnDownloadStateChangedPausedNoWiFi(JNIEnv* inpEnv, jobject inpThis)
{
	if(gpActiveExpansionSystem)
	{
		gpActiveExpansionSystem->OnDownloadStatusChanged(moFlo::AndroidPlatform::DownloadStatus::PAUSED_NO_WIFI);
	}
}

namespace moFlo
{
    namespace AndroidPlatform
    {
    	DEFINE_NAMED_INTERFACE(CGooglePlayExpansionSystem);
		//--------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------
    	CGooglePlayExpansionSystem::CGooglePlayExpansionSystem() : mfInstallProgress(0.0f), mDownloadStatusDelegate(NULL)
    	{
    		gpActiveExpansionSystem = this;

    		mpJavaInterface = new CGooglePlayExpansionJavaInterface();
	        moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(JavaInterfacePtr(mpJavaInterface));

			Core::CApplicationEvents::GetResumeEvent() += Core::ApplicationSystemDelegate(this, &CGooglePlayExpansionSystem::OnApplicationResume);
			Core::CApplicationEvents::GetLateSuspendEvent() += Core::ApplicationSystemDelegate(this, &CGooglePlayExpansionSystem::OnApplicationSuspend);
    	}
        //-------------------------------------------------------------
        /// Is A
        //-------------------------------------------------------------
        bool CGooglePlayExpansionSystem::IsA(Core::InterfaceIDType inID) const
        {
            return inID == CGooglePlayExpansionSystem::InterfaceID;
        }
        //-------------------------------------------------------------
        /// Init
    	//-------------------------------------------------------------
        void CGooglePlayExpansionSystem::Init()
        {
        	mpJavaInterface->Init();
        	mudwNumExpansions = mpJavaInterface->GetNumExpansions();
        }
        //-------------------------------------------------------------
        /// On Application Resume
        ///
        /// Resume listening for the current download
        //-------------------------------------------------------------
        void CGooglePlayExpansionSystem::OnApplicationResume()
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
        void CGooglePlayExpansionSystem::OnApplicationSuspend()
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
        bool CGooglePlayExpansionSystem::DoExpansionDownloadFilesExist() const
        {
        	for(u32 i=0; i<mudwNumExpansions; ++i)
        	{
        		if(!Core::CApplication::GetFileSystemPtr()->DoesFileExist(Core::SL_ROOT, mpJavaInterface->GetExpansionPath(i)))
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
        bool CGooglePlayExpansionSystem::HasEnoughStorageSpace(u64 uddwRequired)
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
        bool CGooglePlayExpansionSystem::IsDownloadRequired()
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
			if(Core::CUtils::ReadJson(Core::SL_CACHE, "GoogleExpansionDownloader.cache", &jDesc))
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
        void CGooglePlayExpansionSystem::Download(const DownloadStatusDelegate& inDelegate)
        {
        	mDownloadStatusDelegate = inDelegate;

        	if(!IsDownloadRequired())
        	{
        		if(mDownloadStatusDelegate)
        		{
        			mDownloadStatusDelegate(DownloadStatus::COMPLETE);
        		}
        		return;
        	}
        	// If Downloaded obb exists (i.e. is downloaded or copied locally)
        	// but not installed, install them
			else if(DoExpansionDownloadFilesExist())
			{
				OnDownloadStatusChanged(DownloadStatus::COMPLETE);
				return;
			}

        	Core::CApplication::GetFileSystemPtr()->DeleteFile(Core::SL_CACHE, "GoogleExpansionDownloader.cache");
        	mpJavaInterface->Download();
        }
        //-------------------------------------------------------------
        /// Pause
        ///
        /// Suspend the currently in-progress download
        //-------------------------------------------------------------
        void CGooglePlayExpansionSystem::Pause()
        {
        	mpJavaInterface->PauseDownload();
        }
        //-------------------------------------------------------------
        /// Resume
        ///
        /// Resume the currently in-progress download
        //-------------------------------------------------------------
        void CGooglePlayExpansionSystem::Resume()
        {
        	mpJavaInterface->ResumeDownload();
        }
        //-------------------------------------------------------------
        /// Get Download Progress
        ///
        /// @return Get the percentage progress of the current download
        //-------------------------------------------------------------
        f32 CGooglePlayExpansionSystem::GetDownloadProgress()
        {
			return mpJavaInterface->GetDownloadProgress();
        }
        //-------------------------------------------------------------
        /// Get Install Progress
        ///
        /// @return Get the percentage progress of the current install
        //-------------------------------------------------------------
        f32 CGooglePlayExpansionSystem::GetInstallProgress() const
        {
			return mfInstallProgress;
        }
        //-------------------------------------------------------------
        /// Do Installed Files Exist
        ///
        /// @return Whether the downloaded files require unzipping
        //-------------------------------------------------------------
        bool CGooglePlayExpansionSystem::DoInstalledFilesExist() const
        {
        	return Core::CApplication::GetFileSystemPtr()->DoesFileExist(Core::SL_CACHE, "GoogleExpansionDownloader.cache");
        }
        //-------------------------------------------------------------
		/// Get Uncompressed Zip Size
		//-------------------------------------------------------------
        u64 CGooglePlayExpansionSystem::GetUncompressedZipSize(const std::string& instrFilePath) const
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
				unzGetCurrentFileInfo(ZippedFile, &FileInfo, byaFileName, uddwFilenameLength, NULL, 0, NULL, 0);
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
        u64 CGooglePlayExpansionSystem::GetRequiredDownloadSpaceInBytes()
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
        u64 CGooglePlayExpansionSystem::GetRequiredInstallSpaceInBytes()
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
        u64 CGooglePlayExpansionSystem::GetRequiredStorageSpaceInBytes()
        {
        	return GetRequiredInstallSpaceInBytes() + GetRequiredDownloadSpaceInBytes();
        }
        //-------------------------------------------------------------
        /// Install
        ///
        /// Unzip the expansions into the DLC directory ready for
        /// use by the file system
        //-------------------------------------------------------------
        void CGooglePlayExpansionSystem::Install()
        {
        	u64 uddwRequiredSpace = GetRequiredInstallSpaceInBytes();

        	if(!HasEnoughStorageSpace(uddwRequiredSpace))
        	{
        		if(mDownloadStatusDelegate)
        		{
        			mDownloadStatusDelegate(DownloadStatus::FAILED_INSUFFICIENT_STORAGE);
        			return;
        		}
        	}

        	Task0 UnzipTask(this, &CGooglePlayExpansionSystem::UnzipTask);
        	CTaskScheduler::ScheduleTask(UnzipTask);
        }
        //-------------------------------------------------------------
        /// Unzip Task
        ///
        /// A task for unzipping the expansions
        //-------------------------------------------------------------
        void CGooglePlayExpansionSystem::UnzipTask()
        {
        	mpJavaInterface->KeepAppAwake();

        	RemoveInstalledFiles();

        	Json::Value jManifest(Json::arrayValue);
			for(u32 i=0; i<mudwNumExpansions; ++i)
			{
				Unzip(mpJavaInterface->GetExpansionPath(i), jManifest);
			}

			Core::CApplication::GetFileSystemPtr()->CreateFile(Core::SL_CACHE, "AndroidExpansion.manifest", (s8*)jManifest.toUnformattedString().data(), jManifest.toUnformattedString().size());

			CachePackageDescriptions();

			mpJavaInterface->AllowAppToSleep();

        	Task1<DownloadStatus::ENUM> CompleteTask(this, &CGooglePlayExpansionSystem::UnzipCompleteTask, DownloadStatus::COMPLETE);
        	CTaskScheduler::ScheduleMainThreadTask(CompleteTask);
        }
        //-------------------------------------------------------------
        /// Remove Installed Files
        ///
        /// Attempt to load the manifest and remove all the files
        /// it contains from the installed location
        //-------------------------------------------------------------
        void CGooglePlayExpansionSystem::RemoveInstalledFiles()
        {
        	//Load the manifest
        	Json::Value jManifest;
        	if(Core::CUtils::ReadJson(Core::SL_CACHE, "AndroidExpansion.manifest", &jManifest))
        	{
        		for(u32 i=0; i<jManifest.size(); ++i)
        		{
        			if(jManifest[i]["IsDirectory"].asBool())
        			{
        				Core::CApplication::GetFileSystemPtr()->DeleteDirectory(Core::SL_DLC, jManifest[i]["Path"].asString());
        			}
        			else
        			{
        				Core::CApplication::GetFileSystemPtr()->DeleteFile(Core::SL_DLC, jManifest[i]["Path"].asString());
        			}
        		}

        		Core::CApplication::GetFileSystemPtr()->DeleteFile(Core::SL_DLC, "AndroidExpansion.manifest");
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
        void CGooglePlayExpansionSystem::Unzip(const std::string& instrZipPath, Json::Value& outjManifest)
        {
        	Core::CApplication::GetFileSystemPtr()->CreateDirectory(Core::SL_DLC, "");

        	unzFile ZippedFile = unzOpen(instrZipPath.c_str());

        	unz_global_info pInfo;
        	unzGetGlobalInfo(ZippedFile, &pInfo);
        	s32 dwTotalNumEntries = pInfo.number_entry;
        	s32 dwCurrentEntry = 1;

        	if(ZippedFile)
        	{
        		s32 dwStatus = unzGoToFirstFile(ZippedFile);
        		u32 udwBufferSize = 0;
        		s8* pbyDataBuffer = NULL;

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
        				SAFE_DELETE_ARRAY(pbyDataBuffer);
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
						Core::CApplication::GetFileSystemPtr()->CreateDirectory(Core::SL_DLC, "/" + strPath);
					}
					else
					{
						//This is just a file
						jManifestEntry["IsDirectory"] = false;
						jManifestEntry["Path"] = GetFileNameExcludingPath(strFilePath);
					}

					if(IsFile(strFilePath))
					{
						Core::CApplication::GetFileSystemPtr()->CreateFile(Core::SL_DLC, "/" + strFilePath, pbyDataBuffer, FileInfo.uncompressed_size);
					}

					outjManifest.append(jManifestEntry);

        			unzCloseCurrentFile(ZippedFile);
        			dwStatus = unzGoToNextFile(ZippedFile);

        			mfInstallProgress = (f32)dwCurrentEntry / (f32)dwTotalNumEntries;
        			dwCurrentEntry++;
        		}

        		SAFE_DELETE_ARRAY(pbyDataBuffer);
        	}
        }
		//-------------------------------------------------------------
		/// Cache Package Descriptions
        ///
        /// Save the package version that will allow us to check
        /// whether we have the latest package
		//-------------------------------------------------------------
        void CGooglePlayExpansionSystem::CachePackageDescriptions()
        {
			Json::Value jDesc(Json::arrayValue);

			for(u32 i=0; i<mudwNumExpansions; ++i)
			{
				Json::Value jExpansion;
				jExpansion["VersionCode"] = mpJavaInterface->GetExpansionVersionCode(i);
				jExpansion["FileSize"] = (u32)mpJavaInterface->GetExpansionFileSizeInBytes(i);
				jDesc.append(jExpansion);
			}

			Core::CApplication::GetFileSystemPtr()->CreateFile(Core::SL_CACHE, "GoogleExpansionDownloader.cache", (s8*)jDesc.toUnformattedString().data(), jDesc.toUnformattedString().size());
        }
		//-------------------------------------------------------------
		/// Unzip Finished Task
        ///
        /// @param Status
		//-------------------------------------------------------------
		void CGooglePlayExpansionSystem::UnzipCompleteTask(DownloadStatus::ENUM ineStatus)
		{
			if (mDownloadStatusDelegate)
			{
				mDownloadStatusDelegate(ineStatus);
			}
		}

        //----Called from Java
        void CGooglePlayExpansionSystem::OnDownloadStatusChanged(DownloadStatus::ENUM ineStatus)
        {
        	//Forward any issues onto the listener but keep the complete back for
        	//ourself so we can install the downloads before telling the listener we are done
        	if(ineStatus != DownloadStatus::COMPLETE)
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
        			mDownloadStatusDelegate(DownloadStatus::INSTALLING);
        		}

        		Install();
        	}
        }
		//--------------------------------------------------------------------
		/// Destructor
		//--------------------------------------------------------------------
    	CGooglePlayExpansionSystem::~CGooglePlayExpansionSystem()
    	{
    		Core::CApplicationEvents::GetResumeEvent() -= Core::ApplicationSystemDelegate(this, &CGooglePlayExpansionSystem::OnApplicationResume);
    		Core::CApplicationEvents::GetLateSuspendEvent() -= Core::ApplicationSystemDelegate(this, &CGooglePlayExpansionSystem::OnApplicationSuspend);

    		if(gpActiveExpansionSystem == this)
    		{
    			gpActiveExpansionSystem = NULL;
    		}
    	}

		//--------------------------------------------------------------------
		/// Java Interface
		//--------------------------------------------------------------------
    	DEFINE_NAMED_INTERFACE(CGooglePlayExpansionJavaInterface);

    	bool CGooglePlayExpansionJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
    	{
    		return CGooglePlayExpansionJavaInterface::InterfaceID == inInterfaceID;
    	}

    	CGooglePlayExpansionJavaInterface::CGooglePlayExpansionJavaInterface()
    	{
    		CreateNativeInterface("com/taggames/moflow/nativeinterface/CExpansionDownloaderNativeInterface");

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

    	void CGooglePlayExpansionJavaInterface::Init()
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("Init"));
    	}
    	void CGooglePlayExpansionJavaInterface::Download()
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("Download"));
    	}
    	void CGooglePlayExpansionJavaInterface::PauseDownload()
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("PauseDownload"));
    	}
    	void CGooglePlayExpansionJavaInterface::ResumeDownload()
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("ResumeDownload"));
    	}
    	f32 CGooglePlayExpansionJavaInterface::GetDownloadProgress()
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallFloatMethod(GetJavaObject(), GetMethodID("GetDownloadProgress"));
    	}
    	u64 CGooglePlayExpansionJavaInterface::GetExternalFreeStorageInBytes()
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallLongMethod(GetJavaObject(), GetMethodID("GetExternalFreeStorageInBytes"));
    	}
    	void CGooglePlayExpansionJavaInterface::KeepAppAwake()
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("KeepAppAwake"));
    	}
    	void CGooglePlayExpansionJavaInterface::AllowAppToSleep()
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("AllowAppToSleep"));
    	}
    	u32 CGooglePlayExpansionJavaInterface::GetNumExpansions()
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallIntMethod(GetJavaObject(), GetMethodID("GetNumExpansions"));
    	}
    	u32 CGooglePlayExpansionJavaInterface::GetExpansionVersionCode(u32 inudwIndex)
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallIntMethod(GetJavaObject(), GetMethodID("GetExpansionVersionCode"), inudwIndex);
    	}
    	u64 CGooglePlayExpansionJavaInterface::GetExpansionFileSizeInBytes(u32 inudwIndex)
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		return env->CallLongMethod(GetJavaObject(), GetMethodID("GetExpansionFileSize"), inudwIndex);
    	}
    	std::string CGooglePlayExpansionJavaInterface::GetExpansionPath(u32 inudwIndex)
    	{
    		JNIEnv* env = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
    		jstring jstrZipPath = (jstring) env->CallObjectMethod(GetJavaObject(), GetMethodID("GetExpansionPath"), inudwIndex);
    		std::string strZipPath = JavaInterfaceUtils::CreateSTDStringFromJString(jstrZipPath);
    		return strZipPath;
    	}
    }
}
