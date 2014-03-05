/*
 *  TwitterAuthenticationActivityJavaInterface.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 14/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterAuthenticationActivityJavaInterface.h>

//function definitions
extern "C"
{
	void Java_com_chillisource_social_TwitterAuthenticationViewNativeInterface_OnPINComplete(JNIEnv* inpEnv, jobject inpThis, jstring injstrPIN);
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
void Java_com_chillisource_social_TwitterAuthenticationViewNativeInterface_OnPINComplete(JNIEnv* inpEnv, jobject inpThis, jstring injstrPIN)
{
	std::string strPIN = ChilliSource::Android::_IJavaInterface::CreateSTDStringFromJString(injstrPIN);
	ChilliSource::Android::TwitterAuthenticationActivityJavaInterface::OnPINComplete(strPIN);
}

namespace ChilliSource
{
	namespace Android
	{
		ChilliSource::Android::TwitterAuthenticationActivity* TwitterAuthenticationActivityJavaInterface::mpActivity = NULL;
		//-----------------------------------------------
		/// Setup Java Interface
		//-----------------------------------------------
		void TwitterAuthenticationActivityJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;

			InitCallableStaticMethod("com/chillisource/social/TwitterAuthenticationViewNativeInterface","TwitterAuthViewPresent", "()V");
			InitCallableStaticMethod("com/chillisource/social/TwitterAuthenticationViewNativeInterface","TwitterAuthViewPresentURL", "(Ljava/lang/String;)V");
			InitCallableStaticMethod("com/chillisource/social/TwitterAuthenticationViewNativeInterface","TwitterAuthViewDismiss", "()V");
		}
		//-----------------------------------------------
		/// Register Activity
		//-----------------------------------------------
		void TwitterAuthenticationActivityJavaInterface::RegisterActivity(ChilliSource::Android::CTwitterAuthenticationActivity* inpActivity)
		{
			mpActivity = inpActivity;
		}
		//-----------------------------------------------
		/// Present
		//-----------------------------------------------
		void TwitterAuthenticationActivityJavaInterface::Present()
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
		void TwitterAuthenticationActivityJavaInterface::PresentURL(const std::string& instrURL)
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
		void TwitterAuthenticationActivityJavaInterface::OnPINComplete(const std::string instrPIN)
		{
			if(mpActivity)
			{
				mpActivity->SavePIN(instrPIN);
				mpActivity->OnPINComplete();
			}
		}
	}
}
