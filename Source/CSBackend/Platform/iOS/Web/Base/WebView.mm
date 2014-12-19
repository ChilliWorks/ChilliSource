//
//  WebView.cpp
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

#import <CSBackend/Platform/iOS/Web/Base/WebView.h>

#import <CSBackend/Platform/iOS/Core/File/FileSystem.h>
#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#import <CSBackend/Platform/iOS/Web/Base/WebViewDelegate.h>
#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Base/Screen.h>
#import <ChilliSource/Core/File/FileSystem.h>
#import <ChilliSource/Core/File/FileStream.h>
#import <ChilliSource/Core/String/StringUtils.h>

#import <UIKit/UIKit.h>
#import <QuartzCore/CALayer.h>

namespace CSBackend
{
	namespace iOS
	{
        namespace
        {
            //---------------------------------------------------------
			/// Breaks the given url path into the file path and anchor.
			///
			/// @author Ian Copland
			///
			/// @param The URL.
			/// @param [Out] The file path.
			/// @param [Out] The anchor.
			//---------------------------------------------------------
			void GetFilePathAndAnchor(const std::string& in_combined, std::string& out_filePath, std::string& out_anchor)
            {
                size_t anchorStart = in_combined.find_last_of('#');
                
				if(anchorStart != std::string::npos)
				{
					out_anchor = in_combined.substr(anchorStart, in_combined.size() - anchorStart);
					out_filePath = in_combined.substr(0, anchorStart);
				}
				else
				{
					out_anchor = "";
					out_filePath = in_combined;
				}
            }
        }
        CS_DEFINE_NAMEDTYPE(WebView);
		//-----------------------------------------------
		//-----------------------------------------------
		WebView::WebView()
            : m_webView(nil), m_dismissButton(nil), m_isPresented(false), m_activityIndicator(nil), m_webViewDelegate(nil), m_dismissButtonRelativeSize(0.0f)
		{
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool WebView::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (CSWeb::WebView::InterfaceID == in_interfaceId || WebView::InterfaceID == in_interfaceId);
        }
		//-----------------------------------------------
		//-----------------------------------------------
		void WebView::Present(const std::string& in_url, const CSCore::UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate)
		{
            @autoreleasepool
            {
                CS_ASSERT(m_isPresented == false, "Cannot present a web view while one is already displayed.");
                
                m_isPresented = true;
                m_dismissedDelegate = in_delegate;
                m_dismissButtonRelativeSize = in_dismissButtonRelativeSize;
                
                if(!m_webView)
                {
                    CreateWebview(in_size);
                    
                    m_anchor = "";
                    
                    NSString* urlString = [NSStringUtils newNSStringWithUTF8String:in_url];
                    NSURL* url = [NSURL URLWithString:urlString];
                    [urlString release];
                    
                    [m_webView loadRequest:[NSURLRequest requestWithURL:url]];
                    
                    m_webView.backgroundColor = [UIColor clearColor];
                    m_webView.opaque = NO;
                    
                    AddActivityIndicator();
                }
                
                Display();
            }
		}
		//-----------------------------------------------
		//-----------------------------------------------
		void WebView::PresentFromFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, const CSCore::UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate)
		{
            @autoreleasepool
            {
                CS_ASSERT(m_isPresented == false, "Cannot present a web view while one is already displayed.");
                
                m_isPresented = true;
                m_dismissedDelegate = in_delegate;
                m_dismissButtonRelativeSize = in_dismissButtonRelativeSize;
                
                CreateWebview(in_size);
                
                std::string filePath;
                GetFilePathAndAnchor(in_filePath, filePath, m_anchor);
                
                CSCore::FileSystem* fileSystem = CSCore::Application::Get()->GetFileSystem();

                std::string fullFilePath;
                switch(in_storageLocation)
                {
                    default:
                        fullFilePath = fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + filePath;
                        break;
                    case CSCore::StorageLocation::k_package:
                        fullFilePath = fileSystem->GetAbsolutePathToFile(CSCore::StorageLocation::k_package, filePath);
                        break;
                    case CSCore::StorageLocation::k_DLC:
                        fullFilePath = fileSystem->GetAbsolutePathToFile(CSCore::StorageLocation::k_DLC, filePath);
                        break;
                }

                std::string strHTMLFileContents;
                CSCore::FileStreamSPtr pHTMLFile = fileSystem->CreateFileStream(in_storageLocation, in_filePath, CSCore::FileMode::k_read);
                pHTMLFile->GetAll(strHTMLFileContents);
                pHTMLFile->Close();
                
                NSString* pstrHTML = [NSStringUtils newNSStringWithUTF8String:strHTMLFileContents];
                NSString* urlString = [NSStringUtils newNSStringWithUTF8String:fullFilePath];
                [m_webView loadHTMLString:pstrHTML baseURL:[NSURL fileURLWithPath:urlString]];
                [urlString release];
                [pstrHTML release];
                
                m_webView.backgroundColor = [UIColor clearColor];
                m_webView.opaque = NO;
                
                AddActivityIndicator();
                
                Display();
            }
		}
        //-----------------------------------------------
		//-----------------------------------------------
		void WebView::PresentInExternalBrowser(const std::string& in_url)
		{
            @autoreleasepool
            {
                NSString* urlString = [NSStringUtils newNSStringWithUTF8String:in_url];
                NSURL* url = [NSURL URLWithString:urlString];
                [urlString release];
                
                [[UIApplication sharedApplication] openURL:url];
            }
		}
        //-----------------------------------------------
		//-----------------------------------------------
		void WebView::Dismiss()
		{
            @autoreleasepool
            {
                if(m_isPresented == true)
                {
                    if (m_activityIndicator != nil)
                    {
                        RemoveActivityIndicator();
                    }
                    
                    if (m_dismissButton != nil)
                    {
                        [m_dismissButton removeFromSuperview];
                        [m_dismissButton release];
                        m_dismissButton = nil;
                    }
                    
                    [m_webViewDelegate release];
                    m_webViewDelegate = nil;
                    
                    [m_webView stopLoading];
                    [m_webView removeFromSuperview];
                    [m_webView release];
                    m_webView = nil;
                    
                    m_isPresented = false;
                    
                    if (m_dismissedDelegate)
                    {
                        DismissedDelegate delegate = m_dismissedDelegate;
                        m_dismissedDelegate = nullptr;
                        delegate();
                    }
                }
            }
		}
        //---------------------------------------------------------
        //---------------------------------------------------------
        bool WebView::IsPresented() const
        {
            return m_isPresented;
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void WebView::OnViewDidFinishLoad()
        {
            if(m_anchor.length() > 0)
            {
                std::string strJavaScript = "window.location.href = '" + m_anchor + "';";
                NSString* nsJavaString = [NSStringUtils newNSStringWithUTF8String:strJavaScript];
                [m_webView stringByEvaluatingJavaScriptFromString:nsJavaString];
                [nsJavaString release];
            }
            
            AddDismissButton();
            RemoveActivityIndicator();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void WebView::OnInit()
        {
            m_screen = CSCore::Application::Get()->GetSystem<CSCore::Screen>();
        }
        //-----------------------------------------------
        //-----------------------------------------------
        void WebView::CreateWebview(const CSCore::UnifiedVector2& in_size)
        {
            CS_ASSERT(m_webView == nil, "Cannot create webview because one already exists!");
            
            CSCore::Vector2 dipsResolution = m_screen->GetResolution() * m_screen->GetInverseDensityScale();
            
            //Create the  view and present it, centered on screen
            m_absoluteSize = (dipsResolution * in_size.GetRelative()) + in_size.GetAbsolute();
            m_absolutePosition = (dipsResolution - m_absoluteSize) / 2.0f;
            m_webView = [[UIWebView alloc] initWithFrame:CGRectMake(m_absolutePosition.x, m_absolutePosition.y, m_absoluteSize.x, m_absoluteSize.y)];
        }
        //-----------------------------------------------
        //-----------------------------------------------
        void WebView::Display()
        {
            UIView* view = [[[[UIApplication sharedApplication] keyWindow] rootViewController] view];
            [view addSubview:m_webView];
            m_webViewDelegate = [[WebViewDelegate alloc] initWithSystem:this];
            m_webView.delegate = m_webViewDelegate;
        }
		//-----------------------------------------------
		//-----------------------------------------------
		void WebView::AddDismissButton()
		{
            CS_ASSERT(m_dismissButton == nil, "Cannot add dismiss button as one already exists.");
            
            UIImage* btnImage = [UIImage imageNamed:@"com_taggames_close_button.png"];
            if(btnImage)
            {
                m_dismissButton = [[UIButton buttonWithType:UIButtonTypeCustom] retain];
                [m_dismissButton addTarget:m_webViewDelegate action:@selector(onDismissButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
                
                f32 size = m_absoluteSize.x * m_dismissButtonRelativeSize;
                [m_dismissButton setFrame:CGRectMake(m_absoluteSize.x - size - 10, 10, size, size)];
                [m_dismissButton setBackgroundImage:btnImage forState:UIControlStateNormal];
                [m_webView addSubview:m_dismissButton];
            }
            else
            {
                m_dismissButton = [[UIButton buttonWithType:UIButtonTypeRoundedRect] retain];
                [m_dismissButton addTarget:m_webViewDelegate action:@selector(onDismissButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
                 
                f32 size = m_absoluteSize.x * m_dismissButtonRelativeSize;
                [m_dismissButton setFrame:CGRectMake(m_absoluteSize.x - size - 10, 10, size, size)];
                [m_dismissButton setTitle:@"X" forState:UIControlStateNormal];
                [m_dismissButton setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
                [m_webView addSubview:m_dismissButton];
            }
		}
        //-----------------------------------------------
        //-----------------------------------------------
        void WebView::AddActivityIndicator()
        {
            CS_ASSERT(m_activityIndicator == nil, "Cannot add activity indicator as one already exists.");
            
            m_activityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
            m_activityIndicator.center = m_webView.center;
            [m_activityIndicator setBackgroundColor:[UIColor grayColor]];
            m_activityIndicator.layer.cornerRadius = 5;
            [m_activityIndicator startAnimating];
            [m_webView addSubview:m_activityIndicator];
            
        }
        //-----------------------------------------------
        //-----------------------------------------------
        void WebView::RemoveActivityIndicator()
        {
            if (m_activityIndicator != nil)
            {
                [m_activityIndicator removeFromSuperview];
                [m_activityIndicator release];
                m_activityIndicator = nil;
            }
        }
        //------------------------------------------------
        //------------------------------------------------
        void WebView::OnDestroy()
        {
            if (IsPresented() == true)
            {
                Dismiss();
            }
        }
	}
}

#endif