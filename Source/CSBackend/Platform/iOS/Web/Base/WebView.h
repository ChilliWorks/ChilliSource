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

#ifdef CS_TARGETPLATFORM_IOS

#import <ChilliSource/ChilliSource.h>
#import <CSBackend/Platform/iOS/ForwardDeclarations.h>
#import <ChilliSource/Core/File/FileSystem.h>
#import <ChilliSource/Core/Math/UnifiedCoordinates.h>
#import <ChilliSource/Web/Base/WebView.h>

@class WebViewDelegate;
@class UIWebView;
@class UIActivityIndicatorView;
@class UIButton;

namespace CSBackend
{
	namespace iOS
	{
        //-------------------------------------------------------
        /// The iOS backend for the web view.
        ///
        /// @author S Downie
        //-------------------------------------------------------
		class WebView final : public CSWeb::WebView
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
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------
            /// Displays the website at the given URL in an in-app
            /// web view.
            ///
            /// @author S Downie
            ///
            /// @param The Url.
            /// @param The size of the webview in GUI coordinates.
            /// @param The relative size of the dismiss button.
            /// @param The dismissed delegate.
            //---------------------------------------------------------
            void Present(const std::string& in_url, const CSCore::UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate) override;
            //---------------------------------------------------------
            /// Displays the website at the given location on disk in
            /// an in-app web view.
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
            /// @param The file path.
            /// @param The size of the webview in GUI coordinates.
            /// @param The relative size of the dismiss button.
            /// @param The dismissed delegate.
            //---------------------------------------------------------
            void PresentFromFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, const CSCore::UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate) override;
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
            /// @author Ian Copland
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
            /// @author Ian Copland
            //---------------------------------------------------------
            void OnViewDidFinishLoad();
            
        private:
            friend CSWeb::WebViewUPtr CSWeb::WebView::Create();
            //---------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //---------------------------------------------------------
            WebView();
            //---------------------------------------------------------
            /// Called when the owning state is initialised.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------
            void OnInit() override;
            //---------------------------------------------------------
            /// Create a webview instance from the size member and
            /// centre it on-screen.
            ///
            /// @author S Downie
            ///
            /// @param The size.
            //---------------------------------------------------------
            void CreateWebview(const CSCore::UnifiedVector2& in_size);
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
            /// Called when the owning state is destroyed.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------
            void OnDestroy() override;
		private:
			
            CSCore::Screen* m_screen;
            
            bool m_isPresented;
            DismissedDelegate m_dismissedDelegate;
			UIWebView* m_webView;
			UIButton * m_dismissButton;
            UIActivityIndicatorView* m_activityIndicator;
			WebViewDelegate* m_webViewDelegate;
			
			CSCore::Vector2 m_absoluteSize;
			CSCore::Vector2 m_absolutePosition;
            f32 m_dismissButtonRelativeSize;
			
            std::string m_anchor;
		};
	}
}

#endif