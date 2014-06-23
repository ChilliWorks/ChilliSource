//
//  EditableLabel.h
//  Chilli Source
//  Created by Scott Downie on 02/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_GUI_GUI_EDITABLE_LABEL
#define _CHILLISOURCE_GUI_GUI_EDITABLE_LABEL

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/GUI/Label/Label.h>
#include <ChilliSource/Input/Keyboard/Keyboard.h>

namespace ChilliSource
{
	namespace GUI
	{
		class EditableLabel : public Label
		{
		public:

			DECLARE_META_CLASS(EditableLabel)

			typedef std::function<void(EditableLabel*)> TextChangeEventDelegate;

			static const s8 kbySecureEntryCharacter = '*';

			EditableLabel();
			EditableLabel(const Core::ParamDictionary& insParams);
			virtual ~EditableLabel();

			//-------------------------------------------------
			/// @author S Downie
            ///
            /// @return A delegate that is called when text
            /// input is enabled.
			//-------------------------------------------------
			Core::IConnectableEvent<Input::Keyboard::KeyboardEventDelegate>& GetTextInputEnabledEvent();
			//-------------------------------------------------
			/// @author S Downie
            ///
            /// @return A delegate that is called when text
            /// input is disabled.
			//-------------------------------------------------
			Core::IConnectableEvent<Input::Keyboard::KeyboardEventDelegate>& GetTextInputDisabledEvent();
			//-------------------------------------------------
			/// @author S Downie
            ///
            /// @return A delegate that is called when text
            /// input is received.
			//-------------------------------------------------
			Core::IConnectableEvent<TextChangeEventDelegate>& GetTextInputReceivedEvent();
            //-------------------------------------------------
			/// Set Keyboard
			///
			/// @param Virtual keyboard
			//-------------------------------------------------
			void SetKeyboard(Input::Keyboard* inpKeyboard);
			//-------------------------------------------------
			/// Get Keyboard Ptr
			///
			/// @return Virtual keyboard
			//-------------------------------------------------
			Input::Keyboard* GetKeyboardPtr();
            //-------------------------------------------------
			/// Show Keyboard
			//-------------------------------------------------
			void ShowKeyboard();
            //-------------------------------------------------
			/// Hide Keyboard
			//-------------------------------------------------
			void HideKeyboard();
			//-------------------------------------------------
			/// Enable Secure Entry
			///
			/// Enabling this will prevent the label from
			/// showing any user input. Each character will
			/// be replaced by a * for security
			///
			/// @param Enable/disable
			//--------------------------------------------------
			void EnableSecureEntry(bool inbEnabled);
			//-------------------------------------------------
			/// Enable Secure Entry
			///
			/// Enabling this will prevent the label from
			/// showing any user input. Each character will
			/// be replaced by a * for security
			///
			/// @param Enable/disable
			//--------------------------------------------------
			bool IsSecureEntryEnabled() const;
			//--------------------------------------------------
			/// Set Input Limit
			///
			/// The maximum number of characters the user
			/// can enter. Zero is infinite
			///
			/// @param Limit
			//--------------------------------------------------
			void SetCharacterLimit(u32 inudwLimit);
			//--------------------------------------------------
			/// Get Input Limit
			///
			/// The maximum number of characters the user
			/// can enter. Zero is infinite
			///
			/// @return Limit
			//--------------------------------------------------
			u32 GetCharacterLimit() const;
			//--------------------------------------------------
			/// Get Input Limit
			///
			/// The maximum number of characters the user
			/// can enter. Zero is infinite
			///
			/// @return Limit
			//--------------------------------------------------
			u32 GetInputLimit() const;
            //-------------------------------------------------------
			/// Clears the text
			//-------------------------------------------------------
            void ClearText();
            //-------------------------------------------------
			/// SetKeyboardInputTypeNumeric
            ///
            /// change displayed keys to numberpad
			//-------------------------------------------------
			void SetKeyboardInputTypeNumeric();
            //-------------------------------------------------
			/// SetKeyboardInputTypeText
            ///
            /// change displayed keys to text entry
			//-------------------------------------------------
			void SetKeyboardInputTypeText();
            //------------------------
            /// Set Keyboard Capitalisation Method
            ///
            /// @param Capitalisation Type
            //------------------------
            void SetKeyboardCapitalisationMethod(Input::Keyboard::Capitalisation ineCapitalisationType);

		protected:

			//-------------------------------------------------
			/// Called when Text Input is enabled on the
            /// keyboard.
            ///
            /// @author S Downie
			//-------------------------------------------------
			void OnKeyboardTextInputEnabled();
            //-------------------------------------------------
			/// Called when the keyboard text input changes.
			///
            /// @author S Downie
            ///
			/// @param Contents of the keyboard (UTF-8).
			/// @param [Out] Whether to accept the input.
			//----------------------------------------------------
			void OnKeyboardTextInputReceived(const std::string& in_text, bool* out_rejectText);
			//-------------------------------------------------
			/// Called when Text Input is disabled on the
            /// keyboard.
            ///
            /// @author S Downie
			//-------------------------------------------------
			void OnKeyboardTextInputDisabled();
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The time stamp.
            /// @param The press type.
            ///
            /// @return Whether touch has been consumed
            //-----------------------------------------------------------
			bool OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType) override;
			//-----------------------------------------------------------
			/// Called when the view stops receiving cursor/touch input
            ///
            /// @author S Downie
			///
			/// @param The pointer
            /// @param The time stamp.
            /// @param The press type.
			//-----------------------------------------------------------
			void OnPointerUp(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType) override;
			//-------------------------------------------------------
			/// Draw
			///
			/// Draw the text using the canvas then draw any
			/// subviews
			///
			/// @param Canvas renderer
			//-------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvas);
			//-------------------------------------------------------
			/// Update
			///
			/// Used for animation
			///
			/// @param Time between frames
			//-------------------------------------------------------
			void Update(f32 infDt);

		private:

			Input::Keyboard* mpKeyboard;

			Core::Event<Input::Keyboard::KeyboardEventDelegate> m_textInputEnabledEvent;
			Core::Event<Input::Keyboard::KeyboardEventDelegate> m_textInputDisabledEvent;
			Core::Event<TextChangeEventDelegate> m_textInputReceivedEvent;
            
            Core::EventConnectionUPtr m_keyboardShownConnection;
            Core::EventConnectionUPtr m_keyboardHiddenConnection;
            Core::EventConnectionUPtr m_keyboardTextChangedConnection;

			f32 mfTimeToShow;
			bool mbShowKeyboard;

			DECLARE_PROPERTY_A(bool, SecureEntry, EnableSecureEntry, IsSecureEntryEnabled);
			DECLARE_PROPERTY_A(u32, CharacterLimit, SetCharacterLimit, GetCharacterLimit);

			static EditableLabel* pKeyboardListener;
            
            bool mbSelected;
		};
	}
}

#endif