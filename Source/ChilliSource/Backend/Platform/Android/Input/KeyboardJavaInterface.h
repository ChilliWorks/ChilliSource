//
//  KeyboardJavaInterface.h
//  moFlow
//
//  Created by Ian Copland on 04/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_ANDROID_INPUT_KEYBOARDJAVAINTERFACE_H_
#define _MOFLOW_PLATFORM_ANDROID_INPUT_KEYBOARDJAVAINTERFACE_H_

#include <ChilliSource/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterface.h>
#include <ChilliSource/Input/VirtualKeyboard.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		//=====================================================
		/// Keyboard Java Interface
		///
		/// A Java Interface for Android keyboard input.
		//======================================================
		class CKeyboardJavaInterface : public IJavaInterface
		{
		public:
			DECLARE_NAMED_INTERFACE(CKeyboardJavaInterface);
			//-----------------------------------------------
			/// Events
			//-----------------------------------------------
			typedef fastdelegate::FastDelegate1<const UTF8String&> TextAddedDelegate;
			typedef fastdelegate::FastDelegate0<> TextDeletedDelegate;
			typedef fastdelegate::FastDelegate0<> KeyboardDismissedDelegate;
			//-----------------------------------------------
			/// Constructor
			//-----------------------------------------------
			CKeyboardJavaInterface();
			//-----------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements
			/// the given interface.
			//-----------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//-----------------------------------------------
			/// Set Text Added Delegate
			///
			/// @param The delegate that will be called when
			/// new keyboard text is added.
			//-----------------------------------------------
			void SetTextAddedDelegate(const TextAddedDelegate& inDelegate);
			//-----------------------------------------------
			/// Set Text Deleted Delegate
			///
			/// @param The delegate that will be called when
			/// keyboard text is deleted.
			//-----------------------------------------------
			void SetTextDeletedDelegate(const TextDeletedDelegate& inDelegate);
			//-----------------------------------------------
			/// Set Keyboard Dismissed Delegate
			///
			/// @param The delegate that will be called when
			/// the keyboard is dismissed.
			//-----------------------------------------------
			void SetKeyboardDismissedDelegate(const KeyboardDismissedDelegate& inDelegate);
			//-----------------------------------------------
			/// Activate
			///
			/// Displays the software keyboard and starts
			/// reading from the hardware keyboard if there
			/// is one.
			//-----------------------------------------------
			void Activate();
			//-----------------------------------------------
			/// Deactivate
			///
			/// Hides the software keyboard and stops reading
			/// from the hardware keyboard if there is one.
			//-----------------------------------------------
			void Deactivate();
			//-------------------------------------------
			/// Set Keyboard Type
			///
			/// @param The keyboard type to be used the
			/// next time the keyboard is displayed.
			//-------------------------------------------
            void SetKeyboardType(Input::KeyboardType ineKeyboardType);
			//-------------------------------------------
			/// Set Capitalisation Method
			///
			/// @param The capitalisation method to be
            /// used the next time the keyboard is
            /// displayed.
			//-------------------------------------------
            void SetCapitalisationMethod(Input::KeyboardCapitalisation ineKeyboardCapitalisation);
			//-----------------------------------------------
			/// On Text Added
			///
			/// Called when the there is new text to add that
            /// has been received from the keyboard.
			///
			/// @param The new text.
			//-----------------------------------------------
			void OnTextAdded(const UTF8String& instrText);
			//-----------------------------------------------
			/// On Text Deleted
			///
			/// Called when keyboard text has been deleted.
			///
			/// @param The new text.
			//-----------------------------------------------
			void OnTextDeleted();
			//-----------------------------------------------
			/// On Keyboard Dismissed
			///
			/// Called when the keyboard is dismissed.
			//-----------------------------------------------
			void OnKeyboardDismissed();
		private:
			TextAddedDelegate mTextAddedDelegate;
			TextDeletedDelegate mTextDeletedDelegate;
			KeyboardDismissedDelegate mKeyboardDismissedDelegate;
		};
	}
}

#endif
