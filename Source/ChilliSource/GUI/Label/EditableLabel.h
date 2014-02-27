//
//  GUIEditableLabel.h
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_GUI_GUI_EDITABLE_LABEL
#define _MO_FLO_GUI_GUI_EDITABLE_LABEL

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Label/Label.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Input/Keyboard/VirtualKeyboard.h>

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
			/// Get Keyboard Show Event
			///
			/// Use to register delegate for when keyboard
			/// becomes active
			///
			/// @param Keyboard Event Delegate
			//-------------------------------------------------
			Core::IEvent<Input::KeyboardEventDelegate>& GetKeyboardShowEvent();
			//-------------------------------------------------
			/// Get Keyboard Hide Event
			///
			/// Use to register delegate for when keyboard
			/// stops being active
			///
			/// @param Keyboard Event Delegate
			//-------------------------------------------------
			Core::IEvent<Input::KeyboardEventDelegate>& GetKeyboardHideEvent();
			//-------------------------------------------------
			/// Get Text Change Event
			///
			/// Use to register delegate for when a character
			/// is input using the keyboard
			///
			/// @param Text Change Event Delegate
			//-------------------------------------------------
			Core::IEvent<TextChangeEventDelegate>& GetTextChangeEvent();
			//-------------------------------------------------
			/// Set Keyboard
			///
			/// @param Virtual keyboard
			//-------------------------------------------------
			void SetKeyboard(Input::VirtualKeyboard* inpKeyboard);
			//-------------------------------------------------
			/// Get Keyboard Ptr
			///
			/// @return Virtual keyboard
			//-------------------------------------------------
			Input::VirtualKeyboard* GetKeyboardPtr();
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
			///
			/// 
			///
			///
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
            void SetKeyboardCapitalisationMethod(Input::KeyboardCapitalisation ineCapitalisationType);
            //-------------------------------------------------------
			/// SetTextSeparator
			///
			/// Sets the separator for text display
			/// @param The string to use as a separator
			//-------------------------------------------------------
            void SetTextSeparator(const Core::UTF8String& inutf8strSeparator);
            //-------------------------------------------------------
			/// SetTextSeparatorSpacing
			///
			/// Sets the characters between separators for diplay
			/// @param The number of spaces between seperators
			//-------------------------------------------------------
            void SetTextSeparatorSpacing(u32 inu32Spacing);
            //-------------------------------------------------------
			/// SetText
			///
			/// Overridden set text method, updates cached separator string
            /// @param instrText text to set as entry
			//-------------------------------------------------------
            void SetText(const Core::UTF8String& instrText);
		protected:

			//-------------------------------------------------
			/// On Keyboard Shown
			///
			/// Triggered when the keyboard displays
			//-------------------------------------------------
			void OnKeyboardShown();
			//----------------------------------------------------
			/// On Keyboard Text Changed
			///
			/// Used to get the text from the keyboard
			///
			/// @param Contents of the keyboard
			/// @param Whether to accept the input
			//----------------------------------------------------
			void OnKeyboardTextChanged(const Core::UTF8String& instrText, bool* inbRejectInput);
			//-------------------------------------------------
			/// On Keyboard Hidden
			///
			/// Triggered when the keyboard disappears
			//-------------------------------------------------
			void OnKeyboardHidden();
            //-----------------------------------------------------------
            /// On Touch Began
            ///
            /// Called when the window receives cursor/touch input
            ///
            /// @param Touch data
            /// @return Whether touch has been consumed
            //-----------------------------------------------------------
			bool OnTouchBegan(const Input::TouchInfo & insTouchInfo);
			//-----------------------------------------------------------
			/// On Touch Ended
			///
			/// Called when the view stops receiving cursor/touch input
			///
			/// @param Touch data
			//-----------------------------------------------------------
			void OnTouchEnded(const Input::TouchInfo & insTouchInfo);
			//-------------------------------------------------------
			/// Draw
			///
			/// Draw the text using the canvas then draw any
			/// subviews
			///
			/// @param Canvas renderer
			//-------------------------------------------------------
			void Draw(Rendering::CCanvasRenderer* inpCanvas);
			//-------------------------------------------------------
			/// Update
			///
			/// Used for animation
			///
			/// @param Time between frames
			//-------------------------------------------------------
			void Update(f32 infDt);
            //-------------------------------------------------------
			/// GetTextWithSeparators
			///
			/// Returns the stored text separated by set separators
			//-------------------------------------------------------
            Core::UTF8String GetTextWithSeparators() const;

		private:

			Input::VirtualKeyboard* mpKeyboard;

			Core::CEvent0<Input::KeyboardEventDelegate> mOnKeyboardShowEvent;
			Core::CEvent0<Input::KeyboardEventDelegate> mOnKeyboardHideEvent;
			Core::CEvent1<TextChangeEventDelegate> mOnTextChangeEvent;

			f32 mfTimeToShow;
			bool mbShowKeyboard;

			DECLARE_PROPERTY_A(bool, SecureEntry, EnableSecureEntry, IsSecureEntryEnabled);
			DECLARE_PROPERTY_A(u32, CharacterLimit, SetCharacterLimit, GetCharacterLimit);

			static EditableLabel* pKeyboardListener;
            
            Core::UTF8String mutf8strTextWithSeparators;
            Core::UTF8String mutf8strSeparator;
            u32 mu32SeparatorSpacing;
            bool mbSelected;
		};
	}
}

#endif