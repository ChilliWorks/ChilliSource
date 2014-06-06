//
//  GUIEditableLabel.cpp
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Label/EditableLabel.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Font/Font.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(EditableLabel)

		DEFINE_PROPERTY(SecureEntry);
		DEFINE_PROPERTY(CharacterLimit);

        EditableLabel* EditableLabel::pKeyboardListener = nullptr;
        
        //-------------------------------------------------
        /// Constructor
        ///
        /// Default
        //-------------------------------------------------
        EditableLabel::EditableLabel() : mpKeyboard(nullptr), SecureEntry(false), CharacterLimit(0), mbShowKeyboard(false), mfTimeToShow(0.0f), mbSelected(false)
        {
            
        }
        //-------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //-------------------------------------------------
        EditableLabel::EditableLabel(const Core::ParamDictionary& insParams) 
        : Label(insParams), mpKeyboard(nullptr), CharacterLimit(0), SecureEntry(false), mbShowKeyboard(false), mfTimeToShow(0.0f)
        {
			std::string strValue;

			//---Texture
			if(insParams.TryGetValue("SecureEntry", strValue))
			{
				EnableSecureEntry(Core::ParseBool(strValue));
			}
			//---Sprite sheet
			if(insParams.TryGetValue("CharacterLimit", strValue))
			{
				SetCharacterLimit(Core::ParseU32(strValue));
			}
        }
        //-------------------------------------------------
        //-------------------------------------------------
        Core::IConnectableEvent<Input::Keyboard::KeyboardEventDelegate>& EditableLabel::GetTextInputEnabledEvent()
        {
            return m_textInputEnabledEvent;
        }
        //-------------------------------------------------
        //-------------------------------------------------
        Core::IConnectableEvent<Input::Keyboard::KeyboardEventDelegate>& EditableLabel::GetTextInputDisabledEvent()
        {
            return m_textInputDisabledEvent;
        }
        //-------------------------------------------------
        //-------------------------------------------------
        Core::IConnectableEvent<EditableLabel::TextChangeEventDelegate>& EditableLabel::GetTextInputReceivedEvent()
        {
            return m_textInputReceivedEvent;
        }
        //-------------------------------------------------
        /// Enable Secure Entry
        ///
        /// Enabling this will prevent the label from
        /// showing any user input. Each character will
        /// be replaced by a * for security
        ///
        /// @param Enable/disable
        //--------------------------------------------------
        void EditableLabel::EnableSecureEntry(bool inbEnabled)
        {
            SecureEntry = inbEnabled;
			mCachedChars.clear();
        }
		//-------------------------------------------------
		/// Enable Secure Entry
		///
		/// Enabling this will prevent the label from
		/// showing any user input. Each character will
		/// be replaced by a * for security
		///
		/// @param Enable/disable
		//--------------------------------------------------
		bool EditableLabel::IsSecureEntryEnabled() const
		{
			return SecureEntry;
		}
        //--------------------------------------------------
        /// Set Input Limit
        ///
        /// The maximum number of characters the user
        /// can enter. Zero is infinite
        ///
        /// @param Limit
        //--------------------------------------------------
        void EditableLabel::SetCharacterLimit(u32 inudwLimit)
        {
            CharacterLimit = inudwLimit;
        }
		//--------------------------------------------------
		/// Get Input Limit
		///
		/// The maximum number of characters the user
		/// can enter. Zero is infinite
		///
		/// @return Limit
		//--------------------------------------------------
		u32 EditableLabel::GetCharacterLimit() const
		{
			return CharacterLimit;
		}
		//-------------------------------------------------
		/// Get Keyboard Ptr
		///
		/// @return Virtual keyboard
		//-------------------------------------------------
		Input::Keyboard* EditableLabel::GetKeyboardPtr()
		{
			return mpKeyboard;
		}
        //-------------------------------------------------
        /// Set Keyboard
        ///
        /// @param Virtual keyboard
        //-------------------------------------------------
        void EditableLabel::SetKeyboard(Input::Keyboard* inpKeyboard)
        {
            if(mpKeyboard)
            {
                //Stop listening to old keyboard
                m_keyboardShownConnection = nullptr;
                m_keyboardHiddenConnection = nullptr;
            }
            
            if(inpKeyboard)
            {
                mpKeyboard = inpKeyboard;
                
                //Stop listening to old keyboard
                m_keyboardShownConnection = mpKeyboard->GetTextInputEnabledEvent().OpenConnection(Core::MakeDelegate(this, &EditableLabel::OnKeyboardTextInputEnabled));
                m_keyboardHiddenConnection = mpKeyboard->GetTextInputDisabledEvent().OpenConnection(Core::MakeDelegate(this, &EditableLabel::OnKeyboardTextInputDisabled));
            }
            else
            {
                HideKeyboard();
            }
        }
        //-------------------------------------------------
        /// Show Keyboard
        //-------------------------------------------------
        void EditableLabel::ShowKeyboard()
        {
            if(mpKeyboard)
            {
                pKeyboardListener = this;
                mpKeyboard->SetTextInputEnabled(true);
            }
        }
        //-------------------------------------------------
        /// Hide Keyboard
        //-------------------------------------------------
        void EditableLabel::HideKeyboard()
        {
            if(mpKeyboard)
            {
                mpKeyboard->SetTextInputEnabled(false);
                mbShowKeyboard = false;
                pKeyboardListener = this;
            }
        }
        //-------------------------------------------------
        //-------------------------------------------------
        void EditableLabel::OnKeyboardTextInputEnabled()
        {
            if(pKeyboardListener == this)
            {
                mpKeyboard->SetText(Text);
                m_keyboardTextChangedConnection = mpKeyboard->GetTextInputReceivedEvent().OpenConnection(Core::MakeDelegate(this, &EditableLabel::OnKeyboardTextInputReceived));
                m_textInputEnabledEvent.NotifyConnections();
            }
        }
        
        void EditableLabel::ClearText()
        {
            mpKeyboard->SetText("");
            Label::SetText("");
        }
        //----------------------------------------------------
		//----------------------------------------------------
		void EditableLabel::OnKeyboardTextInputReceived(const std::string& instrText, bool* inbRejectInput)
		{
            if(CharacterLimit > 0)
            {
                //We can reject the text if it exceeds our input limit
                auto length = Core::UTF8StringUtils::CalcLength(instrText.begin(), instrText.end());
                if(length > CharacterLimit)
                {
                    *inbRejectInput = true;
                    return;
                }
            }

            Label::SetText(instrText);
            *inbRejectInput = false;
            
            m_textInputReceivedEvent.NotifyConnections(this);
		}
        //-------------------------------------------------
        //-------------------------------------------------
        void EditableLabel::OnKeyboardTextInputDisabled()
        {
            if(pKeyboardListener == this)
            {
                m_keyboardTextChangedConnection = nullptr;
                pKeyboardListener = nullptr;
                m_textInputDisabledEvent.NotifyConnections();
            }
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        bool EditableLabel::OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType)
        {
            if(UserInteraction)
            {
                mbSelected = true;
            }
            
            return GUIView::OnPointerDown(in_pointer, in_timestamp, in_inputType);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void EditableLabel::OnPointerUp(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType)
        {
            if(UserInteraction && Visible && mpKeyboard && mbSelected)
            {
               if(Contains(in_pointer.m_location))
               {
                   //Flag the keyboard as hidden and wait a few seconds so we can slide it in again
				   if(!mpKeyboard->IsTextInputEnabled())
				   {
                       ShowKeyboard();
				   }
				   else
				   {
                       HideKeyboard();
				   }
               }
            }
            
            mbSelected = false;
            
            GUIView::OnPointerUp(in_pointer, in_timestamp, in_inputType);
        }
        //-------------------------------------------------------
        /// Update
        ///
        /// Used for animation
        ///
        /// @param Time between frames
        //-------------------------------------------------------
        void EditableLabel::Update(f32 infDt)
        {
            if(mbShowKeyboard && (mfTimeToShow += infDt) > 1.0f)
            {
                mfTimeToShow = 0.0f;
                mbShowKeyboard = false;
                mpKeyboard->SetTextInputEnabled(true);
            }
        }
        //-------------------------------------------------------
        /// Draw
        ///
        /// Draw the text using the canvas then draw any
        /// subviews
        ///
        /// @param Canvas renderer
        //-------------------------------------------------------
        void EditableLabel::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            CS_ASSERT(Font != nullptr, "Cannot draw editable label without a font.");
            
			//Check if this is on screen
			Core::Vector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
			Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
			
			if(vTopRight.y < 0 || vBottomLeft.y > GetScreen()->GetResolution().y || vTopRight.x < 0 || vBottomLeft.x > GetScreen()->GetResolution().x)
			{
				//Offscreen
				return;
			}
            
            DoAutosizing(inpCanvas);
			
            //Calculate the size of the label box
            Core::Vector2 vAbsoluteLabelSize = GetAbsoluteSize();
            Core::Colour AbsCol = GetAbsoluteColour();
            
            //Check if we force clip our children 
            if(ClipSubviews)
            {
                inpCanvas->PushClipBounds(vBottomLeft, vAbsoluteLabelSize);
            }
            
            //Draw ourself
            if(Background)
            {
                inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), mpWhiteTex, Core::Rectangle(Core::Vector2::k_zero, Core::Vector2::k_zero), AbsCol, Rendering::AlignmentAnchor::k_middleCentre);
            }
        
            if(Text.length() > 0)
            {
                std::string strutf8DisplayString(Text);
                
                if(SecureEntry)
                {
                    std::string strSecureText;
                    u32 udwTextLength = Core::UTF8StringUtils::CalcLength(strutf8DisplayString.begin(), strutf8DisplayString.end());
                    
                    for(u32 i=0; i<udwTextLength; ++i)
                    {
                        strSecureText.append("*");
                    }
                    
                    strutf8DisplayString = strSecureText;
                }
                
                if(mCachedChars.empty())
                {
                    f32 fAssetTextScale = GetGlobalTextScale();
                    mCachedChars = inpCanvas->BuildText(strutf8DisplayString, Font, TextScale * fAssetTextScale, LineSpacing, vAbsoluteLabelSize, MaxNumLines, HorizontalJustification, VerticalJustification).m_characters;
                }
                
                inpCanvas->DrawText(mCachedChars, GetTransform(), TextColour, Font->GetTexture());
            }
            //Draw the kids
            for(GUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
                (*it)->Draw(inpCanvas);
            }
            
            if(ClipSubviews)
            {
                inpCanvas->PopClipBounds();
            }
        }
        //-------------------------------------------------
        /// SetKeyboardInputTypeNumeric
        ///
        /// Changes the displayed keys to numberpad
        //-------------------------------------------------
        void EditableLabel::SetKeyboardInputTypeNumeric()
        {
            if(mpKeyboard)
                mpKeyboard->SetType(Input::Keyboard::Type::k_numeric);
        }
        //-------------------------------------------------
        /// SetKeyboardInputTypeText
        ///
        /// Changes the displayed keys to Text entry
        //-------------------------------------------------
        void EditableLabel::SetKeyboardInputTypeText()
        {
            if(mpKeyboard)
                mpKeyboard->SetType(Input::Keyboard::Type::k_text);
        }
        //------------------------
        /// Set Keyboard Capitalisation Method
        ///
        /// @param Capitalisation Type
        //------------------------
        void EditableLabel::SetKeyboardCapitalisationMethod(Input::Keyboard::Capitalisation ineCapitalisationType)
        {
            if(mpKeyboard)
                mpKeyboard->SetCapitalisation(ineCapitalisationType);
        }
        //-------------------------------------------------
        /// Destructor
        ///
        //-------------------------------------------------
        EditableLabel::~EditableLabel() 
        {
            if(pKeyboardListener == this)
            {
                pKeyboardListener = nullptr;
				
				if(mpKeyboard)
				{
					mpKeyboard->SetTextInputEnabled(false);
				}
            }
        }
    }
}
