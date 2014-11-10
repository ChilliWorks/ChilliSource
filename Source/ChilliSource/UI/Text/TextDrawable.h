//
//  TextDrawable.h
//  Chilli Source
//  Created by Ian Copland on 05/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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


#ifndef _CHILLISOURCE_UI_TEXT_TEXTDRAWABLE_H_
#define _CHILLISOURCE_UI_TEXT_TEXTDRAWABLE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Base/HorizontalTextJustification.h>
#include <ChilliSource/Rendering/Base/VerticalTextJustification.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/UI/Base/PropertyMap.h>

#include <vector>

namespace ChilliSource
{
    namespace UI
    {
        //--------------------------------------------------------------------------
        /// A component which can be added to a widget to provide text rendering
        /// functionality. Text contains the following properties:
        ///
        /// "FontLocation": The storage location of the font. This can be the
        /// any of the standard storage location values and defaults to "Package".
        ///
        /// "FontPath": The file path to the font in the given storage location.
        ///
        /// "LocalisedTextLocation": The storage location of the localised
        /// text file. This can be any of the standard storage location values and
        /// defaults to "Pacakge".
        ///
        /// "LocalisedTextPath": The file path ot the storage location in the
        /// given storage location.
        ///
        /// "LocalisedTextId": The Id of the string in the given localised text
        /// file.
        ///
        /// "Text": The UTF-8 format string that that should be displayed. Using
        /// this is not recommended, it is usually better to create text using
        /// a localised text resource.
        ///
        /// "TextColour": The colour of the text. Defaults to white.
        ///
        /// "HorizonalJustification": The justification of the text horizontally.
        /// This can be set to "Left", "Centre" or "Right". Defaults to "Centre".
        ///
        /// "VerticalJustification": The justification of the text vertically. This
        /// can be set to "Bottom", "Centre" or "Top". Defaults to "Centre".
        ///
        /// "AbsCharSpacingOffset": An offset from the standard character spacing in
        /// pixels. This allows characters to be moved appart or closer together.
        /// Defaults to 0.
        ///
        /// "AbsLineSpacingOffset": An offset from the standard line spacing. This
        /// allows lines of text to be moved closer together or further apart.
        /// Defaults to 0.
        ///
        /// "LineSpacingScale": A scale factor to alter the line spacing. This
        /// allows lines of text to be moved closer to together or further apart.
        /// Defaults to 1.0
        ///
        /// "MaxNumberOfLines": The maximum number of lines the text will be displayed
        /// across. If 0, there is no limit. Defaults to 0.
        ///
        /// "TextScale": The scale factor that should be used to render the text.
        /// Scaling text significantly reduces the text quality so this should only
        /// be used when absolutely necessary. Defaults to 1.0.
        ///
        /// @author Ian Copland
        //--------------------------------------------------------------------------
        class TextDrawable final
        {
        public:
            CS_DECLARE_NOCOPY(TextDrawable);
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of properties supported by text.
            //-------------------------------------------------------------------
            static const std::vector<PropertyMap::PropertyDesc>& GetPropertyDescs();
            //-------------------------------------------------------------------
            /// Constructor. Creates the text using a string retreived from a
            /// localised text resource.
            ///
            /// @author Ian Copland
            ///
            /// @param The font.
            /// @param The localised text resource.
            /// @param The localised text Id.
            //-------------------------------------------------------------------
            TextDrawable(const Rendering::FontCSPtr& in_font, const Core::LocalisedTextCSPtr& in_localisedText, const std::string& in_localisedTextId);
            //-------------------------------------------------------------------
            /// Constructor. Creates the text using the given string. This is
            /// not recommended, it is usually better to create text using a
            /// localised text resource.
            ///
            /// @author Ian Copland
            ///
            /// @param The font.
            /// @param The UTF-8 format string.
            //-------------------------------------------------------------------
            TextDrawable(const Rendering::FontCSPtr& in_font, const std::string& in_string);
            //-------------------------------------------------------------------
            /// Constructor that builds the drawable from key-value properties.
            /// The properties used to create the text are described in the
            /// class documentation.
            ///
            /// @author Ian Copland
            ///
            /// @param The property map.
            //-------------------------------------------------------------------
            TextDrawable(const PropertyMap& in_properties);
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The font that will be used to render the text.
            //-------------------------------------------------------------------
            const Rendering::FontCSPtr& GetFont() const;
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The UTF-8 format string that will be rendered.
            //-------------------------------------------------------------------
            const std::string& GetText() const;
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The colour of the text.
            //-------------------------------------------------------------------
            const Core::Colour& GetTextColour() const;
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The horizontal justification of the text within the owning
            /// widget's bounds.
            //-------------------------------------------------------------------
            Rendering::HorizontalTextJustification GetHorizontalJustification() const;
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The vertical justification of the text within the owning
            /// widget's bounds.
            //-------------------------------------------------------------------
            Rendering::VerticalTextJustification GetVerticalJustification() const;
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute pixel offset to the standard character spacing.
            /// This is used to increase of reduce the space between characters in
            /// the rendered text.
            //-------------------------------------------------------------------
            f32 GetAbsoluteCharacterSpacingOffset() const;
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute pixel offset to the standard line spacing
            /// spacing. This is used to increase of reduce the space between
            /// lines in the rendered text.
            //-------------------------------------------------------------------
            f32 GetAbsoluteLineSpacingOffset() const;
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A scale factor which is applied to the line spacing. This
            /// is used to increase of reduce the space between lines in the
            /// rendered text.
            //-------------------------------------------------------------------
            f32 GetLineSpacingScale() const;
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The maximum number of lines of text that will be rendered.
            /// A value of 0 means there is no limit to the number of lines.
            //-------------------------------------------------------------------
            u32 GetMaxNumberOfLines() const;
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The text scale. Scaling text significantly reduces the
            /// quality of the text rendered so this should only be used when
            /// absolutely necessary.
            //-------------------------------------------------------------------
            f32 GetTextScale() const;
            //-------------------------------------------------------------------
            /// Sets the font that will be used to render the text.
            ///
            /// @author Ian Copland
            ///
            /// @param The font.
            //-------------------------------------------------------------------
            void SetFont(const Rendering::FontCSPtr& in_font);
            //-------------------------------------------------------------------
            /// Sets the rendered text from a localised text resource.
            ///
            /// @author Ian Copland
            ///
            /// @param The localised text resource.
            /// @param The localised text id which is used to lookup the string
            /// in the localised text resource.
            //-------------------------------------------------------------------
            void SetText(const Core::LocalisedTextCSPtr& in_localisedText, const std::string& in_localisedTextId);
            //-------------------------------------------------------------------
            /// Directly sets the text that will be rendered. This is not
            /// recommended, usually is it better to set the text using a
            /// localised text resource.
            ///
            /// @author Ian Copland
            ///
            /// @param The text.
            //-------------------------------------------------------------------
            void SetText(const std::string& in_text);
            //-------------------------------------------------------------------
            /// Sets the colour of the rendered text.
            ///
            /// @author Ian Copland
            ///
            /// @param The colour.
            //-------------------------------------------------------------------
            void SetTextColour(const Core::Colour& in_textColour);
            //-------------------------------------------------------------------
            /// Sets the horizontal justification of the text within the owning
            /// widget's bounds.
            ///
            /// @author Ian Copland
            ///
            /// @param The justfication.
            //-------------------------------------------------------------------
            void SetHorizontalJustification(Rendering::HorizontalTextJustification in_horizontalJustification);
            //-------------------------------------------------------------------
            /// Sets the vertical justification of the text within the owning
            /// widget's bounds.
            ///
            /// @author Ian Copland
            ///
            /// @param The justification.
            //-------------------------------------------------------------------
            void SetVerticalJustification(Rendering::VerticalTextJustification in_verticalJustification);
            //-------------------------------------------------------------------
            /// Sets the absolute pixel offset to the standard character spacing.
            /// This is used to increase of reduce the space between characters in
            /// the rendered text.
            ///
            /// @author Ian Copland
            ///
            /// @param The offset.
            //-------------------------------------------------------------------
            void SetAbsoluteCharacterSpacingOffset(f32 in_offset);
            //-------------------------------------------------------------------
            /// Sets the absolute pixel offset to the standard line spacing
            /// spacing. This is used to increase of reduce the space between
            /// lines in the rendered text.
            ///
            /// @author Ian Copland
            ///
            /// @param The offset.
            //-------------------------------------------------------------------
            void SetAbsoluteLineSpacingOffset(f32 in_offset);
            //-------------------------------------------------------------------
            /// Sets the scale factor which is applied to the line spacing. This
            /// is used to increase of reduce the space between lines in the
            /// rendered text.
            ///
            /// @author Ian Copland
            ///
            /// @param The scale factor.
            //-------------------------------------------------------------------
            void SetLineSpacingScale(f32 in_scale);
            //-------------------------------------------------------------------
            /// Sets the maximum number of lines of text that will be rendered.
            /// A value of 0 means there is no limit to the number of lines.
            ///
            /// @author Ian Copland
            ///
            /// @param The number of lines, or 0 if no limit.
            //-------------------------------------------------------------------
            void SetMaxNumberOfLines(u32 in_numLines);
            //-------------------------------------------------------------------
            /// Sets the text scale. Scaling text significantly reduces the
            /// quality of the text rendered so this should only be used when
            /// absolutely necessary.
            ///
            /// @author Ian Copland
            ///
            /// @param The scale factor.
            //-------------------------------------------------------------------
            void SetTextScale(f32 in_scale);
            //-------------------------------------------------------------------
            /// Render the text using the transform and size of the owning
            /// widget.
            ///
            /// @author Ian Copland
            ///
            /// @param The canvas renderer.
            /// @param The final screen space transform.
            /// @param The final screen space size.
            /// @param The final colour.
            //--------------------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour);
            
        private:
            Rendering::FontCSPtr m_font;
            std::string m_text;
            Rendering::HorizontalTextJustification m_horizontalJustification = Rendering::HorizontalTextJustification::k_centre;
            Rendering::VerticalTextJustification m_verticalJustification = Rendering::VerticalTextJustification::k_centre;
            Core::Colour m_textColour;
            f32 m_absCharSpacingOffset = 0.0f;
            f32 m_absLineSpacingOffset = 0.0f;
            f32 m_lineSpacingScale = 1.0f;
            u32 m_maxNumLines = 0;
            f32 m_textScale = 1.0f;
            
            bool m_invalidateCache = true;
            Core::Vector2 m_cachedSize;
            Rendering::CanvasRenderer::BuiltText m_cachedText;
        };
    }
}

#endif