//
//  KeyboardJavaInterface.h
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

#ifndef _MOFLOW_PLATFORM_ANDROID_INPUT_KEYBOARDJAVAINTERFACE_H_
#define _MOFLOW_PLATFORM_ANDROID_INPUT_KEYBOARDJAVAINTERFACE_H_

#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>
#include <ChilliSource/Input/Keyboard/Keyboard.h>

namespace ChilliSource
{
	namespace Android
	{
		//=====================================================
		/// Keyboard Java Interface
		///
		/// A Java Interface for Android keyboard input.
		//======================================================
		class KeyboardJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(KeyboardJavaInterface);
			//-----------------------------------------------
			/// Events
			//-----------------------------------------------
			typedef std::function<void(const std::string&)> TextAddedDelegate;
			typedef std::function<void()> TextDeletedDelegate;
			typedef std::function<void()> KeyboardDismissedDelegate;
			//-----------------------------------------------
			/// Constructor
			//-----------------------------------------------
			KeyboardJavaInterface();
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
            void SetKeyboardType(Input::Keyboard::Type ineKeyboardType);
			//-------------------------------------------
			/// Set Capitalisation Method
			///
			/// @param The capitalisation method to be
            /// used the next time the keyboard is
            /// displayed.
			//-------------------------------------------
            void SetCapitalisationMethod(Input::Keyboard::Capitalisation ineKeyboardCapitalisation);
			//-----------------------------------------------
			/// On Text Added
			///
			/// Called when the there is new text to add that
            /// has been received from the keyboard.
			///
			/// @param The new text (UTF-8).
			//-----------------------------------------------
			void OnTextAdded(const std::string& instrText);
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

#endif