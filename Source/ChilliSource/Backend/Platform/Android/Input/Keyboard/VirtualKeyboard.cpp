//
//  VirtualKeyboard.cpp
//  moFlow
//
//  Created by Ian Copland in 2011
//  Copyright �2010 Tag Games Limited. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Input/Keyboard/VirtualKeyboard.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Input/Keyboard/KeyboardJavaInterface.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Utils.h>

namespace ChilliSource
{
	namespace Android
	{
		//-------------------------------------------
		/// Constructor
		//-------------------------------------------
		VirtualKeyboard::VirtualKeyboard()
		{
			mpKeyboardJavaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<KeyboardJavaInterface>();
			if (mpKeyboardJavaInterface == NULL)
			{
				mpKeyboardJavaInterface = KeyboardJavaInterfacePtr(new KeyboardJavaInterface());
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpKeyboardJavaInterface);
			}

			mpKeyboardJavaInterface->SetTextAddedDelegate(Core::MakeDelegate(this, &VirtualKeyboard::OnTextAdded));
			mpKeyboardJavaInterface->SetTextDeletedDelegate(Core::MakeDelegate(this, &VirtualKeyboard::OnTextDeleted));
			mpKeyboardJavaInterface->SetKeyboardDismissedDelegate(Core::MakeDelegate(this, &VirtualKeyboard::OnKeyboardDismissed));
		}
		//-------------------------------------------
		/// Show
		//-------------------------------------------
		void VirtualKeyboard::Show()
		{
			if(!mbIsActive)
			{
				//Show the keyboard!
				mpKeyboardJavaInterface->Activate();
				mbIsActive = true;

				//Notify our listeners
				mOnKeyboardShowEvent.NotifyConnections();
			}
		}
		//-------------------------------------------
		/// Hide
		//-------------------------------------------
		void VirtualKeyboard::Hide()
		{
			if(mbIsActive)
			{
				mpKeyboardJavaInterface->Deactivate();
				mbIsActive = false;

				//Notify our listeners
				mOnKeyboardHideEvent.NotifyConnections();

				mstrText.clear();
			}
		}
		//-------------------------------------------
		/// Set Keyboard Type
		//-------------------------------------------
        void VirtualKeyboard::SetKeyboardType(Input::KeyboardType ineKeyboardType)
        {
        	mpKeyboardJavaInterface->SetKeyboardType(ineKeyboardType);
        }
		//-------------------------------------------
		/// Set Capitalisation Method
		//-------------------------------------------
        void VirtualKeyboard::SetCapitalisationMethod(Input::KeyboardCapitalisation ineKeyboardCapitalisation)
        {
        	mpKeyboardJavaInterface->SetCapitalisationMethod(ineKeyboardCapitalisation);
        }
		//-------------------------------------------
		/// Set Text
		//-------------------------------------------
		void VirtualKeyboard::SetText(const Core::UTF8String& instrText)
		{
			mstrText = instrText;
		}
		//-------------------------------------------
		/// On Text Added
		//-------------------------------------------
		void VirtualKeyboard::OnTextAdded(const Core::UTF8String& instrText)
		{
			Core::UTF8String strNewText = mstrText + instrText;

			bool bRejectText = false;
			mOnKeyboardTextChangeEvent.NotifyConnections(strNewText, &bRejectText);

			if(!bRejectText)
			{
				mstrText = strNewText;
			}
		}
		//-------------------------------------------
		/// On Text Deleted
		//-------------------------------------------
		void VirtualKeyboard::OnTextDeleted()
		{
			Core::UTF8String strNewText = mstrText;
			if (strNewText.size() > 0)
			{
				strNewText = mstrText.substr(0, mstrText.length() - 1);
			}

			bool bRejectText = false;
			mOnKeyboardTextChangeEvent.NotifyConnections(strNewText, &bRejectText);

			if(!bRejectText)
			{
				mstrText = strNewText;
			}
		}
		//-------------------------------------------
		/// On Keyboard Dismissed
		//-------------------------------------------
		void VirtualKeyboard::OnKeyboardDismissed()
		{
			if(mbIsActive)
			{
				mbIsActive = false;

				//Notify our listeners
				mOnKeyboardHideEvent.NotifyConnections();

				mstrText.clear();
			}
		}
		//--------------------------------------------
		/// Destructor
		//--------------------------------------------
		VirtualKeyboard::~VirtualKeyboard()
		{
			mpKeyboardJavaInterface->SetTextAddedDelegate(NULL);
			mpKeyboardJavaInterface->SetTextDeletedDelegate(NULL);
			mpKeyboardJavaInterface->SetKeyboardDismissedDelegate(NULL);
		}
	}
}
