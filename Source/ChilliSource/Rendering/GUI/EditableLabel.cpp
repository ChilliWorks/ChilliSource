//
//  GUIEditableLabel.cpp
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/GUI/EditableLabel.h>

#include <ChilliSource/Core/Base/Screen.h>

#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

namespace ChilliSource
{
    namespace Rendering
    {
		DEFINE_META_CLASS(CEditableLabel)

		DEFINE_PROPERTY(SecureEntry);
		DEFINE_PROPERTY(CharacterLimit);

        CEditableLabel* CEditableLabel::pKeyboardListener = NULL;
        
        //Res density of iPhone 4, used to normalise text spacing
        const f32 kfScalableFontResDensity = 2.0f;
        
        //-------------------------------------------------
        /// Constructor
        ///
        /// Default
        //-------------------------------------------------
        CEditableLabel::CEditableLabel() : mpKeyboard(NULL), SecureEntry(false), CharacterLimit(0), mbShowKeyboard(false), mfTimeToShow(0.0f), mu32SeparatorSpacing(0), mutf8strSeparator(""), mutf8strTextWithSeparators(""), mbSelected(false)
        {
            
        }
        //-------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //-------------------------------------------------
        CEditableLabel::CEditableLabel(const Core::ParamDictionary& insParams) 
        : CLabel(insParams), mpKeyboard(NULL), CharacterLimit(0), SecureEntry(false), mbShowKeyboard(false), mfTimeToShow(0.0f), mu32SeparatorSpacing(0), mutf8strSeparator(""), mutf8strTextWithSeparators("")
        {
			std::string strValue;

			//---Texture
			if(insParams.TryGetValue("SecureEntry", strValue))
			{
				EnableSecureEntry(Core::CStringConverter::ParseBool(strValue));
			}
			//---Sprite sheet
			if(insParams.TryGetValue("CharacterLimit", strValue))
			{
				SetCharacterLimit(Core::CStringConverter::ParseUnsignedInt(strValue));
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
        IEvent<Input::KeyboardEventDelegate>& CEditableLabel::GetKeyboardShowEvent()
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
        IEvent<Input::KeyboardEventDelegate>& CEditableLabel::GetKeyboardHideEvent()
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
        IEvent<CEditableLabel::TextChangeEventDelegate>& CEditableLabel::GetTextChangeEvent()
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
        void CEditableLabel::EnableSecureEntry(bool inbEnabled)
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
		bool CEditableLabel::IsSecureEntryEnabled() const
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
        void CEditableLabel::SetCharacterLimit(u32 inudwLimit)
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
		u32 CEditableLabel::GetCharacterLimit() const
		{
			return CharacterLimit;
		}
		//-------------------------------------------------
		/// Get Keyboard Ptr
		///
		/// @return Virtual keyboard
		//-------------------------------------------------
		Input::IVirtualKeyboard* CEditableLabel::GetKeyboardPtr()
		{
			return mpKeyboard;
		}
        //-------------------------------------------------
        /// Set Keyboard
        ///
        /// @param Virtual keyboard
        //-------------------------------------------------
        void CEditableLabel::SetKeyboard(Input::IVirtualKeyboard* inpKeyboard)
        {
            if(mpKeyboard)
            {
                //Stop listening to old keyboard
                mpKeyboard->GetKeyboardShowEvent() -= Input::KeyboardEventDelegate(this, &CEditableLabel::OnKeyboardShown);
                mpKeyboard->GetKeyboardHideEvent() -= Input::KeyboardEventDelegate(this, &CEditableLabel::OnKeyboardHidden);
            }
            
            if(inpKeyboard)
            {
                mpKeyboard = inpKeyboard;
                
                //Stop listening to old keyboard
                mpKeyboard->GetKeyboardShowEvent() += Input::KeyboardEventDelegate(this, &CEditableLabel::OnKeyboardShown);
                mpKeyboard->GetKeyboardHideEvent() += Input::KeyboardEventDelegate(this, &CEditableLabel::OnKeyboardHidden);
            }
            else
            {
                HideKeyboard();
            }
        }
        //-------------------------------------------------
        /// Show Keyboard
        //-------------------------------------------------
        void CEditableLabel::ShowKeyboard()
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
        void CEditableLabel::HideKeyboard()
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
        void CEditableLabel::OnKeyboardShown()
        {
            if(pKeyboardListener == this)
            {
                mpKeyboard->SetText(Text);
                mpKeyboard->GetKeyboardTextChangeEvent() += Input::TextInputEventDelegate(this, &CEditableLabel::OnKeyboardTextChanged);
                mOnKeyboardShowEvent.Invoke();
            }
        }
        
        void CEditableLabel::ClearText()
        {
            mpKeyboard->SetText("");
            CLabel::SetText("");
        }
        //----------------------------------------------------
		/// On Keyboard Text Changed
		///
		/// Used to get the text from the keyboard
        ///
        /// @param Contents of the keyboard
        /// @param Whether to accept the input
		//----------------------------------------------------
		void CEditableLabel::OnKeyboardTextChanged(const UTF8String& instrText, bool* inbRejectInput)
		{
            //We can reject the text if it exceeds our input limit
            if(CharacterLimit > 0 && instrText.length() > CharacterLimit)
            {
                *inbRejectInput = true;
            }
            else
            {
                CLabel::SetText(instrText);
                *inbRejectInput = false;
                
                // if we're using separators, calculate the separated string now
                if(mutf8strSeparator.size() > 0)
                    mutf8strTextWithSeparators = GetTextWithSeparators();

                mOnTextChangeEvent.Invoke(this);
            }
		}
        //-------------------------------------------------
        /// On Keyboard Hidden
        ///
        /// Triggered when the keyboard disappears
        //-------------------------------------------------
        void CEditableLabel::OnKeyboardHidden()
        {
            if(pKeyboardListener == this)
            {
                mpKeyboard->GetKeyboardTextChangeEvent() -= Input::TextInputEventDelegate(this, &CEditableLabel::OnKeyboardTextChanged);
                pKeyboardListener = NULL;
                mOnKeyboardHideEvent.Invoke();
               
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
        bool CEditableLabel::OnTouchBegan(const Input::TouchInfo & insTouchInfo)
        {
            if(UserInteraction)
            {
                mbSelected = true;
            }
            
            return CGUIView::OnTouchBegan(insTouchInfo);
        }
        //-----------------------------------------------------------
        /// On Touch Ended
        ///
        /// Called when the view stops receiving cursor/touch input
        ///
        /// @param Touch data
        //-----------------------------------------------------------
        void CEditableLabel::OnTouchEnded(const Input::TouchInfo & insTouchInfo)
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
            
            CGUIView::OnTouchEnded(insTouchInfo);
        }
        //-------------------------------------------------------
        /// Update
        ///
        /// Used for animation
        ///
        /// @param Time between frames
        //-------------------------------------------------------
        void CEditableLabel::Update(f32 infDt)
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
        void CEditableLabel::Draw(Rendering::CCanvasRenderer* inpCanvas)
        {
			//Check if this is on screen
			Core::CVector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topRight);
			Core::CVector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_bottomLeft);
			
			if(vTopRight.y < 0 || vBottomLeft.y > Core::CScreen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::CScreen::GetOrientedWidth())
			{
				//Offscreen
				return;
			}
            
            DoAutosizing(inpCanvas);
			
            //Calculate the size of the label box
            Core::CVector2 vAbsoluteLabelSize = GetAbsoluteSize();
            Core::CColour AbsCol = GetAbsoluteColour();
            
            //Check if we force clip our children 
            if(ClipSubviews)
            {
                inpCanvas->EnableClippingToBounds(vBottomLeft, vAbsoluteLabelSize);
            }
            
            //Draw ourself
            if(Background)
            {
                inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), mpWhiteTex, Core::Rectangle(Core::CVector2::ZERO, Core::CVector2::ZERO), AbsCol);
            }
        
            UTF8String strutf8DisplayString(Text);
            
            // handle separators if we're using one
            if(mutf8strSeparator.length() > 0)
            {
                strutf8DisplayString = mutf8strTextWithSeparators;
            }
            
            if(strutf8DisplayString.length() > 0)
            {
                if(SecureEntry)
                {
                    UTF8String strSecureText;
                    u32 udwTextLength = strutf8DisplayString.length();
                    
                    for(u32 i=0; i<udwTextLength; ++i)
                    {
                        strSecureText.appendChar('*');
                    }
                    
                    strutf8DisplayString = strSecureText;
                }
                
                if(ScalableFont)
                {
                    f32 fAssetScale = Core::IFileSystem::GetDeviceResourcesDensity();
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
            for(CGUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
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
        void CEditableLabel::SetText(const UTF8String& instrText)
        {
            CLabel::SetText(instrText);
            
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
        void CEditableLabel::SetKeyboardInputTypeNumeric()
        {
            if(mpKeyboard)
                mpKeyboard->SetKeyboardType(moFlo::Input::KeyboardType::k_numeric);
        }
        //-------------------------------------------------
        /// SetKeyboardInputTypeText
        ///
        /// Changes the displayed keys to Text entry
        //-------------------------------------------------
        void CEditableLabel::SetKeyboardInputTypeText()
        {
            if(mpKeyboard)
                mpKeyboard->SetKeyboardType(moFlo::Input::KeyboardType::k_text);
        }
        //------------------------
        /// Set Keyboard Capitalisation Method
        ///
        /// @param Capitalisation Type
        //------------------------
        void CEditableLabel::SetKeyboardCapitalisationMethod(moInput::KeyboardCapitalisation ineCapitalisationType)
        {
            if(mpKeyboard)
                mpKeyboard->SetCapitalisationMethod(ineCapitalisationType);
        }
        //-------------------------------------------------
        /// SetText
        ///
        /// override to permit last text length tracking
        //-------------------------------------------------
        UTF8String CEditableLabel::GetTextWithSeparators() const
        {
            UTF8String strutf8Out("");
            
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
        void CEditableLabel::SetTextSeparator(const UTF8String& inutf8strSeparator)
        {
            mutf8strSeparator = inutf8strSeparator;
        }
        //-------------------------------------------------
        /// SetTextSeparatorSpacing
        ///
        /// Sets the chars between separators, 0 for no separators
        //-------------------------------------------------
        void CEditableLabel::SetTextSeparatorSpacing(u32 inu32Spacing)
        {
            mu32SeparatorSpacing = inu32Spacing;
        }
        //-------------------------------------------------
        /// Destructor
        ///
        //-------------------------------------------------
        CEditableLabel::~CEditableLabel() 
        {
            if(pKeyboardListener == this)
            {
                pKeyboardListener = NULL;
				
				if(mpKeyboard)
				{
					mpKeyboard->Hide();
				}
            }
            
            if(mpKeyboard)
            {
                //Stop listening to old keyboard
                mpKeyboard->GetKeyboardShowEvent() -= Input::KeyboardEventDelegate(this, &CEditableLabel::OnKeyboardShown);
                mpKeyboard->GetKeyboardTextChangeEvent() -= Input::TextInputEventDelegate(this, &CEditableLabel::OnKeyboardTextChanged);
                mpKeyboard->GetKeyboardHideEvent() -= Input::KeyboardEventDelegate(this, &CEditableLabel::OnKeyboardHidden);
            }
        }
    }
}
