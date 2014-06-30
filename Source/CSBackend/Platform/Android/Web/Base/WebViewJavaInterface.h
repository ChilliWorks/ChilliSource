//
//  WebViewJavaInterface.h
//  Chilli Source
//  Created by Steven Hendrie on 10/12/2012.
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

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_CORE_WEBVIEW_INTERFACE_
#define _CHILLISOURCE_PLATFORM_ANDROID_CORE_WEBVIEW_INTERFACE_

#include <CSBackend/Platform/Android/Core/JNI/_JavaInterface.h>

namespace CSBackend
{
	namespace Android
	{
		class WebViewJavaInterface : public _IJavaInterface
		{
		public:
			//--------------------------------------------------------------------------------------
			/// Setup Java Interface
			///
			/// Sets up the java interface: stores the Virtual Machine, and Initialises any java
			/// methods callable from native.
			///
			/// @param a pointer to the virtual machine
			//--------------------------------------------------------------------------------------
			static void SetupJavaInterface(JavaVM* inpJavaVM);
			//-----------------------------------------------
			/// Present
			///
			/// Pass the url to the java interface to display
			///
			/// @param index of webview
			/// @param URL
			/// @param size of webview
			/// @param The relative size of the button.
			//-----------------------------------------------
			static void Present(u32 inudwIndex, const std::string& instrURL, const CSCore::Vector2& invSize, f32 in_dismissButtonRelativeSize);
			//-----------------------------------------------
			/// Present from file
			///
			/// Pass the html string to java interface to display
			///
			/// @param index of webview
			/// @param HTML content
			/// @param size of webview
			/// @param Base path of assets
			/// @param Anchor
			/// @param The relative size of the button.
			//-----------------------------------------------
			static void PresentFromFile(u32 inudwIndex, const std::string& instrHTMLContent, const CSCore::Vector2& invSize, const std::string& instrBasePath, const std::string& instrAnchor, f32 in_dismissButtonRelativeSize);
			//-----------------------------------------------
			/// Present from external browser
			///
			/// Display url in external default browser
			///
			/// @param URL
			//-----------------------------------------------
			static void PresentInExternalBrowser(const std::string& instrURL);
			//-----------------------------------------------
			/// Dismiss
			///
			/// Remove the specified webview from the screen
			///
			/// @param index of webview
			//-----------------------------------------------
			static void Dismiss(u32 inudwIndex);
		};

	}
}

#endif

#endif
