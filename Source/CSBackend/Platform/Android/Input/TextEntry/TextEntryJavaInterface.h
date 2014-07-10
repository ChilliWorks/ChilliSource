//
//  TextEntryJavaInterface.h
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

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_INPUT_TEXTENTRY_TEXTENTRYJAVAINTERFACE_H_
#define _CHILLISOURCE_PLATFORM_ANDROID_INPUT_TEXTENTRY_TEXTENTRYJAVAINTERFACE_H_

#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>
#include <ChilliSource/Input/TextEntry/TextEntry.h>

namespace CSBackend
{
	namespace Android
	{
		//=====================================================
		/// Text Entry Java Interface
		///
		/// A Java Interface for Android virtual keyboard input.
		//======================================================
		class TextEntryJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(TextEntryJavaInterface);
			//-----------------------------------------------
			/// Events
			//-----------------------------------------------
			typedef std::function<void(const std::string&)> TextChangedDelegate;
			typedef std::function<void()> KeyboardDismissedDelegate;
			//-----------------------------------------------
			/// Constructor
			//-----------------------------------------------
			TextEntryJavaInterface();
			//-----------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements
			/// the given interface.
			//-----------------------------------------------
			bool IsA(CSCore::InterfaceIDType inInterfaceID) const;
			//-------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The new text input buffer (UTF-8).
			//-------------------------------------------------------
			void SetTextBuffer(const std::string& in_text);
			//-----------------------------------------------
			/// Set the delegate that is called whenever
			/// the text buffer changes
			///
			/// @author S Downie
			///
			/// @param Delegate
			//-----------------------------------------------
			void SetTextChangedDelegate(const TextChangedDelegate& inDelegate);
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
            void SetKeyboardType(CSInput::TextEntry::Type ineKeyboardType);
			//-------------------------------------------
			/// Set Capitalisation Method
			///
			/// @param The capitalisation method to be
            /// used the next time the keyboard is
            /// displayed.
			//-------------------------------------------
            void SetCapitalisationMethod(CSInput::TextEntry::Capitalisation ineKeyboardCapitalisation);
			//-----------------------------------------------
			/// Called when the contents of the text buffer
            /// has changed
            ///
            /// @author S Downie
			///
			/// @param The new text (UTF-8).
			//-----------------------------------------------
			void OnTextChanged(const std::string& in_text);
			//-----------------------------------------------
			/// On Keyboard Dismissed
			///
			/// Called when the keyboard is dismissed.
			//-----------------------------------------------
			void OnKeyboardDismissed();
		private:
			TextChangedDelegate m_textChangedDelegate;
			KeyboardDismissedDelegate mKeyboardDismissedDelegate;
		};
	}
}

#endif

#endif
