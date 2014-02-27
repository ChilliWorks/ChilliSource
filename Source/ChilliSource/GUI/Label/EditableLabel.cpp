//
//  GUIEditableLabel.cpp
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Label/EditableLabel.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(EditableLabel)

		DEFINE_PROPERTY(SecureEntry);
		DEFINE_PROPERTY(CharacterLimit);

        EditableLabel* EditableLabel::pKeyboardListener = nullptr;
        
        //Res density of iPhone 4, used to normalise text spacing
        const f32 kfScalableFontResDensity = 2.0f;
        
        //-------------------------------------------------
        /// Constructor
        ///
        /// Default
        //-------------------------------------------------
        EditableLabel::EditableLabel() : mpKeyboard(nullptr), SecureEntry(false), CharacterLimit(0), mbShowKeyboard(false), mfTimeToShow(0.0f), mu32SeparatorSpacing(0), mutf8strSeparator(""), mutf8strTextWithSeparators(""), mbSelected(false)
        {
            
        }
        //-------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //-------------------------------------------------
        EditableLabel::EditableLabel(const Core::ParamDictionary& insParams) 
        : Label(insParams), mpKeyboard(nullptr), CharacterLimit(0), SecureEntry(false), mbShowKeyboard(false), mfTimeToShow(0.0f), mu32SeparatorSpacing(0), mutf8strSeparator(""), mutf8strTextWithSeparators("")
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
        /// Get Keyboard Show Event
        ///
        /// Use to register delegate for when keyboard
        /// becomes active
        ///
        /// @param Keyboard Event Delegate
        //-------------------------------------------------
        Core::IConnectableEvent<Input::KeyboardEventDelegate>& EditableLabel::GetKeyboardShowEvent()
        {
            return mOnKeyboardShowEvent;
        }
        //-------------------------------------------------
        /// Get Keyboard Hide Event
        ///
        /// Use to register delegate for when keyboard
        /// stops being active
        ///
        /// @param Keyboard Event Delegate
        //-------------------------------------------------
        Core::IConnectableEvent<Input::KeyboardEventDelegate>& EditableLabel::GetKeyboardHideEvent()
        {
            return mOnKeyboardHideEvent;
        }
        //-------------------------------------------------
        /// Get Text Change Event
        ///
        /// Use to register delegate for when a character
        /// is input using the keyboard
        ///
        /// @param Text Change Event Delegate
        //-------------------------------------------------
        Core::IConnectableEvent<EditableLabel::TextChangeEventDelegate>& EditableLabel::GetTextChangeEvent()
        {
            return mOnTextChangeEvent;
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
		Input::VirtualKeyboard* EditableLabel::GetKeyboardPtr()
		{
			return mpKeyboard;
		}
        //-------------------------------------------------
        /// Set Keyboard
        ///
        /// @param Virtual keyboard
        //-------------------------------------------------
        void EditableLabel::SetKeyboard(Input::VirtualKeyboard* inpKeyboard)
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
                m_keyboardShownConnection = mpKeyboard->GetKeyboardShowEvent().OpenConnection(Core::MakeDelegate(this, &EditableLabel::OnKeyboardShown));
                m_keyboardHiddenConnection = mpKeyboard->GetKeyboardHideEvent().OpenConnection(Core::MakeDelegate(this, &EditableLabel::OnKeyboardHidden));
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
                mpKeyboard->Show();
            }
        }
        //-------------------------------------------------
        /// Hide Keyboard
        //-------------------------------------------------
        void EditableLabel::HideKeyboard()
        {
            if(mpKeyboard)
            {
                mpKeyboard->Hide();
                mbShowKeyboard = false;
                pKeyboardListener = this;
            }
        }
        //-------------------------------------------------
        /// On Keyboard Shown
        ///
        /// Triggered when the keyboard displays
        //-------------------------------------------------
        void EditableLabel::OnKeyboardShown()
        {
            if(pKeyboardListener == this)
            {
                mpKeyboard->SetText(Text);
                m_keyboardTextChangedConnection = mpKeyboard->GetKeyboardTextChangeEvent().OpenConnection(Core::MakeDelegate(this, &EditableLabel::OnKeyboardTextChanged));
                mOnKeyboardShowEvent.NotifyConnections();
            }
        }
        
        void EditableLabel::ClearText()
        {
            mpKeyboard->SetText("");
            Label::SetText("");
        }
        //----------------------------------------------------
		/// On Keyboard Text Changed
		///
		/// Used to get the text from the keyboard
        ///
        /// @param Contents of the keyboard
        /// @param Whether to accept the input
		//----------------------------------------------------
		void EditableLabel::OnKeyboardTextChanged(const Core::UTF8String& instrText, bool* inbRejectInput)
		{
            //We can reject the text if it exceeds our input limit
            if(CharacterLimit > 0 && instrText.length() > CharacterLimit)
            {
                *inbRejectInput = true;
            }
            else
            {
                Label::SetText(instrText);
                *inbRejectInput = false;
                
                // if we're using separators, calculate the separated string now
                if(mutf8strSeparator.size() > 0)
                    mutf8strTextWithSeparators = GetTextWithSeparators();

                mOnTextChangeEvent.NotifyConnections(this);
            }
		}
        //-------------------------------------------------
        /// On Keyboard Hidden
        ///
        /// Triggered when the keyboard disappears
        //-------------------------------------------------
        void EditableLabel::OnKeyboardHidden()
        {
            if(pKeyboardListener == this)
            {
                m_keyboardTextChangedConnection = nullptr;
                pKeyboardListener = nullptr;
                mOnKeyboardHideEvent.NotifyConnections();
               
            }
        }
        //-----------------------------------------------------------
        /// On Touch Began
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        /// @return Whether touch has been consumed
        //-----------------------------------------------------------
        bool EditableLabel::OnTouchBegan(const Input::TouchInfo & insTouchInfo)
        {
            if(UserInteraction)
            {
                mbSelected = true;
            }
            
            return GUIView::OnTouchBegan(insTouchInfo);
        }
        //-----------------------------------------------------------
        /// On Touch Ended
        ///
        /// Called when the view stops receiving cursor/touch input
        ///
        /// @param Touch data
        //-----------------------------------------------------------
        void EditableLabel::OnTouchEnded(const Input::TouchInfo & insTouchInfo)
        {
            if(UserInteraction && Visible && mpKeyboard && mbSelected)
            {
               if(Contains(insTouchInfo.vLocation))
               {
                   //Flag the keyboard as hidden and wait a few seconds so we can slide it in again
				   if(!mpKeyboard->IsActive())
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
            
            GUIView::OnTouchEnded(insTouchInfo);
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
                mpKeyboard->Show();
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
			//Check if this is on screen
			Core::Vector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
			Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
			
			if(vTopRight.y < 0 || vBottomLeft.y > Core::Screen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::Screen::GetOrientedWidth())
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
                inpCanvas->EnableClippingToBounds(vBottomLeft, vAbsoluteLabelSize);
            }
            
            //Draw ourself
            if(Background)
            {
                inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), mpWhiteTex, Core::Rectangle(Core::Vector2::ZERO, Core::Vector2::ZERO), AbsCol);
            }
        
            Core::UTF8String strutf8DisplayString(Text);
            
            // handle separators if we're using one
            if(mutf8strSeparator.length() > 0)
            {
                strutf8DisplayString = mutf8strTextWithSeparators;
            }
            
            if(strutf8DisplayString.length() > 0)
            {
                if(SecureEntry)
                {
                    Core::UTF8String strSecureText;
                    u32 udwTextLength = strutf8DisplayString.length();
                    
                    for(u32 i=0; i<udwTextLength; ++i)
                    {
                        strSecureText.appendChar('*');
                    }
                    
                    strutf8DisplayString = strSecureText;
                }
                
                if(ScalableFont)
                {
                    f32 fAssetScale = Core::FileSystem::GetDeviceResourcesDensity();
                    f32 fAssetTextScale = ScalableHeight == 0 ? fAssetScale : (ScalableHeight * fAssetScale) / Font->GetLineHeight();
					f32 fCharacterSpacingScaled =  CharacterSpacing * fAssetScale;
                    
                    if(TextOutlined)
                    {
                        inpCanvas->DrawDistanceOutlinedString(strutf8DisplayString, GetTransform(), TextScale * fAssetTextScale, Font, mCachedChars, TextColour * GetInheritedOpacity(), TextOutlineColour * GetInheritedOpacity(),
                                                              vAbsoluteLabelSize, fCharacterSpacingScaled, LineSpacing, HorizontalJustification, VerticalJustification, FlipVertical, TextOverflowBehaviour::k_clip, MaxNumLines);
                    }
                    else
                    {
                        inpCanvas->DrawDistanceString(strutf8DisplayString, GetTransform(), TextScale * fAssetTextScale, Font, mCachedChars, TextColour * GetInheritedOpacity(),
                                                      vAbsoluteLabelSize, fCharacterSpacingScaled, LineSpacing, HorizontalJustification, VerticalJustification, FlipVertical, TextOverflowBehaviour::k_clip, MaxNumLines);
                    }
                }
                else
                {
                    inpCanvas->DrawString(strutf8DisplayString, GetTransform(), TextScale, Font, mCachedChars, TextColour,
                                          vAbsoluteLabelSize, CharacterSpacing, LineSpacing, HorizontalJustification, VerticalJustification, false, TextOverflowBehaviour::k_follow, MaxNumLines);
                }
            }
            //Draw the kids
            for(GUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
                (*it)->Draw(inpCanvas);
            }
            
            if(ClipSubviews)
            {
                inpCanvas->DisableClippingToBounds();
            }
        }
        //-------------------------------------------------------
        /// SetText
        ///
        /// Overridden set text method, updates cached separator string
        /// @param instrText text to set as entry
        //-------------------------------------------------------
        void EditableLabel::SetText(const Core::UTF8String& instrText)
        {
            Label::SetText(instrText);
            
            // handle cached separator string
            if(mutf8strSeparator.size() > 0)
                mutf8strTextWithSeparators = GetTextWithSeparators();
            else
                mutf8strTextWithSeparators = instrText;
        }
        //-------------------------------------------------
        /// SetKeyboardInputTypeNumeric
        ///
        /// Changes the displayed keys to numberpad
        //-------------------------------------------------
        void EditableLabel::SetKeyboardInputTypeNumeric()
        {
            if(mpKeyboard)
                mpKeyboard->SetKeyboardType(ChilliSource::Input::KeyboardType::k_numeric);
        }
        //-------------------------------------------------
        /// SetKeyboardInputTypeText
        ///
        /// Changes the displayed keys to Text entry
        //-------------------------------------------------
        void EditableLabel::SetKeyboardInputTypeText()
        {
            if(mpKeyboard)
                mpKeyboard->SetKeyboardType(ChilliSource::Input::KeyboardType::k_text);
        }
        //------------------------
        /// Set Keyboard Capitalisation Method
        ///
        /// @param Capitalisation Type
        //------------------------
        void EditableLabel::SetKeyboardCapitalisationMethod(Input::KeyboardCapitalisation ineCapitalisationType)
        {
            if(mpKeyboard)
                mpKeyboard->SetCapitalisationMethod(ineCapitalisationType);
        }
        //-------------------------------------------------
        /// SetText
        ///
        /// override to permit last text length tracking
        //-------------------------------------------------
        Core::UTF8String EditableLabel::GetTextWithSeparators() const
        {
            Core::UTF8String strutf8Out("");
            
            // put in spacers for readability
            for(u32 i(0), sep(0); i < Text.size(); ++i)
            {
                strutf8Out += Text[i];
                
                // every seglength, push a separator, unless we're at the end
                if(sep == mu32SeparatorSpacing-1 && i < Text.size()-1)
                {
                    for(u32 s(0); s < mutf8strSeparator.size(); ++s)
                    {
                        strutf8Out += mutf8strSeparator[s];
                    }
                    sep = 0;
                }
                else
                {
                    ++sep;
                }
            }
            
            return strutf8Out;
        }
        //-------------------------------------------------
        /// SetTextSeparator
        ///
        /// Sets the separator that will be used in GetTextWithSeparators
        //-------------------------------------------------
        void EditableLabel::SetTextSeparator(const Core::UTF8String& inutf8strSeparator)
        {
            mutf8strSeparator = inutf8strSeparator;
        }
        //-------------------------------------------------
        /// SetTextSeparatorSpacing
        ///
        /// Sets the chars between separators, 0 for no separators
        //-------------------------------------------------
        void EditableLabel::SetTextSeparatorSpacing(u32 inu32Spacing)
        {
            mu32SeparatorSpacing = inu32Spacing;
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
					mpKeyboard->Hide();
				}
            }
        }
    }
}
