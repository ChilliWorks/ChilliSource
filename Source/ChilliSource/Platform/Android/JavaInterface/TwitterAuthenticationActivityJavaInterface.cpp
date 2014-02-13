/*
 *  TwitterAuthenticationActivityJavaInterface.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 14/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Platform/Android/JavaInterface/TwitterAuthenticationActivityJavaInterface.h>

//function definitions
extern "C"
{
	void Java_com_taggames_moflow_nativeinterface_CTwitterAuthenticationViewNativeInterface_OnPINComplete(JNIEnv* inpEnv, jobject inpThis, jstring injstrPIN);
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
void Java_com_taggames_moflow_nativeinterface_CTwitterAuthenticationViewNativeInterface_OnPINComplete(JNIEnv* inpEnv, jobject inpThis, jstring injstrPIN)
{
	std::string strPIN = moFlo::AndroidPlatform::_IJavaInterface::CreateSTDStringFromJString(injstrPIN);
	moFlo::AndroidPlatform::SCTwitterAuthenticationActivityJavaInterface::OnPINComplete(strPIN);
}

namespace moFlo
{
	namespace AndroidPlatform
	{

		moFlo::AndroidPlatform::CTwitterAuthenticationActivity* SCTwitterAuthenticationActivityJavaInterface::mpActivity = NULL;
		//-----------------------------------------------
		/// Setup Java Interface
		//-----------------------------------------------
		void SCTwitterAuthenticationActivityJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;

			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CTwitterAuthenticationViewNativeInterface","TwitterAuthViewPresent", "()V");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CTwitterAuthenticationViewNativeInterface","TwitterAuthViewPresentURL", "(Ljava/lang/String;)V");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CTwitterAuthenticationViewNativeInterface","TwitterAuthViewDismiss", "()V");
		}
		//-----------------------------------------------
		/// Register Activity
		//-----------------------------------------------
		void SCTwitterAuthenticationActivityJavaInterface::RegisterActivity(moFlo::AndroidPlatform::CTwitterAuthenticationActivity* inpActivity)
		{
			mpActivity = inpActivity;
		}
		//-----------------------------------------------
		/// Present
		//-----------------------------------------------
		void SCTwitterAuthenticationActivityJavaInterface::Present()
		{
			MethodReference sdMethodRef = GetStaticMethodReference("TwitterAuthViewPresent");

			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();

				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID);
			}
		}
		//-----------------------------------------------
		/// Present URL
		//-----------------------------------------------
		void SCTwitterAuthenticationActivityJavaInterface::PresentURL(const std::string& instrURL)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("TwitterAuthViewPresentURL");

			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				jstring jstrURL = CreateJStringFromSTDString(instrURL);

				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, jstrURL);

				env->DeleteLocalRef(jstrURL);
			}
		}
		//-----------------------------------------------
		/// On PIN Complete
		//-----------------------------------------------
		void SCTwitterAuthenticationActivityJavaInterface::OnPINComplete(const std::string instrPIN)
		{
			if(mpActivity)
			{
				mpActivity->SavePIN(instrPIN);
				mpActivity->OnPINComplete();
			}
		}
	}
}
