/*
 *  WebViewActivity.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 25/07/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Web/Base/WebViewActivity.h>
#include <ChilliSource/Backend/Platform/iOS/Core/File/FileSystem.h>

#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Base/Screen.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <UIKit/UIKit.h>
#include <QuartzCore/CALayer.h>

namespace ChilliSource
{
	namespace iOS
	{
		//-----------------------------------------------
		/// Constructor
		///
		//-----------------------------------------------
		WebViewActivity::WebViewActivity() : mpWebView(nil), mpDismissButton(nil), mvUnifiedSize(1.0f,1.0f,0.0f,0.0f), mpActivityIndicator(nil), mfDismissButtonScale(0.075f)
		{
		}
		//-----------------------------------------------
		/// Present
		/// 
		/// Show the web view and load the given
		/// URL
		///
		/// @param URL
		//-----------------------------------------------
		void WebViewActivity::Present(const std::string& instrURL, f32 infDismissButtonScale)
		{
            mfDismissButtonScale = infDismissButtonScale;
            
			if(!mpWebView)
			{
                CreateWebview();
				
                mstrAnchor = "";
                
				NSURL* pUrl = [NSURL URLWithString:Core::StringUtils::StringToNSString(instrURL)];
				
				[mpWebView loadRequest:[NSURLRequest requestWithURL:pUrl]];
				
				mpWebView.backgroundColor = [UIColor clearColor];
				mpWebView.opaque = NO;
                
                AddActivityIndicator();
			}
            
            Display();
		}
		//-----------------------------------------------
		/// Present From File
		/// 
		/// Show the web view and load the given
		/// html file
		///
		/// @param HTML file name
		//-----------------------------------------------
		void WebViewActivity::PresentFromFile(Core::StorageLocation ineStorageLocation, const std::string& instrFile, f32 infDismissButtonScale)
		{
            mfDismissButtonScale = infDismissButtonScale;
            
			if(!mpWebView)
			{
                CreateWebview();
				
                std::string strFile;
                
                size_t udwAnchorStart = instrFile.find_last_of('#');
                bool bHasAnchor = (udwAnchorStart != std::string::npos);
                
                if(bHasAnchor)
                {
                    mstrAnchor = instrFile.substr(udwAnchorStart, instrFile.size() - udwAnchorStart);
                    strFile = instrFile.substr(0, udwAnchorStart);
                }
                else
                {
                    mstrAnchor = "";
                    strFile = instrFile;
                }
                
				iOS::FileSystem* pFileSystem = static_cast<iOS::FileSystem*>(Core::Application::Get()->GetFileSystem());

				std::string strPath;
                
                switch(ineStorageLocation)
                {
                    default:
                        strPath = pFileSystem->GetAbsolutePathToStorageLocation(ineStorageLocation);
                        break;
                    case Core::StorageLocation::k_package:
                        strPath = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToFile(Core::StorageLocation::k_package, strFile);
                        break;
                    case Core::StorageLocation::k_DLC:
                        strPath = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToFile(Core::StorageLocation::k_DLC, strFile);
                        break;
                }

                std::string strHTMLFileContents;
                Core::FileStreamSPtr pHTMLFile = pFileSystem->CreateFileStream(ineStorageLocation, strFile, Core::FileMode::k_read);
                pHTMLFile->GetAll(strHTMLFileContents);
                
                NSString* pstrHTML = Core::StringUtils::StringToNSString(strHTMLFileContents);

                [mpWebView loadHTMLString:pstrHTML baseURL:[NSURL fileURLWithPath:Core::StringUtils::StringToNSString(strPath)]];
                
                mpWebView.backgroundColor = [UIColor clearColor];
                mpWebView.opaque = NO;
                
                AddActivityIndicator();
			}
            
			Display();
		}
        
        //-----------------------------------------------
        /// Create Webview
        ///
        /// Create a webview instance from the size
        /// member and centre it on-screen
        //-----------------------------------------------
        void WebViewActivity::CreateWebview()
        {
            //Create the  view and present it, centered on screen
            mvAbsoluteSize = (Core::Screen::GetOrientedDimensions() * Core::Screen::GetInverseDensity() * mvUnifiedSize.GetRelative()) + mvUnifiedSize.GetAbsolute();
            mvAbsolutePosition =  (Core::Screen::GetOrientedDimensions() * Core::Screen::GetInverseDensity() - mvAbsoluteSize)/2;
            mpWebView = [[UIWebView alloc] initWithFrame:CGRectMake(mvAbsolutePosition.x, mvAbsolutePosition.y, mvAbsoluteSize.x, mvAbsoluteSize.y)];
        }
        //-----------------------------------------------
		/// Present In External Browser
		///
		/// Opens up device default web browser and displays url
		///
		/// @param URL
		//-----------------------------------------------
		void WebViewActivity::PresentInExternalBrowser(const std::string& instrURL)
		{
			[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:instrURL.c_str()]] ];
		}
        //-----------------------------------------------
        /// Display
        ///
        /// Adds the webview to the main view
        //-----------------------------------------------
        void WebViewActivity::Display()
        {
            UIView* pView = [[[[UIApplication sharedApplication] keyWindow] rootViewController] view];
            [pView addSubview:mpWebView];
            mpWebDelegate = [[UIWebDelegate alloc] init];
            [mpWebDelegate SetCPPDelegate:this];
            mpWebView.delegate = mpWebDelegate;
        }
		//-----------------------------------------------
		/// Dismiss
		/// 
		/// Dismiss the web view interface
		//-----------------------------------------------
		void WebViewActivity::Dismiss()
		{
			if(mpWebView)
			{
                [mpDismissButton removeFromSuperview];
				[mpWebView stopLoading];
				[mpWebView removeFromSuperview];
				[mpWebView release];
				[mpWebDelegate release];
				
				mpWebView = nil;
				mpDismissButton = nil;
				mpWebDelegate = nil;
				
				mOnDismissedEvent.NotifyConnections(this);
			}
		}
		//-----------------------------------------------
		/// Add Dismiss Button
		///
		/// Create a button that can dismiss the web view
		//-----------------------------------------------
		void WebViewActivity::AddDismissButton()
		{
			if(!mpDismissButton)
			{
                UIImage* btnImage = [UIImage imageNamed:@"com_taggames_close_button.png"];
                if(btnImage)
                {
                    mpDismissButton = [UIButton buttonWithType:UIButtonTypeCustom];
                    [mpDismissButton addTarget:mpWebDelegate action:@selector(OnDismissButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
                    
                    f32 fSize = mvAbsoluteSize.x * mfDismissButtonScale;
                    [mpDismissButton setFrame:CGRectMake(mvAbsoluteSize.x - fSize - 10, 10, fSize, fSize)];
                    [mpDismissButton setBackgroundImage:btnImage forState:UIControlStateNormal];
                    [mpWebView addSubview:mpDismissButton];
                }
                else
                {
                    mpDismissButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
                    [mpDismissButton addTarget:mpWebDelegate action:@selector(OnDismissButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
                     
                    f32 fSize = mvAbsoluteSize.x * mfDismissButtonScale;
                    [mpDismissButton setFrame:CGRectMake(mvAbsoluteSize.x - fSize - 10, 10, fSize, fSize)];
                    [mpDismissButton setTitle:@"X" forState:UIControlStateNormal];
                    [mpDismissButton setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
                    [mpWebView addSubview:mpDismissButton];
                }
			}
		}
        //-----------------------------------------------
        /// Add Activity Indicator
        ///
        //-----------------------------------------------
        void WebViewActivity::AddActivityIndicator()
        {
            if(mpActivityIndicator != nil)
                return;
            
            mpActivityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
            mpActivityIndicator.center = mpWebView.center;
            [mpActivityIndicator setBackgroundColor:[UIColor grayColor]];
            mpActivityIndicator.layer.cornerRadius = 5;
            [mpActivityIndicator startAnimating];
            [mpWebView addSubview:mpActivityIndicator];
            
        }
        //-----------------------------------------------
        /// Remove Activity Indicator
        ///
        //-----------------------------------------------
        void WebViewActivity::RemoveActivityIndicator()
        {
            if(mpActivityIndicator == nil)
                return;
            
            [mpActivityIndicator removeFromSuperview];
            [mpActivityIndicator release];
            mpActivityIndicator = nil;
        }
		//-----------------------------------------------
		/// Set Size
		///
		/// Set the size of the webview on the screen
		///
		/// @param Unified Vector
		//-----------------------------------------------
		void WebViewActivity::SetSize(const Core::UnifiedVector2 & invSize)
		{
			mvUnifiedSize = invSize;
		}
		//-----------------------------------------------
		/// Get Size
		///
		/// Get the size of the webview on the screen
		///
		/// return Unified Vector of size
		//-----------------------------------------------
		Core::UnifiedVector2 WebViewActivity::GetSize() const
        {
			return mvUnifiedSize;			
		}
        
        bool WebViewActivity::HasAnchor()
        {
            return (!mstrAnchor.empty());
        }
        
        const std::string& WebViewActivity::GetAnchor() const
        {
            return mstrAnchor;
        }
	}
}

@implementation UIWebDelegate


-(void) SetCPPDelegate:(ChilliSource::iOS::WebViewActivity*) inpDelegate
{
	mpDelegate = inpDelegate;
}

-(void) OnDismissButtonPressed:(id)inpObject
{
	mpDelegate->Dismiss();
}
-(BOOL)webView:(UIWebView*)webView shouldStartLoadWithRequest:(NSURLRequest*)request navigationType:(UIWebViewNavigationType)navigationType 
{
	if(navigationType == UIWebViewNavigationTypeLinkClicked)
	{
        std::string strScheme = ChilliSource::Core::StringUtils::NSStringToString([[request URL] scheme]);
        
        if(strScheme != "file")
        {
            //Load all other links externally
            [[UIApplication sharedApplication] openURL:request.URL];
            return NO;
        }
	}

    return YES;
}
-(void) webViewDidFinishLoad:(UIWebView *)webView
{
    if(mpDelegate->HasAnchor())
    {
        std::string strJavaScript = "window.location.href = '" + mpDelegate->GetAnchor() + "';";
        [webView stringByEvaluatingJavaScriptFromString:ChilliSource::Core::StringUtils::StringToNSString(strJavaScript)];
    }
    
    mpDelegate->AddDismissButton();
    mpDelegate->RemoveActivityIndicator();
}
-(void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    NSLog(@"%@", [error localizedDescription]);
    
    if(mpDelegate)
    {
        mpDelegate->RemoveActivityIndicator();
        mpDelegate->Dismiss();
    }
}
@end
