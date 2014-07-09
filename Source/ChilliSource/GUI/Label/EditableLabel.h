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

#ifndef _CHILLISOURCE_GUI_LABEL_EDITABLELABEL_H_
#define _CHILLISOURCE_GUI_LABEL_EDITABLELABEL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/GUI/Label/Label.h>
#include <ChilliSource/Input/TextEntry/TextEntry.h>

namespace ChilliSource
{
	namespace GUI
	{
		class EditableLabel final : public Label
		{
		public:

			DECLARE_META_CLASS(EditableLabel)

			typedef std::function<void(EditableLabel*)> TextEventDelegate;

			static const s8 kbySecureEntryCharacter = '*';

			EditableLabel();
			EditableLabel(const Core::ParamDictionary& insParams);
            ~EditableLabel();
			//-------------------------------------------------
			/// @author S Downie
            ///
            /// @return A delegate that is called when text
            /// input is enabled.
			//-------------------------------------------------
			Core::IConnectableEvent<TextEventDelegate>& GetTextInputEnabledEvent();
			//-------------------------------------------------
			/// @author S Downie
            ///
            /// @return A delegate that is called when text
            /// input is disabled.
			//-------------------------------------------------
			Core::IConnectableEvent<TextEventDelegate>& GetTextInputDisabledEvent();
			//-------------------------------------------------
			/// @author S Downie
            ///
            /// @return A delegate that is called when text
            /// input is received.
			//-------------------------------------------------
			Core::IConnectableEvent<TextEventDelegate>& GetTextInputReceivedEvent();
            //-------------------------------------------------
            /// Enable text input which will show the virtual
            /// keyboard if required
            ///
			/// @author S Downie
			//-------------------------------------------------
			void EnableTextInput();
            //-------------------------------------------------
            /// Disable text input which will hide the virtual
            /// keyboard if required
            ///
			/// @author S Downie
			//-------------------------------------------------
			void DisableTextInput();
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
			/// SetInputTypeNumeric
            ///
            /// change displayed keys to numberpad
			//-------------------------------------------------
			void SetInputTypeNumeric();
            //-------------------------------------------------
			/// SetInputTypeText
            ///
            /// change displayed keys to text entry
			//-------------------------------------------------
			void SetInputTypeText();
            //------------------------
            /// Set Capitalisation Method
            ///
            /// @param Capitalisation Type
            //------------------------
            void SetCapitalisationMethod(Input::TextEntry::Capitalisation ineCapitalisationType);
            //-------------------------------------------------------
            /// Set Text
            ///
            /// @param Text string (UTF-8)
            //-------------------------------------------------------
            void SetText(const std::string& instrText) override;

		protected:

            //-------------------------------------------------
			/// Called when the text buffer changes.
			///
            /// @author S Downie
            ///
			/// @param Contents of the text entry buffer (UTF-8).
            ///
			/// @return Whether to accept the input.
			//----------------------------------------------------
			bool OnTextBufferChanged(const std::string& in_text);
            //-------------------------------------------------
			/// Called when the text input is disabled.
			///
            /// @author S Downie
			//----------------------------------------------------
			void OnTextInputDisabled();
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
			bool OnPointerDown(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType) override;
			//-----------------------------------------------------------
			/// Called when the view stops receiving cursor/touch input
            ///
            /// @author S Downie
			///
			/// @param The pointer
            /// @param The time stamp.
            /// @param The press type.
			//-----------------------------------------------------------
			void OnPointerUp(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType) override;
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

			Input::TextEntryUPtr m_textEntrySystem;

			Core::Event<TextEventDelegate> m_textInputEnabledEvent;
			Core::Event<TextEventDelegate> m_textInputDisabledEvent;
			Core::Event<TextEventDelegate> m_textInputReceivedEvent;

			f32 mfTimeToShow;
			bool mbShowKeyboard;

			DECLARE_PROPERTY_A(bool, SecureEntry, EnableSecureEntry, IsSecureEntryEnabled);
			DECLARE_PROPERTY_A(u32, CharacterLimit, SetCharacterLimit, GetCharacterLimit);

            bool mbSelected;
		};
	}
}

#endif