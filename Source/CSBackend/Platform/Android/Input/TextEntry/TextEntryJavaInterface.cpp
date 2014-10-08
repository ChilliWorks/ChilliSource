//
//  TextEntryJavaInterface.cpp
//  Chilli Source
//  Created by Ian Copland on 04/02/2014.
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

#include <CSBackend/Platform/Android/Input/TextEntry/TextEntryJavaInterface.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

//-----------------------------------------------
/// C function declarations
//-----------------------------------------------
extern "C"
{
	//-----------------------------------------------
	/// Called from java when the text buffer changes
	///
	/// @author S Downie
	///
	/// @param The jni environment.
	/// @param The java object calling the function.
	/// @param The new contents of the text buffer.
	//-----------------------------------------------
	void Java_com_chilliworks_chillisource_input_TextEntryNativeInterface_nativeOnTextChanged(JNIEnv* in_env, jobject in_this, jstring in_text);
	//-----------------------------------------------
	/// Native On Keyboard Dismissed
	///
	/// Called from java when the keyboard is dismissed.
	///
	/// @param The jni environment.
	/// @param The java object calling the function.
	//-----------------------------------------------
	void Java_com_chilliworks_chillisource_input_TextEntryNativeInterface_nativeOnKeyboardDismissed(JNIEnv* inpEnv, jobject inThis);
}
//-----------------------------------------------
//-----------------------------------------------
void Java_com_chilliworks_chillisource_input_TextEntryNativeInterface_nativeOnTextChanged(JNIEnv* in_env, jobject in_this, jstring in_text)
{
	CSBackend::Android::TextEntryJavaInterfaceSPtr textEntryJI = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::TextEntryJavaInterface>();
	if (textEntryJI != nullptr)
	{
		std::string text = CSBackend::Android::JavaInterfaceUtils::CreateSTDStringFromJString(in_text);
		auto task = std::bind(&CSBackend::Android::TextEntryJavaInterface::OnTextChanged, textEntryJI.get(), text);
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
	}
	in_env->DeleteLocalRef(in_text);
}
//-----------------------------------------------
//-----------------------------------------------
void Java_com_chilliworks_chillisource_input_TextEntryNativeInterface_nativeOnKeyboardDismissed(JNIEnv* in_env, jobject in_this)
{
	CSBackend::Android::TextEntryJavaInterfaceSPtr textEntryJI = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::TextEntryJavaInterface>();
	if (textEntryJI != nullptr)
	{
		auto task = std::bind(&CSBackend::Android::TextEntryJavaInterface::OnKeyboardDismissed, textEntryJI.get());
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
	}
}

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
			//-----------------------------------------------
			/// Keyboard Type To Integer
			///
			/// Converts from the keyboard type enum to an
			/// integer that can be passed on to the Java
			/// side of the engine.
			///
			/// @param The keyboard type to convert.
			/// @return The keyboard type in integer form.
			//-----------------------------------------------
			s32 KeyboardTypeToInteger(CSInput::TextEntry::Type ineKeyboardType)
			{
				switch (ineKeyboardType)
				{
				case CSInput::TextEntry::Type::k_text:
					return 0;
				case CSInput::TextEntry::Type::k_numeric:
					return 1;
				default:
					CS_LOG_ERROR("Invalid keyboard type, cannot be converted.");
					return 0;
				}
			}
			//-----------------------------------------------
			/// Keyboard Capitalisation To Integer
			///
			/// Converts from the Keyboard Capitalisation enum
			/// to an integer that can be passed on to the
			/// Java side of the engine.
			///
			/// @param The Keyboard Capitalisation to convert.
			/// @return The Keyboard Capitalisation in integer
			/// form.
			//-----------------------------------------------
			s32 KeyboardCapitalisationToInteger(CSInput::TextEntry::Capitalisation ineKeyboardCapitalisation)
			{
				switch (ineKeyboardCapitalisation)
				{
				case CSInput::TextEntry::Capitalisation::k_none:
					return 0;
				case CSInput::TextEntry::Capitalisation::k_sentences:
					return 1;
				case CSInput::TextEntry::Capitalisation::k_words:
					return 2;
				case CSInput::TextEntry::Capitalisation::k_all:
					return 3;
				default:
					CS_LOG_ERROR("Invalid keyboard capitalisation, cannot be converted.");
					return 0;
				}
			}
		}

		CS_DEFINE_NAMEDTYPE(TextEntryJavaInterface);
		//-----------------------------------------------
		//-----------------------------------------------
		TextEntryJavaInterface::TextEntryJavaInterface()
		{
			CreateNativeInterface("com/chilliworks/chillisource/input/TextEntryNativeInterface");
			CreateMethodReference("activate", "()V");
			CreateMethodReference("deactivate", "()V");
			CreateMethodReference("setKeyboardType", "(I)V");
			CreateMethodReference("setCapitalisationMethod", "(I)V");
			CreateMethodReference("setTextBuffer", "(Ljava/lang/String;)V");
		}
		//-----------------------------------------------
		//-----------------------------------------------
		bool TextEntryJavaInterface::IsA(CSCore::InterfaceIDType inInterfaceID) const
		{
			return (TextEntryJavaInterface::InterfaceID == inInterfaceID);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		void TextEntryJavaInterface::SetTextChangedDelegate(const TextChangedDelegate& in_delegate)
		{
			m_textChangedDelegate = in_delegate;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		void TextEntryJavaInterface::SetKeyboardDismissedDelegate(const KeyboardDismissedDelegate& inDelegate)
		{
			mKeyboardDismissedDelegate = inDelegate;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		void TextEntryJavaInterface::Activate()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("activate"));
		}
		//-----------------------------------------------
		//-----------------------------------------------
		void TextEntryJavaInterface::Deactivate()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("deactivate"));
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntryJavaInterface::SetTextBuffer(const std::string& in_text)
		{
        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	jstring text = JavaInterfaceUtils::CreateJStringFromSTDString(in_text);
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("setTextBuffer"), text);
        	env->DeleteLocalRef(text);
		}
		//-------------------------------------------
		//-------------------------------------------
        void TextEntryJavaInterface::SetKeyboardType(CSInput::TextEntry::Type ineKeyboardType)
        {
        	JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	s32 dwKeyboardType = KeyboardTypeToInteger(ineKeyboardType);
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("setKeyboardType"), dwKeyboardType);
        }
		//-------------------------------------------
		//-------------------------------------------
        void TextEntryJavaInterface::SetCapitalisationMethod(CSInput::TextEntry::Capitalisation ineKeyboardCapitalisation)
        {
        	JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	s32 dwKeyboardCapitalisation = KeyboardCapitalisationToInteger(ineKeyboardCapitalisation);
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("setCapitalisationMethod"), dwKeyboardCapitalisation);
        }
		//-----------------------------------------------
		//-----------------------------------------------
		void TextEntryJavaInterface::OnTextChanged(const std::string& in_text)
		{
			if (m_textChangedDelegate != nullptr)
			{
				m_textChangedDelegate(in_text);
			}
		}
		//-----------------------------------------------
		//-----------------------------------------------
		void TextEntryJavaInterface::OnKeyboardDismissed()
		{
			if (mKeyboardDismissedDelegate != nullptr)
			{
				mKeyboardDismissedDelegate();
			}
		}
	}
}

#endif
