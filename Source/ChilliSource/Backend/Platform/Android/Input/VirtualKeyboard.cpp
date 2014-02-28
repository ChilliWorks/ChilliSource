//
//  VirtualKeyboard.cpp
//  moFlow
//
//  Created by Ian Copland in 2011
//  Copyright �2010 Tag Games Limited. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Input/VirtualKeyboard.h>
#include <ChilliSource/Backend/Platform/Android/Input/KeyboardJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Utils.h>

namespace ChilliSource
{
	namespace Android
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

			mpKeyboardJavaInterface->SetTextAddedDelegate(Core::MakeDelegate(this, &CVirtualKeyboard::OnTextAdded));
			mpKeyboardJavaInterface->SetTextDeletedDelegate(Core::MakeDelegate(this, &CVirtualKeyboard::OnTextDeleted));
			mpKeyboardJavaInterface->SetKeyboardDismissedDelegate(Core::MakeDelegate(this, &CVirtualKeyboard::OnKeyboardDismissed));
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
				mOnKeyboardShowEvent.NotifyConnections();
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
				mOnKeyboardHideEvent.NotifyConnections();

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
		void CVirtualKeyboard::SetText(const Core::UTF8String& instrText)
		{
			mstrText = instrText;
		}
		//-------------------------------------------
		/// On Text Added
		//-------------------------------------------
		void CVirtualKeyboard::OnTextAdded(const Core::UTF8String& instrText)
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
		void CVirtualKeyboard::OnTextDeleted()
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
		void CVirtualKeyboard::OnKeyboardDismissed()
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
		CVirtualKeyboard::~CVirtualKeyboard()
		{
			mpKeyboardJavaInterface->SetTextAddedDelegate(NULL);
			mpKeyboardJavaInterface->SetTextDeletedDelegate(NULL);
			mpKeyboardJavaInterface->SetKeyboardDismissedDelegate(NULL);
		}
	}
}
