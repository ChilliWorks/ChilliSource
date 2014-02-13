//
//  VirtualKeyboard.cpp
//  moFlow
//
//  Created by Ian Copland in 2011
//  Copyright �2010 Tag Games Limited. All rights reserved.
//

#include <moFlo/Platform/Android/Input/VirtualKeyboard.h>
#include <moFlo/Platform/Android/Input/KeyboardJavaInterface.h>
#include <moFlo/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <moFlo/Core/Utils.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		//-------------------------------------------
		/// Constructor
		//-------------------------------------------
		CVirtualKeyboard::CVirtualKeyboard()
		{
			mpKeyboardJavaInterface = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CKeyboardJavaInterface>();
			if (mpKeyboardJavaInterface == NULL)
			{
				mpKeyboardJavaInterface = KeyboardJavaInterfacePtr(new CKeyboardJavaInterface());
				CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpKeyboardJavaInterface);
			}

			mpKeyboardJavaInterface->SetTextAddedDelegate(fastdelegate::MakeDelegate(this, &CVirtualKeyboard::OnTextAdded));
			mpKeyboardJavaInterface->SetTextDeletedDelegate(fastdelegate::MakeDelegate(this, &CVirtualKeyboard::OnTextDeleted));
			mpKeyboardJavaInterface->SetKeyboardDismissedDelegate(fastdelegate::MakeDelegate(this, &CVirtualKeyboard::OnKeyboardDismissed));
		}
		//-------------------------------------------
		/// Show
		//-------------------------------------------
		void CVirtualKeyboard::Show()
		{
			if(!mbIsActive)
			{
				//Show the keyboard!
				mpKeyboardJavaInterface->Activate();
				mbIsActive = true;

				//Notify our listeners
				mOnKeyboardShowEvent.Invoke();
			}
		}
		//-------------------------------------------
		/// Hide
		//-------------------------------------------
		void CVirtualKeyboard::Hide()
		{
			if(mbIsActive)
			{
				mpKeyboardJavaInterface->Deactivate();
				mbIsActive = false;

				//Notify our listeners
				mOnKeyboardHideEvent.Invoke();

				mstrText.clear();
			}
		}
		//-------------------------------------------
		/// Set Keyboard Type
		//-------------------------------------------
        void CVirtualKeyboard::SetKeyboardType(Input::KeyboardType ineKeyboardType)
        {
        	mpKeyboardJavaInterface->SetKeyboardType(ineKeyboardType);
        }
		//-------------------------------------------
		/// Set Capitalisation Method
		//-------------------------------------------
        void CVirtualKeyboard::SetCapitalisationMethod(Input::KeyboardCapitalisation ineKeyboardCapitalisation)
        {
        	mpKeyboardJavaInterface->SetCapitalisationMethod(ineKeyboardCapitalisation);
        }
		//-------------------------------------------
		/// Set Text
		//-------------------------------------------
		void CVirtualKeyboard::SetText(const UTF8String& instrText)
		{
			mstrText = instrText;
		}
		//-------------------------------------------
		/// On Text Added
		//-------------------------------------------
		void CVirtualKeyboard::OnTextAdded(const UTF8String& instrText)
		{
			UTF8String strNewText = mstrText + instrText;

			bool bRejectText = false;
			mOnKeyboardTextChangeEvent.Invoke<const UTF8String&, bool*>(strNewText, &bRejectText);

			if(!bRejectText)
			{
				mstrText = strNewText;
			}
		}
		//-------------------------------------------
		/// On Text Deleted
		//-------------------------------------------
		void CVirtualKeyboard::OnTextDeleted()
		{
			UTF8String strNewText = mstrText;
			if (strNewText.size() > 0)
			{
				strNewText = mstrText.substr(0, mstrText.length() - 1);
			}

			bool bRejectText = false;
			mOnKeyboardTextChangeEvent.Invoke<const UTF8String&, bool*>(strNewText, &bRejectText);

			if(!bRejectText)
			{
				mstrText = strNewText;
			}
		}
		//-------------------------------------------
		/// On Keyboard Dismissed
		//-------------------------------------------
		void CVirtualKeyboard::OnKeyboardDismissed()
		{
			if(mbIsActive)
			{
				mbIsActive = false;

				//Notify our listeners
				mOnKeyboardHideEvent.Invoke();

				mstrText.clear();
			}
		}
		//--------------------------------------------
		/// Destructor
		//--------------------------------------------
		CVirtualKeyboard::~CVirtualKeyboard()
		{
			mpKeyboardJavaInterface->SetTextAddedDelegate(NULL);
			mpKeyboardJavaInterface->SetTextDeletedDelegate(NULL);
			mpKeyboardJavaInterface->SetKeyboardDismissedDelegate(NULL);
		}
	}
}
