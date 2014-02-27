//
//  ContentManagementSystem.cpp
//  moFlow
//
//  Created by Scott Downie on 04/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Networking/ContentDownload/ContentManagementSystem.h>

#include <ChilliSource/Core/File/TweakableConstants.h>
#include <ChilliSource/Core/Cryptographic/HashMD5.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/File/LocalDataStore.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/Minizip/unzip.h>

namespace ChilliSource
{
    namespace Networking
    {
        DEFINE_NAMED_INTERFACE(ContentManagementSystem);
        
        //-----------------------------------------------------------
        /// Constructor
        ///
        /// @param Content downloader
		/// @param Current application version 
        /// @param Array of tags
        //-----------------------------------------------------------
        ContentManagementSystem::ContentManagementSystem(IContentDownloader* inpContentDownloader) 
        : mpContentDownloader(inpContentDownloader), mpServerManifest(nullptr), 
        muRunningToDownloadTotal(0), muRunningDownloadedTotal(0), mbDLCCachePurged(false)
        {
            mstrContentDirectory = Core::CApplication::GetFileSystemPtr()->GetStorageLocationDirectory(Core::StorageLocation::k_DLC);
        }
        //-----------------------------------------------------------
        /// Load Local Manifest 
        ///
        /// Load the client side cached content manifest if one
        /// exists
        ///
        /// @param TiXmlDocument
        //-----------------------------------------------------------
        void ContentManagementSystem::LoadLocalManifest(TiXmlDocument* inpCurrentManifest)
        {
            //The manifest lives in the documents directory
            inpCurrentManifest->LoadFile(Core::StorageLocation::k_DLC, "ContentManifest.moman");
            
            //If there is no DLC we should check to see if there ever was any
            if(!inpCurrentManifest->RootElement() && Core::CLocalDataStore::GetSingletonPtr()->HasValueForKey("MOCMSCachedDLC"))
            {
                mbDLCCachePurged = true;
            }
        }
        //-----------------------------------------------------------
        /// Get Manifest Checksum for File
        ///
        /// @param File name
        /// @return Checksum of file as found in the current manifest
        //-----------------------------------------------------------
        std::string ContentManagementSystem::GetManifestChecksumForFile(const std::string& instrFilename)
        {
            return CalculateChecksum(Core::StorageLocation::k_DLC, instrFilename);
        }
		//-----------------------------------------------------------
		/// Calculate Checksum
		///
		/// Calculate a checksum for the file. This involves
		/// performing an MD5 hash of the file and converting that
		/// to base 64 encoded and then trimming the trailing '='
		///
		/// @param File location
		/// @param File path
		/// @return Checksum string
		//-----------------------------------------------------------
		std::string ContentManagementSystem::CalculateChecksum(Core::StorageLocation ineLocation, const std::string& instrFilePath)
		{
            std::string strMD5Checksum = Core::CApplication::GetFileSystemPtr()->GetFileMD5Checksum(ineLocation, instrFilePath);
			std::string strBase64Encoded = Core::CBaseEncoding::Base64Encode(strMD5Checksum);
			Core::CStringUtils::ChopTrailingChars(strBase64Encoded, '=');
			return strBase64Encoded;
		}
        //-----------------------------------------------------------
        /// Is A
        ///
        /// @param Interface ID to compare
        /// @return Whether system is of that type
        //-----------------------------------------------------------
        bool ContentManagementSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == ContentManagementSystem::InterfaceID;
        }
        //-----------------------------------------------------------
        /// Clear Download Data
        ///
        /// Data is cached in memory and only written to disc
        /// when the download is successful; at which point
        /// we can clear the data and remove any temp files
        //-----------------------------------------------------------
        void ContentManagementSystem::ClearDownloadData()
        {
        	//Clear the old crap
            CS_SAFE_DELETE(mpServerManifest);
            mRemovePackageIDs.clear();
            mPackageDetails.clear();
        }
        //-----------------------------------------------------------
        /// Check For Updates
        ///
        /// Get the content manifest from the asset server and
        /// check for any changes with the local copy
        ///
        /// The location of the asset server is usually the Tag 
        /// server but may be a third party storage solution based on
        /// the SKU (i.e. Google's Asset Store). The URL is taken
        /// from the tweak constants
        ///
        /// @param Delegate to callback notifying whether an update
        /// is required
        //-----------------------------------------------------------
        void ContentManagementSystem::CheckForUpdates(const ContentManagementSystem::CheckForUpdateDelegate& inDelegate)
        {
            CS_DEBUG_LOG("CMS: Checking for content updates...");
            
            //Clear any stale data from last update check
            ClearDownloadData();
            
            //Have the downloader request the manifest in it's own way
            if(mpContentDownloader->DownloadContentManifest(Core::MakeDelegate(this, &ContentManagementSystem::OnContentManifestDownloadComplete)))
            {
                //The request has started successfully
                mOnUpdateCheckCompleteDelegate = inDelegate;
            }
            else
            {
                //The request has failed to start most likely due to internet connection
                CS_ERROR_LOG("CMS: Internet not reachable");
                if(mbDLCCachePurged)
                {
                    inDelegate(UpdateResult::k_updateCheckFailedBlocking);
                }
                else
                {
                    inDelegate(UpdateResult::k_updateCheckFailed);
                }
            }
        }
        //-----------------------------------------------------------
        /// Download Updates
        ///
        /// Having checked for updates and been notified whether
        /// there are updates pending this function should be
        /// called to begin downloading any out of date content
        ///
        /// Call GetDownloadProgress to get the progress value
        /// to update any progress UI
        ///
        /// @param Delegate to call when download is complete
        //-----------------------------------------------------------
        void ContentManagementSystem::DownloadUpdates(const ContentManagementSystem::CompleteDelegate& inDelegate)
        {
        	mOnDownloadCompleteDelegate = inDelegate;
            mudwCurrentPackageDownload = 0;
            
            if(!mPackageDetails.empty())
            {
            	//Add a temp directory so that the packages are stored atomically and only overwrite
                //the originals on full success
                Core::CApplication::GetFileSystemPtr()->CreateDirectory(Core::StorageLocation::k_DLC, "Temp");
                mpContentDownloader->DownloadPackage(mPackageDetails[mudwCurrentPackageDownload].strURL, Core::MakeDelegate(this, &ContentManagementSystem::OnContentDownloadComplete));
            }
            else
            {
            	CS_DEBUG_LOG("CMS: Content update finished");
                mOnDownloadCompleteDelegate(Result::k_contentSucceeded);
            }
        }
        //-----------------------------------------------------------
        /// Download Next Package
        ///
        /// Perform the HTTP request for the next DLC package
        //-----------------------------------------------------------
        void ContentManagementSystem::DownloadNextPackage()
        {
            mudwCurrentPackageDownload++;
            mpContentDownloader->DownloadPackage(mPackageDetails[mudwCurrentPackageDownload].strURL, Core::MakeDelegate(this, &ContentManagementSystem::OnContentDownloadComplete));
        }
        //-----------------------------------------------------------
        /// Install Updates
        ///
        /// Having downloaded the update packages this method
        /// unzips the packages and overwrites any old assets
        ///
        /// @param Delegate to call when Install is complete
        //-----------------------------------------------------------
        void ContentManagementSystem::InstallUpdates(const CompleteDelegate& inDelegate)
        {
            if(!mPackageDetails.empty() || !mRemovePackageIDs.empty())
            {
                CS_DEBUG_LOG("CMS: Installing content updates...");
				
				if(!mPackageDetails.empty())
				{
					//Unzip all the files and overwrite the old manifest
					Core::WaitCondition waitCondition(mPackageDetails.size());
					
					Core::CTaskScheduler::ForEach(mPackageDetails.begin(), mPackageDetails.end(), this, &ContentManagementSystem::ExtractFilesFromPackage, &waitCondition);
					
					//Wait on all the packages being unzipped
					waitCondition.Wait();
				}
                
                //Remove the temp zips
                DeleteDirectory("Temp");
                
                mPackageDetails.clear();
			
				if(!mRemovePackageIDs.empty())
				{
					//Remove any unused files from the documents
					Core::WaitCondition waitCondition(mRemovePackageIDs.size());
					
					Core::CTaskScheduler::ForEach(mRemovePackageIDs.begin(), mRemovePackageIDs.end(), this, &ContentManagementSystem::DeleteDirectory, &waitCondition);
					
					//Wait on all the packages being removed
					waitCondition.Wait();
				}
                
                //Save the new content manifest
                mpServerManifest->SaveFile(Core::StorageLocation::k_DLC, "ContentManifest.moman");
                
                CS_DEBUG_LOG("CMS: Installing content updates complete");
                
                mbDLCCachePurged = false;
                
                //Store that we have DLC cached. If there is no DLC on next check then 
                //we know the cache has been purged and we have to block on download
                Core::CLocalDataStore::GetSingletonPtr()->SetValueForKey("MOCMSCachedDLC", true);
                
                //Tell the delegate all is good
                inDelegate(Result::k_contentSucceeded);
            }
            else
            {
                //Tell the delegate all is bad
                inDelegate(Result::k_contentFailed);
            }
            
            ClearDownloadData();
        }
        //-----------------------------------------------------------
        /// On Content Manifest Download Complete
        ///
        /// The manifest file has downloaded we can now compare
        /// and contrast to check for outdated files
        ///
        /// @param Request result
        /// @param Request response
        //-----------------------------------------------------------
        void ContentManagementSystem::OnContentManifestDownloadComplete(ContentDownloader::Result ineResult, const std::string& instrManifest)
        {
            switch(ineResult)
            {
                case ContentDownloader::Result::k_succeeded:
                    CS_DEBUG_LOG("CMS: Content manifest download complete");
                    mstrServerManifestData += instrManifest;
                    BuildDownloadList(mstrServerManifestData); 
                    mstrServerManifestData.clear();
                    break;
                case ContentDownloader::Result::k_failed:
                    CS_DEBUG_LOG("CMS: Content manifest download failed: " + instrManifest);
                    mstrServerManifestData.clear();
                    mbDLCCachePurged ? mOnUpdateCheckCompleteDelegate(UpdateResult::k_updateCheckFailedBlocking) : mOnUpdateCheckCompleteDelegate(UpdateResult::k_updateCheckFailed);
                    break;
                case ContentDownloader::Result::k_flushed:
                    CS_DEBUG_LOG("CMS: Content manifest download flushed");
                    mstrServerManifestData += instrManifest;
                    break;
            };
            
            //Reset the listener
            mOnUpdateCheckCompleteDelegate = nullptr;
        }
        //-----------------------------------------------------------
        /// On Content Download Complete
        ///
        /// The file has downloaded we can now save it to the the 
        /// cache
        ///
        /// @param Request result
        /// @param Request response
        //-----------------------------------------------------------
        void ContentManagementSystem::OnContentDownloadComplete(ContentDownloader::Result ineResult, const std::string& instrData)
        {
        	switch(ineResult)
            {
                case ContentDownloader::Result::k_succeeded:
                {
                    CS_DEBUG_LOG("CMS: " + mPackageDetails[mudwCurrentPackageDownload].strID + " Package download complete");
                    if(SavePackageToFile(mPackageDetails[mudwCurrentPackageDownload], instrData, true))
                    {
                        muRunningDownloadedTotal += mPackageDetails[mudwCurrentPackageDownload].udwSize;
                        
                        //Don't overwrite the old manifest until all the content has been downloaded 
                        if(mudwCurrentPackageDownload >= (mPackageDetails.size() - 1))
                        {
                            CS_DEBUG_LOG("CMS: Content update finished");
                            mOnDownloadCompleteDelegate(Result::k_contentSucceeded);
                        }
                        else
                        {
                            DownloadNextPackage();
                        }
                        
                        break;
                    }
                }
                case ContentDownloader::Result::k_failed:
                {
                	CS_DEBUG_LOG("CMS: " + mPackageDetails[mudwCurrentPackageDownload].strID + " Package download failed");
                    
                    //Delete all temp zip files and cancel the outstanding requests
                    DeleteDirectory("Temp");
                    
                    if(mOnDownloadCompleteDelegate)
                    {
                        mOnDownloadCompleteDelegate(Result::k_contentFailed);
                    }
                    break;
                }
                case ContentDownloader::Result::k_flushed:
                {
                	CS_DEBUG_LOG("CMS: " + mPackageDetails[mudwCurrentPackageDownload].strID + " Package exceeds buffer size and is being flushed");
                    SavePackageToFile(mPackageDetails[mudwCurrentPackageDownload], instrData, false);
                    break;
                }
            }
        }
        //-----------------------------------------------------------
        /// Build Download List
        ///
        /// Check if an existing content manifest exists and
        /// construct a list of the files that require updating
        /// then overwrite the old manifest with the new one
        ///
        /// @param String containing the server manifest
        //-----------------------------------------------------------
        void ContentManagementSystem::BuildDownloadList(const std::string& instrServerManifest)
        {
			//Validate the server manifest
            mpServerManifest = new TiXmlDocument();
            mpServerManifest->Parse(instrServerManifest.c_str(), 0, TIXML_ENCODING_UTF8);
            
            if(!mpServerManifest->RootElement())
            {
                CS_ERROR_LOG("CMS: Server content manifest is invalid");
                if(mbDLCCachePurged)
                {
                    mOnUpdateCheckCompleteDelegate(UpdateResult::k_updateCheckFailedBlocking);
                }
                else
                {
                    mOnUpdateCheckCompleteDelegate(UpdateResult::k_updateCheckFailed);
                }

                return;
            }
            
            //Check if DLC is enabled
            if(!Core::XMLUtils::GetAttributeValueOrDefault<bool>(mpServerManifest->RootElement(), "DLCEnabled", false))
            {
                CS_DEBUG_LOG("CMS: DLC disabled by server");
				mOnUpdateCheckCompleteDelegate(UpdateResult::k_updateNotAvailable);
                return;
            }
            
            TiXmlDocument* pCurrentManifest = new TiXmlDocument();
            LoadLocalManifest(pCurrentManifest);
            
			//If we have not successfully loaded a manifest from file we need to check if any of the assets 
            //are in the bundle and pull down the others
			if(!pCurrentManifest || !pCurrentManifest->RootElement())
			{
                //Grab all the URL's from the new manifest
                TiXmlElement* pPackageEl = Core::XMLUtils::FirstChildElementWithName(mpServerManifest->RootElement(), "Package");
                while(pPackageEl)
                {
                    //If the package is not in the bundle it will download
                    AddToDownloadListIfNotInBundle(pPackageEl);
                    
                    //On to the next package
                    pPackageEl = Core::XMLUtils::NextSiblingElementWithName(pPackageEl);
                }
			}
            //Lets find out what we need already have in the manifest
			else
            {
                std::unordered_map<std::string, std::string> mapPackageIDToChecksum;
                
                //Store the data from the local manifest to make a comparison with the server manifest
                if(pCurrentManifest->RootElement())
                {
                    //Loop round 
                    TiXmlElement* pLocalPackageEl = Core::XMLUtils::FirstChildElementWithName(pCurrentManifest->RootElement(), "Package");
                    while(pLocalPackageEl)
                    {
                        //Store the local ID's and checksums for comparison later
                        std::string strLocalPackageID = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pLocalPackageEl, "ID", "");
                        std::string strLocalPackageChecksum = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pLocalPackageEl, "Checksum", "");
                        
                        mapPackageIDToChecksum.insert(std::make_pair(strLocalPackageID, strLocalPackageChecksum));
                        
                        //On to the next package
                        pLocalPackageEl = Core::XMLUtils::NextSiblingElementWithName(pLocalPackageEl);
                    }
                }
				
                //Now process the server manifest and see whats different between the two
                TiXmlElement* pServerPackageEl = Core::XMLUtils::FirstChildElementWithName(mpServerManifest->RootElement(), "Package");
                while(pServerPackageEl)
                {
                    //Store the local ID's and checksums for comparison later
                    std::string strServerPackageID = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pServerPackageEl, "ID", "");
                    std::string strServerPackageChecksum = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pServerPackageEl, "Checksum", "");
					std::string strMinVersionForPackage = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pServerPackageEl, "MinVersion", "");
                    
                    std::unordered_map<std::string, std::string>::iterator it = mapPackageIDToChecksum.find(strServerPackageID);
				
                    //Based on the state of the file decide whether it needs updating
                    if(it != mapPackageIDToChecksum.end())
                    {
                        //The package exists in the documents directory
                        if(it->second != strServerPackageChecksum)
                        {
                            //The package in documents is outdated lets check if the files all exist in the bundle
                            //that way we don't need to download
                            AddToDownloadListIfNotInBundle(pServerPackageEl);                            
                        }
                        else
                        {
                            //The checksum in the local manifest matches that on the
                            //server but the file may have been altered locally
                            //Check if the files within are corrupt
                            //Check all the file names
                            TiXmlElement* pFileEl = Core::XMLUtils::FirstChildElementWithName(pServerPackageEl, "File");
                            while(pFileEl)
                            {
                                std::string strFullPath = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pFileEl, "Location", "");
                                std::string strChecksum = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pFileEl, "Checksum", "");
                                
                                if(strFullPath.empty()) //Maintain backwards compatability with old versions
                                {
                                    std::string strFileName = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pFileEl, "Name", "");
                                    strFullPath = strServerPackageID + "/" + strFileName;
                                }
                                
                                if(!DoesFileExist(strFullPath, strChecksum, false))
                                {
                                    std::string strPackageUrl = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pServerPackageEl, "URL", "");
                                    u32 udwPackageSize = Core::XMLUtils::GetAttributeValueOrDefault<u32>(pServerPackageEl, "Size", 0);
                                    muRunningToDownloadTotal += udwPackageSize;
                                    
                                    CS_DEBUG_LOG("CMS: " + strServerPackageID + " package requires updating of size : " + Core::ToString(udwPackageSize));
                                    
                                    PackageDetails sPackageDetails;
                                    sPackageDetails.strID = strServerPackageID;
                                    sPackageDetails.strURL = strPackageUrl;
                                    sPackageDetails.strChecksum = strServerPackageChecksum;
                                    sPackageDetails.udwSize = udwPackageSize;
                                    mPackageDetails.push_back(sPackageDetails);
                                    break;
                                }
                                
                                pFileEl = Core::XMLUtils::NextSiblingElementWithName(pFileEl);
                            }
                        }
                        
                        mapPackageIDToChecksum.erase(it);
                    }
                    else
                    {
                        //Couldn't find the package in the local manifest if it isn't
                        //in the bundle then we need to download it
                        AddToDownloadListIfNotInBundle(pServerPackageEl);
                    }
                    
                    //On to the next package
                    pServerPackageEl = Core::XMLUtils::NextSiblingElementWithName(pServerPackageEl);
                }
               
                //Any packages left in the local manifest need to be removed
                for(std::unordered_map<std::string, std::string>::iterator it = mapPackageIDToChecksum.begin(); it != mapPackageIDToChecksum.end(); ++it)
                {
                    mRemovePackageIDs.push_back(it->first);
                }
            }
            
            //Notify the delegate of our completion and whether the need to update anything
            bool bRequiresUpdating = (!mRemovePackageIDs.empty() || !mPackageDetails.empty());
            
            if(bRequiresUpdating && mbDLCCachePurged)
            {
                mOnUpdateCheckCompleteDelegate(UpdateResult::k_updateAvailableBlocking);
            }
            else if(bRequiresUpdating && !mbDLCCachePurged)
            {
                mOnUpdateCheckCompleteDelegate(UpdateResult::k_updateAvailable);
            }
            else
            {
                mOnUpdateCheckCompleteDelegate(UpdateResult::k_updateNotAvailable);
            }
            
            CS_SAFE_DELETE(pCurrentManifest);
        }
        //-----------------------------------------------------------
        /// Add To Download List if not in Bundle
        ///
        /// The package may be outdated in documents but are
        /// all the files in bundle up to date
        ///
        /// @param Package element
        //-----------------------------------------------------------
        void ContentManagementSystem::AddToDownloadListIfNotInBundle(TiXmlElement* pPackageEl) 
        {
            //Check all the file names
            TiXmlElement* pFileEl = Core::XMLUtils::FirstChildElementWithName(pPackageEl, "File");
            while(pFileEl)
            {
                std::string strFileName = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pFileEl, "Name", "");
                std::string strChecksum = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pFileEl, "Checksum", "");
                std::string strPackageID = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pPackageEl, "ID", "");
                
                if(!DoesFileExist(strPackageID + "/" + strFileName, strChecksum, true))
                {
                    //It doesn't exist in the bundle either!
                    std::string strPackageUrl = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pPackageEl, "URL", "");
                    std::string strPackageChecksum = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pPackageEl, "Checksum", "");
                    
                    u32 udwPackageSize = Core::XMLUtils::GetAttributeValueOrDefault<u32>(pPackageEl, "Size", 0);
					muRunningToDownloadTotal += udwPackageSize;
                    
					CS_DEBUG_LOG("CMS: " + strPackageID + " package content is different on server than on device");
                    CS_DEBUG_LOG("CMS: " + strPackageID + " package requires updating of size : " + Core::ToString(udwPackageSize));
                    
                    PackageDetails sPackageDetails;
                    sPackageDetails.strID = strPackageID;
                    sPackageDetails.strURL = strPackageUrl;
                    sPackageDetails.strChecksum = strPackageChecksum;
                    sPackageDetails.udwSize = udwPackageSize;
                    mPackageDetails.push_back(sPackageDetails);
                    //It doesn't matter if all the other files exist we need to pull down this package anyway
                    break;
                }
                else
                {
                    //It exists in the bundle let's remove the old version from DLC cache
                    //Remove old content
                    Core::CApplication::GetFileSystemPtr()->DeleteFile(Core::StorageLocation::k_DLC, strPackageID + "/" + strFileName);
                    
                    //On to the next file
                    pFileEl = Core::XMLUtils::NextSiblingElementWithName(pFileEl);
                }
            }
        }
        //-----------------------------------------------------------
        /// Save Package To File
        ///
        /// Save the zip file to documents directory
        ///
        /// @param Package details
        /// @param Binary zip
        /// @param Whether the file has finished downloading
		/// @return Success
        //-----------------------------------------------------------
        bool ContentManagementSystem::SavePackageToFile(const PackageDetails& insPackageDetails, const std::string& instrZippedPackage, bool inbFullyDownloaded)
        {
            std::string strFile = "Temp/" + insPackageDetails.strID + ".packzip";

            //Append to the file as it can take multiple writes
            Core::FileStreamPtr pFileStream = Core::CApplication::GetFileSystemPtr()->CreateFileStream(Core::StorageLocation::k_DLC, strFile, Core::FileMode::k_writeBinaryAppend);
			pFileStream->Write((s8*)instrZippedPackage.data(), (s32)instrZippedPackage.size());
            pFileStream->Close();
            
            //Check if the full file has been written and perform a checksum validation 
            if(inbFullyDownloaded)
            {
                std::string strChecksum = CalculateChecksum(Core::StorageLocation::k_DLC, strFile);
                CS_DEBUG_LOG("CMS: Package Checksum: " + strChecksum + " Pristine Checksum: " + insPackageDetails.strChecksum);
                if(strChecksum != insPackageDetails.strChecksum)
                {
                    CS_ERROR_LOG("CMS: " + insPackageDetails.strID + " Package download corrupted");
                    return false;
                }
            }
                
            return true;
        }
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
        //-----------------------------------------------------------
        /// Extract Files From Package
        ///
        /// Unzip the package and save all the files to the
        /// documents directory
        ///
        /// @param Zipped package
        //-----------------------------------------------------------
        void ContentManagementSystem::ExtractFilesFromPackage(const ContentManagementSystem::PackageDetails& insPackageDetails) const
        {
			//Open zip
			std::string strZipFilePath(mstrContentDirectory + "/Temp/" + insPackageDetails.strID + ".packzip");
			
			unzFile ZippedFile = unzOpen(strZipFilePath.c_str());
			if(!ZippedFile)
			{
				CS_ERROR_LOG("CMS: Cannot unzip content package: " + insPackageDetails.strID);
				return;
			}

            //Remove old content before installing the new stuff
            DeleteDirectory(insPackageDetails.strID);
            
            //Go to the first file in the zip
            const u64 uddwFilenameLength = 256;
            s8 byaFileName[uddwFilenameLength];
            
            s32 dwStatus = unzGoToFirstFile(ZippedFile);
            
            while(dwStatus == UNZ_OK)
            {
                //Open the next file
                if (unzOpenCurrentFile(ZippedFile) != UNZ_OK)
                    break;
                
                //Get file information
                unz_file_info FileInfo;
                unzGetCurrentFileInfo(ZippedFile, &FileInfo, byaFileName, uddwFilenameLength, nullptr, 0, nullptr, 0);
                
                //Load the file into memory and then save it out to the directory
                s8* pbyDataBuffer =  new s8[FileInfo.uncompressed_size];
                unzReadCurrentFile(ZippedFile, pbyDataBuffer, FileInfo.uncompressed_size);
                
                //Create new stuff
                std::string strFilePath = std::string(byaFileName);
                if(ContainsDirectoryPath(strFilePath))
                {
                    //There is a nested folder so we need to create the directory structure
                    std::string strPath = GetPathExcludingFileName(strFilePath);
                    Core::CApplication::GetFileSystemPtr()->CreateDirectory(Core::StorageLocation::k_DLC, "/" + strPath);
                }
                
                if(IsFile(strFilePath))
                {
                    Core::CApplication::GetFileSystemPtr()->CreateFile(Core::StorageLocation::k_DLC, "/" + strFilePath, pbyDataBuffer, FileInfo.uncompressed_size);
                }
                
                //Close current file and jump to the next
                CS_SAFE_DELETE_ARRAY(pbyDataBuffer);
                unzCloseCurrentFile(ZippedFile);
                dwStatus = unzGoToNextFile(ZippedFile);
            }
            
            //Close the zip
            unzClose(ZippedFile);
        }
		//-----------------------------------------------------------
		/// Get Running Total To Download
		///
		/// Returns the current running total of the size of data to download
		///
		/// @return The size of the data needing to be downloaded
		//-----------------------------------------------------------
		u32 ContentManagementSystem::GetRunningTotalToDownload()
		{
			return muRunningToDownloadTotal;
		}
		//-----------------------------------------------------------
		/// Get Running Total Downloaded
		///
		/// Returns the current running total of the size of data downloaded
		///
		/// @return The current running total of the size of data downloaded
		//-----------------------------------------------------------
		u32 ContentManagementSystem::GetRunningTotalDownloaded()
		{
			return muRunningDownloadedTotal + mpContentDownloader->GetCurrentDownloadedBytes();
		}
        //-----------------------------------------------------------
        /// Does File Exist
        ///
        /// Checks whether the file is within the application and if the
        /// the checksums match
        ///
        /// @param Filename
        /// @param Checksum
        /// @param Whether to check only the bundle
        ///
        /// @return Whether the file exists
        //-----------------------------------------------------------
        bool ContentManagementSystem::DoesFileExist(const std::string& instrFilename, const std::string instrChecksum, bool inbCheckOnlyBundle) 
        {
            if(inbCheckOnlyBundle)
            {
                if(Core::CApplication::GetFileSystemPtr()->DoesFileExist(Core::StorageLocation::k_package, Core::CApplication::GetFileSystemPtr()->GetPackageDLCDirectory() + instrFilename))
                {
                    //Check if the file has become corrupted
                    return (CalculateChecksum(Core::StorageLocation::k_package, Core::CApplication::GetFileSystemPtr()->GetPackageDLCDirectory() + instrFilename) == instrChecksum);
                }
                
                return false;
            }
            else
            {
                if(Core::CApplication::GetFileSystemPtr()->DoesFileExist(Core::StorageLocation::k_DLC, instrFilename))
                {
                    //Check if the file has become corrupted
                    return (CalculateChecksum(Core::StorageLocation::k_DLC, instrFilename) == instrChecksum);
                }
                
                return false;
            }
        }
        //-----------------------------------------------------------
        /// Delete Directory
        ///
        /// Deletes a directory from the DLC Storage Location.
        ///
        /// @return The directory
        //-----------------------------------------------------------
        void ContentManagementSystem::DeleteDirectory(const std::string& instrDirectory) const
        {
            ChilliSource::Core::CApplication::GetFileSystemPtr()->DeleteDirectory(Core::StorageLocation::k_DLC, instrDirectory);
        }
    }
}
