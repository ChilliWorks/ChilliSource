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

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_WEB_BASE_WEBVIEW_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_WEB_BASE_WEBVIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Web/Base/WebView.h>

@class WebViewDelegate;
@class UIWebView;
@class UIActivityIndicatorView;
@class UIButton;

namespace ChilliSource
{
	namespace iOS
	{
		class WebView final : public Web::WebView
		{
		public:
            CS_DECLARE_NAMEDTYPE(WebView);
            //-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author S Downie
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------
            /// Displays the website at the given URL in an in-app
            /// web view.
            ///
            /// @author S Downie
            ///
            /// @param The Url.
            /// @param The dismissed delegate.
            /// @param The size of the webview in GUI coordinates.
            /// @param The size of the dismiss button.
            //---------------------------------------------------------
            void Present(const std::string& in_url, const DismissedDelegate& in_delegate, const Core::UnifiedVector2& in_size, f32 in_dismissButtonScale) override;
            //---------------------------------------------------------
            /// Displays the website at the given location on disk in
            /// an in-app web view.
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
            /// @param The file path.
            /// @param The dismissed delegate.
            /// @param The size of the webview in GUI coordinates.
            /// @param The size of the dismiss button.
            //---------------------------------------------------------
            void PresentFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const DismissedDelegate& in_delegate, const Core::UnifiedVector2& in_size, f32 in_dismissButtonScale) override;
            //---------------------------------------------------------
            /// Displays the website at the given Url in an external
            /// browser.
            ///
            /// @author S Downie
            ///
            /// @param The Url.
            //---------------------------------------------------------
            void PresentInExternalBrowser(const std::string& in_url) override;
            //---------------------------------------------------------
            /// Dismiss the web view if it is currently presented.
            ///
            /// @author S Downie
            //---------------------------------------------------------
            void Dismiss() override;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return Whether or not the web view is currently
            /// presented.
            //---------------------------------------------------------
            bool IsPresented() const override;
            //---------------------------------------------------------
            /// Called from the webview delegate to inform the system
            /// that the webview has finished loading. This should not
            /// be called manually by the user.
            ///
            /// @author I Copland
            //---------------------------------------------------------
            void OnViewDidFinishLoad();
            
        private:
            friend Web::WebViewUPtr Web::WebView::Create();
            //---------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //---------------------------------------------------------
            WebView();
            //---------------------------------------------------------
            /// Create a webview instance from the size member and
            /// centre it on-screen.
            ///
            /// @author S Downie
            ///
            /// @param The size.
            //---------------------------------------------------------
            void CreateWebview(const Core::UnifiedVector2& in_size);
            //---------------------------------------------------------
            /// Adds the webview to the main view.
            ///
            /// @author S Downie
            //---------------------------------------------------------
            void Display();
			//---------------------------------------------------------
			/// Create a button that can dismiss the web view.
            ///
            /// @author S Downie
			//---------------------------------------------------------
			void AddDismissButton();
            //---------------------------------------------------------
			/// Adds the activity indicatator while loading the web
            /// view.
            ///
			/// @author S Downie
			//---------------------------------------------------------
			void AddActivityIndicator();
            //---------------------------------------------------------
			/// Removes the acitivity indicator once web view loading
            /// has completed.
			///
            /// @author S Downie
			//---------------------------------------------------------
			void RemoveActivityIndicator();
			//---------------------------------------------------------
			/// Returns true if last PresentFromFile had an anchor in
            /// its file path.
            ///
            /// @author J Dixon
			//---------------------------------------------------------
            bool HasAnchor();
			//---------------------------------------------------------
			/// Returns the anchor string of last PresentFromFile
            ///
            /// @author J Dixon
			//---------------------------------------------------------
            const std::string& GetAnchor() const;
		private:
			
            bool m_isPresented;
            DismissedDelegate m_dismissedDelegate;
			UIWebView* m_webView;
			UIButton * m_dismissButton;
            UIActivityIndicatorView* m_activityIndicator;
			WebViewDelegate* m_webViewDelegate;
			
			Core::Vector2 m_absoluteSize;
			Core::Vector2 m_absolutePosition;
            f32 m_dismissButtonScale;
			
            std::string m_anchor;
		};
	}
}



#endif