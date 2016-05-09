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

#include <CSBackend/Platform/Android/Main/JNI/Web/Base/WebView.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/File/FileSystem.h>
#include <CSBackend/Platform/Android/Main/JNI/Web/Base/WebViewJavaInterface.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/File/TaggedFilePathResolver.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
			//--------------------------------------------------------------------------------------
			/// Breaks the given url path into the file path and anchor.
			///
			/// @author Ian Copland
			///
			/// @param in_combined - The URL.
			/// @param out_filePath - [Out] The file path.
			/// @param out_anchor - [Out] The anchor.
			//--------------------------------------------------------------------------------------
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

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void WebView::OnWebViewDismissed(s32 in_index)
		{
			auto webViewIt = s_indexToWebViewMap.find(in_index);
			if (webViewIt != s_indexToWebViewMap.end())
			{
				webViewIt->second->OnWebViewDismissed();
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
        bool WebView::OnLinkClicked(s32 in_index, const std::string& in_url)
        {
			auto webViewIt = s_indexToWebViewMap.find(in_index);
            if (webViewIt != s_indexToWebViewMap.end())
            {
            	return webViewIt->second->OnLinkClicked(in_url);
            }

            return false;
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		WebView::WebView()
			: m_index(s_nextIndex++), m_isPresented(false)
		{
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool WebView::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (ChilliSource::WebView::InterfaceID == in_interfaceId || WebView::InterfaceID == in_interfaceId);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void WebView::Present(const std::string& in_url, const ChilliSource::UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate, const CustomLinkHandlerDelegate& in_customLinkHandler)
		{
			CS_ASSERT(m_isPresented == false, "Cannot present a web view while one is already displayed.");

			m_isPresented = true;
			m_delegate = in_delegate;
			m_customLinkHandlerDelegate = in_customLinkHandler;

			ChilliSource::Vector2 absoluteSize = (m_screen->GetResolution() * in_size.GetRelative()) + in_size.GetAbsolute();

			WebViewJavaInterface::Present(m_index, in_url, absoluteSize, in_dismissButtonRelativeSize);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void WebView::PresentFromFile(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, const ChilliSource::UnifiedVector2& in_size, f32 in_dismissButtonRelativeSize, const DismissedDelegate& in_delegate, const CustomLinkHandlerDelegate& in_customLinkHandler)
		{
			CS_ASSERT(m_isPresented == false, "Cannot present a web view while one is already displayed.");

			auto fileSystem = static_cast<CSBackend::Android::FileSystem*>(ChilliSource::Application::Get()->GetFileSystem());

			std::string anchor;
			std::string filePath;
			GetFilePathAndAnchor(in_filePath, filePath, anchor);
			auto taggedFilePath = ChilliSource::Application::Get()->GetTaggedFilePathResolver()->ResolveFilePath(in_storageLocation, filePath);

			std::string htmlFileContents;
			if (fileSystem->ReadFile(in_storageLocation, taggedFilePath, htmlFileContents) == false)
			{
				CS_LOG_ERROR("Could not open WebView: " + taggedFilePath);
				return;
			}

			std::string fullFilePath;
			if (in_storageLocation == ChilliSource::StorageLocation::k_package || in_storageLocation == ChilliSource::StorageLocation::k_package ||
            	(in_storageLocation == ChilliSource::StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(taggedFilePath) == false))
			{
#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY
				fullFilePath = m_contentProvider->CallStringMethod("getContentPathPrefix") + fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + taggedFilePath;
#elif defined(CS_ANDROIDFLAVOUR_AMAZON)
				fullFilePath = "file:///android_asset/" + fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + taggedFilePath;
#else
				CS_LOG_FATAL("WebView doesn't support this Android flavour.");
#endif
			}
			else
			{
				fullFilePath = "file://" + fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + taggedFilePath;
			}

			//Get the directory.
			u32 offset = fullFilePath.find_last_of("/");
			if(offset != std::string::npos)
			{
				fullFilePath = fullFilePath.substr(0, offset) + "/";
			}

			m_isPresented = true;
			m_delegate = in_delegate;
			m_customLinkHandlerDelegate = in_customLinkHandler;

			ChilliSource::Vector2 absoluteSize = (m_screen->GetResolution() * in_size.GetRelative()) + in_size.GetAbsolute();
			WebViewJavaInterface::PresentFromFile(m_index, htmlFileContents, absoluteSize, fullFilePath, anchor, in_dismissButtonRelativeSize);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void WebView::PresentInExternalBrowser(const std::string& in_url)
		{
			WebViewJavaInterface::PresentInExternalBrowser(in_url);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void WebView::Dismiss()
		{
			WebViewJavaInterface::Dismiss(m_index);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
        bool WebView::IsPresented() const
        {
        	return m_isPresented;
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void WebView::OnInit()
		{
			m_screen = ChilliSource::Application::Get()->GetSystem<ChilliSource::Screen>();
			s_indexToWebViewMap.emplace(m_index, this);

#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY
			JavaStaticClassDef classDef("com/chilliworks/chillisource/core/ApkExpansionContentProvider");
			classDef.AddMethod("getContentPathPrefix", "()Ljava/lang/String;");
			m_contentProvider = JavaStaticClassUPtr(new JavaStaticClass(classDef));
#endif
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
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
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
        bool WebView::OnLinkClicked(const std::string& in_url)
        {
			if(m_customLinkHandlerDelegate)
			{
				return m_customLinkHandlerDelegate(in_url);
			}

			return false;
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
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