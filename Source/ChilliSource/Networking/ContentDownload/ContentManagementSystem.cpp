//
//  ContentManagementSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 04/07/2011.
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

#include <ChilliSource/Networking/ContentDownload/ContentManagementSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Cryptographic/HashMD5.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/File/AppDataStore.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <minizip/unzip.h>

namespace ChilliSource
{
    namespace
    {
        const char k_adsKeyHasCached[] = "_CMSCachedDLC";
        const char k_tempDirectory[] = "_Temp-CMS/";
        const char k_manifestFile[] = "ContentManifest.moman";
        const char k_tempDownloadedManifestFile[] = "DownloadedContentManifestTemp.moman";
        const char k_tempManifestFile[] = "ContentManifestTemp.moman";
        const char k_packageExtension[] = "packzip";
        const char k_packageExtensionFull[] = ".packzip";
        
        const std::string k_tempManifestFilePath = std::string(k_tempDirectory) + k_tempManifestFile;
        
        //--------------------------------------------------------
        /// @author S Downie
        ///
        /// @param A filepath.
        ///
        /// @return Whether or not the given path contains a
        /// directory path.
        //--------------------------------------------------------
        bool ContainsDirectoryPath(const std::string& instrPath)
        {
            return (instrPath.find_first_of("/") != std::string::npos);
        }
        //--------------------------------------------------------
        /// @author S Downie
        ///
        /// @param A filepath.
        ///
        /// @return Whether or not the filepath points to a file.
        //--------------------------------------------------------
        bool IsFile(const std::string& instrPath)
        {
            return (instrPath.find_first_of(".") != std::string::npos);
        }
        //--------------------------------------------------------
        /// @author S Downie
        ///
        /// @param A filepath.
        ///
        /// @return The path of the directory the file is contained
        /// in.
        //--------------------------------------------------------
        std::string GetPathExcludingFileName(const std::string& instrPath)
        {
            auto udwOffset = instrPath.find_last_of("/");
            if(udwOffset != std::string::npos)
            {
                return instrPath.substr(0, udwOffset);
            }
            
            return "";
        }
        //-----------------------------------------------------------
        /// Writes a checksum safe version of a manifest xml to path
        /// i.e. Will remove the Timestamp key from the manifest
        /// before saving, to ensure checksum correctness with any
        /// new manifests downloaded later
        ///
        /// @author HMcLaughlin
        ///
        /// @param in_manifestDoc - Original Manifest XML Document
        /// @param in_filePath - File path to save to
        ///
        /// @return If the saving was successful
        //-----------------------------------------------------------
        bool SaveTempManifest(const XML::Document* in_manifestDoc, const std::string& in_filePath)
        {
            //Clone the manifest before we modify it
            auto xmlString = XMLUtils::ToString(in_manifestDoc);
            auto doc = XMLUtils::ParseDocument(xmlString);
            
            //Remove the TimeStamp attribute as it interferes with the checksum comparison
            auto manifestNode = doc->GetDocument()->first_node();
            if(manifestNode)
            {
                for(rapidxml::xml_attribute<>* attribute = manifestNode->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (XMLUtils::GetName(attribute) == "Timestamp")
                    {
                        manifestNode->remove_attribute(attribute);
                        break;
                    }
                }
            }
            
            return XMLUtils::WriteDocument(doc->GetDocument(), StorageLocation::k_DLC, in_filePath);
        }
        //-----------------------------------------------------------
        /// Deletes a directory from the DLC Storage Location.
        ///
        /// @author S Downie
        //-----------------------------------------------------------
        void DeleteDirectory(const std::string& in_directory)
        {
            Application::Get()->GetFileSystem()->DeleteDirectory(StorageLocation::k_DLC, in_directory);
        }
        //-----------------------------------------------------------
        /// Clears the temp download folder
        ///
        /// @author HMcLaughlin
        //-----------------------------------------------------------
        void ClearTempDownloadFolder()
        {
            DeleteDirectory(k_tempDirectory);
        }
    }
    
    CS_DEFINE_NAMEDTYPE(ContentManagementSystem);
    //--------------------------------------------------------
    //--------------------------------------------------------
    ContentManagementSystemUPtr ContentManagementSystem::Create(IContentDownloader* in_contentDownloader)
    {
        return ContentManagementSystemUPtr(new ContentManagementSystem(in_contentDownloader));
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    ContentManagementSystem::ContentManagementSystem(IContentDownloader* in_contentDownloader)
    :m_contentDownloader(in_contentDownloader)
    ,m_serverManifest(nullptr)
    {
    }
    //------------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::OnInit()
    {
        m_contentDirectory = Application::Get()->GetFileSystem()->GetAbsolutePathToStorageLocation(StorageLocation::k_DLC);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    bool ContentManagementSystem::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == ContentManagementSystem::InterfaceID;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    XMLUPtr ContentManagementSystem::LoadLocalManifest()
    {
        //The manifest lives in the documents directory
        XMLUPtr xml = XMLUtils::ReadDocument(StorageLocation::k_DLC, k_manifestFile);
        if (xml != nullptr && xml->GetDocument() != nullptr)
        {
            XML::Node* rootNode = XMLUtils::GetFirstChildElement(xml->GetDocument());
            
            //If there is no DLC we should check to see if there ever was any
            AppDataStore* ads = Application::Get()->GetSystem<AppDataStore>();
            if(rootNode == nullptr && ads->Contains(k_adsKeyHasCached) == true)
            {
                m_dlcCachePurged = true;
            }
        }
        return xml;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    std::string ContentManagementSystem::GetManifestChecksumForFile(const std::string& in_filename) const
    {
        return CalculateChecksum(StorageLocation::k_DLC, in_filename);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    std::string ContentManagementSystem::CalculateChecksum(StorageLocation in_location, const std::string& in_filePath) const
    {
        if(m_checksumDelegate)
        {
            // Custom checksum calculation
            return m_checksumDelegate(in_location, in_filePath);
        }
        
        std::string checksum = Application::Get()->GetFileSystem()->GetFileChecksumSHA1(in_location, in_filePath);
        StringUtils::ToLowerCase(checksum);
        std::string base64Encoded = BaseEncoding::Base64Encode(checksum);
        StringUtils::ChopTrailingChars(base64Encoded, '=');
        return base64Encoded;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::ClearDownloadData()
    {
        //Clear the old crap
        m_serverManifest.reset();
        m_removePackageIds.clear();
        m_packageDetails.clear();
        m_cachedPackageDetails.clear();
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::CheckForUpdates(const ContentManagementSystem::CheckForUpdateDelegate& in_delegate)
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "This can only be called on the main thread.");
        
        //Clear any stale data from last update check
        ClearDownloadData();
        
        //Have the downloader request the manifest in its own way
        if(m_contentDownloader->DownloadContentManifest(MakeDelegate(this, &ContentManagementSystem::OnContentManifestDownloadComplete)))
        {
            //The request has started successfully
            m_onUpdateCheckCompleteDelegate = in_delegate;
        }
        else
        {
            //The request has failed to start most likely due to internet connection
            CS_LOG_ERROR("CMS: Internet not reachable");
            if(m_dlcCachePurged)
            {
                in_delegate(CheckForUpdatesResult::k_checkFailedBlocking);
            }
            else
            {
                in_delegate(CheckForUpdatesResult::k_checkFailed);
            }
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::DownloadUpdates(const ContentManagementSystem::CompleteDelegate& in_delegate, const DownloadProgressDelegate& in_progressDelegate)
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "This can only be called on the main thread.");
        
        CS_ASSERT(!m_downloadInProgress, "Cannot call DownloadUpdates while updates are being downloaded!");

        m_onDownloadCompleteDelegate = in_delegate;
        m_onDownloadProgressDelegate = in_progressDelegate;
        
        m_currentPackageDownload = 0;
        m_downloadInProgress = true;

        if(!m_packageDetails.empty())
        {
            //Add a temp directory so that the packages are stored atomically and only overwrite
            //the originals on full success
            Application::Get()->GetFileSystem()->CreateDirectoryPath(StorageLocation::k_DLC, k_tempDirectory);
            DownloadPackage(m_currentPackageDownload);
        }
        else
        {
            m_downloadInProgress = false;
            m_onDownloadCompleteDelegate(Result::k_succeeded);
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::DownloadNextPackage()
    {
        m_currentPackageDownload++;
        DownloadPackage(m_currentPackageDownload);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::DownloadPackage(u32 in_packageIndex, bool in_checkCached)
    {
        CS_ASSERT(in_packageIndex < m_packageDetails.size(), "Package index out of range");
        
        const auto& package = m_packageDetails[in_packageIndex];
        bool existsInCache = VectorUtils::Contains<PackageDetails>(m_cachedPackageDetails, package);
        
        if(in_checkCached && existsInCache)
        {
            //We call the progress function for this index
            OnContentDownloadProgress(package.m_url, 1.0f);
            
            m_runningDownloadedTotal += package.m_size;
            
            //Check if this is the last download
            if(in_packageIndex >= (m_packageDetails.size() - 1))
            {
                m_downloadInProgress = false;
                m_onDownloadCompleteDelegate(Result::k_succeeded);
            }
            else
            {
                //Else move on to next package
                DownloadNextPackage();
            }
        }
        else
        {
            m_contentDownloader->DownloadPackage(package.m_url, MakeDelegate(this, &ContentManagementSystem::OnContentDownloadComplete),
                                                 MakeDelegate(this, &ContentManagementSystem::OnContentDownloadProgress));
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::InstallUpdates(const CompleteDelegate& inDelegate)
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "This can only be called on the main thread.");
        
        if(!m_packageDetails.empty() || !m_removePackageIds.empty())
        {
            if(!m_packageDetails.empty())
            {
                //Unzip all the files and overwrite the old manifest
                for (const auto& details : m_packageDetails)
                {
                    ExtractFilesFromPackage(details);
                }
            }
            
            //Remove the temp zips
            ClearTempDownloadFolder();
            
            m_packageDetails.clear();
        
            if(!m_removePackageIds.empty())
            {
                //Remove any unused files from the documents
                for (const auto& packageId : m_removePackageIds)
                {
                    DeleteDirectory(packageId);
                }
            }
            
            //Save the new content manifest
            XMLUtils::WriteDocument(m_serverManifest->GetDocument(), StorageLocation::k_DLC, k_manifestFile);
            
            m_dlcCachePurged = false;
            
            //Store that we have DLC cached. If there is no DLC on next check then 
            //we know the cache has been purged and we have to block on download
            AppDataStore* ads = Application::Get()->GetSystem<AppDataStore>();
            ads->SetValue(k_adsKeyHasCached, true);
            
            //Tell the delegate all is good
            inDelegate(Result::k_succeeded);
        }
        else
        {
            //Tell the delegate all is bad
            inDelegate(Result::k_failed);
        }
        
        ClearDownloadData();
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::OnContentManifestDownloadComplete(IContentDownloader::Result in_result, const std::string& in_manifest)
    {
        switch(in_result)
        {
            case IContentDownloader::Result::k_succeeded:
                m_serverManifestData += in_manifest;
                BuildDownloadList(m_serverManifestData);
                m_serverManifestData.clear();
                break;
            case IContentDownloader::Result::k_failed:
                m_serverManifestData.clear();
                m_dlcCachePurged ? m_onUpdateCheckCompleteDelegate(CheckForUpdatesResult::k_checkFailedBlocking) : m_onUpdateCheckCompleteDelegate(CheckForUpdatesResult::k_checkFailed);
                break;
            case IContentDownloader::Result::k_flushed:
                m_serverManifestData += in_manifest;
                break;
        };
        
        if(in_result != IContentDownloader::Result::k_flushed)
        {
            //Reset the listener
            m_onUpdateCheckCompleteDelegate = nullptr;
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::OnContentDownloadComplete(IContentDownloader::Result in_result, const std::string& in_data)
    {
        switch(in_result)
        {
            case IContentDownloader::Result::k_succeeded:
            {
                if(SavePackageToFile(m_packageDetails[m_currentPackageDownload], in_data, true))
                {
                    m_runningDownloadedTotal += m_packageDetails[m_currentPackageDownload].m_size;
                    
                    //Don't overwrite the old manifest until all the content has been downloaded 
                    if(m_currentPackageDownload >= (m_packageDetails.size() - 1))
                    {
                        m_downloadInProgress = false;
                        m_onDownloadCompleteDelegate(Result::k_succeeded);
                    }
                    else
                    {
                        DownloadNextPackage();
                    }
                    
                    break;
                }
            }
            case IContentDownloader::Result::k_failed:
            {
                if(m_onDownloadCompleteDelegate)
                {
                    m_downloadInProgress = false;
                    m_onDownloadCompleteDelegate(Result::k_failed);
                }
                break;
            }
            case IContentDownloader::Result::k_flushed:
            {
                SavePackageToFile(m_packageDetails[m_currentPackageDownload], in_data, false);
                break;
            }
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::BuildDownloadList(const std::string& in_serverManifest)
    {
        //Validate the server manifest
        m_serverManifest = XMLUtils::ParseDocument(in_serverManifest);
        
        XML::Node* serverManifestRootNode = XMLUtils::GetFirstChildElement(m_serverManifest->GetDocument());
        if(serverManifestRootNode == nullptr)
        {
            CS_LOG_ERROR("CMS: Server content manifest is invalid");
            if(m_dlcCachePurged)
            {
                m_onUpdateCheckCompleteDelegate(CheckForUpdatesResult::k_checkFailedBlocking);
            }
            else
            {
                m_onUpdateCheckCompleteDelegate(CheckForUpdatesResult::k_checkFailed);
            }

            return;
        }
        
        //Check if DLC is enabled
        if(!XMLUtils::GetAttributeValue<bool>(serverManifestRootNode, "DLCEnabled", false))
        {
            m_onUpdateCheckCompleteDelegate(CheckForUpdatesResult::k_notAvailable);
            return;
        }
        
        XMLUPtr currentManifest = LoadLocalManifest();
        
        //If we have not successfully loaded a manifest from file we need to check if any of the assets 
        //are in the bundle and pull down the others
        if(currentManifest == nullptr || XMLUtils::GetFirstChildElement(currentManifest->GetDocument()) == nullptr)
        {
            //Grab all the URL's from the new manifest
            
            XML::Node* serverPackageEl = XMLUtils::GetFirstChildElement(serverManifestRootNode, "Package");
            
            while(serverPackageEl)
            {
                //If the package is not in the bundle it will download
                AddToDownloadListIfNotInBundle(serverPackageEl);
                
                //On to the next package
                serverPackageEl = XMLUtils::GetNextSiblingElement(serverPackageEl, "Package");
            }
        }
        //Lets find out what we need already have in the manifest
        else
        {
            std::unordered_map<std::string, std::string> mapPackageIDToChecksum;
            
            //Store the data from the local manifest to make a comparison with the server manifest
            XML::Node* currentRoot = XMLUtils::GetFirstChildElement(currentManifest->GetDocument());
            if(currentRoot != nullptr)
            {
                //Loop round 
                XML::Node* pLocalPackageEl = XMLUtils::GetFirstChildElement(currentRoot, "Package");
                while(pLocalPackageEl)
                {
                    //Store the local ID's and checksums for comparison later
                    std::string strLocalPackageID = XMLUtils::GetAttributeValue<std::string>(pLocalPackageEl, "ID", "");
                    std::string strLocalPackageChecksum = XMLUtils::GetAttributeValue<std::string>(pLocalPackageEl, "Checksum", "");
                    
                    mapPackageIDToChecksum.insert(std::make_pair(strLocalPackageID, strLocalPackageChecksum));
                    
                    //On to the next package
                    pLocalPackageEl = XMLUtils::GetNextSiblingElement(pLocalPackageEl, "Package");
                }
            }
            
            //Now process the server manifest and see whats different between the two
            XML::Node* pServerPackageEl = XMLUtils::GetFirstChildElement(serverManifestRootNode, "Package");
            while(pServerPackageEl)
            {
                //Store the local ID's and checksums for comparison later
                std::string strServerPackageID = XMLUtils::GetAttributeValue<std::string>(pServerPackageEl, "ID", "");
                std::string strServerPackageChecksum = XMLUtils::GetAttributeValue<std::string>(pServerPackageEl, "Checksum", "");
                std::string strMinVersionForPackage = XMLUtils::GetAttributeValue<std::string>(pServerPackageEl, "MinVersion", "");
                
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
                        XML::Node* pFileEl = XMLUtils::GetFirstChildElement(pServerPackageEl, "File");
                        while(pFileEl)
                        {
                            std::string strFullPath = XMLUtils::GetAttributeValue<std::string>(pFileEl, "Location", "");
                            std::string strChecksum = XMLUtils::GetAttributeValue<std::string>(pFileEl, "Checksum", "");
                            
                            if(strFullPath.empty()) //Maintain backwards compatability with old versions
                            {
                                std::string strFileName = XMLUtils::GetAttributeValue<std::string>(pFileEl, "Name", "");
                                strFullPath = strServerPackageID + "/" + strFileName;
                            }
                            
                            if(!DoesFileExist(strFullPath, strChecksum, false))
                            {
                                std::string strPackageUrl = XMLUtils::GetAttributeValue<std::string>(pServerPackageEl, "URL", "");
                                u32 udwPackageSize = XMLUtils::GetAttributeValue<u32>(pServerPackageEl, "Size", 0);
                                m_runningToDownloadTotal += udwPackageSize;

                                PackageDetails packageDetails;
                                packageDetails.m_id = strServerPackageID;
                                packageDetails.m_url = strPackageUrl;
                                packageDetails.m_checksum = strServerPackageChecksum;
                                packageDetails.m_size = udwPackageSize;
                                m_packageDetails.push_back(packageDetails);
                                break;
                            }
                            
                            pFileEl = XMLUtils::GetNextSiblingElement(pFileEl, "File");
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
                pServerPackageEl = XMLUtils::GetNextSiblingElement(pServerPackageEl, "Package");
            }
           
            //Any packages left in the local manifest need to be removed
            for(std::unordered_map<std::string, std::string>::iterator it = mapPackageIDToChecksum.begin(); it != mapPackageIDToChecksum.end(); ++it)
            {
                m_removePackageIds.push_back(it->first);
            }
        }
        
        //Notify the delegate of our completion and whether the need to update anything
        bool bRequiresUpdating = (!m_removePackageIds.empty() || !m_packageDetails.empty());
        
        if(bRequiresUpdating)
        {
            RefreshIncompleteDownloadInfo();
        }
        
        if(bRequiresUpdating && m_dlcCachePurged)
        {
            m_onUpdateCheckCompleteDelegate(CheckForUpdatesResult::k_availableBlocking);
        }
        else if(bRequiresUpdating && !m_dlcCachePurged)
        {
            m_onUpdateCheckCompleteDelegate(CheckForUpdatesResult::k_available);
        }
        else
        {
            m_onUpdateCheckCompleteDelegate(CheckForUpdatesResult::k_notAvailable);
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::AddToDownloadListIfNotInBundle(XML::Node* in_packageEl)
    {
        //Check all the file names
        XML::Node* pFileEl = XMLUtils::GetFirstChildElement(in_packageEl, "File");
        while(pFileEl)
        {
            const std::string& strFileName = XMLUtils::GetAttributeValue<std::string>(pFileEl, "Name", "");
            const std::string& strLocation = XMLUtils::GetAttributeValue<std::string>(pFileEl, "Location", "");
            const std::string& strChecksum = XMLUtils::GetAttributeValue<std::string>(pFileEl, "Checksum", "");
            const std::string& strPackageID = XMLUtils::GetAttributeValue<std::string>(in_packageEl, "ID", "");
            
            std::string filePath = strLocation.empty() ? strPackageID + "/" + strFileName : strLocation;
            if(!DoesFileExist(filePath, strChecksum, true))
            {
                //It doesn't exist in the bundle either!
                std::string strPackageUrl = XMLUtils::GetAttributeValue<std::string>(in_packageEl, "URL", "");
                std::string strPackageChecksum = XMLUtils::GetAttributeValue<std::string>(in_packageEl, "Checksum", "");
                
                u32 udwPackageSize = XMLUtils::GetAttributeValue<u32>(in_packageEl, "Size", 0);
                m_runningToDownloadTotal += udwPackageSize;
                
                PackageDetails packageDetails;
                packageDetails.m_id = strPackageID;
                packageDetails.m_url = strPackageUrl;
                packageDetails.m_checksum = strPackageChecksum;
                packageDetails.m_size = udwPackageSize;
                m_packageDetails.push_back(packageDetails);
                //It doesn't matter if all the other files exist we need to pull down this package anyway
                break;
            }
            else
            {
                //It exists in the bundle let's remove the old version from DLC cache
                //Remove old content
                Application::Get()->GetFileSystem()->DeleteFile(StorageLocation::k_DLC, filePath);
                
                //On to the next file
                pFileEl = XMLUtils::GetNextSiblingElement(pFileEl, "File");
            }
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    bool ContentManagementSystem::SavePackageToFile(const PackageDetails& in_packageDetails, const std::string& in_zippedPackage, bool in_fullyDownloaded)
    {
        std::string strFile = k_tempDirectory + in_packageDetails.m_id + k_packageExtensionFull;

        //Append to the file as it can take multiple writes
        FileStreamUPtr pFileStream = Application::Get()->GetFileSystem()->CreateFileStream(StorageLocation::k_DLC, strFile, FileMode::k_writeBinaryAppend);
        if (pFileStream == nullptr)
        {
            CS_LOG_ERROR("CMS: " + in_packageDetails.m_id + " Couldn't write package.");
            return false;
        }

        pFileStream->Write((s8*)in_zippedPackage.data(), (s32)in_zippedPackage.size());
        pFileStream.reset();
        
        //Check if the full file has been written and perform a checksum validation 
        if(in_fullyDownloaded)
        {
            std::string strChecksum = CalculateChecksum(StorageLocation::k_DLC, strFile);
            if(strChecksum != in_packageDetails.m_checksum)
            {
                CS_LOG_ERROR("CMS: " + in_packageDetails.m_id + " Package download corrupted");
                return false;
            }
        }
            
        return true;
    }

    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::ExtractFilesFromPackage(const ContentManagementSystem::PackageDetails& in_packageDetails) const
    {
        //Open zip
        std::string strZipFilePath(m_contentDirectory + "/" + k_tempDirectory + in_packageDetails.m_id + k_packageExtensionFull);
        
        unzFile ZippedFile = unzOpen(strZipFilePath.c_str());
        if(!ZippedFile)
        {
            CS_LOG_ERROR("CMS: Cannot unzip content package: " + in_packageDetails.m_id);
            return;
        }

        //Remove old content before installing the new stuff
        DeleteDirectory(in_packageDetails.m_id);
        
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
            CS_ASSERT(FileInfo.uncompressed_size < static_cast<uLong>(std::numeric_limits<u32>::max()), "File is too large. It cannot exceed " + ToString(std::numeric_limits<u32>::max()) + " bytes.");
            
            //Load the file into memory and then save it out to the directory
            s8* pbyDataBuffer =  new s8[FileInfo.uncompressed_size];
            unzReadCurrentFile(ZippedFile, pbyDataBuffer, static_cast<unsigned>(FileInfo.uncompressed_size));
            
            //Create new stuff
            std::string strFilePath = std::string(byaFileName);
            if(ContainsDirectoryPath(strFilePath))
            {
                //There is a nested folder so we need to create the directory structure
                std::string strPath = GetPathExcludingFileName(strFilePath);
                Application::Get()->GetFileSystem()->CreateDirectoryPath(StorageLocation::k_DLC, "/" + strPath);
            }
            
            if(IsFile(strFilePath))
            {
                Application::Get()->GetFileSystem()->WriteFile(StorageLocation::k_DLC, "/" + strFilePath, pbyDataBuffer, static_cast<u32>(FileInfo.uncompressed_size));
            }
            
            //Close current file and jump to the next
            CS_SAFEDELETE_ARRAY(pbyDataBuffer);
            unzCloseCurrentFile(ZippedFile);
            dwStatus = unzGoToNextFile(ZippedFile);
        }
        
        //Close the zip
        unzClose(ZippedFile);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    u32 ContentManagementSystem::GetRunningTotalToDownload() const
    {
        return m_runningToDownloadTotal;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    u32 ContentManagementSystem::GetRunningTotalDownloaded() const
    {
        return m_runningDownloadedTotal;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    IContentDownloader* ContentManagementSystem::GetContentDownloader() const
    {
        return m_contentDownloader;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::SetChecksumDelegate(const ChecksumDelegate& in_delegate)
    {
        m_checksumDelegate = in_delegate;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    bool ContentManagementSystem::DoesFileExist(const std::string& in_filename, const std::string in_checksum, bool in_checkOnlyBundle) const
    {
        if(in_checkOnlyBundle)
        {
            if(Application::Get()->GetFileSystem()->DoesFileExist(StorageLocation::k_package, Application::Get()->GetFileSystem()->GetPackageDLCPath() + in_filename))
            {
                //Check if the file has become corrupted
                return (CalculateChecksum(StorageLocation::k_package, Application::Get()->GetFileSystem()->GetPackageDLCPath() + in_filename) == in_checksum);
            }
            
            return false;
        }
        else
        {
            if(Application::Get()->GetFileSystem()->DoesFileExist(StorageLocation::k_DLC, in_filename))
            {
                //Check if the file has become corrupted
                return (CalculateChecksum(StorageLocation::k_DLC, in_filename) == in_checksum);
            }
            
            return false;
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::OnContentDownloadProgress(const std::string& in_url, f32 in_progress)
    {
        if(m_onDownloadProgressDelegate)
        {
            CS_ASSERT(m_packageDetails.size() > m_currentPackageDownload,
                      "Current package index out of range - " + ToString(m_currentPackageDownload) + ", " + ToString((u32)m_packageDetails.size()));
            
            f32 increment = 1.0f / m_packageDetails.size();
            f32 totalProgress = m_currentPackageDownload * increment + (in_progress * increment);
            
            m_onDownloadProgressDelegate(m_packageDetails[m_currentPackageDownload].m_id, totalProgress);
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::RefreshIncompleteDownloadInfo()
    {
        //Check if the temp file exists before writing it, if it does then we are resuming
        if(!Application::Get()->GetFileSystem()->DoesFileExist(StorageLocation::k_DLC, k_tempManifestFilePath))
        {
            Application::Get()->GetFileSystem()->CreateDirectoryPath(StorageLocation::k_DLC, k_tempDirectory);
            
            //Save the temperary manifest for resuming later
            SaveTempManifest(m_serverManifest->GetDocument(), k_tempManifestFilePath);
        }
        else
        {
            const std::string tempDownloadedContentPath = std::string(k_tempDirectory) + k_tempDownloadedManifestFile;
            
            //We need to compare checksums to see if the downloaded one is different from the cached one
            SaveTempManifest(m_serverManifest->GetDocument(), tempDownloadedContentPath);
            
            const std::string downloadedChecksum = CalculateChecksum(StorageLocation::k_DLC, tempDownloadedContentPath);
            const std::string cachedChecksum = CalculateChecksum(StorageLocation::k_DLC, k_tempManifestFilePath);
            
            //Manifests are different, clear the temperary folder
            if(downloadedChecksum != cachedChecksum)
            {
                //We should purge the cache
                ClearTempDownloadFolder();
            }
            else
            {
                //Verify and store a list of already downloaded packages
                VerifyTemporaryDownloads(k_tempManifestFilePath);
            }
        }
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void ContentManagementSystem::VerifyTemporaryDownloads(const std::string& in_manifestPath)
    {
        auto manifest = XMLUtils::ReadDocument(StorageLocation::k_DLC, in_manifestPath);
        XML::Node* manifestRootNode = XMLUtils::GetFirstChildElement(m_serverManifest->GetDocument());
        
        auto tempPackageFiles = Application::Get()->GetFileSystem()->GetFilePathsWithExtension(StorageLocation::k_DLC, k_tempDirectory, false, k_packageExtension);
        
        const XML::Node* serverPackageEl = XMLUtils::GetFirstChildElement(manifestRootNode, "Package");
        
        std::vector<ContentManagementSystem::PackageDetails> alreadyCachedPackages;
        
        while(serverPackageEl && (tempPackageFiles.size() != alreadyCachedPackages.size()))
        {
            //Check if this is the one were after
            auto id = XMLUtils::GetAttributeValue<std::string>(serverPackageEl, "ID", "");
            CS_ASSERT(!id.empty(), "Cannot be empty!");
            
            //Go through all packages and verify
            for(const auto& packageName : tempPackageFiles)
            {
                std::string name;
                std::string extension;
                
                StringUtils::SplitBaseFilename(packageName, name, extension);
                
                if(id == name)
                {
                    //Validate the file
                    std::string cachedFilePath = k_tempDirectory + packageName;
                    
                    std::string cachedFileChecksum = CalculateChecksum(StorageLocation::k_DLC, cachedFilePath);
                    std::string expectedChecksum = XMLUtils::GetAttributeValue<std::string>(serverPackageEl, "Checksum", "");
                    CS_ASSERT(!expectedChecksum.empty(), "Cannot be empty!");
                    
                    if(cachedFileChecksum == expectedChecksum)
                    {
                        //Add a validated package
                        std::string url = XMLUtils::GetAttributeValue<std::string>(serverPackageEl, "URL", "");
                        u32 packageSize = XMLUtils::GetAttributeValue<u32>(serverPackageEl, "Size", 0);
                        CS_ASSERT(packageSize > 0, "Package size cannot be 0!");
                        
                        PackageDetails details;
                        details.m_url = url;
                        details.m_size = packageSize;
                        details.m_checksum = expectedChecksum;
                        details.m_id = id;
                        
                        alreadyCachedPackages.push_back(details);
                    }
                    else
                    {
                        //Cached package is corrupt/incomplete, remove it
                        Application::Get()->GetFileSystem()->DeleteFile(StorageLocation::k_DLC, cachedFilePath);
                    }
                }
            }
            
            //On to the next package
            serverPackageEl = XMLUtils::GetNextSiblingElement(serverPackageEl, "Package");
        }
        
        m_cachedPackageDetails = alreadyCachedPackages;
    }
}
