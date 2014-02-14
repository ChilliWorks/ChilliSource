/*
 *  WebViewActivity.cpp
 *  Android Template
 *
 *  Created by Steven Hendrie on 10/1/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Platform/Android/WebViewActivity.h>
#include <ChilliSource/Platform/Android/FileIO/FileSystem.h>

#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Main/Screen.h>
#include <ChilliSource/Core/Main/Application.h>

#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>

#include <ChilliSource/Platform/Android/JavaInterface/WebViewJavaInterface.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		u32 CWebViewActivity::msudwCurrentIndex = 1;
		CWebViewActivity::MapIndexToWebView CWebViewActivity::mmapIndexToWebView;
		//-----------------------------------------------
		/// Constructor
		///
		//-----------------------------------------------
		CWebViewActivity::CWebViewActivity() : mvUnifiedSize(1.0f,1.0f,0.0f,0.0f)
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
		void CWebViewActivity::Present(const std::string& instrURL, f32 infDismissButtonScale)
		{
			//Transform webview from unified vector to absolute for use in java
			mvAbsoluteSize = (Core::CScreen::GetOrientedDimensions() * mvUnifiedSize.GetRelative()) + mvUnifiedSize.GetAbsolute();
			SCWebViewJavaInterface::Present(mudwIndex,instrURL,mvAbsoluteSize);
			CWebViewActivity::mmapIndexToWebView.insert(std::make_pair(mudwIndex,this));
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
		void CWebViewActivity::PresentFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFile, f32 infDismissButtonScale)
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

			AndroidPlatform::CFileSystem* pFileSystem = static_cast<AndroidPlatform::CFileSystem*>(Core::CApplication::GetFileSystemPtr());

			Core::FileStreamPtr pHTMLFile = pFileSystem->CreateFileStream(ineStorageLocation, strFile, Core::FM_READ);
			std::string strHTMLFileContents;
			pHTMLFile->GetAll(strHTMLFileContents);
			pHTMLFile->Close();

			std::string strPath;
		    pFileSystem->GetBestPathToFile(ineStorageLocation, strFile, strPath);

            switch(ineStorageLocation)
            {
                case Core::SL_PACKAGE:
                default:
                	//Remove assets and add this special path
                	strPath = "file:///android_asset/" + strPath;
                    break;
            	case Core::SL_CACHE:
            		strPath = "file://" + strPath;
            		break;
                case Core::SL_DLC:
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

            mvAbsoluteSize = (Core::CScreen::GetOrientedDimensions() * mvUnifiedSize.GetRelative()) + mvUnifiedSize.GetAbsolute();

			SCWebViewJavaInterface::PresentFromFile(mudwIndex, strHTMLFileContents, mvAbsoluteSize, strPath, strAnchor);
			CWebViewActivity::mmapIndexToWebView.insert(std::make_pair(mudwIndex,this));
			AddDismissButton(mvAbsoluteSize.x * infDismissButtonScale);
		}
		//-----------------------------------------------
		/// Present In External Browser
		///
		/// Opens up device default web browser and displays url
		///
		/// @param URL
		//-----------------------------------------------
		void CWebViewActivity::PresentInExternalBrowser(const std::string& instrURL)
		{
			SCWebViewJavaInterface::PresentInExternalBrowser(instrURL);
		}
		//-----------------------------------------------
		/// Dismiss
		///
		/// Dismiss the web view interface
		//-----------------------------------------------
		void CWebViewActivity::Dismiss()
		{
			SCWebViewJavaInterface::Dismiss(mudwIndex);
		}
		//-----------------------------------------------
		/// Add Dismiss Button
		///
		/// Create a button that can dismiss the web view
		/// @param Size of the webview
		//-----------------------------------------------
		void CWebViewActivity::AddDismissButton(f32 infSize)
		{
			SCWebViewJavaInterface::AddDismissButton(mudwIndex, infSize);
		}

		//-----------------------------------------------
		/// Set Size
		///
		/// Set the size of the webview on the screen
		///
		/// @param Unified Vector
		//-----------------------------------------------
		void CWebViewActivity::SetSize(const moFlo::UnifiedVector2 & invSize)
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
		moFlo::UnifiedVector2 CWebViewActivity::GetSize() const {
			return mvUnifiedSize;
		}

		void CWebViewActivity::OnWebViewDismissed(u32 inudwIndex)
		{
			MapIndexToWebViewItr pIndex = CWebViewActivity::mmapIndexToWebView.find(inudwIndex);
			if(pIndex != CWebViewActivity::mmapIndexToWebView.end())
			{
				if(pIndex->second)
					pIndex->second->mOnDismissedEvent.Invoke(pIndex->second);
			}
		}
	}
}
