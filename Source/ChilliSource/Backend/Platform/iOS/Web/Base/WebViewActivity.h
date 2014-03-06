/*
 *  WebViewActivity.h
 *  iOSTemplate
 *
 *  Created by Scott Downie on 25/07/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */


#ifndef _MO_FLO_PLATFORM_IOS_WEB_VIEW_ACTIVITY_H_
#define _MO_FLO_PLATFORM_IOS_WEB_VIEW_ACTIVITY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Web/Base/WebViewActivity.h>

#include <Foundation/NSObject.h>
#include <UIKit/UIWebView.h>

@class UIButton;
@class UIWebDelegate;
@class UIActivityIndicatorView;
@protocol UIWebViewDelegate;

namespace ChilliSource
{
	namespace iOS
	{
		class WebViewActivity : public Web::WebViewActivity
		{
		public:

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
			void PresentFromFile(Core::StorageLocation ineStorageLocation, const std::string& instrFile, f32 infDismissButtonScale = 0.075f);
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
			void SetSize(const Core::UnifiedVector2 & invSize);
			//-----------------------------------------------
			/// Get Size
			///
			/// Get the size of the webview on the screen
			///
			/// return Unified Vector of size
			//-----------------------------------------------
			Core::UnifiedVector2 GetSize() const;
            //-----------------------------------------------
            /// Display
            ///
            /// Adds the webview to the main view
            //-----------------------------------------------
            void Display();
			//-----------------------------------------------
			/// Add Dismiss Button
			///
			/// Create a button that can dismiss the web view
			//-----------------------------------------------
			void AddDismissButton();
            //-----------------------------------------------
			/// Add Activity Indicator
			///
			//-----------------------------------------------
			void AddActivityIndicator();
            //-----------------------------------------------
			/// Remove Activity Indicator
			///
			//-----------------------------------------------
			void RemoveActivityIndicator();
			//-----------------------------------------------
			/// Has Anchor
			///
			/// Returns true if last PresentFromFile had
            /// an anchor in its file path
			//-----------------------------------------------
            bool HasAnchor();
			//-----------------------------------------------
			/// Get Anchor
			///
			/// Returns the anchor string of last
            /// PresentFromFile
			//-----------------------------------------------
            const std::string& GetAnchor() const;
            
        private:
            friend Web::WebViewActivityUPtr Web::WebViewActivity::Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            WebViewActivity();
            //-----------------------------------------------
            /// Create Webview
            ///
            /// Create a webview instance from the size
            /// member and centre it on-screen
            //-----------------------------------------------
            void CreateWebview();
            

			
		private:
			
			UIWebView* mpWebView;
			UIButton * mpDismissButton;
            UIActivityIndicatorView* mpActivityIndicator;
			UIWebDelegate* mpWebDelegate;
			
			Core::UnifiedVector2 mvUnifiedSize;
			Core::Vector2 mvAbsoluteSize;
			Core::Vector2 mvAbsolutePosition;
            f32 mfDismissButtonScale;
			
            std::string mstrAnchor;
		};
	}
}

@interface UIWebDelegate : NSObject<UIWebViewDelegate>
{
	ChilliSource::iOS::WebViewActivity* mpDelegate;
}
-(void) SetCPPDelegate:(ChilliSource::iOS::WebViewActivity*) inpDelegate;
-(void) OnDismissButtonPressed:(id)inpObject;

@end

#endif