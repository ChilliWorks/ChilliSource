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

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Web/Base/WebView.h>

#include <unordered_map>

namespace CSBackend
{
	namespace Android
	{
		//-------------------------------------------------------
		/// The Android backend for the web view.
		///
		/// @author S Hendrie
		//-------------------------------------------------------
		class WebView final : public CSWeb::WebView
		{
		public:
            CS_DECLARE_NAMEDTYPE(WebView);
			//---------------------------------------------------------
			/// Called when webview has been dismissed. This is for
            /// internal use and should not be called manually by the
            /// user.
            ///
            /// @author S Hendrie
			///
			/// @param index of webview
			//---------------------------------------------------------
			static void OnWebViewDismissed(s32 in_index);
            //-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author S Hendrie
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------
            /// Displays the website at the given URL in an in-app
            /// web view.
            ///
            /// @author S Hendrie
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
            /// @author S Hendrie
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
            /// @author S Hendrie
            ///
            /// @param The Url.
            //---------------------------------------------------------
            void PresentInExternalBrowser(const std::string& in_url) override;
            //---------------------------------------------------------
            /// Dismiss the web view if it is currently presented.
            ///
            /// @author S Hendrie
            //---------------------------------------------------------
            void Dismiss() override;
            //---------------------------------------------------------
            /// @author S Hendrie
            ///
            /// @return Whether or not the web view is currently
            /// presented.
            //---------------------------------------------------------
            bool IsPresented() const override;
		private:
            friend CSWeb::WebViewUPtr CSWeb::WebView::Create();
            //---------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //---------------------------------------------------------
            WebView();
            //---------------------------------------------------------
            /// Called when the the owning state is initialised.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------
			void OnInit() override;
            //---------------------------------------------------------
            /// Called when the webview is dismissed.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------
            void OnWebViewDismissed();
            //---------------------------------------------------------
            /// Called when the the owning state is destroyed.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------
			void OnDestroy() override;
		private:

			CSCore::Screen* m_screen;
			bool m_isPresented;
			const s32 m_index;
			DismissedDelegate m_delegate;

			static s32 s_nextIndex;
			static std::unordered_map<s32, WebView*> s_indexToWebViewMap;
		};

	}
}

#endif

#endif