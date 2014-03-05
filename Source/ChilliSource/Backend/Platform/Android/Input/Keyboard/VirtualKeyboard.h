//
//  VirtualKeyboard.h
//  moFlow
//
//  Created by Ian Copland in 2011
//  Copyright �2010 Tag Games Limited. All rights reserved.
//

#ifndef _MO_FLO_PLATFORM_ANDROID_VIRTUAL_KEYBOARD_H_
#define _MO_FLO_PLATFORM_ANDROID_VIRTUAL_KEYBOARD_H_

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Input/Keyboard/VirtualKeyboard.h>

namespace ChilliSource
{
	namespace Android
	{
		class VirtualKeyboard : public Input::VirtualKeyboard
		{
		public:
			VirtualKeyboard();
			virtual ~VirtualKeyboard();
			//-------------------------------------------
			/// Show
			///
			/// Display the keyboard
			/// Triggers OnKeyboardShow event
			///
			/// @param Default text
			//-------------------------------------------
			void Show();
			//-------------------------------------------
			/// Hide
			///
			/// Remove the keyboard
			/// Triggers OnKeyboardHide event
			//-------------------------------------------
			void Hide();
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
			//-------------------------------------------
			/// Set Text
			///
			/// @param Text cache for keyboard
			//-------------------------------------------
			void SetText(const Core::UTF8String& instrText);
		private:
			//-------------------------------------------
			/// On Text Added
			///
			/// Updates the keyboard text when new text
			/// is received from the keyboard.
			//-------------------------------------------
			void OnTextAdded(const Core::UTF8String& instrText);
			//-------------------------------------------
			/// On Text Deleted
			///
			/// Removes from the keyboard text when a
			/// text delete event is received.
			//-------------------------------------------
			void OnTextDeleted();
			//-------------------------------------------
			/// On Keyboard Dismissed
			///
			/// Cleans up after the keyboard closed itself
			//-------------------------------------------
			void OnKeyboardDismissed();

			Core::UTF8String mstrText;
			KeyboardJavaInterfacePtr mpKeyboardJavaInterface;
		};
	}
}

#endif


