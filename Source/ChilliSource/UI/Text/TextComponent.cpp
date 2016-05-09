//
//  TextComponent.cpp
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

#include <ChilliSource/UI/Text/TextComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Container/Property/PropertyTypes.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
    namespace
    {
        const char k_imageReplacementKey = ':';
        
        const char k_fontKey[] = "Font";
        const char k_localisedTextKey[] = "LocalisedText";
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
        const char k_minTextScaleKey[] = "MinTextAutoScale";
        const char k_enableAutoScaledTextKey[] = "EnableAutoTextScale";
        
        const char k_keywordImage[] = "img";
        const char k_keywordVariable[] = "var";
        
        
        const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
        {
            {PropertyTypes::Font(), k_fontKey},
            {PropertyTypes::LocalisedText(), k_localisedTextKey},
            {PropertyTypes::String(), k_localisedTextIdKey},
            {PropertyTypes::String(), k_textKey},
            {PropertyTypes::Colour(), k_textColourKey},
            {PropertyTypes::HorizontalTextJustification(), k_horizontalJustificationKey},
            {PropertyTypes::VerticalTextJustification(), k_verticalJustificationKey},
            {PropertyTypes::Float(), k_absCharSpacingOffsetKey},
            {PropertyTypes::Float(), k_absLineSpacingOffsetKey},
            {PropertyTypes::Float(), k_lineSpacingScaleKey},
            {PropertyTypes::Int(), k_maxNumberOfLinesKey},
            {PropertyTypes::Float(), k_textScaleKey},
            {PropertyTypes::Float(), k_minTextScaleKey},
            {PropertyTypes::Bool(), k_enableAutoScaledTextKey}
        };
    }
    
    CS_DEFINE_NAMEDTYPE(TextComponent);
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& TextComponent::GetPropertyDescs()
    {
        return k_propertyDescs;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    TextComponent::TextComponent(const std::string& in_componentName, const PropertyMap& in_properties)
        : UIComponent(in_componentName)
    {
        auto resourcePool = Application::Get()->GetResourcePool();
        SetFont(resourcePool->LoadResource<Font>(StorageLocation::k_chilliSource, "Fonts/CarlitoMed.csfont"));
        
        RegisterProperty<FontCSPtr>(PropertyTypes::Font(), k_fontKey, MakeDelegate(this, &TextComponent::GetFont), MakeDelegate(this, &TextComponent::SetFont));
        RegisterProperty<LocalisedTextCSPtr>(PropertyTypes::LocalisedText(), k_localisedTextKey, MakeDelegate(this, &TextComponent::GetLocalisedText), MakeDelegate(this, &TextComponent::SetLocalisedText));
        RegisterProperty<std::string>(PropertyTypes::String(), k_localisedTextIdKey, MakeDelegate(this, &TextComponent::GetLocalisedTextId), MakeDelegate<TextComponent, TextComponent, void, const std::string&>(this, &TextComponent::SetLocalisedTextId));
        RegisterProperty<std::string>(PropertyTypes::String(), k_textKey, MakeDelegate(this, &TextComponent::GetText), MakeDelegate<TextComponent, TextComponent, void, const std::string&>(this, &TextComponent::SetText));
        RegisterProperty<Colour>(PropertyTypes::Colour(), k_textColourKey, MakeDelegate(this, &TextComponent::GetTextColour), MakeDelegate(this, &TextComponent::SetTextColour));
        RegisterProperty<HorizontalTextJustification>(PropertyTypes::HorizontalTextJustification(), k_horizontalJustificationKey, MakeDelegate(this, &TextComponent::GetHorizontalJustification), MakeDelegate(this, &TextComponent::SetHorizontalJustification));
        RegisterProperty<VerticalTextJustification>(PropertyTypes::VerticalTextJustification(), k_verticalJustificationKey, MakeDelegate(this, &TextComponent::GetVerticalJustification), MakeDelegate(this, &TextComponent::SetVerticalJustification));
        RegisterProperty<f32>(PropertyTypes::Float(), k_absCharSpacingOffsetKey, MakeDelegate(this, &TextComponent::GetAbsoluteCharacterSpacingOffset), MakeDelegate(this, &TextComponent::SetAbsoluteCharacterSpacingOffset));
        RegisterProperty<f32>(PropertyTypes::Float(), k_absLineSpacingOffsetKey, MakeDelegate(this, &TextComponent::GetAbsoluteLineSpacingOffset), MakeDelegate(this, &TextComponent::SetAbsoluteLineSpacingOffset));
        RegisterProperty<f32>(PropertyTypes::Float(), k_lineSpacingScaleKey, MakeDelegate(this, &TextComponent::GetLineSpacingScale), MakeDelegate(this, &TextComponent::SetLineSpacingScale));
        RegisterProperty<s32>(PropertyTypes::Int(), k_maxNumberOfLinesKey, MakeDelegate(this, &TextComponent::GetMaxNumberOfLines), MakeDelegate(this, &TextComponent::SetMaxNumberOfLines));
        RegisterProperty<f32>(PropertyTypes::Float(), k_textScaleKey, MakeDelegate(this, &TextComponent::GetTextScale), MakeDelegate(this, &TextComponent::SetTextScale));
        RegisterProperty<f32>(PropertyTypes::Float(), k_minTextScaleKey, MakeDelegate(this, &TextComponent::GetMinAutoTextScale), MakeDelegate(this, &TextComponent::SetMinAutoTextScale));
        RegisterProperty<bool>(PropertyTypes::Bool(), k_enableAutoScaledTextKey, MakeDelegate(this, &TextComponent::IsTextAutoScaleEnabled), MakeDelegate(this, &TextComponent::SetTextAutoScaleEnabled));
        
        ApplyRegisteredProperties(in_properties);
        
        // Register a new parser
        MarkupDef markupDef;
        markupDef.AddKeyword(k_keywordImage, false);
        markupDef.AddKeyword(k_keywordVariable, true);
        m_markupParser = StringMarkupParser(markupDef);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool TextComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIComponent::InterfaceID == in_interfaceId || TextComponent::InterfaceID == in_interfaceId);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const FontCSPtr& TextComponent::GetFont() const
    {
        return m_font;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const LocalisedTextCSPtr& TextComponent::GetLocalisedText() const
    {
        return m_localisedText;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const std::string& TextComponent::GetLocalisedTextId() const
    {
        return m_localisedTextId;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const std::string& TextComponent::GetText() const
    {
        return m_text;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const Colour& TextComponent::GetTextColour() const
    {
        return m_textColour;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    HorizontalTextJustification TextComponent::GetHorizontalJustification() const
    {
        return m_textProperties.m_horizontalJustification;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    VerticalTextJustification TextComponent::GetVerticalJustification() const
    {
        return m_textProperties.m_verticalJustification;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    f32 TextComponent::GetAbsoluteCharacterSpacingOffset() const
    {
        return m_textProperties.m_absCharSpacingOffset;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    f32 TextComponent::GetAbsoluteLineSpacingOffset() const
    {
        return m_textProperties.m_absLineSpacingOffset;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    f32 TextComponent::GetLineSpacingScale() const
    {
        return m_textProperties.m_lineSpacingScale;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    s32 TextComponent::GetMaxNumberOfLines() const
    {
        return m_textProperties.m_maxNumLines;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    f32 TextComponent::GetTextScale() const
    {
        return m_textProperties.m_textScale;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    f32 TextComponent::GetMinAutoTextScale() const
    {
        return m_textProperties.m_minTextScale;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool TextComponent::IsTextAutoScaleEnabled() const
    {
        return m_textProperties.m_shouldAutoScale;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetFont(const FontCSPtr& in_font)
    {
        CS_ASSERT(in_font != nullptr, "Cannot set a null font on a Text UIDrawable.");
        CS_ASSERT(in_font->GetLoadState() == Resource::LoadState::k_loaded, "Cannot set an incomplete font on a Text UIDrawable.");
        
        m_font = in_font;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetLocalisedText(const LocalisedTextCSPtr& in_localisedText)
    {
#ifdef CS_ENABLE_DEBUG
        if (in_localisedText != nullptr)
        {
            CS_ASSERT(in_localisedText->GetLoadState() == Resource::LoadState::k_loaded, "Cannot set text using an incomplete localised text.");
        }
#endif
        
        m_iconIndices.clear();
        m_localisedText = in_localisedText;
        
        if (m_localisedText != nullptr && m_localisedText->Contains(m_localisedTextId) == true)
        {
            m_text = m_localisedText->GetText(m_localisedTextId);
        }
        else
        {
            m_text = "";
        }
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetLocalisedTextId(const std::string& in_localisedTextId)
    {
        m_iconIndices.clear();
        m_localisedTextId = in_localisedTextId;
        
        if (m_localisedText != nullptr && m_localisedText->Contains(m_localisedTextId) == true)
        {
            m_text = m_localisedText->GetText(m_localisedTextId);
        }
        else
        {
            m_text = "";
        }
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetLocalisedTextId(const std::string& in_localisedTextId, const ParamDictionary& in_params, const TextIconDictionary& in_imageData)
    {
        CS_ASSERT(m_localisedText != nullptr, "Cannot set text using a null localised text.");
        
        ReplaceVariables(m_localisedText->GetText(in_localisedTextId), in_params, in_imageData);
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetText(const std::string& in_text)
    {
        m_iconIndices.clear();
        m_text = in_text;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetText(const std::string& in_text, const TextIconDictionary& in_imageData)
    {
        ReplaceVariables(in_text, {}, in_imageData);
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetTextColour(const Colour& in_textColour)
    {
        m_textColour = in_textColour;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetHorizontalJustification(HorizontalTextJustification in_horizontalJustification)
    {
        m_textProperties.m_horizontalJustification = in_horizontalJustification;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetVerticalJustification(VerticalTextJustification in_verticalJustification)
    {
        m_textProperties.m_verticalJustification = in_verticalJustification;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetAbsoluteCharacterSpacingOffset(f32 in_offset)
    {
        m_textProperties.m_absCharSpacingOffset = in_offset;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetAbsoluteLineSpacingOffset(f32 in_offset)
    {
        m_textProperties.m_absLineSpacingOffset = in_offset;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetLineSpacingScale(f32 in_scale)
    {
        m_textProperties.m_lineSpacingScale = in_scale;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetMaxNumberOfLines(s32 in_numLines)
    {
        CS_ASSERT(in_numLines >= 0, "The maximum number of lines cannot be below 0.");
        
        m_textProperties.m_maxNumLines = in_numLines;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetTextScale(f32 in_scale)
    {
        m_textProperties.m_textScale = in_scale;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetMinAutoTextScale(f32 in_scale)
    {
        m_textProperties.m_minTextScale = in_scale;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::SetTextAutoScaleEnabled(bool in_enable)
    {
        m_textProperties.m_shouldAutoScale = in_enable;
        
        m_invalidateCache = true;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::ReplaceVariables(const std::string& in_text, const ParamDictionary& in_params, const TextIconDictionary& in_iconDictionary)
    {
        m_text.clear();
        m_text.shrink_to_fit();
        
        m_iconIndices.clear();
        m_iconIndices.shrink_to_fit();
        
        // Marker for images
        Font::CharacterInfo markerInfo;
        m_font->TryGetCharacterInfo(k_imageReplacementKey, markerInfo);
        
        // Spacing info
        Font::CharacterInfo spaceInfo;
        m_font->TryGetCharacterInfo(' ', spaceInfo);
        
        // Parses the text to replace variables
        m_text = m_markupParser.Parse(in_text, [=](const std::string& in_name, const std::string& in_keywordValue, u32 in_indexInString) -> std::string
        {
            std::string value;
            if (in_name == k_keywordVariable)
            {
                in_params.TryGetValue(in_keywordValue, value);
            }
            else if (in_name == k_keywordImage)
            {
                value = AddIcon(m_font, in_iconDictionary, in_keywordValue, spaceInfo, markerInfo, in_indexInString, m_iconIndices);
            }
            
            return value;
        });
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    std::string TextComponent::AddIcon(const FontCSPtr& in_font, const TextIconDictionary& in_iconDictionary, const std::string& in_iconName, const Font::CharacterInfo& in_spaceInfo,
                                       const Font::CharacterInfo& in_markerInfo, u32& out_index, std::vector<TextIconIndex>& out_iconIndices)
    {
        std::string iconText;
        
        auto iconIt = in_iconDictionary.find(in_iconName);
        
        CS_ASSERT(iconIt != in_iconDictionary.end(), "Unknown icon name in TextComponent: " + in_iconName);
        
        //Create the Text Icon Index entry.
        TextIconIndex iconIndex;
        iconIndex.m_icon = iconIt->second;
        iconIndex.m_indexInText = out_index + static_cast<u32>(out_iconIndices.size());
        out_iconIndices.push_back(iconIndex);
        
        //Make space for the Icon in the string. This doesn't include the text scale since that will be accounted for later.
        f32 aspectRatio = iconIndex.m_icon.GetOriginalSize().x / iconIndex.m_icon.GetOriginalSize().y;
        f32 width = aspectRatio * in_font->GetLineHeight() * iconIndex.m_icon.GetScale();
        
        u32 spacesNeeded = 0;
        if(in_spaceInfo.m_advance > 0.0f)
        {
            spacesNeeded = u32(std::ceil((width - in_spaceInfo.m_size.x) / in_spaceInfo.m_advance));
            if(spacesNeeded % 2 == 1)
            {
                ++spacesNeeded;
            }
        }
        
        iconText.append(&k_imageReplacementKey, 1);
        for(u32 i = 0; i < spacesNeeded / 2; ++i)
        {
            iconText = " " + iconText + " ";
        }
        
        ++out_index;
        
        return iconText;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    std::vector<TextComponent::TextIconCachedData> TextComponent::BuildIcons(const FontCSPtr& in_font, const CanvasRenderer::BuiltText& in_builtText,
                                                                             const std::vector<TextIconIndex>& in_iconIndices, f32 in_textScale)
    {
        std::vector<TextIconCachedData> output;
        
        for(const auto& iconIndex : in_iconIndices)
        {
            if (iconIndex.m_indexInText < in_builtText.m_characters.size())
            {
                const Vector2& charSize = in_builtText.m_characters[iconIndex.m_indexInText].m_packedImageSize;
                f32 charPosX = in_builtText.m_characters[iconIndex.m_indexInText].m_position.x + charSize.x * 0.5f;
                f32 charPosY = in_builtText.m_characters[iconIndex.m_indexInText].m_position.y - charSize.y * 0.5f;
                
                f32 croppedHeightRatio = iconIndex.m_icon.GetCroppedSize().y / iconIndex.m_icon.GetOriginalSize().y;
                f32 height = in_font->GetLineHeight() * croppedHeightRatio * iconIndex.m_icon.GetScale() * in_textScale;
                
                f32 croppedIconAspectRatio = iconIndex.m_icon.GetCroppedSize().x / iconIndex.m_icon.GetCroppedSize().y;
                f32 width = croppedIconAspectRatio * height;
                
                //calculate the offset from the centre of the original image quad to cropped
                f32 croppedOffsetFromCentreX = iconIndex.m_icon.GetOffset().x + iconIndex.m_icon.GetCroppedSize().x * 0.5f - iconIndex.m_icon.GetOriginalSize().x * 0.5f;
                f32 croppedOffsetFromCentreY = -(iconIndex.m_icon.GetOffset().y + iconIndex.m_icon.GetCroppedSize().y * 0.5f - iconIndex.m_icon.GetOriginalSize().y * 0.5f);
                auto croppedOffset = Vector2(croppedOffsetFromCentreX, croppedOffsetFromCentreY) * iconIndex.m_icon.GetScale() * in_textScale;
                
                TextIconCachedData iconData;
                iconData.m_texture = iconIndex.m_icon.GetTexture();
                iconData.m_size = Vector2(width, height);
                iconData.m_uvs = iconIndex.m_icon.GetUVs();
                iconData.m_offset.x = croppedOffset.x + charPosX;
                iconData.m_offset.y = croppedOffset.y + charPosY;
                
                output.push_back(iconData);
            }
        }
        
        return output;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TextComponent::OnDraw(CanvasRenderer* in_renderer, const Matrix3& in_transform, const Vector2& in_absSize, const Colour& in_absColour)
    {
        if (m_cachedSize != in_absSize)
        {
            m_cachedSize = in_absSize;
            m_invalidateCache = true;
        }
        
        if (m_invalidateCache == true)
        {
            m_invalidateCache = false;
            
            f32 textScale = 1.0f;
            m_cachedText = in_renderer->BuildText(m_text, m_font, in_absSize, m_textProperties, textScale);
            m_cachedIcons = BuildIcons(m_font, m_cachedText, m_iconIndices, textScale);
        }
        
        // Draw text
        in_renderer->DrawText(m_cachedText.m_characters, in_transform, m_textColour * GetWidget()->GetFinalColour(), m_font->GetTexture());
        
        // Draw images
        for(const auto& iconData : m_cachedIcons)
        {
            in_renderer->DrawBox(in_transform, iconData.m_size, iconData.m_offset, iconData.m_texture, iconData.m_uvs, GetWidget()->GetFinalColour(), AlignmentAnchor::k_middleCentre);
        }
    }
}