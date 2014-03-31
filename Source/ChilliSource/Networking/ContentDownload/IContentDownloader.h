//
//  IContentDownloader.h
//  Chilli Source
//
//  Created by S Downie on 23/01/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_NETWORKING_CONTENTDOWNLOAD_ICONTENTDOWNLOADER_H_
#define _CHILLISOURCE_NETWORKING_CONTENTDOWNLOAD_ICONTENTDOWNLOADER_H_

#include <ChilliSource/ChilliSource.h>

#include <functional>

namespace ChilliSource
{
    namespace Networking
    {
        //----------------------------------------------------------------
        /// An interface for content downloaders. A downloader implementing
        /// this interface should be used in conjunction with the Content
        /// Management System.
        ///
        /// @author S Downie
        //----------------------------------------------------------------
        class IContentDownloader
        {
        public:
            //----------------------------------------------------------
            /// An enum describing the possible results from a download
            /// request.
            ///
            /// @author S Downie
            //----------------------------------------------------------
            enum class Result
            {
                k_succeeded,
                k_failed,
                k_flushed
            };
            //----------------------------------------------------------
            /// A delegate that can be used to get the response from a
            /// download request.
            ///
            /// @param The result of the request
            /// @param The response string.
            ///
            /// @author S Downie
            //----------------------------------------------------------
            typedef std::function<void(Result, const std::string&)> Delegate;
            //----------------------------------------------------------
            /// Pull the .csman file from the server and callback when
            /// the download is complete
            ///
            /// @author S Downie
            ///
            /// @param The delegate.
            /// @return Whether the manifest download has begun.
            //----------------------------------------------------------
            virtual bool DownloadContentManifest(const Delegate& in_delegate) = 0;
            //----------------------------------------------------------
            /// Download the package file from the given URL
            ///
            /// @author S Downie
            ///
            /// @param URL string
            /// @param Delegate
            //---------------------------------------------------------
            virtual void DownloadPackage(const std::string& in_url, const Delegate& in_delegate) = 0;
            //---------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The amount of bytes read by the current request,
            /// if any
            //---------------------------------------------------------
            virtual u32 GetCurrentDownloadedBytes() const = 0;
            //---------------------------------------------------------
            /// The destructor.
            ///
            /// @author S Downie
            //---------------------------------------------------------
            virtual ~IContentDownloader() {}
        };
    }
}

#endif
