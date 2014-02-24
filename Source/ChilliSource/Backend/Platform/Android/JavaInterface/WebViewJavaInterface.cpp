/*
 *  WebViewJavaInterface.h
 *  Android Interface
 *
 *  Created by Steven Hendrie on 10/12/12.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include "WebViewJavaInterface.h"
#include <ChilliSource/Platform/Android/WebViewActivity.h>

//function definitions
extern "C"
{
void Java_com_taggames_moflow_nativeinterface_CWebViewNativeInterface_OnWebviewDismissed(JNIEnv* inpEnv, jobject thiz, u32 inudwIndex);
}
//--------------------------------------------------------------------------------------
/// On Update Text
///
/// Interface function called from java. This is called when the keyboard text has been
/// updated.
///
/// @param The jni environment.
/// @param the java object calling the function
/// @param the updated keyboard text
/// @return whether or not the text was accepted.
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CWebViewNativeInterface_OnWebviewDismissed(JNIEnv* inpEnv, jobject thiz, u32 inudwIndex)
{
	ChilliSource::Android::CWebViewActivity::OnWebViewDismissed(inudwIndex);
}

namespace ChilliSource
{
	namespace Android
	{
		void SCWebViewJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;

			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CWebViewNativeInterface","Present", "(ILjava/lang/String;II)V");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CWebViewNativeInterface","PresentFromFile", "(ILjava/lang/String;IILjava/lang/String;Ljava/lang/String;)V");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CWebViewNativeInterface","PresentInExternalBrowser", "(Ljava/lang/String;)V");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CWebViewNativeInterface","Dismiss", "(I)V");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CWebViewNativeInterface","AddDismissButton", "(IF)V");
		}
		void SCWebViewJavaInterface::Present(u32 inudwIndex, const std::string& instrURL, const ChilliSource::Core::CVector2& invSize)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("Present");

			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				jstring jstrURL = CreateJStringFromSTDString(instrURL);
				u32 udwIndex = inudwIndex;
				u32 udwWidth = invSize.x;
				u32 udwHeight = invSize.y;
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID,udwIndex,jstrURL,udwWidth,udwHeight);

				//delete local reference
				env->DeleteLocalRef(jstrURL);
			}
		}
		void SCWebViewJavaInterface::PresentFromFile(u32 inudwIndex, const std::string& instrHTMLContent, const ChilliSource::Core::CVector2& invSize, const std::string& instrBasePath, const std::string& instrAnchor)
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
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, udwIndex, jstrContent, udwWidth, udwHeight, jstrBasePath, jstrAnchor);

				//delete local reference
				env->DeleteLocalRef(jstrContent);
				env->DeleteLocalRef(jstrBasePath);
				env->DeleteLocalRef(jstrAnchor);
			}
		}
		void SCWebViewJavaInterface::PresentInExternalBrowser(const std::string& instrURL)
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
		void SCWebViewJavaInterface::Dismiss(u32 inudwIndex)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("Dismiss");

			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				u32 udwIndex = inudwIndex;
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, udwIndex);
			}
		}
		void SCWebViewJavaInterface::AddDismissButton(u32 inudwIndex, f32 infSize)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("AddDismissButton");

			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				u32 udwIndex = inudwIndex;
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID,udwIndex,infSize);
			}
		}
	}
}
