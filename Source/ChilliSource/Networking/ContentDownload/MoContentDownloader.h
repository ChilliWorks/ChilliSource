//
//  MoContentDownloader.h
//  MoFlow
//
//  Created by Scott Downie on 23/01/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_NETWORKING_MO_CONTENT_DOWNLOADER_H_
#define _MOFLOW_NETWORKING_MO_CONTENT_DOWNLOADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Networking/ContentDownload/IContentDownloader.h>
#include <ChilliSource/Networking/Http/HttpConnectionSystem.h>

namespace ChilliSource
{
    namespace Networking
    {
        class MoContentDownloader : public IContentDownloader
        {
        public:
            //----------------------------------------------------------------
            /// Constructor
            ///
            /// @param HTTP request system
            /// @param Asset server URL
            /// @param Dynamic array of tags that determine content
            //----------------------------------------------------------------
            MoContentDownloader(HttpConnectionSystem* inpRequestSystem, const std::string& instrAssetServerURL, const std::vector<std::string>& inastrTags);
            //----------------------------------------------------------------
            /// Download Content Manifest
            ///
            /// Pull the .moman file from the server and callback when
            /// the download is complete
            ///
            /// @param Delegate
            /// @return Whether the manifest download has begun
            //----------------------------------------------------------------
            bool DownloadContentManifest(const Delegate& inDelegate);
            //----------------------------------------------------------------
            /// Download Package
            ///
            /// Download the package file from the given URL
            ///
            /// @param URL string
            /// @param Delegate
            //----------------------------------------------------------------
            void DownloadPackage(const std::string& instrURL, const Delegate& inDelegate);
            //----------------------------------------------------------------
            /// Get CurrentDownloaded Bytes
            ///
            /// @return The amount of bytes read by the current request, if any
            //----------------------------------------------------------------
            u32 GetCurrentDownloadedBytes() const;
            //----------------------------------------------------------------
            /// Get Tags
            ///
            /// @return The current tags of this downloader
            //----------------------------------------------------------------
            inline const std::vector<std::string>& GetTags() const { return mastrTags; }
            //----------------------------------------------------------------
            /// Set Tags
            ///
            /// Set the tags for this downloader
            //----------------------------------------------------------------
            inline void SetTags(const std::vector<std::string>& inastrTags) { mastrTags = inastrTags; }
            
        private:
            //----------------------------------------------------------------
            /// On Content Manifest Download Complete
            ///
            /// Triggered when the manifest download has completed
            ///
            /// @param Request response
            /// @param Request result
            //----------------------------------------------------------------
            void OnContentManifestDownloadComplete(HttpRequest* inpRequest, HttpRequest::Result ineResult);
            //----------------------------------------------------------------
            /// On Content Download Complete
            ///
            /// Triggered when a package download has completed
            ///
            /// @param Request response
            /// @param Request result
            //----------------------------------------------------------------
            void OnContentDownloadComplete(HttpRequest* inpRequest, HttpRequest::Result ineResult);
            
        private:
            
            std::vector<std::string> mastrTags;
            
            std::string mstrAssetServerURL;
            Delegate mOnContentManifestDownloadCompleteDelegate;
            Delegate mOnContentDownloadCompleteDelegate;
            
            HttpConnectionSystem* mpHttpConnectionSystem;
            
            HttpRequest* mpCurrentRequest;
        };
    }
}

#endif
