//
//  KeyboardJavaInterface.cpp
//  moFlow
//
//  Created by Ian Copland on 04/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Input/Keyboard/KeyboardJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

//-----------------------------------------------
/// C function declarations
//-----------------------------------------------
extern "C"
{
	void Java_com_chillisource_input_KeyboardNativeInterface_NativeOnTextAdded(JNIEnv* inpEnv, jobject inThis, jstring injstrText);
	void Java_com_chillisource_input_KeyboardNativeInterface_NativeOnTextDeleted(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_input_KeyboardNativeInterface_NativeOnKeyboardDismissed(JNIEnv* inpEnv, jobject inThis);
}
//-----------------------------------------------
/// Native On Text Added
///
/// Called from java when new keyboard text has
/// been received to be added.
///
/// @param The jni environment.
/// @param The java object calling the function.
/// @param The additional text.
//-----------------------------------------------
void Java_com_chillisource_input_KeyboardNativeInterface_NativeOnTextAdded(JNIEnv* inpEnv, jobject inThis, jstring injstrText)
{
	ChilliSource::Android::KeyboardJavaInterfaceSPtr pKeyboardJI = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::KeyboardJavaInterface>();
	if (pKeyboardJI != nullptr)
	{
		std::string strText = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(injstrText);
		auto task = std::bind(&ChilliSource::Android::KeyboardJavaInterface::OnTextAdded, pKeyboardJI.get(), strText);
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
	}
	inpEnv->DeleteLocalRef(injstrText);
}
//-----------------------------------------------
/// Native On Text Deleted
///
/// Called from java when a keyboard text deletion
/// event is received.
///
/// @param The jni environment.
/// @param The java object calling the function.
//-----------------------------------------------
void Java_com_chillisource_input_KeyboardNativeInterface_NativeOnTextDeleted(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::KeyboardJavaInterfaceSPtr pKeyboardJI = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::KeyboardJavaInterface>();
	if (pKeyboardJI != nullptr)
	{
		auto task = std::bind(&ChilliSource::Android::KeyboardJavaInterface::OnTextDeleted, pKeyboardJI.get());
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
	}
}
//-----------------------------------------------
/// Native On Keyboard Dismissed
///
/// Called from java when the keyboard is dismissed.
///
/// @param The jni environment.
/// @param The java object calling the function.
//-----------------------------------------------
void Java_com_chillisource_input_KeyboardNativeInterface_NativeOnKeyboardDismissed(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::KeyboardJavaInterfaceSPtr pKeyboardJI = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::KeyboardJavaInterface>();
	if (pKeyboardJI != nullptr)
	{
		auto task = std::bind(&ChilliSource::Android::KeyboardJavaInterface::OnKeyboardDismissed, pKeyboardJI.get());
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
	}
}

namespace ChilliSource
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
			s32 KeyboardTypeToInteger(Input::Keyboard::Type ineKeyboardType)
			{
				switch (ineKeyboardType)
				{
				case Input::Keyboard::Type::k_text:
					return 0;
				case Input::Keyboard::Type::k_numeric:
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
			s32 KeyboardCapitalisationToInteger(Input::Keyboard::Capitalisation ineKeyboardCapitalisation)
			{
				switch (ineKeyboardCapitalisation)
				{
				case Input::Keyboard::Capitalisation::k_none:
					return 0;
				case Input::Keyboard::Capitalisation::k_sentences:
					return 1;
				case Input::Keyboard::Capitalisation::k_words:
					return 2;
				case Input::Keyboard::Capitalisation::k_all:
					return 3;
				default:
					CS_LOG_ERROR("Invalid keyboard capitalisation, cannot be converted.");
					return 0;
				}
			}
		}

		CS_DEFINE_NAMEDTYPE(KeyboardJavaInterface);
		//-----------------------------------------------
		/// Constructor
		//-----------------------------------------------
		KeyboardJavaInterface::KeyboardJavaInterface()
		{
			CreateNativeInterface("com/chillisource/input/KeyboardNativeInterface");
			CreateMethodReference("Activate", "()V");
			CreateMethodReference("Deactivate", "()V");
			CreateMethodReference("SetKeyboardType", "(I)V");
			CreateMethodReference("SetCapitalisationMethod", "(I)V");
		}
		//-----------------------------------------------
		/// Is A
		//-----------------------------------------------
		bool KeyboardJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (KeyboardJavaInterface::InterfaceID == inInterfaceID);
		}
		//-----------------------------------------------
		/// Set Text Added Delegate
		//-----------------------------------------------
		void KeyboardJavaInterface::SetTextAddedDelegate(const TextAddedDelegate& inDelegate)
		{
			mTextAddedDelegate = inDelegate;
		}
		//-----------------------------------------------
		/// Set Text Deleted Delegate
		//-----------------------------------------------
		void KeyboardJavaInterface::SetTextDeletedDelegate(const TextDeletedDelegate& inDelegate)
		{
			mTextDeletedDelegate = inDelegate;
		}
		//-----------------------------------------------
		/// Set Keyboard Dismissed Delegate
		//-----------------------------------------------
		void KeyboardJavaInterface::SetKeyboardDismissedDelegate(const KeyboardDismissedDelegate& inDelegate)
		{
			mKeyboardDismissedDelegate = inDelegate;
		}
		//-----------------------------------------------
		/// Activate
		//-----------------------------------------------
		void KeyboardJavaInterface::Activate()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Activate"));
		}
		//-----------------------------------------------
		/// Deactivate
		//-----------------------------------------------
		void KeyboardJavaInterface::Deactivate()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Deactivate"));
		}
		//-------------------------------------------
		/// Set Keyboard Type
		//-------------------------------------------
        void KeyboardJavaInterface::SetKeyboardType(Input::Keyboard::Type ineKeyboardType)
        {
        	JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	s32 dwKeyboardType = KeyboardTypeToInteger(ineKeyboardType);
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("SetKeyboardType"), dwKeyboardType);
        }
		//-------------------------------------------
		/// Set Capitalisation Method
		//-------------------------------------------
        void KeyboardJavaInterface::SetCapitalisationMethod(Input::Keyboard::Capitalisation ineKeyboardCapitalisation)
        {
        	JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	s32 dwKeyboardCapitalisation = KeyboardCapitalisationToInteger(ineKeyboardCapitalisation);
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("SetCapitalisationMethod"), dwKeyboardCapitalisation);
        }
		//-----------------------------------------------
		/// On Text Added
		//-----------------------------------------------
		void KeyboardJavaInterface::OnTextAdded(const std::string& instrText)
		{
			if (mTextAddedDelegate != nullptr)
			{
				mTextAddedDelegate(instrText);
			}
		}
		//-----------------------------------------------
		/// On Text Deleted
		//-----------------------------------------------
		void KeyboardJavaInterface::OnTextDeleted()
		{
			if (mTextDeletedDelegate != nullptr)
			{
				mTextDeletedDelegate();
			}
		}
		//-----------------------------------------------
		/// On Keyboard Dismissed
		//-----------------------------------------------
		void KeyboardJavaInterface::OnKeyboardDismissed()
		{
			if (mKeyboardDismissedDelegate != nullptr)
			{
				mKeyboardDismissedDelegate();
			}
		}
	}
}
