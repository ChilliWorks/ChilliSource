//
//  WebView.cpp
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

#include <CSBackend/Platform/Android/Web/Base/WebView.h>

#include <CSBackend/Platform/Android/Core/File/FileSystem.h>
#include <CSBackend/Platform/Android/Web/Base/WebViewJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace CSBackend
{
	namespace Android
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

		s32 WebView::s_nextIndex = 1;
		std::unordered_map<s32, WebView*> WebView::s_indexToWebViewMap;

		CS_DEFINE_NAMEDTYPE(WebView);
		//-----------------------------------------------
		//-----------------------------------------------
		void WebView::OnWebViewDismissed(s32 in_index)
		{
			auto webViewIt = s_indexToWebViewMap.find(in_index);
			if (webViewIt != s_indexToWebViewMap.end())
			{
				webViewIt->second->OnWebViewDismissed();
			}
		}
        //---------------------------------------------------------
        //---------------------------------------------------------
		WebView::WebView()
			: m_index(s_nextIndex++), m_isPresented(false)
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
			CS_ASSERT(m_isPresented == false, "Cannot present a web view while one is already displayed.");

			m_isPresented = true;
			m_delegate = in_delegate;
			CSCore::Vector2 absoluteSize = (m_screen->GetResolution() * in_size.GetRelative()) + in_size.GetAbsolute();

			WebViewJavaInterface::Present(m_index, in_url, absoluteSize, in_dismissButtonRelativeSize);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		void WebView::PresentFromFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, const CSCore::UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate)
		{
			CS_ASSERT(m_isPresented == false, "Cannot present a web view while one is already displayed.");

			std::string anchor;
			std::string filePath;
			GetFilePathAndAnchor(in_filePath, filePath, anchor);

			CSBackend::Android::FileSystem* fileSystem = static_cast<CSBackend::Android::FileSystem*>(CSCore::Application::Get()->GetFileSystem());

			std::string htmlFileContents;
			CSCore::FileStreamUPtr htmlFile = fileSystem->CreateFileStream(in_storageLocation, filePath, CSCore::FileMode::k_read);
			htmlFile->GetAll(htmlFileContents);
			htmlFile->Close();
			htmlFile.reset();

			std::string fullFilePath = fileSystem->GetAbsolutePathToFile(in_storageLocation, filePath);

            switch(in_storageLocation)
            {
                case CSCore::StorageLocation::k_package:
                {
                	fullFilePath = "file:///android_asset/" + fullFilePath;
                    break;
                }
                case CSCore::StorageLocation::k_DLC:
                {
                	if(fileSystem->DoesFileExistInCachedDLC(filePath) == true)
                	{
                		fullFilePath = "file://" + fullFilePath;
                	}
                	else
                	{
                		fullFilePath = "file:///android_asset/" + fullFilePath;
                	}
                    break;
                }
                default:
                {
                	fullFilePath = "file://" + fullFilePath;
            		break;
                }
            }

			u32 offset = fullFilePath.find_last_of("/");
			if(offset != std::string::npos)
			{
				fullFilePath = fullFilePath.substr(0, offset) + "/";
			}

			m_isPresented = true;
			m_delegate = in_delegate;
			CSCore::Vector2 absoluteSize = (m_screen->GetResolution() * in_size.GetRelative()) + in_size.GetAbsolute();
			WebViewJavaInterface::PresentFromFile(m_index, htmlFileContents, absoluteSize, fullFilePath, anchor, in_dismissButtonRelativeSize);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		void WebView::PresentInExternalBrowser(const std::string& in_url)
		{
			WebViewJavaInterface::PresentInExternalBrowser(in_url);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		void WebView::Dismiss()
		{
			WebViewJavaInterface::Dismiss(m_index);
		}
        //---------------------------------------------------------
        //---------------------------------------------------------
        bool WebView::IsPresented() const
        {
        	return m_isPresented;
        }
        //-----------------------------------------------
        //-----------------------------------------------
		void WebView::OnInit()
		{
			m_screen = CSCore::Application::Get()->GetSystem<CSCore::Screen>();
			s_indexToWebViewMap.emplace(m_index, this);
		}
		//---------------------------------------------------------
        //---------------------------------------------------------
        void WebView::OnWebViewDismissed()
        {
        	m_isPresented = false;

        	if (m_delegate != nullptr)
        	{
        		DismissedDelegate delegate = m_delegate;
        		m_delegate = nullptr;
        		delegate();
        	}
        }
        //-----------------------------------------------
        //-----------------------------------------------
		void WebView::OnDestroy()
		{
			if (IsPresented() == true)
			{
				Dismiss();
			}

			s_indexToWebViewMap.erase(m_index);
		}
	}
}

#endif