//
//  Label.h
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_LABEL_H_
#define _MOFLO_GUI_LABEL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

namespace ChilliSource
{
    namespace GUI
    {
		enum class TextJustification
        {
            k_left,
            k_right,
            k_centre,
            k_top,
            k_bottom
        };
		
		enum class TextOverflowBehaviour
		{
            k_none,
            k_clip,
            k_follow
		};
		
        class Label : public GUIView
        {
        public:
            
			DECLARE_META_CLASS(Label)

            Label();
            Label(const Core::ParamDictionary& insParams);
            //-------------------------------------------------------
            /// Set Text
            ///
            /// @param Text string
            //-------------------------------------------------------
            virtual void SetText(const Core::UTF8String& instrText);
			//-------------------------------------------------------
			/// Get Text
			///
			/// @returnText string
			//-------------------------------------------------------
			const Core::UTF8String& GetText() const;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Localised text resource used in conjunction with
            /// id
            //-------------------------------------------------------
            void SetLocalisedText(const Core::LocalisedTextCSPtr& in_text);
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Localised text resource used in conjunction with
            /// id
            //-------------------------------------------------------
            const Core::LocalisedTextCSPtr& GetLocalisedText() const;
			//-------------------------------------------------------
			/// Set Text ID
			///
			/// @param Text string representing lookup ID
			//-------------------------------------------------------
			void SetLocalisedTextID(const std::string& instrText);
			//-------------------------------------------------------
			/// Get Text ID
			///
			/// @return Text string representing lookup ID
			//-------------------------------------------------------
			const std::string& GetLocalisedTextID() const;
            //-------------------------------------------------------
            /// Set Font
            ///
            /// @param Font used to display text
            //-------------------------------------------------------
            void SetFont(const Rendering::FontCSPtr& inpFont);
            //-------------------------------------------------------
            /// Get Font
            ///
            /// @return Font used to display text
            //-------------------------------------------------------
            const Rendering::FontCSPtr& GetFont() const;
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
            void SetNumberOfLines(u32 inudwNumLines);
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
            u32 GetNumberOfLines() const;
            //-------------------------------------------------------
            /// Set Line Spacing
            ///
            /// Set the space left vertically between lines 
            ///
            /// @param Absolute Scalar
            //-------------------------------------------------------
            void SetLineSpacing(f32 infSpacing);
			//-------------------------------------------------------
			/// Get Line Spacing
			///
			/// @return The space left vertically between lines 
			//-------------------------------------------------------
			f32 GetLineSpacing() const;
            //-------------------------------------------------------
            /// Set Character Spacing
            ///
            /// Set the space left horizontally between letters 
            ///
            /// @param Absolute Scalar
            //-------------------------------------------------------
            void SetCharacterSpacing(f32 infSpacing);
			//-------------------------------------------------------
			/// Get Character Spacing
			///
			/// @return The space left horizontally between letters 
			//-------------------------------------------------------
			f32 GetCharacterSpacing() const;
            //-------------------------------------------------------
            /// Set Text Scale
            ///
            /// @param Absolute Scalar
            //-------------------------------------------------------
            virtual void SetTextScale(f32 infScale);
			//-------------------------------------------------------
			/// Get Text Scale
			///
			/// @return Absolute Scalar
			//-------------------------------------------------------
			f32 GetTextScale() const;
            //-------------------------------------------------------
            /// Set Horizontal Justification
            ///
            /// @param Horizontal justification
            //-------------------------------------------------------
            void SetHorizontalJustification(TextJustification ineHorizontalJustification);
            //-------------------------------------------------------
            /// Set Vertical Justification
            ///
			/// @param Vertical justification
            //-------------------------------------------------------
            void SetVerticalJustification(TextJustification ineVerticalJustification);
            //-------------------------------------------------------
            /// Get Horizontal Justification
            ///
            /// @return Horizontal justification
            //-------------------------------------------------------
            TextJustification GetHorizontalJustification() const;
            //-------------------------------------------------------
            /// Get Vertical Justification
            ///
			/// @return Vertical justification
            //-------------------------------------------------------
            TextJustification GetVerticalJustification() const;
            //-------------------------------------------------------
            /// Set Text Colour
            ///
            /// @param Colour
            //-------------------------------------------------------
            virtual void SetTextColour(const Core::Colour& inColour);
			//-------------------------------------------------------
			/// Get Text Colour
			///
			/// @return Colour
			//-------------------------------------------------------
			const Core::Colour& GetTextColour() const;
            //-------------------------------------------------------
            /// Set Text Outline Colour
            ///
            /// @param Text outline colour for scaleable fonts
            //-------------------------------------------------------
            void SetTextOutlineColour(const Core::Colour& inColour);
			//-------------------------------------------------------
			/// Get Text Outline Colour
			///
			/// @return Colour for outline of scaleable fonts
			//-------------------------------------------------------
			const Core::Colour& GetTextOutlineColour() const;
            //-------------------------------------------------------
            /// Enable Background
            ///
            /// @param Whether the label has a coloured background
            //-------------------------------------------------------
            void EnableBackground(bool inbEnabled);
			//-------------------------------------------------------
			/// Is Background Enabled
			///
			/// @return Whether the label has a coloured background
			//-------------------------------------------------------
			bool IsBackgroundEnabled() const;
			//-----------------------------------------------------------
			/// On Screen Resolution Changed
			///
			/// Triggered if the screen resolution changes so we can
			/// resize ourself
			//-----------------------------------------------------------
			void OnScreenResolutionChanged();
            //-------------------------------------------------------
            /// Draw
            ///
            /// Draw the text using the canvas then draw any
            /// subviews
            ///
            /// @param Canvas renderer
            //-------------------------------------------------------
            virtual void Draw(Rendering::CanvasRenderer* inpCanvas);
            //-------------------------------------------------------
            /// Set Maximum Size
            ///
            /// The unified size to which this label will expand 
            /// after which the text will clip
            ///
            /// @param Unified vector
            //-------------------------------------------------------
            void SetMaximumSize(const Core::UnifiedVector2& invSize);
            //-------------------------------------------------------
            /// Set Minimum Size
            ///
            /// The unified size to which this label will shrink 
            ///
            /// @param Unified vector
            //-------------------------------------------------------
            void SetMinimumSize(const Core::UnifiedVector2& invSize);
            //-------------------------------------------------------
            /// Set Maximum Size
            ///
            /// The unified size to which this label will expand 
            /// after which the text will clip
            ///
            /// @param Unified vector RX, RY, AX & AY
            //-------------------------------------------------------
            void SetMaximumSize(f32 infRx, f32 infRy, f32 infAx, f32 infAy);
            //-------------------------------------------------------
            /// Set Minimum Size
            ///
            /// The unified size to which this label will shrink 
            ///
            /// @param Unified vector RX, RY, AX & AY
            //-------------------------------------------------------
            void SetMinimumSize(f32 infRx, f32 infRy, f32 infAx, f32 infAy);
			//-------------------------------------------------------
			/// Get Minimum Size
			///
			/// The unified size to which this label will shrink 
			///
			/// @return Unified vector
			//-------------------------------------------------------
			const Core::UnifiedVector2& GetMinimumSize() const;
			//-------------------------------------------------------
			/// Get Maximum Size
			///
			/// The unified size to which this label will grow 
			///
			/// @return Unified vector
			//-------------------------------------------------------
			const Core::UnifiedVector2& GetMaximumSize() const;
            //-------------------------------------------------------
            /// Enable Autosizing
            ///
            /// @param Whether the label should grow and shrink
            /// based on the text contents
            //-------------------------------------------------------
            void EnableAutosizing(bool inbEnabled);
			//-------------------------------------------------------
			/// Is Autosizing Enabled
			///
			/// @return Whether the label should grow and shrink
			/// based on the text contents
			//-------------------------------------------------------
			bool IsAutosizingEnabled() const;
			//-------------------------------------------------------
			/// Do Autosizing
			///
			/// based on the text contents
			//-------------------------------------------------------
			void DoAutosizing(Rendering::CanvasRenderer* inpCanvas);
            //-----------------------------------------------------------
			/// Set Flipped Vertical
			///
			/// Flip the label about it's local y-axis
			///
			/// @param Whether to flip or not
			//-----------------------------------------------------------
			void EnableVerticalFlip(bool inbValue);
			//-----------------------------------------------------------
			/// Get Flipped Vertical
			///
			/// @param Whether label is flipped about it's local y-axis
			//-----------------------------------------------------------
			bool IsVerticalFlipEnabled() const;
			//-------------------------------------------------------
            /// Justification From String
            ///
            /// Convert the string to a justification enum. This is
            /// used when creating labels from script files
            ///
            /// @param Text representation of justifcation
            /// @return Justification enum
            //-------------------------------------------------------
            static TextJustification JustificationFromString(const std::string& instrJustification);
			//-------------------------------------------------------
            /// Set Global Text Scale
            ///
            /// This text scale is applied to every label along
            /// with the per label text scale. This can be
            /// used to size text relative to screen size
            ///
            /// @param scale
            //-------------------------------------------------------
            static void SetGlobalTextScale(f32 infScale);
            //-------------------------------------------------------
            /// Get Global Text Scale
            ///
            /// @return The global text scale for this label (calculated if the
            /// font is scalable
            //-------------------------------------------------------
            f32 GetGlobalTextScale();
            
        protected:
            
            Rendering::CanvasRenderer::CharacterList mCachedChars;
            
            Rendering::TextureCSPtr mpWhiteTex;
            
            Core::LocalisedTextCSPtr m_localisedText;
            
            static f32 mfGlobalTextScale;
            
            // label appearance debugging
            bool mbLastDrawWasClipped;
            bool mbLastDrawHadInvalidCharacter;
            
            //-----------------------------------------------------
			/// On Transform Changed
			///
			/// Dirty the transform and force it to be recalculated
			//-----------------------------------------------------
			void OnTransformChanged(u32 inudwInvalidFlags);
            
			//---Properties
			DECLARE_PROPERTY_A(Core::UTF8String, Text, SetText, GetText);
			DECLARE_PROPERTY_A(std::string, LocalisedTextID, SetLocalisedTextID, GetLocalisedTextID);
			DECLARE_PROPERTY_A(u32, MaxNumLines, SetNumberOfLines, GetNumberOfLines);
			DECLARE_PROPERTY_A(f32, TextScale, SetTextScale, GetTextScale);
			DECLARE_PROPERTY_A(f32, CharacterSpacing, SetCharacterSpacing, GetCharacterSpacing);
			DECLARE_PROPERTY_A(f32, LineSpacing, SetLineSpacing, GetLineSpacing);
			DECLARE_PROPERTY_A(TextJustification, HorizontalJustification, SetHorizontalJustification, GetHorizontalJustification);
			DECLARE_PROPERTY_A(TextJustification, VerticalJustification, SetVerticalJustification, GetVerticalJustification);
			DECLARE_PROPERTY_A(Rendering::FontCSPtr, Font, SetFont, GetFont);
			DECLARE_PROPERTY_A(Core::Colour, TextColour, SetTextColour, GetTextColour);
            DECLARE_PROPERTY_A(Core::Colour, TextOutlineColour, SetTextOutlineColour, GetTextOutlineColour);
			DECLARE_PROPERTY_A(bool, Background, EnableBackground, IsBackgroundEnabled);
			DECLARE_PROPERTY_A(bool, Autosizing, EnableAutosizing, IsAutosizingEnabled);
			DECLARE_PROPERTY_A(Core::UnifiedVector2, UnifiedMaxSize, SetMaximumSize, GetMaximumSize);
			DECLARE_PROPERTY_A(Core::UnifiedVector2, UnifiedMinSize, SetMinimumSize, GetMinimumSize);
            DECLARE_PROPERTY_A(bool, FlipVertical, EnableVerticalFlip, IsVerticalFlipEnabled);
        };
    }
}

#endif