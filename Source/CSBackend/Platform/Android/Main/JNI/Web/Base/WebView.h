//
//  WebView.h
//  Chilli Source
//  Created by Steven Hendrie on 10/12/12.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_WEB_BASE_WEBVIEW_H_
#define _CSBACKEND_PLATFORM_ANDROID_WEB_BASE_WEBVIEW_H_

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaStaticClass.h>

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Web/Base/WebView.h>

#include <unordered_map>

namespace CSBackend
{
	namespace Android
	{
		//--------------------------------------------------------------------------------------
		/// The Android backend for the web view.
		///
		/// @author S Hendrie
		//--------------------------------------------------------------------------------------
		class WebView final : public ChilliSource::WebView
		{
		public:
            CS_DECLARE_NAMEDTYPE(WebView);
			//--------------------------------------------------------------------------------------
			/// Called when webview has been dismissed. This is for
            /// internal use and should not be called manually by the
            /// user.
            ///
            /// @author S Hendrie
			///
			/// @param index of webview
            //--------------------------------------------------------------------------------------
			static void OnWebViewDismissed(s32 in_index);
            //--------------------------------------------------------------------------------------
            /// Called from the Java webview to inform the system
            /// that a link on the webview has been clicked. This will
            /// pass the link to any external handlers (if any) and
            /// return based on whether it will be handled elsewhere
            ///
            /// This should not be called manually by the user.
            ///
            /// @author HMcLaughlin
            ///
            /// @param in_url - Index of webview
            /// @param in_url - URL Clicked
            ///
            /// @return If this link will be handled elsewhere
            //--------------------------------------------------------------------------------------
            static bool OnLinkClicked(s32 in_index, const std::string& in_url);
            //-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author S Hendrie
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//--------------------------------------------------------------------------------------
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------------------------------------------
            /// Displays the website at the given URL in an in-app
            /// web view.
            ///
            /// @author S Hendrie
            ///
            /// @param in_url - The Url.
            /// @param in_size - The size of the webview in GUI coordinates.
            /// @param in_dismissButtonRelativeSize - The relative size of the dismiss button.
            /// @param in_delegate - The dismissed delegate.
            /// @param in_customLinkHandler - The delegate to call when a link is clicked on
            /// the displayed page
            //--------------------------------------------------------------------------------------
            void Present(const std::string& in_url, const ChilliSource::UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate, const CustomLinkHandlerDelegate& in_customLinkHandler = nullptr) override;
            //--------------------------------------------------------------------------------------
            /// Displays the website at the given location on disk in
            /// an in-app web view.
            ///
            /// @author S Hendrie
            ///
            /// @param in_storageLocation - The storage location.
            /// @param in_filePath - The file path.
            /// @param in_size - The size of the webview in GUI coordinates.
            /// @param in_dismissButtonRelativeSize - The relative size of the dismiss button.
            /// @param in_delegate - The dismissed delegate.
            /// @param in_customLinkHandler - The delegate to call when a link is clicked on
            /// the displayed page
            //--------------------------------------------------------------------------------------
            void PresentFromFile(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, const ChilliSource::UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate, const CustomLinkHandlerDelegate& in_customLinkHandler = nullptr) override;
            //--------------------------------------------------------------------------------------
            /// Displays the website at the given Url in an external
            /// browser.
            ///
            /// @author S Hendrie
            ///
            /// @param The Url.
            //--------------------------------------------------------------------------------------
            void PresentInExternalBrowser(const std::string& in_url) override;
            //--------------------------------------------------------------------------------------
            /// Dismiss the web view if it is currently presented.
            ///
            /// @author S Hendrie
            //--------------------------------------------------------------------------------------
            void Dismiss() override;
            //--------------------------------------------------------------------------------------
            /// @author S Hendrie
            ///
            /// @return Whether or not the web view is currently
            /// presented.
            //--------------------------------------------------------------------------------------
            bool IsPresented() const override;
		private:
            friend ChilliSource::WebViewUPtr ChilliSource::WebView::Create();
            //--------------------------------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------------------------------------
            WebView();
            //--------------------------------------------------------------------------------------
            /// Called when the the owning state is initialised.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
			void OnInit() override;
            //--------------------------------------------------------------------------------------
            /// Called when the webview is dismissed.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
            void OnWebViewDismissed();
            //--------------------------------------------------------------------------------------
            /// Called from the Java webview to inform the system
            /// that a link on the webview has been clicked. This will
            /// pass the link to any external handlers (if any) and
            /// return based on whether it will be handled elsewhere
            ///
            /// @author HMcLaughlin
            ///
            /// @param in_url - URL Clicked
            ///
            /// @return If this link will be handled elsewhere
            //--------------------------------------------------------------------------------------
            bool OnLinkClicked(const std::string& in_url);
            //--------------------------------------------------------------------------------------
            /// Called when the the owning state is destroyed.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
			void OnDestroy() override;
		private:

			ChilliSource::Screen* m_screen;

			bool m_isPresented;
			const s32 m_index;

			DismissedDelegate m_delegate;
            CustomLinkHandlerDelegate m_customLinkHandlerDelegate;

			static s32 s_nextIndex;
			static std::unordered_map<s32, WebView*> s_indexToWebViewMap;

#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY
            JavaStaticClassUPtr m_contentProvider;
#endif
		};

	}
}

#endif

#endif