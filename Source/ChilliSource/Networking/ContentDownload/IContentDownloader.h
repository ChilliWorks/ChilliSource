//
//  IContentDownloader.h
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
            /// The destructor.
            ///
            /// @author S Downie
            //---------------------------------------------------------
            virtual ~IContentDownloader() {}
        };
    }
}

#endif
