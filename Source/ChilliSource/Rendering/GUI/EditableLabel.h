//
//  GUIEditableLabel.h
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_GUI_GUI_EDITABLE_LABEL
#define _MO_FLO_GUI_GUI_EDITABLE_LABEL

#include <ChilliSource/Rendering/GUI/Label.h>

#include <ChilliSource/Core/Event/GenericEvent.h>

#include <ChilliSource/Input/Keyboard/VirtualKeyboard.h>

namespace moFlo
{
	namespace GUI
	{
		class CEditableLabel : public CLabel
		{
		public:

			DECLARE_META_CLASS(CEditableLabel)

			typedef fastdelegate::FastDelegate1<CEditableLabel*> TextChangeEventDelegate;

			static const s8 kbySecureEntryCharacter = '*';

			CEditableLabel();
			CEditableLabel(const Core::ParamDictionary& insParams);
			virtual ~CEditableLabel();

			//-------------------------------------------------
			/// Get Keyboard Show Event
			///
			/// Use to register delegate for when keyboard
			/// becomes active
			///
			/// @param Keyboard Event Delegate
			//-------------------------------------------------
			IEvent<Input::KeyboardEventDelegate>& GetKeyboardShowEvent();
			//-------------------------------------------------
			/// Get Keyboard Hide Event
			///
			/// Use to register delegate for when keyboard
			/// stops being active
			///
			/// @param Keyboard Event Delegate
			//-------------------------------------------------
			IEvent<Input::KeyboardEventDelegate>& GetKeyboardHideEvent();
			//-------------------------------------------------
			/// Get Text Change Event
			///
			/// Use to register delegate for when a character
			/// is input using the keyboard
			///
			/// @param Text Change Event Delegate
			//-------------------------------------------------
			IEvent<TextChangeEventDelegate>& GetTextChangeEvent();
			//-------------------------------------------------
			/// Set Keyboard
			///
			/// @param Virtual keyboard
			//-------------------------------------------------
			void SetKeyboard(Input::IVirtualKeyboard* inpKeyboard);
			//-------------------------------------------------
			/// Get Keyboard Ptr
			///
			/// @return Virtual keyboard
			//-------------------------------------------------
			Input::IVirtualKeyboard* GetKeyboardPtr();
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
            void SetKeyboardCapitalisationMethod(moInput::KeyboardCapitalisation ineCapitalisationType);
            //-------------------------------------------------------
			/// SetTextSeparator
			///
			/// Sets the separator for text display
			/// @param The string to use as a separator
			//-------------------------------------------------------
            void SetTextSeparator(const UTF8String& inutf8strSeparator);
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
            void SetText(const UTF8String& instrText);
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
			void OnKeyboardTextChanged(const UTF8String& instrText, bool* inbRejectInput);
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
            UTF8String GetTextWithSeparators() const;

		private:

			Input::IVirtualKeyboard* mpKeyboard;

			CEvent0<Input::KeyboardEventDelegate> mOnKeyboardShowEvent;
			CEvent0<Input::KeyboardEventDelegate> mOnKeyboardHideEvent;
			CEvent1<TextChangeEventDelegate> mOnTextChangeEvent;

			f32 mfTimeToShow;
			bool mbShowKeyboard;

			DECLARE_PROPERTY_A(bool, SecureEntry, EnableSecureEntry, IsSecureEntryEnabled);
			DECLARE_PROPERTY_A(u32, CharacterLimit, SetCharacterLimit, GetCharacterLimit);

			static CEditableLabel* pKeyboardListener;
            
            UTF8String mutf8strTextWithSeparators;
            UTF8String mutf8strSeparator;
            u32 mu32SeparatorSpacing;
            bool mbSelected;
		};
	}
}

#endif