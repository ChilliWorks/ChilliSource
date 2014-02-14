/*
 *  WebViewActivity.h
 *  Android Template
 *
 *  Created by Steven Hendrie on 10/12/12.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_PLATFORM_ANDROID_WEB_VIEW_ACTIVITY_H_
#define _MO_FLO_PLATFORM_ANDROID_WEB_VIEW_ACTIVITY_H_

#include <ChilliSource/Core/WebViewActivity.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		class CWebViewActivity: public moFlo::IWebViewActivity
		{
		public:
			CWebViewActivity();

			//-----------------------------------------------
			/// Present
			///
			/// Show the web view and load the given
			/// URL
			///
			/// @param URL
			//-----------------------------------------------
			void Present(const std::string& instrURL, f32 infDismissButtonScale = 0.075f);
			//-----------------------------------------------
			/// Present From File
			///
			/// Show the web view and load the given
			/// html file
			///
			/// @param HTML file name
			//-----------------------------------------------
			void PresentFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFile, f32 infDismissButtonScale = 0.075f);
			//-----------------------------------------------
			/// Present In External Browser
			///
			/// Opens up device default web browser and displays url
			///
			/// @param URL
			//-----------------------------------------------
			void PresentInExternalBrowser(const std::string& instrURL);
			//-----------------------------------------------
			/// Dismiss
			///
			/// Dismiss the web view interface
			//-----------------------------------------------
			void Dismiss();
			//-----------------------------------------------
			/// Set Size
			///
			/// Set the size of the webview on the screen
			///
			/// @param Unified Vector of size
			//-----------------------------------------------
			void SetSize(const moFlo::UnifiedVector2 & invSize);
			//-----------------------------------------------
			/// Get Size
			///
			/// Get the size of the webview on the screen
			///
			/// return Unified Vector of size
			//-----------------------------------------------
			moFlo::UnifiedVector2 GetSize() const;
			//-----------------------------------------------
			/// On WebView Dismissed
			///
			/// Called when webview has been dismissed to
			/// the delegate to be called
			///
			/// @param index of webview
			//-----------------------------------------------
			static void OnWebViewDismissed(u32 inudwIndex);
		private:
			//-----------------------------------------------
			/// Add Dismiss Button
			///
			/// Create a button that can dismiss the web view
			/// @param Size of the webview
			//-----------------------------------------------
			void AddDismissButton(f32 infSize);

		private:
			moFlo::UnifiedVector2 mvUnifiedSize;
			moFlo::Core::CVector2 mvAbsoluteSize;
			static u32 msudwCurrentIndex;
			u32 mudwIndex;

			typedef HASH_MAP<u32, CWebViewActivity*> MapIndexToWebView;
			typedef MapIndexToWebView::iterator MapIndexToWebViewItr;

			static MapIndexToWebView mmapIndexToWebView;
		};

	}
}
#endif
