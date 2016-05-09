//
//  WebViewJavaInterface.cpp
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

#include <CSBackend/Platform/Android/Main/JNI/Web/Base/WebViewJavaInterface.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <CSBackend/Platform/Android/Main/JNI/Web/Base/WebView.h>

//function definitions
extern "C"
{
	//--------------------------------------------------------------------------------------
	/// Called when the webview is dismissed
	///
	/// @author HMcLaughlin
	///
	/// @param in_env - The jni environment.
	/// @param in_object - The java object calling the function
	/// @param in_index - The request index
	///
	/// @return whether or not the text was accepted.
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_web_WebViewNativeInterface_onWebviewDismissed(JNIEnv* in_env, jobject in_object, u32 in_index);
	//--------------------------------------------------------------------------------------
	/// Called when a link is clicked from an internal webview
	///
	/// @author HMcLaughlin
	///
	/// @param in_env - The jni environment.
	/// @param in_object - The java object calling the function
	/// @param in_index - The request index
	/// @param in_url - The request URL
	///
	/// @return whether or not the link was handled externally.
	//--------------------------------------------------------------------------------------
	bool Java_com_chilliworks_chillisource_web_WebViewNativeInterface_onLinkClicked(JNIEnv* in_env, jobject in_object, u32 in_index, jstring in_url);
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_web_WebViewNativeInterface_onWebviewDismissed(JNIEnv* inpEnv, jobject thiz, u32 inudwIndex)
{
	CSBackend::Android::WebView::OnWebViewDismissed(inudwIndex);
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
bool Java_com_chilliworks_chillisource_web_WebViewNativeInterface_onLinkClicked(JNIEnv* in_env, jobject in_object, u32 in_index, jstring in_url)
{
	return CSBackend::Android::WebView::OnLinkClicked(in_index, CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_url));
}

namespace CSBackend
{
	namespace Android
	{
		void WebViewJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;

			InitCallableStaticMethod("com/chilliworks/chillisource/web/WebViewNativeInterface","Present", "(ILjava/lang/String;IIF)V");
			InitCallableStaticMethod("com/chilliworks/chillisource/web/WebViewNativeInterface","PresentFromFile", "(ILjava/lang/String;IILjava/lang/String;Ljava/lang/String;F)V");
			InitCallableStaticMethod("com/chilliworks/chillisource/web/WebViewNativeInterface","PresentInExternalBrowser", "(Ljava/lang/String;)V");
			InitCallableStaticMethod("com/chilliworks/chillisource/web/WebViewNativeInterface","Dismiss", "(I)V");
		}
		void WebViewJavaInterface::Present(u32 inudwIndex, const std::string& instrURL, const ChilliSource::Vector2& invSize, f32 in_dismissButtonRelativeSize)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("Present");

			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				jstring jstrURL = CreateJStringFromSTDString(instrURL);
				u32 udwIndex = inudwIndex;
				u32 udwWidth = invSize.x;
				u32 udwHeight = invSize.y;
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID,udwIndex,jstrURL,udwWidth,udwHeight, in_dismissButtonRelativeSize);

				//delete local reference
				env->DeleteLocalRef(jstrURL);
			}
		}
		void WebViewJavaInterface::PresentFromFile(u32 inudwIndex, const std::string& instrHTMLContent, const ChilliSource::Vector2& invSize, const std::string& instrBasePath, const std::string& instrAnchor, f32 in_dismissButtonRelativeSize)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("PresentFromFile");

			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				jstring jstrContent = CreateJStringFromSTDString(instrHTMLContent);
				jstring jstrBasePath = CreateJStringFromSTDString(instrBasePath);
				jstring jstrAnchor = CreateJStringFromSTDString(instrAnchor);
				u32 udwIndex = inudwIndex;
				u32 udwWidth = invSize.x;
				u32 udwHeight = invSize.y;
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, udwIndex, jstrContent, udwWidth, udwHeight, jstrBasePath, jstrAnchor, in_dismissButtonRelativeSize);

				//delete local reference
				env->DeleteLocalRef(jstrContent);
				env->DeleteLocalRef(jstrBasePath);
				env->DeleteLocalRef(jstrAnchor);
			}
		}
		void WebViewJavaInterface::PresentInExternalBrowser(const std::string& instrURL)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("PresentInExternalBrowser");

			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				jstring jstrContent = CreateJStringFromSTDString(instrURL);
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID,jstrContent);

				//delete local reference
				env->DeleteLocalRef(jstrContent);
			}
		}
		void WebViewJavaInterface::Dismiss(u32 inudwIndex)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("Dismiss");

			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				u32 udwIndex = inudwIndex;
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, udwIndex);
			}
		}
	}
}

#endif
