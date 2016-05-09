//
//  MoContentDownloader.h
//  Chilli Source
//  Created by Scott Downie on 23/01/2012.
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

#ifndef _CHILLISOURCE_NETWORKING_MO_CONTENT_DOWNLOADER_H_
#define _CHILLISOURCE_NETWORKING_MO_CONTENT_DOWNLOADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Time.h>
#include <ChilliSource/Networking/ContentDownload/IContentDownloader.h>
#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

namespace ChilliSource
{
    class MoContentDownloader final : public IContentDownloader
    {
    public:
        //----------------------------------------------------------------
        /// Constructor
        ///
        /// @param HTTP request system
        /// @param Asset server URL
        /// @param Dynamic array of tags that determine content
        //----------------------------------------------------------------
        MoContentDownloader(HttpRequestSystem* inpRequestSystem, const std::string& instrAssetServerURL, const std::vector<std::string>& inastrTags);
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
        /// Download the package file from the given URL
        ///
        /// @author HMcLaughlin
        ///
        /// @param in_url - Url to download
        /// @param in_completiondelegate - Delegate to call on completion
        /// @param in_progressDelegate - Download Progress Delegate
        //----------------------------------------------------------------
        void DownloadPackage(const std::string& in_url, const Delegate& in_completiondelegate, const DownloadProgressDelegate& in_progressDelegate);
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
        //------------------------------------------------------------
        /// @author HMcLaughlin
        ///
        /// @return The progress of the current package
        //------------------------------------------------------------
        f32 GetDownloadProgress() const;
        
    private:
        //----------------------------------------------------------------
        /// Triggered when the manifest download has completed
        ///
        /// @author S Downie
        ///
        /// @param Original request
        /// @param Request response
        //----------------------------------------------------------------
        void OnContentManifestDownloadComplete(const HttpRequest* in_request, const HttpResponse& in_response);
        //----------------------------------------------------------------
        /// Triggered when a package download has completed
        ///
        /// @author S Downie
        ///
        /// @param Original request
        /// @param Request response
        //----------------------------------------------------------------
        void OnContentDownloadComplete(const HttpRequest* in_request, const HttpResponse& in_response);
        
    private:
        
        std::vector<std::string> mastrTags;
        
        std::string mstrAssetServerURL;
        Delegate mOnContentManifestDownloadCompleteDelegate;
        Delegate mOnContentDownloadCompleteDelegate;
        
        HttpRequestSystem* mpHttpRequestSystem;
        
        HttpRequest* mpCurrentRequest;
        
        TimerSPtr m_downloadProgressUpdateTimer;
        EventConnectionUPtr m_downloadProgressEventConnection;
    };
}

#endif
