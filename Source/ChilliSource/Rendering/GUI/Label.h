//
//  Label.h
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_LABEL_H_
#define _MOFLO_GUI_LABEL_H_

#include <ChilliSource/Rendering/GUI/ForwardDeclarations.h>
#include <ChilliSource/Rendering/GUI/GUIView.h>

#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/ForwardDeclarations.h>

#define DEBUG_STRING_CLIPPING 0

namespace moFlo
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
		
        class CLabel : public CGUIView
        {
        public:
            
			DECLARE_META_CLASS(CLabel)

            CLabel();
            CLabel(const Core::ParamDictionary& insParams);
            //-------------------------------------------------------
            /// Set Text
            ///
            /// @param Text string
            //-------------------------------------------------------
            virtual void SetText(const UTF8String& instrText);
			//-------------------------------------------------------
			/// Get Text
			///
			/// @returnText string
			//-------------------------------------------------------
			const UTF8String& GetText() const;
			//-------------------------------------------------------
			/// Set Text ID
			///
			/// @param Text string representing lookup ID
			//-------------------------------------------------------
			void SetTextID(const std::string& instrText);
			//-------------------------------------------------------
			/// Get Text ID
			///
			/// @return Text string representing lookup ID
			//-------------------------------------------------------
			const std::string& GetTextID() const;
            //-------------------------------------------------------
            /// Set Font
            ///
            /// @param Font used to display text
            //-------------------------------------------------------
            void SetFont(const Rendering::FontPtr& inpFont);
            //-------------------------------------------------------
            /// Get Font
            ///
            /// @return Font used to display text
            //-------------------------------------------------------
            const Rendering::FontPtr& GetFont() const;
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
            virtual void SetTextColour(const Core::CColour& inColour);
			//-------------------------------------------------------
			/// Get Text Colour
			///
			/// @return Colour
			//-------------------------------------------------------
			const Core::CColour& GetTextColour() const;
            //-------------------------------------------------------
            /// Set Text Outline Colour
            ///
            /// @param Text outline colour for scaleable fonts
            //-------------------------------------------------------
            void SetTextOutlineColour(const Core::CColour& inColour);
			//-------------------------------------------------------
			/// Get Text Outline Colour
			///
			/// @return Colour for outline of scaleable fonts
			//-------------------------------------------------------
			const Core::CColour& GetTextOutlineColour() const;
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
			/// On Screen Orientation Changed
			///
			/// Triggered if the screen orientation changes so we can
			/// resize ourself
			//-----------------------------------------------------------
			void OnScreenOrientationChanged();
            //-------------------------------------------------------
            /// Draw
            ///
            /// Draw the text using the canvas then draw any
            /// subviews
            ///
            /// @param Canvas renderer
            //-------------------------------------------------------
            virtual void Draw(Rendering::CCanvasRenderer* inpCanvas);
            //-------------------------------------------------------
            /// Set Maximum Size
            ///
            /// The unified size to which this label will expand 
            /// after which the text will clip
            ///
            /// @param Unified vector
            //-------------------------------------------------------
            void SetMaximumSize(const UnifiedVector2& invSize);
            //-------------------------------------------------------
            /// Set Minimum Size
            ///
            /// The unified size to which this label will shrink 
            ///
            /// @param Unified vector
            //-------------------------------------------------------
            void SetMinimumSize(const UnifiedVector2& invSize);
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
			const UnifiedVector2& GetMinimumSize() const;
			//-------------------------------------------------------
			/// Get Maximum Size
			///
			/// The unified size to which this label will grow 
			///
			/// @return Unified vector
			//-------------------------------------------------------
			const UnifiedVector2& GetMaximumSize() const;
            //-------------------------------------------------------
            /// Enable Text Outline
            ///
            /// @param Whether the scaleable text has an outline
            //-------------------------------------------------------
            void EnableTextOutline(bool inbEnabled);
			//-------------------------------------------------------
			/// Is Text Outline Enabled
			///
			/// @return Whether the scaleable text has an outline
			//-------------------------------------------------------
			bool IsTextOutlineEnabled() const;
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
			void DoAutosizing(Rendering::CCanvasRenderer* inpCanvas);
            //-------------------------------------------------------
            /// Enable Scalable Font
            ///
            /// @param Whether the label should use a scalable font
			/// or not
            //-------------------------------------------------------
            void EnableScalableFont(bool inbEnabled);
			//-------------------------------------------------------
			/// Enable Scalable Font
			///
			/// @return Whether the label should use a scalable font
			/// or not
			//-------------------------------------------------------
			bool IsScalableFontEnabled() const;
			//-------------------------------------------------------
            /// Set Scalable Font Height
            ///
            /// @param Sets the height of the scalable font
            //-------------------------------------------------------
            void SetScalableFontHeight(f32 infHeight);
			//-------------------------------------------------------
			/// Returns the Height of Scalable Font
			///
			/// @return Height of scalable font
			//-------------------------------------------------------
			f32 GetScalableFontHeight() const;
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
            
            Rendering::CharacterList mCachedChars;
            
            Rendering::TexturePtr mpWhiteTex;
            
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
			DECLARE_PROPERTY_A(UTF8String, Text, SetText, GetText);
			DECLARE_PROPERTY_A(std::string, TextID, SetTextID, GetTextID);
			DECLARE_PROPERTY_A(u32, MaxNumLines, SetNumberOfLines, GetNumberOfLines);
			DECLARE_PROPERTY_A(f32, TextScale, SetTextScale, GetTextScale);
			DECLARE_PROPERTY_A(f32, CharacterSpacing, SetCharacterSpacing, GetCharacterSpacing);
			DECLARE_PROPERTY_A(f32, LineSpacing, SetLineSpacing, GetLineSpacing);
			DECLARE_PROPERTY_A(TextJustification, HorizontalJustification, SetHorizontalJustification, GetHorizontalJustification);
			DECLARE_PROPERTY_A(TextJustification, VerticalJustification, SetVerticalJustification, GetVerticalJustification);
			DECLARE_PROPERTY_A(Rendering::FontPtr, Font, SetFont, GetFont);
            DECLARE_PROPERTY_A(bool, ScalableFont, SetScalableFont, GetScalableFont)
            DECLARE_PROPERTY_A(f32, ScalableHeight, SetScalableHeight, GetScalableHeight);
			DECLARE_PROPERTY_A(Core::CColour, TextColour, SetTextColour, GetTextColour);
            DECLARE_PROPERTY_A(Core::CColour, TextOutlineColour, SetTextOutlineColour, GetTextOutlineColour);
			DECLARE_PROPERTY_A(bool, Background, EnableBackground, IsBackgroundEnabled);
			DECLARE_PROPERTY_A(bool, Autosizing, EnableAutosizing, IsAutosizingEnabled);
            DECLARE_PROPERTY_A(bool, TextOutlined, EnableTextOutline, IsTextOutlineEnabled);
			DECLARE_PROPERTY_A(UnifiedVector2, UnifiedMaxSize, SetMaximumSize, GetMaximumSize);
			DECLARE_PROPERTY_A(UnifiedVector2, UnifiedMinSize, SetMinimumSize, GetMinimumSize);
            DECLARE_PROPERTY_A(bool, FlipVertical, EnableVerticalFlip, IsVerticalFlipEnabled);
        };
    }
}

#endif