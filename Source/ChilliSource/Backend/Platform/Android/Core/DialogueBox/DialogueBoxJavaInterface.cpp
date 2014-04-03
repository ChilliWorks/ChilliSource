//
//  DialogueBoxJavaInterface.h
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Core/DialogueBox/DialogueBoxJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/DialogueBox/DialogueBoxSystem.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Application.h>

#include <jni.h>

//-------------------------------------------------
/// C function declarations
//-------------------------------------------------
extern "C"
{
	void Java_com_chillisource_core_DialogueBoxNativeInterface_onDialogueConfirmPressed(JNIEnv* in_env, jobject in_this, s32 in_id);
	void Java_com_chillisource_core_DialogueBoxNativeInterface_onDialogueCancelPressed(JNIEnv* in_env, jobject in_this, s32 in_id);
}

//-------------------------------------------------
/// Interface function called from java. This is
/// called when a system dialog has it's confirm
/// option pressed.
///
/// @author I Copland
///
/// @param The jni environment.
/// @param The java object calling the function
/// @param Dialog ID
//-------------------------------------------------
void Java_com_chillisource_core_DialogueBoxNativeInterface_onDialogueConfirmPressed(JNIEnv* in_env, jobject in_this, s32 in_id)
{
	ChilliSource::Android::DialogueBoxSystem* dialogueBoxSystem = ChilliSource::Core::Application::Get()->GetSystem_Old<ChilliSource::Android::DialogueBoxSystem>();
	if (dialogueBoxSystem != nullptr)
	{
		dialogueBoxSystem->OnSystemConfirmDialogueResult((u32)in_id, ChilliSource::Core::DialogueBoxSystem::DialogueResult::k_confirm);
	}
}
//-------------------------------------------------
/// Interface function called from java. This is
/// called when a system dialog has it's cancel
/// option pressed
///
/// @author I Copland
///
/// @param The jni environment.
/// @param The java object calling the function
/// @param Dialog ID
//------------------------------------------------
void Java_com_chillisource_core_DialogueBoxNativeInterface_onDialogueCancelPressed(JNIEnv* in_env, jobject in_this, s32 in_id)
{
	ChilliSource::Android::DialogueBoxSystem* dialogueBoxSystem = ChilliSource::Core::Application::Get()->GetSystem_Old<ChilliSource::Android::DialogueBoxSystem>();
	if (dialogueBoxSystem != nullptr)
	{
		dialogueBoxSystem->OnSystemConfirmDialogueResult((u32)in_env, ChilliSource::Core::DialogueBoxSystem::DialogueResult::k_cancel);
	}
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(DialogueBoxJavaInterface);

		//--------------------------------------------------
		//--------------------------------------------------
		DialogueBoxJavaInterface::DialogueBoxJavaInterface()
		{
			CreateNativeInterface("com/chillisource/core/DialogueBoxNativeInterface");
			CreateMethodReference("makeToast", "(Ljava/lang/String;)V");
			CreateMethodReference("showSystemConfirmDialogue", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			CreateMethodReference("showSystemDialogue", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
		}
		//------------------------------------------------------
		//------------------------------------------------------
		bool DialogueBoxJavaInterface::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == DialogueBoxJavaInterface::InterfaceID);
		}
        //------------------------------------------------------
        //------------------------------------------------------
        void DialogueBoxJavaInterface::MakeToast(const Core::UTF8String& in_text)
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring message = JavaInterfaceUtils::CreateJStringFromUTF8String(in_text);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("makeToast"), message);
			env->DeleteLocalRef(message);
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void DialogueBoxJavaInterface::ShowSystemConfirmDialogue(s32 in_dialogID, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm, const Core::UTF8String& in_cancel)
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring title = JavaInterfaceUtils::CreateJStringFromUTF8String(in_title);
			jstring message = JavaInterfaceUtils::CreateJStringFromUTF8String(in_message);
			jstring confirm = JavaInterfaceUtils::CreateJStringFromUTF8String(in_confirm);
			jstring cancel = JavaInterfaceUtils::CreateJStringFromUTF8String(in_cancel);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("showSystemConfirmDialogue"), in_dialogID, title, message, confirm, cancel);
			env->DeleteLocalRef(title);
			env->DeleteLocalRef(message);
			env->DeleteLocalRef(confirm);
			env->DeleteLocalRef(cancel);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxJavaInterface::ShowSystemDialogue(s32 in_dialogID, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm)
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring title = JavaInterfaceUtils::CreateJStringFromUTF8String(in_title);
			jstring message = JavaInterfaceUtils::CreateJStringFromUTF8String(in_message);
			jstring confirm = JavaInterfaceUtils::CreateJStringFromUTF8String(in_confirm);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("showSystemDialogue"), in_dialogID, title, message, confirm);
			env->DeleteLocalRef(title);
			env->DeleteLocalRef(message);
			env->DeleteLocalRef(confirm);
        }
	}
}
