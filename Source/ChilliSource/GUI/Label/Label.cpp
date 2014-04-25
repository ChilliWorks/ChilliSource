//
//  Label.cpp
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Label/Label.h>

#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(Label)

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
        
        f32 Label::mfGlobalTextScale = 1.0f;
        
        //-------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //-------------------------------------------------------
        Label::Label() : MaxNumLines(0), TextScale(1.0f), CharacterSpacing(0.0f), LineSpacing(1.0f), HorizontalJustification(TextJustification::k_left),
		VerticalJustification(TextJustification::k_centre), Background(true), Autosizing(false), FlipVertical(false), mbLastDrawWasClipped(false), mbLastDrawHadInvalidCharacter(false)
        {
            SetColour(Core::Colour(0.18f, 0.3f, 0.4f, 0.6f));

            mpWhiteTex = Core::Application::Get()->GetResourcePool()->GetResource<Rendering::Texture>("_GUIBackgroundTex");
            if(mpWhiteTex == nullptr)
            {
                const u32 k_numPixels = 4;
                const u32 k_numBytesPerPixel = 4;
                Rendering::Texture::Descriptor desc;
                desc.m_width = 2;
                desc.m_height = 2;
                desc.m_format = Core::ImageFormat::k_RGBA8888;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_dataSize = k_numPixels * k_numBytesPerPixel;
                u8* data = new u8[desc.m_dataSize];
                memset(data, 255, desc.m_dataSize);
                
                Rendering::TextureSPtr texture = Core::Application::Get()->GetResourcePool()->CreateResource<Rendering::Texture>("_GUIBackgroundTex");
                texture->Build(desc, Rendering::Texture::TextureDataUPtr(data));
                mpWhiteTex = texture;
            }
            
            //Grab the default font
            Font = Core::Application::Get()->GetDefaultFont();
            
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
        Label::Label(const Core::ParamDictionary& insParams) 
        : GUIView(insParams), MaxNumLines(0), TextScale(1.0f), CharacterSpacing(0.0f), LineSpacing(1.0f), HorizontalJustification(TextJustification::k_left),
		VerticalJustification(TextJustification::k_centre), Background(true), Autosizing(false), FlipVertical(false)
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
                Core::Vector4 vRawSize = Core::ParseVector4(strValue);
                SetMaximumSize(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w);
            }
            //---Unified maximum size
            if(insParams.TryGetValue("UnifiedMinSize", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::Vector4 vRawSize = Core::ParseVector4(strValue);
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
                Font = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Font>(eFontLocation, strValue);
            }
            //---Text Colour
            if(insParams.TryGetValue("TextColour", strValue))
            {
                TextColour = Core::ParseColour(strValue);
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
            //---UV Flipped Y
            if(insParams.TryGetValue("FlipVertical", strValue))
            {
                FlipVertical = true;
            }
            
            mpWhiteTex = Core::Application::Get()->GetResourcePool()->GetResource<Rendering::Texture>("_GUIBackgroundTex");
            if(mpWhiteTex == nullptr)
            {
                mpWhiteTex = Core::Application::Get()->GetResourcePool()->CreateResource<Rendering::Texture>("_GUIBackgroundTex");
            }
            
            if(!Font)
            {
                //Load default placeholder font
                Font = Core::Application::Get()->GetDefaultFont();
            }
        }
        //-------------------------------------------------------
        /// Set Text
        ///
        /// @param Text string
        //-------------------------------------------------------
        void Label::SetText(const Core::UTF8String& instrText)
        {
            Text = instrText;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Get Text
        ///
        /// @returnText string
        //-------------------------------------------------------
        const Core::UTF8String& Label::GetText() const
        {
            return Text;
        }
		//-------------------------------------------------------
		/// Set Text ID
		///
		/// @param Text string representing lookup ID
		//-------------------------------------------------------
		void Label::SetTextID(const std::string& instrText)
		{
			TextID = instrText;

			if(!TextID.empty())
			{
				SetText(Core::LocalisedText::GetText(instrText));
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
		const std::string& Label::GetTextID() const
		{
			return TextID;
		}
        //-------------------------------------------------------
        /// Set Font
        ///
        /// @param Font used to display text
        //-------------------------------------------------------
        void Label::SetFont(const Rendering::FontCSPtr& inpFont)
        {
            Font = inpFont;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Get Font
        ///
        /// @return Font used to display text
        //-------------------------------------------------------
        const Rendering::FontCSPtr& Label::GetFont() const
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
        void Label::SetMaximumSize(const Core::UnifiedVector2& invSize)
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
        void Label::SetMinimumSize(const Core::UnifiedVector2& invSize)
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
        void Label::SetMaximumSize(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
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
        void Label::SetMinimumSize(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
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
		const Core::UnifiedVector2& Label::GetMinimumSize() const
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
		const Core::UnifiedVector2& Label::GetMaximumSize() const
		{
			return UnifiedMaxSize;
		}
        //-------------------------------------------------------
        /// Enable Autosizing
        ///
        /// @param Whether the label should grow and shrink
        /// based on the text contents
        //-------------------------------------------------------
        void Label::EnableAutosizing(bool inbEnabled)
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
		bool Label::IsAutosizingEnabled() const
		{
			return Autosizing;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Label::OnScreenResolutionChanged()
		{
			mCachedChars.clear();

			GUIView::OnScreenResolutionChanged();
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
        void Label::SetNumberOfLines(u32 inudwNumLines)
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
        u32 Label::GetNumberOfLines() const
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
        void Label::SetLineSpacing(f32 infSpacing)
        {
            LineSpacing = infSpacing;
            
            mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Line Spacing
		///
		/// @return The space left vertically between lines 
		//-------------------------------------------------------
		f32 Label::GetLineSpacing() const
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
        void Label::SetCharacterSpacing(f32 infSpacing)
        {
            CharacterSpacing = infSpacing;
            
            mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Character Spacing
		///
		/// @return The space left horizontally between letters 
		//-------------------------------------------------------
		f32 Label::GetCharacterSpacing() const
		{
			return CharacterSpacing;
		}
        //-------------------------------------------------------
        /// Set Text Scale
        ///
        /// @param Absolute value by which the text is scaled
        //-------------------------------------------------------
        void Label::SetTextScale(f32 infScale)
        {
            TextScale = infScale;
            
            mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Text Scale
		///
		/// @return Absolute Scalar
		//-------------------------------------------------------
		f32 Label::GetTextScale() const
		{
			return TextScale;
		}
        //-------------------------------------------------------
        /// Set Text Colour
        ///
        /// @param Colour
        //-------------------------------------------------------
        void Label::SetTextColour(const Core::Colour& inColour)
        {
            TextColour = inColour;
        }
		//-------------------------------------------------------
		/// Get Text Colour
		///
		/// @return Colour
		//-------------------------------------------------------
		const Core::Colour& Label::GetTextColour() const
		{
			return TextColour;
		}
		//-------------------------------------------------------
		/// Set Horizontal Justification
		///
		/// @param Horizontal justification
		//-------------------------------------------------------
		void Label::SetHorizontalJustification(TextJustification ineHorizontalJustification)
		{
			HorizontalJustification = ineHorizontalJustification;

			mCachedChars.clear();
		}
        //-------------------------------------------------------
        /// Set Vertical Justification
        ///
        /// @param Vertical justification
        //-------------------------------------------------------
        void Label::SetVerticalJustification(TextJustification ineVerticalJustification)
        {
			VerticalJustification = ineVerticalJustification;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Get Horizontal Justification
        ///
        /// @return Horizontal justification
        //-------------------------------------------------------
        TextJustification Label::GetHorizontalJustification() const
        {
            return HorizontalJustification;
        }
        //-------------------------------------------------------
        /// Get Vertical Justification
        ///
        /// @return Vertical justification
        //-------------------------------------------------------
        TextJustification Label::GetVerticalJustification() const
        {
            return VerticalJustification;
        }
        //-------------------------------------------------------
        /// Enable Background
        ///
        /// @param Whether the label has a coloured background
        //-------------------------------------------------------
        void Label::EnableBackground(bool inbEnabled)
        {
            Background = inbEnabled;
        }
		//-------------------------------------------------------
		/// Is Background Enabled
		///
		/// @return Whether the label has a coloured background
		//-------------------------------------------------------
		bool Label::IsBackgroundEnabled() const
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
        TextJustification Label::JustificationFromString(const std::string& instrJustification)
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
            
            CS_LOG_FATAL("No justification matches type");
            
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
        void Label::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            if(Visible)
            {
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
                    inpCanvas->EnableClippingToBounds(vBottomLeft, vAbsoluteLabelSize);
                }
                
                //Draw ourself
                if(Background)
                {
                    inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), mpWhiteTex, Core::Rectangle(Core::Vector2::ZERO, Core::Vector2::ZERO), AbsCol);
                }
                
                f32 fAssetTextScale = GetGlobalTextScale();
                
                    Core::Colour sDrawColour = TextColour * GetAbsoluteColour();
                    inpCanvas->DrawString(Text, GetTransform(), TextScale * fAssetTextScale, Font, mCachedChars, sDrawColour,
                                          vAbsoluteLabelSize, CharacterSpacing, LineSpacing, HorizontalJustification, VerticalJustification, FlipVertical, TextOverflowBehaviour::k_clip, MaxNumLines,&mbLastDrawWasClipped,&mbLastDrawHadInvalidCharacter);
            
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
        }
        
        //-------------------------------------------------------
        /// Do Autosizing
        ///
        /// based on the text contents
        //-------------------------------------------------------
        void Label::DoAutosizing(Rendering::CanvasRenderer* inpCanvas)
        {
            if(Autosizing && mCachedChars.empty())
            {
                //Get the size of the text contents and resize the label
                //to match. Clamp the bounds between the minimum and
                //maximum size
                
                //Convert min and max to absolute co-ordinates
                Core::Vector2 vAbsMaxSize = mpParentView ? (mpParentView->GetAbsoluteSize() * UnifiedMaxSize.GetRelative()) + UnifiedMaxSize.GetAbsolute() : UnifiedMaxSize.GetAbsolute();
                Core::Vector2 vAbsMinSize = mpParentView ? (mpParentView->GetAbsoluteSize() * UnifiedMinSize.GetRelative()) + UnifiedMinSize.GetAbsolute() : UnifiedMinSize.GetAbsolute();
                
                f32 fNewRelWidth = 0.0f;
                f32 fNewRelHeight = 0.0f;
                f32 fNewAbsWidth = 0.0f;
                f32 fNewAbsHeight = 0.0f;
                f32 fTextHeight = 0.0f;
                
                //Calculate the width of the label based on the bounds and the length of the text
                f32 fTextWidth = inpCanvas->CalculateStringWidth(Text, Font, TextScale * mfGlobalTextScale, CharacterSpacing, false);
                
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
        //-----------------------------------------------------------
		/// Set Flipped Vertical
		///
		/// Flip the label about it's local y-axis
		///
		/// @param Whether to flip or not
		//-----------------------------------------------------------
		void Label::EnableVerticalFlip(bool inbValue)
		{
			FlipVertical = inbValue;
		}
		//-----------------------------------------------------------
		/// Get Flipped Vertical
		///
		/// @param Whether label is flipped about it's local y-axis
		//-----------------------------------------------------------
		bool Label::IsVerticalFlipEnabled() const
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
        void Label::SetGlobalTextScale(f32 infScale)
        {
            mfGlobalTextScale = infScale;
        }
        //-------------------------------------------------------
        /// Get Global Text Scale
        //-------------------------------------------------------
        f32 Label::GetGlobalTextScale()
        {
            return mfGlobalTextScale;
        }
        //-------------------------------------------------------
        /// Set Text Outline Colour
        ///
        /// @param Text outline colour for scaleable fonts
        //-------------------------------------------------------
        void Label::SetTextOutlineColour(const Core::Colour& inColour)
        {
            TextOutlineColour = inColour;
        }
        //-------------------------------------------------------
        /// Get Text Outline Colour
        ///
        /// @return Colour for outline of scaleable fonts
        //-------------------------------------------------------
        const Core::Colour& Label::GetTextOutlineColour() const
        {
            return TextOutlineColour;
        }
        //-----------------------------------------------------
        /// On Transform Changed
        ///
        /// Dirty the transform and force it to be recalculated
        //-----------------------------------------------------
        void Label::OnTransformChanged(u32 inudwInvalidFlags)
        {
            GUIView::OnTransformChanged(inudwInvalidFlags);
            if ((inudwInvalidFlags & (u32)TransformCache::k_absSize) == (u32)TransformCache::k_absSize)
            {
                mCachedChars.clear();
            }
        }
    }
}
