//
//  Label.cpp
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Label/Label.h>

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Font/FontManager.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(CLabel)

		//---Properties
		DEFINE_PROPERTY(Text);
		DEFINE_PROPERTY(TextID);
		DEFINE_PROPERTY(MaxNumLines);
		DEFINE_PROPERTY(TextScale);
		DEFINE_PROPERTY(CharacterSpacing);
		DEFINE_PROPERTY(LineSpacing);
		DEFINE_PROPERTY(HorizontalJustification);
		DEFINE_PROPERTY(VerticalJustification);
		DEFINE_PROPERTY(Font);
		DEFINE_PROPERTY(TextColour);
		DEFINE_PROPERTY(Background);
		DEFINE_PROPERTY(Autosizing);
		DEFINE_PROPERTY(UnifiedMaxSize);
		DEFINE_PROPERTY(UnifiedMinSize);
        DEFINE_PROPERTY(TextOutlined);
        DEFINE_PROPERTY(TextOutlineColour);
        DEFINE_PROPERTY(FlipVertical);
        
        f32 CLabel::mfGlobalTextScale = 1.0f;

        //Res density of iPhone 4, used to normalise text spacing
        const f32 kfScalableFontResDensity = 2.0f;
        
        //-------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //-------------------------------------------------------
        CLabel::CLabel() : MaxNumLines(0), TextScale(1.0f), CharacterSpacing(0.0f), LineSpacing(1.0f), HorizontalJustification(TextJustification::k_left),
		VerticalJustification(TextJustification::k_centre), Background(true), Autosizing(false), ScalableFont(false), ScalableHeight(0), TextOutlined(false), FlipVertical(false), mbLastDrawWasClipped(false), mbLastDrawHadInvalidCharacter(false)
        {
            SetColour(Core::CColour(0.18f, 0.3f, 0.4f, 0.6f));
            Rendering::ITextureManager* pMgr = Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerWithInterface<Rendering::ITextureManager>();
            mpWhiteTex = pMgr->GetDefaultTexture();
            
            //Grab the default font
            Font = Core::CApplication::GetDefaultFont();
            
            ConsumesTouches = false;
            
			UnifiedSize = Core::UnifiedVector2(0.2f, 0.05f, 0.0f, 0.0f);
            UnifiedMaxSize = GetSize();
            UnifiedMinSize = GetSize();
        }
        //-------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //-------------------------------------------------------
        CLabel::CLabel(const Core::ParamDictionary& insParams) 
        : CGUIView(insParams), MaxNumLines(0), TextScale(1.0f), CharacterSpacing(0.0f), LineSpacing(1.0f), HorizontalJustification(TextJustification::k_left),
		VerticalJustification(TextJustification::k_centre), Background(true), Autosizing(false), ScalableFont(false), ScalableHeight(0), TextOutlined(false), FlipVertical(false)
        {
            std::string strValue;
            
            UnifiedMaxSize = GetSize();
            UnifiedMinSize = GetSize();
            
            //---Text ID
            if(insParams.TryGetValue("TextID", strValue))
            {
                SetTextID(strValue);
            }
            //---Text
            if(insParams.TryGetValue("Text", strValue))
            {
                SetText(Core::UTF8String(strValue));
            }
            //---Number of lines
            if(insParams.TryGetValue("NumLines", strValue))
            {
                MaxNumLines = Core::ParseU32(strValue);
            }
            //---Text Scale
            if(insParams.TryGetValue("TextScale", strValue))
            {
                TextScale = Core::ParseF32(strValue);
            }
            //---Character spacing
            if(insParams.TryGetValue("CharacterSpacing", strValue))
            {
                CharacterSpacing = Core::ParseF32(strValue);
            }
            //---Unified maximum size
            if(insParams.TryGetValue("UnifiedMaxSize", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::CVector4 vRawSize = Core::ParseVector4(strValue);
                SetMaximumSize(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w);
            }
            //---Unified maximum size
            if(insParams.TryGetValue("UnifiedMinSize", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::CVector4 vRawSize = Core::ParseVector4(strValue);
                SetMinimumSize(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w);
            }
            //---Line spacing
            if(insParams.TryGetValue("LineSpacing", strValue))
            {
                LineSpacing = Core::ParseF32(strValue);
            }
            //---Text justification
            if(insParams.TryGetValue("HorizontalJustification", strValue))
            {
                HorizontalJustification = JustificationFromString(strValue);
            }
			if(insParams.TryGetValue("VerticalJustification", strValue))
            {
                VerticalJustification = JustificationFromString(strValue);
            }
            //---Font
            Core::StorageLocation eFontLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("FontLocation", strValue))
            {
                eFontLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("Font", strValue))
            {
                Font = LOAD_RESOURCE(Rendering::CFont, eFontLocation, strValue);
            }
            //---Text Colour
            if(insParams.TryGetValue("TextColour", strValue))
            {
                TextColour = Core::ParseColour(strValue);
            }
            //---Text Outline
            if(insParams.TryGetValue("EnableTextOutline", strValue))
            {
                TextOutlined = Core::ParseBool(strValue);
            }
            //---Text Outline Colour
            if(insParams.TryGetValue("TextOutlineColour", strValue))
            {
                TextOutlined = true;
                TextOutlineColour = Core::ParseColour(strValue);
            }
            //---Enable Background Colour
            if(insParams.TryGetValue("EnableBackground", strValue))
            {
                Background = Core::ParseBool(strValue);
            }
            //---Enable Autosizing
            if(insParams.TryGetValue("EnableAutosizing", strValue))
            {
                Autosizing = Core::ParseBool(strValue);
            }
            if(insParams.TryGetValue("ScalableFont", strValue))
            {
                ScalableFont = Core::ParseBool(strValue);
            }
            if(insParams.TryGetValue("ScalableHeight", strValue))
            {
                ScalableHeight = Core::ParseF32(strValue);
            }
            //---UV Flipped Y
            if(insParams.TryGetValue("FlipVertical", strValue))
            {
                FlipVertical = true;
            }
            
            Rendering::ITextureManager* pMgr = Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerWithInterface<Rendering::ITextureManager>();
            mpWhiteTex = pMgr->GetDefaultTexture();
            
            if(!Font)
            {
                //Load default placeholder font
                Font = Core::CApplication::GetDefaultFont();
            }
        }
        //-------------------------------------------------------
        /// Set Text
        ///
        /// @param Text string
        //-------------------------------------------------------
        void CLabel::SetText(const Core::UTF8String& instrText)
        {
            Text = instrText;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Get Text
        ///
        /// @returnText string
        //-------------------------------------------------------
        const Core::UTF8String& CLabel::GetText() const
        {
            return Text;
        }
		//-------------------------------------------------------
		/// Set Text ID
		///
		/// @param Text string representing lookup ID
		//-------------------------------------------------------
		void CLabel::SetTextID(const std::string& instrText)
		{
			TextID = instrText;

			if(!TextID.empty())
			{
				SetText(Core::CLocalisedText::GetText(instrText));
			}
			else
			{
				SetText("");
			}
		}
		//-------------------------------------------------------
		/// Get Text ID
		///
		/// @return Text string representing lookup ID
		//-------------------------------------------------------
		const std::string& CLabel::GetTextID() const
		{
			return TextID;
		}
        //-------------------------------------------------------
        /// Set Font
        ///
        /// @param Font used to display text
        //-------------------------------------------------------
        void CLabel::SetFont(const Rendering::FontPtr& inpFont)
        {
            Font = inpFont;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Get Font
        ///
        /// @return Font used to display text
        //-------------------------------------------------------
        const Rendering::FontPtr& CLabel::GetFont() const
        {
            return Font;
        }
        //-------------------------------------------------------
        /// Set Maximum Size
        ///
        /// The unified size to which this label will expand 
        /// after which the text will clip
        ///
        /// @param Unified vector
        //-------------------------------------------------------
        void CLabel::SetMaximumSize(const Core::UnifiedVector2& invSize)
        {
            UnifiedMaxSize = invSize;

			mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Set Minimum Size
        ///
        /// The unified size to which this label will shrink 
        ///
        /// @param Unified vector
        //-------------------------------------------------------
        void CLabel::SetMinimumSize(const Core::UnifiedVector2& invSize)
        {
            UnifiedMinSize = invSize;

			mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Set Maximum Size
        ///
        /// The unified size to which this label will expand 
        /// after which the text will clip
        ///
        /// @param Unified vector RX, RY, AX & AY
        //-------------------------------------------------------
        void CLabel::SetMaximumSize(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
        {
            UnifiedMaxSize.vRelative.x = infRx;
            UnifiedMaxSize.vRelative.y = infRy;
            UnifiedMaxSize.vAbsolute.x = infAx;
            UnifiedMaxSize.vAbsolute.y = infAy;

			mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Set Minimum Size
        ///
        /// The unified size to which this label will shrink 
        ///
        /// @param Unified vector RX, RY, AX & AY
        //-------------------------------------------------------
        void CLabel::SetMinimumSize(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
        {
            UnifiedMinSize.vRelative.x = infRx;
            UnifiedMinSize.vRelative.y = infRy;
            UnifiedMinSize.vAbsolute.x = infAx;
            UnifiedMinSize.vAbsolute.y = infAy;

			mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Minimum Size
		///
		/// The unified size to which this label will shrink 
		///
		/// @return Unified vector
		//-------------------------------------------------------
		const Core::UnifiedVector2& CLabel::GetMinimumSize() const
		{
			return UnifiedMinSize;
		}
		//-------------------------------------------------------
		/// Get Maximum Size
		///
		/// The unified size to which this label will grow 
		///
		/// @return Unified vector
		//-------------------------------------------------------
		const Core::UnifiedVector2& CLabel::GetMaximumSize() const
		{
			return UnifiedMaxSize;
		}
        //-------------------------------------------------------
        /// Enable Autosizing
        ///
        /// @param Whether the label should grow and shrink
        /// based on the text contents
        //-------------------------------------------------------
        void CLabel::EnableAutosizing(bool inbEnabled)
        {
            Autosizing = inbEnabled;

			mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Enable Autosizing
		///
		/// @return Whether the label should grow and shrink
		/// based on the text contents
		//-------------------------------------------------------
		bool CLabel::IsAutosizingEnabled() const
		{
			return Autosizing;
		}
		//-----------------------------------------------------------
		/// On Screen Orientation Changed
		///
		/// Triggered if the screen orientation changes so we can
		/// resize ourself
		//-----------------------------------------------------------
		void CLabel::OnScreenOrientationChanged()
		{
			mCachedChars.clear();

			CGUIView::OnScreenOrientationChanged();
		}
        //-------------------------------------------------------
        /// Set Number of Lines
        ///
        /// The maximum number of lines for which the text will
        /// wrap. If this is zero then the number of lines is
        /// not fixed
        ///
        /// The text will wrap based on the width of the label
        ///
        /// @param Num lines
        //-------------------------------------------------------
        void CLabel::SetNumberOfLines(u32 inudwNumLines)
        {
            MaxNumLines = inudwNumLines;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Set Number of Lines
        ///
        /// The maximum number of lines for which the text will
        /// wrap. If this is zero then the number of lines is
        /// not fixed
        ///
        /// The text will wrap based on the width of the label
        ///
        /// @return Num lines
        //-------------------------------------------------------
        u32 CLabel::GetNumberOfLines() const
        {
            return MaxNumLines;
        }
        //-------------------------------------------------------
        /// Set Line Spacing
        ///
        /// Set the space left vertically between lines 
        ///
        /// @param Absolute Scalar
        //-------------------------------------------------------
        void CLabel::SetLineSpacing(f32 infSpacing)
        {
            LineSpacing = infSpacing;
            
            mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Line Spacing
		///
		/// @return The space left vertically between lines 
		//-------------------------------------------------------
		f32 CLabel::GetLineSpacing() const
		{
			return LineSpacing;
		}
        //-------------------------------------------------------
        /// Set Character Spacing
        ///
        /// Set the space left horizontally between letters 
        ///
        /// @param Absolute Scalar
        //-------------------------------------------------------
        void CLabel::SetCharacterSpacing(f32 infSpacing)
        {
            CharacterSpacing = infSpacing;
            
            mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Character Spacing
		///
		/// @return The space left horizontally between letters 
		//-------------------------------------------------------
		f32 CLabel::GetCharacterSpacing() const
		{
			return CharacterSpacing;
		}
        //-------------------------------------------------------
        /// Set Text Scale
        ///
        /// @param Absolute value by which the text is scaled
        //-------------------------------------------------------
        void CLabel::SetTextScale(f32 infScale)
        {
            TextScale = infScale;
            
            mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Text Scale
		///
		/// @return Absolute Scalar
		//-------------------------------------------------------
		f32 CLabel::GetTextScale() const
		{
			return TextScale;
		}
        //-------------------------------------------------------
        /// Set Text Colour
        ///
        /// @param Colour
        //-------------------------------------------------------
        void CLabel::SetTextColour(const Core::CColour& inColour)
        {
            TextColour = inColour;
        }
		//-------------------------------------------------------
		/// Get Text Colour
		///
		/// @return Colour
		//-------------------------------------------------------
		const Core::CColour& CLabel::GetTextColour() const
		{
			return TextColour;
		}
		//-------------------------------------------------------
		/// Set Horizontal Justification
		///
		/// @param Horizontal justification
		//-------------------------------------------------------
		void CLabel::SetHorizontalJustification(TextJustification ineHorizontalJustification)
		{
			HorizontalJustification = ineHorizontalJustification;

			mCachedChars.clear();
		}
        //-------------------------------------------------------
        /// Set Vertical Justification
        ///
        /// @param Vertical justification
        //-------------------------------------------------------
        void CLabel::SetVerticalJustification(TextJustification ineVerticalJustification)
        {
			VerticalJustification = ineVerticalJustification;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Get Horizontal Justification
        ///
        /// @return Horizontal justification
        //-------------------------------------------------------
        TextJustification CLabel::GetHorizontalJustification() const
        {
            return HorizontalJustification;
        }
        //-------------------------------------------------------
        /// Get Vertical Justification
        ///
        /// @return Vertical justification
        //-------------------------------------------------------
        TextJustification CLabel::GetVerticalJustification() const
        {
            return VerticalJustification;
        }
        //-------------------------------------------------------
        /// Enable Background
        ///
        /// @param Whether the label has a coloured background
        //-------------------------------------------------------
        void CLabel::EnableBackground(bool inbEnabled)
        {
            Background = inbEnabled;
        }
		//-------------------------------------------------------
		/// Is Background Enabled
		///
		/// @return Whether the label has a coloured background
		//-------------------------------------------------------
		bool CLabel::IsBackgroundEnabled() const
		{
			return Background;
		}
        //-------------------------------------------------------
        /// Justification From String
        ///
        /// Convert the string to a justification enum. This is
        /// used when creating labels from script files
        ///
        /// @param Text representation of justification
        /// @return Justification enum
        //-------------------------------------------------------
        TextJustification CLabel::JustificationFromString(const std::string& instrJustification)
        {
            if(instrJustification == "Left")
            {
                return TextJustification::k_left;
            }
            else if(instrJustification == "Right")
            {
                return TextJustification::k_right;
            }
            else if(instrJustification == "Centre")
            {
                return TextJustification::k_centre;
            }
			else if(instrJustification == "Top")
            {
                return TextJustification::k_top;
            }
            else if(instrJustification == "Bottom")
            {
                return TextJustification::k_bottom;
            }
            
            CS_FATAL_LOG("No justification matches type");
            
            return TextJustification::k_left;
        }
        //-------------------------------------------------------
        /// Draw
        ///
        /// Draw the text using the canvas then draw any
        /// subviews
        ///
        /// @param Canvas renderer
        //-------------------------------------------------------
        void CLabel::Draw(Rendering::CCanvasRenderer* inpCanvas)
        {
            if(Visible)
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
                
                f32 fAssetTextScale = GetGlobalTextScale();
                
                if(ScalableFont)
                {
					f32 fCharacterSpacingScaled = CharacterSpacing * kfScalableFontResDensity;
                    
                    if(TextOutlined)
                    {
                        inpCanvas->DrawDistanceOutlinedString(Text, GetTransform(), TextScale * fAssetTextScale, Font, mCachedChars, TextColour * GetInheritedOpacity(), TextOutlineColour * GetAbsoluteColour(),
                                          vAbsoluteLabelSize, fCharacterSpacingScaled, LineSpacing, HorizontalJustification, VerticalJustification, FlipVertical, TextOverflowBehaviour::k_clip, MaxNumLines);
                    }
                    else
                    {
                        inpCanvas->DrawDistanceString(Text, GetTransform(), TextScale * fAssetTextScale, Font, mCachedChars, TextColour * GetAbsoluteColour(),
                                                      vAbsoluteLabelSize, fCharacterSpacingScaled, LineSpacing, HorizontalJustification, VerticalJustification, FlipVertical, TextOverflowBehaviour::k_clip, MaxNumLines);
                    }
                }
                else
                {
                    Core::CColour sDrawColour = TextColour * GetAbsoluteColour();
#if DEBUG_STRING_CLIPPING
                    if(mbLastDrawWasClipped || mbLastDrawHadInvalidCharacter)
                    {
                        sDrawColour.r = (mbLastDrawWasClipped? 1.0 : 0.0);
                        sDrawColour.g = (mbLastDrawHadInvalidCharacter? 1.0 : 0.0);
                        sDrawColour.b = 0.0;
                        sDrawColour.a = 1.0;
                        TimeIntervalSecs uqwMillis = Core::CApplication::GetSystemTimeInMilliseconds() & 511;
                        f32 fFade = ((f32)uqwMillis) / 511.0f;
                        
                        if(fFade<0.5){
                            sDrawColour.r*=(fFade * 2.0);
                            sDrawColour.g*=(fFade * 2.0);
                            sDrawColour.b*=(fFade * 2.0);
                        }
                        else
                        {
                            sDrawColour.r=std::min(sDrawColour.r + ((fFade-0.5) * 2.0),  1.0);
                            sDrawColour.g=std::min(sDrawColour.g + ((fFade-0.5) * 2.0),  1.0);
                            sDrawColour.b=std::min(sDrawColour.b + ((fFade-0.5) * 2.0),  1.0);
                        }
                    }
#endif
                    inpCanvas->DrawString(Text, GetTransform(), TextScale * fAssetTextScale, Font, mCachedChars, sDrawColour,
                                          vAbsoluteLabelSize, CharacterSpacing, LineSpacing, HorizontalJustification, VerticalJustification, FlipVertical, TextOverflowBehaviour::k_clip, MaxNumLines,&mbLastDrawWasClipped,&mbLastDrawHadInvalidCharacter);
                    
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
        }
        
        //-------------------------------------------------------
        /// Do Autosizing
        ///
        /// based on the text contents
        //-------------------------------------------------------
        void CLabel::DoAutosizing(Rendering::CCanvasRenderer* inpCanvas)
        {
            if(Autosizing && mCachedChars.empty())
            {
                //Get the size of the text contents and resize the label
                //to match. Clamp the bounds between the minimum and
                //maximum size
                
                //Convert min and max to absolute co-ordinates
                Core::CVector2 vAbsMaxSize = mpParentView ? (mpParentView->GetAbsoluteSize() * UnifiedMaxSize.GetRelative()) + UnifiedMaxSize.GetAbsolute() : UnifiedMaxSize.GetAbsolute();
                Core::CVector2 vAbsMinSize = mpParentView ? (mpParentView->GetAbsoluteSize() * UnifiedMinSize.GetRelative()) + UnifiedMinSize.GetAbsolute() : UnifiedMinSize.GetAbsolute();
                
                f32 fNewRelWidth = 0.0f;
                f32 fNewRelHeight = 0.0f;
                f32 fNewAbsWidth = 0.0f;
                f32 fNewAbsHeight = 0.0f;
                f32 fTextHeight = 0.0f;
                
                //Calculate the width of the label based on the bounds and the length of the text
                f32 fTextWidth = inpCanvas->CalculateStringWidth(Text, Font, TextScale * mfGlobalTextScale, CharacterSpacing);
                
                if(fTextWidth > vAbsMaxSize.x)
                {
                    fNewRelWidth = UnifiedMaxSize.vRelative.x;
                    fNewAbsWidth = UnifiedMaxSize.vAbsolute.x;
                    
                    //Now that we have calculated the width of the label we
                    //can use that to work out the height
                    fTextHeight = inpCanvas->CalculateStringHeight(Text, Font, vAbsMaxSize.x, TextScale * mfGlobalTextScale, CharacterSpacing, LineSpacing, MaxNumLines);
                }
                else if(fTextWidth < vAbsMinSize.x)
                {
                    fNewRelWidth = UnifiedMinSize.vRelative.x;
                    fNewAbsWidth = UnifiedMinSize.vAbsolute.x;
                    
                    //Now that we have calculated the width of the label we
                    //can use that to work out the height
                    fTextHeight = inpCanvas->CalculateStringHeight(Text, Font, vAbsMinSize.x, TextScale * mfGlobalTextScale, CharacterSpacing, LineSpacing, MaxNumLines);
                }
                else
                {
                    fNewAbsWidth = fTextWidth;
                    
                    //Now that we have calculated the width of the label we
                    //can use that to work out the height
                    fTextHeight = inpCanvas->CalculateStringHeight(Text, Font, fTextWidth, TextScale * mfGlobalTextScale, CharacterSpacing, LineSpacing, MaxNumLines);
                }
                
                if(fTextHeight > vAbsMaxSize.y)
                {
                    fNewRelHeight = UnifiedMaxSize.vRelative.y;
                    fNewAbsHeight = UnifiedMaxSize.vAbsolute.y;
                }
                else if(fTextHeight < vAbsMinSize.y)
                {
                    fNewRelHeight = UnifiedMinSize.vRelative.y;
                    fNewAbsHeight = UnifiedMinSize.vAbsolute.y;
                }
                else
                {
                    fNewAbsHeight = fTextHeight;
                }
                
                //Resize the label
                SetSize(fNewRelWidth, fNewRelHeight, fNewAbsWidth, fNewAbsHeight);
            }
        }
		//-------------------------------------------------------
		/// Enable Scalable Font
		///
		/// @param Whether the label should use a scalable font
		/// or not
		//-------------------------------------------------------
		void CLabel::EnableScalableFont(bool inbEnabled)
		{
			ScalableFont = inbEnabled;
		}
		//-------------------------------------------------------
		/// Enable Scalable Font
		///
		/// @return Whether the label should use a scalable font
		/// or not
		//-------------------------------------------------------
		bool CLabel::IsScalableFontEnabled() const
		{
			return ScalableFont;
		}
		//-------------------------------------------------------
		/// Set Scalable Font Height
		///
		/// @param Sets the height of the scalable font
		//-------------------------------------------------------
		void CLabel::SetScalableFontHeight(f32 infHeight)
		{
			ScalableHeight = infHeight;
		}
		//-------------------------------------------------------
		/// Returns the Height of Scalable Font
		///
		/// @return Height of scalable font
		//-------------------------------------------------------
		f32 CLabel::GetScalableFontHeight() const
		{
			return ScalableHeight;
		}
        //-----------------------------------------------------------
		/// Set Flipped Vertical
		///
		/// Flip the label about it's local y-axis
		///
		/// @param Whether to flip or not
		//-----------------------------------------------------------
		void CLabel::EnableVerticalFlip(bool inbValue)
		{
			FlipVertical = inbValue;
		}
		//-----------------------------------------------------------
		/// Get Flipped Vertical
		///
		/// @param Whether label is flipped about it's local y-axis
		//-----------------------------------------------------------
		bool CLabel::IsVerticalFlipEnabled() const
		{
			return FlipVertical;
		}
        //-------------------------------------------------------
        /// Set Global Text Scale
        ///
        /// This text scale is applied to every label along
        /// with the per label text scale. This can be
        /// used to size text relative to screen size
        ///
        /// @param scale
        //-------------------------------------------------------
        void CLabel::SetGlobalTextScale(f32 infScale)
        {
            mfGlobalTextScale = infScale;
        }
        //-------------------------------------------------------
        /// Get Global Text Scale
        //-------------------------------------------------------
        f32 CLabel::GetGlobalTextScale()
        {
            if(ScalableFont)
            {
                f32 fAssetScale = Core::IFileSystem::GetDeviceResourcesDensity();
                return ScalableHeight == 0 ? fAssetScale : (ScalableHeight * fAssetScale) / Font->GetLineHeight();
            }
            return mfGlobalTextScale;
        }
        //-------------------------------------------------------
        /// Enable Text Outline
        ///
        /// @param Whether the scaleable text has an outline
        //-------------------------------------------------------
        void CLabel::EnableTextOutline(bool inbEnabled)
        {
            TextOutlined = inbEnabled;
        }
        //-------------------------------------------------------
        /// Is Text Outline Enabled
        ///
        /// @return Whether the scaleable text has an outline
        //-------------------------------------------------------
        bool CLabel::IsTextOutlineEnabled() const
        {
            return TextOutlined;
        }
        //-------------------------------------------------------
        /// Set Text Outline Colour
        ///
        /// @param Text outline colour for scaleable fonts
        //-------------------------------------------------------
        void CLabel::SetTextOutlineColour(const Core::CColour& inColour)
        {
            TextOutlineColour = inColour;
        }
        //-------------------------------------------------------
        /// Get Text Outline Colour
        ///
        /// @return Colour for outline of scaleable fonts
        //-------------------------------------------------------
        const Core::CColour& CLabel::GetTextOutlineColour() const
        {
            return TextOutlineColour;
        }
        //-----------------------------------------------------
        /// On Transform Changed
        ///
        /// Dirty the transform and force it to be recalculated
        //-----------------------------------------------------
        void CLabel::OnTransformChanged(u32 inudwInvalidFlags)
        {
            CGUIView::OnTransformChanged(inudwInvalidFlags);
            if ((inudwInvalidFlags & (u32)TransformCache::k_absSize) == (u32)TransformCache::k_absSize)
            {
                mCachedChars.clear();
            }
        }
    }
}
