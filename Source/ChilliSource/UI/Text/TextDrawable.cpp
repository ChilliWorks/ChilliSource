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

#include <ChilliSource/UI/Text/TextDrawable.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Font/Font.h>

//////////// REMOVE ME!
#include <ChilliSource/GUI/Label/Label.h>
///////////////////////

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const char k_fontLocationKey[] = "FontLocation";
            const char k_fontPathKey[] = "FontPath";
            const char k_localisedTextLocationKey[] = "LocalisedTextLocation";
            const char k_localisedTextPathKey[] = "LocalisedTextPath";
            const char k_localisedTextIdKey[] = "LocalisedTextId";
            const char k_textKey[] = "Text";
            const char k_textColourKey[] = "TextColour";
            const char k_horizontalJustificationKey[] = "HorizontalJustification";
            const char k_verticalJustificationKey[] = "VerticalJustification";
            const char k_absCharSpacingOffsetKey[] = "AbsCharSpacingOffset";
            const char k_absLineSpacingOffsetKey[] = "AbsLineSpacingOffset";
            const char k_lineSpacingScaleKey[] = "LineSpacingScale";
            const char k_maxNumberOfLinesKey[] = "MaxNumberOfLines";
            const char k_textScaleKey[] = "TextScale";
            
            const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyType::k_string, k_fontLocationKey},
                {PropertyType::k_string, k_fontPathKey},
                {PropertyType::k_string, k_localisedTextLocationKey},
                {PropertyType::k_string, k_localisedTextPathKey},
                {PropertyType::k_string, k_localisedTextIdKey},
                {PropertyType::k_string, k_textKey},
                {PropertyType::k_colour, k_textColourKey},
                {PropertyType::k_string, k_horizontalJustificationKey},
                {PropertyType::k_string, k_verticalJustificationKey},
                {PropertyType::k_float, k_absCharSpacingOffsetKey},
                {PropertyType::k_float, k_absLineSpacingOffsetKey},
                {PropertyType::k_float, k_lineSpacingScaleKey},
                {PropertyType::k_int, k_maxNumberOfLinesKey},
                {PropertyType::k_float, k_textScaleKey}
            };
        }
        
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::vector<PropertyMap::PropertyDesc>& TextDrawable::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        TextDrawable::TextDrawable(const Rendering::FontCSPtr& in_font, const Core::LocalisedTextCSPtr& in_localisedText, const std::string& in_localisedTextId)
        {
            SetFont(in_font);
            SetText(in_localisedText, in_localisedTextId);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        TextDrawable::TextDrawable(const Rendering::FontCSPtr& in_font, const std::string& in_string)
        {
            SetFont(in_font);
            SetText(in_string);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        TextDrawable::TextDrawable(const PropertyMap& in_properties)
        {
            auto resourcePool = Core::Application::Get()->GetResourcePool();
            
            //load the font
            std::string fontLocation = in_properties.GetPropertyOrDefault(k_fontLocationKey, "Package");
            std::string fontPath = in_properties.GetPropertyOrDefault(k_fontPathKey, "");
            CS_ASSERT((fontLocation.empty() == false && fontPath.empty() == false), "Must supply a valid font file path.");
            SetFont(resourcePool->LoadResource<Rendering::Font>(Core::ParseStorageLocation(fontLocation), fontPath));
            
            //load the localised text if one if supplied, otherwise fall back on the manually supplied text.
            if (in_properties.HasValue(k_localisedTextPathKey) == true)
            {
                std::string localisedTextLocation = in_properties.GetPropertyOrDefault(k_localisedTextLocationKey, "Package");
                std::string localisedTextPath = in_properties.GetProperty<std::string>(k_localisedTextPathKey);
           
                auto localisedText = resourcePool->LoadResource<Core::LocalisedText>(Core::ParseStorageLocation(localisedTextLocation), localisedTextPath);
                std::string localisedTextId = in_properties.GetPropertyOrDefault(k_localisedTextIdKey, "");
                SetText(localisedText, localisedTextId);
            }
            else
            {
                std::string text = in_properties.GetPropertyOrDefault(k_textKey, "");
                SetText(text);
            }
            
            //read the other properties
            SetTextColour(in_properties.GetPropertyOrDefault(k_textColourKey, GetTextColour()));

            if (in_properties.HasValue(k_horizontalJustificationKey) == true)
            {
                std::string horizontalJustification = in_properties.GetProperty(k_horizontalJustificationKey);
                SetHorizontalJustification(Rendering::ParseHorizontalTextJustification(horizontalJustification));
            }
            
            if (in_properties.HasValue(k_verticalJustificationKey) == true)
            {
                std::string verticalJustification = in_properties.GetProperty(k_verticalJustificationKey);
                SetVerticalJustification(Rendering::ParseVerticalTextJustification(verticalJustification));
            }
            
            SetAbsoluteCharacterSpacingOffset(in_properties.GetPropertyOrDefault(k_absCharSpacingOffsetKey, GetAbsoluteCharacterSpacingOffset()));
            SetAbsoluteLineSpacingOffset(in_properties.GetPropertyOrDefault(k_absLineSpacingOffsetKey, GetAbsoluteLineSpacingOffset()));
            SetLineSpacingScale(in_properties.GetPropertyOrDefault(k_lineSpacingScaleKey, GetLineSpacingScale()));
            SetMaxNumberOfLines(in_properties.GetPropertyOrDefault(k_maxNumberOfLinesKey, (s32)GetMaxNumberOfLines()));
            SetTextScale(in_properties.GetPropertyOrDefault(k_textScaleKey, GetTextScale()));
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Rendering::FontCSPtr& TextDrawable::GetFont() const
        {
            return m_font;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::string& TextDrawable::GetText() const
        {
            return m_text;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Core::Colour& TextDrawable::GetTextColour() const
        {
            return m_textColour;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        Rendering::HorizontalTextJustification TextDrawable::GetHorizontalJustification() const
        {
            return m_horizontalJustification;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        Rendering::VerticalTextJustification TextDrawable::GetVerticalJustification() const
        {
            return m_verticalJustification;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextDrawable::GetAbsoluteCharacterSpacingOffset() const
        {
            return m_absCharSpacingOffset;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextDrawable::GetAbsoluteLineSpacingOffset() const
        {
            return m_absLineSpacingOffset;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextDrawable::GetLineSpacingScale() const
        {
            return m_lineSpacingScale;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        u32 TextDrawable::GetMaxNumberOfLines() const
        {
            return m_maxNumLines;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextDrawable::GetTextScale() const
        {
            return m_textScale;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetFont(const Rendering::FontCSPtr& in_font)
        {
            CS_ASSERT(in_font != nullptr, "Cannot set a null font on a Text Drawable.");
            CS_ASSERT(in_font->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot set an incomplete font on a Text Drawable.");
            
            m_font = in_font;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetText(const Core::LocalisedTextCSPtr& in_localisedText, const std::string& in_localisedTextId)
        {
            CS_ASSERT(in_localisedText != nullptr, "Cannot set text using a null localised text.");
            CS_ASSERT(in_localisedText->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot set text using an incomplete localised text.");
            
            m_text = in_localisedText->GetText(in_localisedTextId);
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetText(const std::string& in_text)
        {
            m_text = in_text;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetTextColour(const Core::Colour& in_textColour)
        {
            m_textColour = in_textColour;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetHorizontalJustification(Rendering::HorizontalTextJustification in_horizontalJustification)
        {
            m_horizontalJustification = in_horizontalJustification;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetVerticalJustification(Rendering::VerticalTextJustification in_verticalJustification)
        {
            m_verticalJustification = in_verticalJustification;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetAbsoluteCharacterSpacingOffset(f32 in_offset)
        {
            m_absCharSpacingOffset = in_offset;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetAbsoluteLineSpacingOffset(f32 in_offset)
        {
            m_absLineSpacingOffset = in_offset;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetLineSpacingScale(f32 in_scale)
        {
            m_lineSpacingScale = in_scale;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetMaxNumberOfLines(u32 in_numLines)
        {
            m_maxNumLines = in_numLines;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextDrawable::SetTextScale(f32 in_scale)
        {
            m_textScale = in_scale;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //--------------------------------------------------------------------
        void TextDrawable::Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            if (m_cachedSize != in_absSize)
            {
                m_cachedSize = in_absSize;
                m_invalidateCache = true;
            }
            
            if (m_invalidateCache == true)
            {
                m_invalidateCache = false;
                m_cachedText = in_renderer->BuildText(m_text, m_font, m_textScale, m_absCharSpacingOffset, m_absLineSpacingOffset, m_lineSpacingScale, in_absSize, m_maxNumLines, m_horizontalJustification, m_verticalJustification);
            }
        
            in_renderer->DrawText(m_cachedText.m_characters, in_transform, m_textColour, m_font->GetTexture());
        }
    }
}