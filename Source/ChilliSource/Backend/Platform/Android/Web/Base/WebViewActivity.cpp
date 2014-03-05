/*
 *  WebViewActivity.cpp
 *  Android Template
 *
 *  Created by Steven Hendrie on 10/1/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Web/Base/WebViewActivity.h>

#include <ChilliSource/Backend/Platform/Android/Core/File/FileSystem.h>
#include <ChilliSource/Backend/Platform/Android/Web/Base/WebViewJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
	namespace Android
	{
		u32 WebViewActivity::msudwCurrentIndex = 1;
		WebViewActivity::MapIndexToWebView WebViewActivity::mmapIndexToWebView;
		//-----------------------------------------------
		/// Constructor
		///
		//-----------------------------------------------
		WebViewActivity::WebViewActivity() : mvUnifiedSize(1.0f,1.0f,0.0f,0.0f)
		{
			mudwIndex = msudwCurrentIndex++;
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
			//Transform webview from unified vector to absolute for use in java
			mvAbsoluteSize = (Core::Screen::GetOrientedDimensions() * mvUnifiedSize.GetRelative()) + mvUnifiedSize.GetAbsolute();
			WebViewJavaInterface::Present(mudwIndex,instrURL,mvAbsoluteSize);
			WebViewActivity::mmapIndexToWebView.insert(std::make_pair(mudwIndex,this));
			AddDismissButton(mvAbsoluteSize.x * infDismissButtonScale);
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
			size_t udwAnchorStart = instrFile.find_last_of('#');
			bool bHasAnchor=(udwAnchorStart!=std::string::npos);
			
			std::string strAnchor, strFile;
			
			if(bHasAnchor)
			{
				strAnchor = instrFile.substr(udwAnchorStart, instrFile.size()-udwAnchorStart);
				strFile = instrFile.substr(0,udwAnchorStart);
			}
			else
			{
				strAnchor = "";
				strFile = instrFile;                    
			}

			Android::FileSystem* pFileSystem = static_cast<Android::FileSystem*>(Core::Application::GetFileSystemPtr());

			Core::FileStreamSPtr pHTMLFile = pFileSystem->CreateFileStream(ineStorageLocation, strFile, Core::FileMode::k_read);
			std::string strHTMLFileContents;
			pHTMLFile->GetAll(strHTMLFileContents);
			pHTMLFile->Close();

			std::string strPath;
		    pFileSystem->GetBestPathToFile(ineStorageLocation, strFile, strPath);

            switch(ineStorageLocation)
            {
                case Core::StorageLocation::k_package:
                default:
                	//Remove assets and add this special path
                	strPath = "file:///android_asset/" + strPath;
                    break;
            	case Core::StorageLocation::k_cache:
            		strPath = "file://" + strPath;
            		break;
                case Core::StorageLocation::k_DLC:
                	if(pFileSystem->DoesFileExistInCachedDLC(strFile))
                	{
                		strPath = "file://" + strPath;
                	}
                	else
                	{
                		strPath = "file:///android_asset/" + strPath;
                	}
                    break;
            }

			u32 udwOffset = strPath.find_last_of("/");
			if(udwOffset != std::string::npos)
			{
				strPath = strPath.substr(0, udwOffset) + "/";
			}

            mvAbsoluteSize = (Core::Screen::GetOrientedDimensions() * mvUnifiedSize.GetRelative()) + mvUnifiedSize.GetAbsolute();

			WebViewJavaInterface::PresentFromFile(mudwIndex, strHTMLFileContents, mvAbsoluteSize, strPath, strAnchor);
			WebViewActivity::mmapIndexToWebView.insert(std::make_pair(mudwIndex,this));
			AddDismissButton(mvAbsoluteSize.x * infDismissButtonScale);
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
			WebViewJavaInterface::PresentInExternalBrowser(instrURL);
		}
		//-----------------------------------------------
		/// Dismiss
		///
		/// Dismiss the web view interface
		//-----------------------------------------------
		void WebViewActivity::Dismiss()
		{
			WebViewJavaInterface::Dismiss(mudwIndex);
		}
		//-----------------------------------------------
		/// Add Dismiss Button
		///
		/// Create a button that can dismiss the web view
		/// @param Size of the webview
		//-----------------------------------------------
		void WebViewActivity::AddDismissButton(f32 infSize)
		{
			WebViewJavaInterface::AddDismissButton(mudwIndex, infSize);
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

		void WebViewActivity::OnWebViewDismissed(u32 inudwIndex)
		{
			MapIndexToWebViewItr pIndex = WebViewActivity::mmapIndexToWebView.find(inudwIndex);
			if(pIndex != WebViewActivity::mmapIndexToWebView.end())
			{
				if(pIndex->second)
					pIndex->second->mOnDismissedEvent.NotifyConnections(pIndex->second);
			}
		}
	}
}
