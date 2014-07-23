//
//  EditableLabel.cpp
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

#include <ChilliSource/GUI/Label/EditableLabel.h>

#include <ChilliSource/Core/Base/Application.h>
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
        
        namespace
        {
            EditableLabel* g_activeEditableLabel = nullptr;
        }

        //-------------------------------------------------
        /// Constructor
        ///
        /// Default
        //-------------------------------------------------
        EditableLabel::EditableLabel() : SecureEntry(false), CharacterLimit(0), mbShowKeyboard(false), mfTimeToShow(0.0f), mbSelected(false)
        {
            m_textEntrySystem = Core::Application::Get()->GetSystem<Input::TextEntry>();
        }
        //-------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //-------------------------------------------------
        EditableLabel::EditableLabel(const Core::ParamDictionary& insParams) 
        : Label(insParams), CharacterLimit(0), SecureEntry(false), mbShowKeyboard(false), mfTimeToShow(0.0f)
        {
            m_textEntrySystem = Core::Application::Get()->GetSystem<Input::TextEntry>();
            
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
        Core::IConnectableEvent<EditableLabel::TextEventDelegate>& EditableLabel::GetTextInputEnabledEvent()
        {
            return m_textInputEnabledEvent;
        }
        //-------------------------------------------------
        //-------------------------------------------------
        Core::IConnectableEvent<EditableLabel::TextEventDelegate>& EditableLabel::GetTextInputDisabledEvent()
        {
            return m_textInputDisabledEvent;
        }
        //-------------------------------------------------
        //-------------------------------------------------
        Core::IConnectableEvent<EditableLabel::TextEventDelegate>& EditableLabel::GetTextInputReceivedEvent()
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
        //-------------------------------------------------
        void EditableLabel::EnableTextInput()
        {
            if(m_textEntrySystem)
            {
                mbShowKeyboard = true;
                
                if(g_activeEditableLabel != nullptr)
                {
                    g_activeEditableLabel->DisableTextInput();
                }
            }
        }
        //-------------------------------------------------
        /// Hide Keyboard
        //-------------------------------------------------
        void EditableLabel::DisableTextInput()
        {
            if(g_activeEditableLabel != this)
                return;
            
            m_textEntrySystem->Deactivate();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void EditableLabel::SetText(const std::string& instrText)
        {
            m_textEntrySystem->SetTextBuffer(instrText);
            Label::SetText(instrText);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void EditableLabel::ClearText()
        {
            m_textEntrySystem->SetTextBuffer("");
            Label::SetText("");
        }
        //----------------------------------------------------
		//----------------------------------------------------
		bool EditableLabel::OnTextBufferChanged(const std::string& in_text)
		{
            if(CharacterLimit > 0)
            {
                //We can reject the text if it exceeds our input limit
                auto length = Core::UTF8StringUtils::CalcLength(in_text.begin(), in_text.end());
                if(length > CharacterLimit)
                {
                    return false;
                }
            }

            Label::SetText(in_text);
            m_textInputReceivedEvent.NotifyConnections(this);
            return true;
		}
        //-------------------------------------------------
        //----------------------------------------------------
        void EditableLabel::OnTextInputDisabled()
        {
            m_textInputDisabledEvent.NotifyConnections(this);
            g_activeEditableLabel = nullptr;
            mbShowKeyboard = false;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        bool EditableLabel::OnPointerDown(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType)
        {
            if(UserInteraction)
            {
                mbSelected = true;
            }
            
            return GUIView::OnPointerDown(in_pointer, in_timestamp, in_inputType);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void EditableLabel::OnPointerUp(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType)
        {
            if(UserInteraction && Visible && m_textEntrySystem && mbSelected)
            {
               if(Contains(in_pointer.GetPosition()))
               {
                   //Flag the keyboard as hidden and wait a few seconds so we can slide it in again
				   if(g_activeEditableLabel != this)
				   {
                       EnableTextInput();
				   }
				   else
				   {
                       DisableTextInput();
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
            if(mbShowKeyboard && (mfTimeToShow += infDt) > 0.5f)
            {
                mfTimeToShow = 0.0f;
                mbShowKeyboard = false;
                m_textEntrySystem->Activate(Text, m_type, m_capitalisation,
                                            CSCore::MakeDelegate(this, &EditableLabel::OnTextBufferChanged),
                                            CSCore::MakeDelegate(this, &EditableLabel::OnTextInputDisabled));
                m_textInputEnabledEvent.NotifyConnections(this);
                g_activeEditableLabel = this;
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
                inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), Core::Vector2::k_zero, mpWhiteTex, Rendering::UVs(0.0f, 0.0f, 1.0f, 1.0f), AbsCol, Rendering::AlignmentAnchor::k_middleCentre);
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
        //-------------------------------------------------
        void EditableLabel::SetInputTypeNumeric()
        {
            m_type = Input::TextEntry::Type::k_numeric;
        }
        //-------------------------------------------------
        //-------------------------------------------------
        void EditableLabel::SetInputTypeText()
        {
            m_type = Input::TextEntry::Type::k_text;
        }
        //------------------------
        //------------------------
        void EditableLabel::SetCapitalisationMethod(Input::TextEntry::Capitalisation ineCapitalisationType)
        {
            m_capitalisation = ineCapitalisationType;
        }
        //------------------------
        //------------------------
        EditableLabel::~EditableLabel()
        {
            DisableTextInput();
        }
    }
}
