//
//  ContentDownloader.h
//  MoFlow
//
//  Created by Scott Downie on 23/01/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_NETWORKING_CONTENT_DOWNLOADER_H_
#define _MOFLOW_NETWORKING_CONTENT_DOWNLOADER_H_

#include <ChilliSource/Core/Main/FastDelegate.h>

namespace moFlo
{
    namespace Networking
    {
        namespace ContentDownloader
        {
            enum Result
            {
                SUCCEEDED,
                FAILED,
                FLUSHED
            };
            
            typedef fastdelegate::FastDelegate2<Result, const std::string&> Delegate;
        }
        
        class IContentDownloader
        {
        public:
            virtual ~IContentDownloader(){}
            
            //----------------------------------------------------------------
            /// Download Content Manifest
            ///
            /// Pull the .moman file from the server and callback when
            /// the download is complete
            ///
            /// @param Delegate
            /// @return Whether the manifest download has begun
            //----------------------------------------------------------------
            virtual bool DownloadContentManifest(const ContentDownloader::Delegate& inDelegate) = 0;
            //----------------------------------------------------------------
            /// Download Package
            ///
            /// Download the package file from the given URL
            ///
            /// @param URL string
            /// @param Delegate
            //----------------------------------------------------------------
            virtual void DownloadPackage(const std::string& instrURL, const ContentDownloader::Delegate& inDelegate) = 0;
            //----------------------------------------------------------------
            /// Get CurrentDownloaded Bytes
            ///
            /// @return The amount of bytes read by the current request, if any
            //----------------------------------------------------------------
            virtual u32 GetCurrentDownloadedBytes() const = 0;
        };
    }
}

#endif
