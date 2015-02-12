//
//  FacebookJavaInterface.cpp
//  Chilli Source
//  Created by Robert Henning on 01/05/2012.
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

#ifdef CS_ANDROIDEXTENSION_FACEBOOK

#include <CSBackend/Platform/Android/Extensions/Facebook/FacebookJavaInterface.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Extensions/Facebook/FacebookAuthenticationSystem.h>
#include <CSBackend/Platform/Android/Extensions/Facebook/FacebookPostSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

CSBackend::Android::FacebookAuthenticationSystem* gpAndroidAuthSystem = nullptr;
CSBackend::Android::FacebookPostSystem* gpAndroidPostSystem = nullptr;

//function definitions
extern "C"
{
	void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnAuthenticationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess);
	void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnReadAuthorisationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess);
	void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnWriteAuthorisationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess);
	void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnPostToFeedComplete(JNIEnv* inpEnv, jobject inpThis, s32 in_result);
	void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnPostRequestComplete(JNIEnv* inpEnv, jobject inpThis, s32 in_result);
}

//------------------------------------------------------------
// (Java) OnAuthenticationComplete
//
// Calls when authentication has been achieved either through login or an existing session
//
// @param Java environment
// @param The java object calling the function
// @param The success of the authentication
//------------------------------------------------------------
void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnAuthenticationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess)
{
	if(gpAndroidAuthSystem)
	{
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&CSBackend::Android::FacebookAuthenticationSystem::OnAuthenticationComplete, gpAndroidAuthSystem, inbSuccess));
	}
}
//------------------------------------------------------------
// (Java) OnReadAuthorisationComplete
//
// Calls when the read permissions have been authorised via facebook
//
// @param Java environment
// @param The java object calling the function
// @param The success of the authorised
//------------------------------------------------------------
void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnReadAuthorisationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess)
{
	if(gpAndroidAuthSystem)
	{
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&CSBackend::Android::FacebookAuthenticationSystem::OnAuthoriseReadPermissionsComplete, gpAndroidAuthSystem, inbSuccess));
	}
}
//------------------------------------------------------------
// (Java) OnWriteAuthorisationComplete
//
// Calls when the write permissions have been authorised via facebook
//
// @param Java environment
// @param The java object calling the function
// @param The success of the authorised
//------------------------------------------------------------
void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnWriteAuthorisationComplete(JNIEnv* inpEnv, jobject inpThis, bool inbSuccess)
{
	if(gpAndroidAuthSystem)
	{
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&CSBackend::Android::FacebookAuthenticationSystem::OnAuthoriseWritePermissionsComplete, gpAndroidAuthSystem, inbSuccess));
	}
}
//------------------------------------------------------------
// (Java) OnPostToFeedComplete
//
// Calls when the results of the attempt to post to feed are received
//
// @param Java environment
// @param The java object calling the function
// @param The result of the authorised
//------------------------------------------------------------
void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnPostToFeedComplete(JNIEnv* inpEnv, jobject inpThis, s32 in_result)
{
	if(gpAndroidPostSystem)
	{
		CSSocial::FacebookPostSystem::PostResult result = CSSocial::FacebookPostSystem::PostResult::k_failed;
		switch(in_result)
		{
		case 0:
			result = CSSocial::FacebookPostSystem::PostResult::k_success;
			break;
		case 1:
			result = CSSocial::FacebookPostSystem::PostResult::k_cancelled;
			break;
		default:
			result = CSSocial::FacebookPostSystem::PostResult::k_failed;
			break;
		}

		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&CSBackend::Android::FacebookPostSystem::OnPostToFeedComplete, gpAndroidPostSystem, result));
	}
}
//------------------------------------------------------------
// (Java) OnPostRequestComplete
//
// Calls when the results of the attempt to post a request are received
//
// @param Java environment
// @param The java object calling the function
// @param The result of the authorised
//------------------------------------------------------------
void Java_com_chilliworks_chillisource_facebook_FacebookNativeInterface_OnPostRequestComplete(JNIEnv* inpEnv, jobject inpThis, s32 in_result)
{
	if(gpAndroidPostSystem)
	{
		CSSocial::FacebookPostSystem::PostResult result = CSSocial::FacebookPostSystem::PostResult::k_failed;
		switch(in_result)
		{
		case 0:
			result = CSSocial::FacebookPostSystem::PostResult::k_success;
			break;
		case 1:
			result = CSSocial::FacebookPostSystem::PostResult::k_cancelled;
			break;
		default:
			result = CSSocial::FacebookPostSystem::PostResult::k_failed;
			break;
		}

		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&CSBackend::Android::FacebookPostSystem::OnPostRequestComplete, gpAndroidPostSystem, result));
	}
}


namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(FacebookJavaInterface);

        //--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		FacebookJavaInterface::FacebookJavaInterface()
		{
			CreateNativeInterface("com/chilliworks/chillisource/facebook/FacebookNativeInterface");

			CreateMethodReference("Authenticate", "([Ljava/lang/String;)V");
			CreateMethodReference("IsSignedIn", "()Z");
			CreateMethodReference("GetActiveToken", "()Ljava/lang/String;");
			CreateMethodReference("HasPermission", "(Ljava/lang/String;)Z");
			CreateMethodReference("AuthoriseReadPermissions", "([Ljava/lang/String;)V");
			CreateMethodReference("AuthoriseWritePermissions", "([Ljava/lang/String;)V");
			CreateMethodReference("SignOut", "()V");
			CreateMethodReference("MakeRequestToUser", "([Ljava/lang/String;)V");
			CreateMethodReference("Post", "(Ljava/lang/String;[Ljava/lang/String;)V");
		}
        //--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		bool FacebookJavaInterface::IsA(CSCore::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == FacebookJavaInterface::InterfaceID;
		}
        //--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void FacebookJavaInterface::SetAuthenticationSystem(FacebookAuthenticationSystem* inpAuthSystem)
		{
			gpAndroidAuthSystem = inpAuthSystem;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void FacebookJavaInterface::SetPostSystem(FacebookPostSystem* inpPostSystem)
		{
			gpAndroidPostSystem = inpPostSystem;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void FacebookJavaInterface::Authenticate(const std::vector<std::string>& inaReadPerms)
		{
			JNIEnv* pEnv = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrEmptyString = pEnv->NewStringUTF("");
			jclass jStringClass = pEnv->FindClass("java/lang/String");
	    	jobjectArray jaPermissions = pEnv->NewObjectArray(inaReadPerms.size(), jStringClass, jstrEmptyString);
	    	u32 udwPermissionCount = 0;
	    	for(std::vector<std::string>::const_iterator it = inaReadPerms.begin(); it != inaReadPerms.end(); ++it)
	    	{
	    		jstring jstrPermission = JavaInterfaceUtils::CreateJStringFromSTDString((*it));
	    		pEnv->SetObjectArrayElement(jaPermissions, udwPermissionCount, jstrPermission);
	    		pEnv->DeleteLocalRef(jstrPermission);
	    		++udwPermissionCount;
	    	}

	    	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Authenticate"), jaPermissions);
	    	pEnv->DeleteLocalRef(jstrEmptyString);
	    	pEnv->DeleteLocalRef(jStringClass);
	    	pEnv->DeleteLocalRef(jaPermissions);
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		bool FacebookJavaInterface::IsSignedIn()
		{
			JNIEnv* pEnv = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("IsSignedIn"));
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string FacebookJavaInterface::GetActiveToken()
		{
			JNIEnv* pEnv = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrToken = (jstring)pEnv->CallObjectMethod(GetJavaObject(), GetMethodID("GetActiveToken"));
			std::string strToken = CSBackend::Android::JavaInterfaceUtils::CreateSTDStringFromJString(jstrToken);
			pEnv->DeleteLocalRef(jstrToken);
			return strToken;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
	    bool FacebookJavaInterface::HasPermission(const std::string& instrPermission)
	    {
			JNIEnv* pEnv = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrPermission = CSBackend::Android::JavaInterfaceUtils::CreateJStringFromSTDString(instrPermission);
			bool bHasPermission = pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("HasPermission"), jstrPermission);
			pEnv->DeleteLocalRef(jstrPermission);
			return bHasPermission;
	    }
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
	    void FacebookJavaInterface::AuthoriseReadPermissions(const std::vector<std::string>& inaReadPerms)
	    {
	    	JNIEnv* pEnv = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrEmptyString = pEnv->NewStringUTF("");
			jclass jStringClass = pEnv->FindClass("java/lang/String");
	    	jobjectArray jaPermissions = pEnv->NewObjectArray(inaReadPerms.size(), jStringClass, jstrEmptyString);
	    	u32 udwPermissionCount = 0;
	    	for(std::vector<std::string>::const_iterator it = inaReadPerms.begin(); it != inaReadPerms.end(); ++it)
	    	{
	    		jstring jstrPermission = JavaInterfaceUtils::CreateJStringFromSTDString((*it));
	    		pEnv->SetObjectArrayElement(jaPermissions, udwPermissionCount, jstrPermission);
	    		pEnv->DeleteLocalRef(jstrPermission);
	    		++udwPermissionCount;
	    	}

	    	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("AuthoriseReadPermissions"), jaPermissions);
	    	pEnv->DeleteLocalRef(jstrEmptyString);
	    	pEnv->DeleteLocalRef(jStringClass);
	    	pEnv->DeleteLocalRef(jaPermissions);
	    }
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
	    void FacebookJavaInterface::AuthoriseWritePermissions(const std::vector<std::string>& inaWritePerms)
	    {
	    	JNIEnv* pEnv = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrEmptyString = pEnv->NewStringUTF("");
			jclass jStringClass = pEnv->FindClass("java/lang/String");
	    	jobjectArray jaPermissions = pEnv->NewObjectArray(inaWritePerms.size(), jStringClass, jstrEmptyString);
	    	u32 udwPermissionCount = 0;
	    	for(std::vector<std::string>::const_iterator it = inaWritePerms.begin(); it != inaWritePerms.end(); ++it)
	    	{
	    		jstring jstrPermission = JavaInterfaceUtils::CreateJStringFromSTDString((*it));
	    		pEnv->SetObjectArrayElement(jaPermissions, udwPermissionCount, jstrPermission);
	    		pEnv->DeleteLocalRef(jstrPermission);
	    		++udwPermissionCount;
	    	}

	    	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("AuthoriseWritePermissions"), jaPermissions);
	    	pEnv->DeleteLocalRef(jstrEmptyString);
	    	pEnv->DeleteLocalRef(jStringClass);
	    	pEnv->DeleteLocalRef(jaPermissions);
	    }
		//--------------------------------------------------------------------------------------
	    //--------------------------------------------------------------------------------------
	    void FacebookJavaInterface::SignOut()
	    {
			JNIEnv* pEnv = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("SignOut"));
	    }
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookJavaInterface::Post(const std::string& in_graphPath, const std::vector<std::string>& in_postParams)
        {
	    	JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring emptyString = env->NewStringUTF("");
			jclass stringClass = env->FindClass("java/lang/String");
	    	jobjectArray keyValues = env->NewObjectArray(in_postParams.size(), stringClass, emptyString);
	    	u32 i = 0;
	    	for (const auto& postParam : in_postParams)
	    	{
	    		jstring element = JavaInterfaceUtils::CreateJStringFromSTDString(postParam);
	    		env->SetObjectArrayElement(keyValues, i, element);
	    		env->DeleteLocalRef(element);
	    		++i;
	    	}

    		jstring graphPath = JavaInterfaceUtils::CreateJStringFromSTDString(in_graphPath);
    		env->CallVoidMethod(GetJavaObject(), GetMethodID("Post"), graphPath, keyValues);

	    	env->DeleteLocalRef(keyValues);
	    	env->DeleteLocalRef(graphPath);
	    	env->DeleteLocalRef(stringClass);
	    	env->DeleteLocalRef(emptyString);
        }
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
	    void FacebookJavaInterface::TryPostRequest(const std::vector<std::string>& inaKeyValues)
	    {
	    	JNIEnv* pEnv = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrEmptyString = pEnv->NewStringUTF("");
			jclass jStringClass = pEnv->FindClass("java/lang/String");
	    	jobjectArray jaKeyValues = pEnv->NewObjectArray(inaKeyValues.size(), jStringClass, jstrEmptyString);
	    	u32 i = 0;
	    	for(std::vector<std::string>::const_iterator it = inaKeyValues.begin(); it != inaKeyValues.end(); ++it, ++i)
	    	{
	    		jstring jstrElement = JavaInterfaceUtils::CreateJStringFromSTDString((*it));
	    		pEnv->SetObjectArrayElement(jaKeyValues, i, jstrElement);
	    		pEnv->DeleteLocalRef(jstrElement);
	    	}

    		pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("MakeRequestToUser"), jaKeyValues);
	    	pEnv->DeleteLocalRef(jaKeyValues);
	    	pEnv->DeleteLocalRef(jStringClass);
	    	pEnv->DeleteLocalRef(jstrEmptyString);
	    }
	}
}

#endif

#endif
