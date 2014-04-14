/*
 *  WebViewJavaInterface.h
 *  Android Interface
 *
 *  Created by Steven Hendrie on 10/12/12.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */
#ifndef _MOFLOW_PLATFORM_ANDROID_CORE_WEBVIEW_INTERFACE_
#define _MOFLOW_PLATFORM_ANDROID_CORE_WEBVIEW_INTERFACE_

#include <ChilliSource/Backend/Platform/Android/Core/JNI/_JavaInterface.h>

namespace ChilliSource
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
			/// @param The size of the button.
			//-----------------------------------------------
			static void Present(u32 inudwIndex, const std::string& instrURL, const ChilliSource::Core::Vector2& invSize, f32 in_dismissButtonScale);
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
			//-----------------------------------------------
			static void PresentFromFile(u32 inudwIndex, const std::string& instrHTMLContent, const ChilliSource::Core::Vector2& invSize, const std::string& instrBasePath, const std::string& instrAnchor, f32 in_dismissButtonScale);
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
