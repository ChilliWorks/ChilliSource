//
//  WebView.h
//  Chilli Source
//  Created by Scott Downie on 25/07/2011.
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

#ifndef _CHILLISOURCE_WEB_BASE_WEBVIEW_H_
#define _CHILLISOURCE_WEB_BASE_WEBVIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Core/System/StateSystem.h>

#include <functional>

namespace ChilliSource
{
    //---------------------------------------------------------------
    /// A state system for displaying Web pages, either in application
    /// or in an external browser.
    ///
    /// @author S Downie
    //---------------------------------------------------------------
    class WebView : public StateSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(WebViewActivity);
        //------------------------------------------------------------------------------
        /// A delegate called when the webview is dismissed.
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        using DismissedDelegate = std::function<void()>;
        //------------------------------------------------------------------------------
        /// A delegate called when a link is clicked on an
        /// internal webview.
        ///
        /// @author HMcLaughlin
        ///
        /// @param in_url - URL clicked on
        ///
        /// @return True if the URL will be handled by the
        /// delegate owner, False will allow the webview to
        /// display as normal
        //------------------------------------------------------------------------------
        using CustomLinkHandlerDelegate = std::function<bool(const std::string& in_url)>;
        //------------------------------------------------------------------------------
        /// Displays the website at the given URL in an in-app
        /// web view.
        ///
        /// @author S Downie
        ///
        /// @param in_url - The Url.
        /// @param in_size - The size of the webview in GUI coordinates.
        /// @param in_dismissButtonRelativeSize - The relative size of the dismiss
        /// button.
        /// @param in_delegate - The dismissed delegate.
        /// @param in_customURLClickHandler - The delegate to call when a link is
        /// clicked on the displayed page
        //------------------------------------------------------------------------------
        virtual void Present(const std::string& in_url, const UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate, const CustomLinkHandlerDelegate& in_customLinkHandler = nullptr) = 0;
        //------------------------------------------------------------------------------
        /// Displays the website at the given location on disk in
        /// an in-app web view.
        ///
        /// @author S Downie
        ///
        /// @param in_storageLocation - The storage location.
        /// @param in_filePath - The file path.
        /// @param in_size - The size of the webview in GUI coordinates.
        /// @param in_dismissButtonRelativeSize - The relative size of the dismiss
        /// button.
        /// @param in_delegate - The dismissed delegate.
        /// @param in_customURLClickHandler - The delegate to call when a link is
        /// clicked on the displayed page
        //------------------------------------------------------------------------------
        virtual void PresentFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, const UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate, const CustomLinkHandlerDelegate& in_customLinkHandler = nullptr) = 0;
        //------------------------------------------------------------------------------
        /// Displays the website at the given Url in an external
        /// browser.
        ///
        /// @author S Downie
        ///
        /// @param The Url.
        //------------------------------------------------------------------------------
        virtual void PresentInExternalBrowser(const std::string& in_url) = 0;
        //------------------------------------------------------------------------------
        /// Dismiss the web view if it is currently presented.
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        virtual void Dismiss() = 0;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return Whether or not the web view is currently
        /// presented.
        //------------------------------------------------------------------------------
        virtual bool IsPresented() const = 0;
        
    protected:
        friend class State;
        //------------------------------------------------------------------------------
        /// Create the platform dependent backend.
        ///
        /// @author S Downie
        ///
        /// @return New backend instance
        //------------------------------------------------------------------------------
        static WebViewUPtr Create();
        //------------------------------------------------------------------------------
        /// protected constructor to force use of factory method
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        WebView() = default;
    };
}

#endif
