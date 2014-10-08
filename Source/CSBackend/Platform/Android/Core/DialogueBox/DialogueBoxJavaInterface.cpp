//
//  DialogueBoxJavaInterface.cpp
//  Chilli Source
//  Created by Ian Copland on 04/03/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <CSBackend/Platform/Android/Core/DialogueBox/DialogueBoxJavaInterface.h>

#include <CSBackend/Platform/Android/Core/DialogueBox/DialogueBoxSystem.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Application.h>

#include <jni.h>

//-------------------------------------------------
/// C function declarations
//-------------------------------------------------
extern "C"
{
	void Java_com_chilliworks_chillisource_core_DialogueBoxNativeInterface_onDialogueConfirmPressed(JNIEnv* in_env, jobject in_this, s32 in_id);
	void Java_com_chilliworks_chillisource_core_DialogueBoxNativeInterface_onDialogueCancelPressed(JNIEnv* in_env, jobject in_this, s32 in_id);
}

//-------------------------------------------------
/// Interface function called from java. This is
/// called when a system dialog has it's confirm
/// option pressed.
///
/// @author Ian Copland
///
/// @param The jni environment.
/// @param The java object calling the function
/// @param Dialog ID
//-------------------------------------------------
void Java_com_chilliworks_chillisource_core_DialogueBoxNativeInterface_onDialogueConfirmPressed(JNIEnv* in_env, jobject in_this, s32 in_id)
{
	CSBackend::Android::DialogueBoxSystem* dialogueBoxSystem = CSCore::Application::Get()->GetSystem<CSBackend::Android::DialogueBoxSystem>();
	if (dialogueBoxSystem != nullptr)
	{
		dialogueBoxSystem->OnSystemConfirmDialogueResult((u32)in_id, CSCore::DialogueBoxSystem::DialogueResult::k_confirm);
	}
}
//-------------------------------------------------
/// Interface function called from java. This is
/// called when a system dialog has it's cancel
/// option pressed
///
/// @author Ian Copland
///
/// @param The jni environment.
/// @param The java object calling the function
/// @param Dialog ID
//------------------------------------------------
void Java_com_chilliworks_chillisource_core_DialogueBoxNativeInterface_onDialogueCancelPressed(JNIEnv* in_env, jobject in_this, s32 in_id)
{
	CSBackend::Android::DialogueBoxSystem* dialogueBoxSystem = CSCore::Application::Get()->GetSystem<CSBackend::Android::DialogueBoxSystem>();
	if (dialogueBoxSystem != nullptr)
	{
		dialogueBoxSystem->OnSystemConfirmDialogueResult((u32)in_env, CSCore::DialogueBoxSystem::DialogueResult::k_cancel);
	}
}

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(DialogueBoxJavaInterface);

		//--------------------------------------------------
		//--------------------------------------------------
		DialogueBoxJavaInterface::DialogueBoxJavaInterface()
		{
			CreateNativeInterface("com/chilliworks/chillisource/core/DialogueBoxNativeInterface");
			CreateMethodReference("makeToast", "(Ljava/lang/String;)V");
			CreateMethodReference("showSystemConfirmDialogue", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			CreateMethodReference("showSystemDialogue", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
		}
		//------------------------------------------------------
		//------------------------------------------------------
		bool DialogueBoxJavaInterface::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == DialogueBoxJavaInterface::InterfaceID);
		}
        //------------------------------------------------------
        //------------------------------------------------------
        void DialogueBoxJavaInterface::MakeToast(const std::string& in_text)
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring message = JavaInterfaceUtils::CreateJStringFromSTDString(in_text);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("makeToast"), message);
			env->DeleteLocalRef(message);
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void DialogueBoxJavaInterface::ShowSystemConfirmDialogue(s32 in_dialogID, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel)
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring title = JavaInterfaceUtils::CreateJStringFromSTDString(in_title);
			jstring message = JavaInterfaceUtils::CreateJStringFromSTDString(in_message);
			jstring confirm = JavaInterfaceUtils::CreateJStringFromSTDString(in_confirm);
			jstring cancel = JavaInterfaceUtils::CreateJStringFromSTDString(in_cancel);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("showSystemConfirmDialogue"), in_dialogID, title, message, confirm, cancel);
			env->DeleteLocalRef(title);
			env->DeleteLocalRef(message);
			env->DeleteLocalRef(confirm);
			env->DeleteLocalRef(cancel);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxJavaInterface::ShowSystemDialogue(s32 in_dialogID, const std::string& in_title, const std::string& in_message, const std::string& in_confirm)
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring title = JavaInterfaceUtils::CreateJStringFromSTDString(in_title);
			jstring message = JavaInterfaceUtils::CreateJStringFromSTDString(in_message);
			jstring confirm = JavaInterfaceUtils::CreateJStringFromSTDString(in_confirm);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("showSystemDialogue"), in_dialogID, title, message, confirm);
			env->DeleteLocalRef(title);
			env->DeleteLocalRef(message);
			env->DeleteLocalRef(confirm);
        }
	}
}

#endif
