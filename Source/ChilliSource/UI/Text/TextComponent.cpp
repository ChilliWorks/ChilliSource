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
    namespace UI
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
            
            
            const std::vector<Core::PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyTypes::Font(), k_fontKey},
                {PropertyTypes::LocalisedText(), k_localisedTextKey},
                {Core::PropertyTypes::String(), k_localisedTextIdKey},
                {Core::PropertyTypes::String(), k_textKey},
                {Core::PropertyTypes::Colour(), k_textColourKey},
                {PropertyTypes::HorizontalTextJustification(), k_horizontalJustificationKey},
                {PropertyTypes::VerticalTextJustification(), k_verticalJustificationKey},
                {Core::PropertyTypes::Float(), k_absCharSpacingOffsetKey},
                {Core::PropertyTypes::Float(), k_absLineSpacingOffsetKey},
                {Core::PropertyTypes::Float(), k_lineSpacingScaleKey},
                {Core::PropertyTypes::Int(), k_maxNumberOfLinesKey},
                {Core::PropertyTypes::Float(), k_textScaleKey},
                {Core::PropertyTypes::Float(), k_minTextScaleKey},
                {Core::PropertyTypes::Bool(), k_enableAutoScaledTextKey}
            };
        }
        
        CS_DEFINE_NAMEDTYPE(TextComponent);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::vector<Core::PropertyMap::PropertyDesc>& TextComponent::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TextComponent::TextComponent(const std::string& in_componentName, const Core::PropertyMap& in_properties)
            : Component(in_componentName)
        {
            auto resourcePool = Core::Application::Get()->GetResourcePool();
            SetFont(resourcePool->LoadResource<Rendering::Font>(Core::StorageLocation::k_chilliSource, "Fonts/CarlitoMed.csfont"));
            
            RegisterProperty<Rendering::FontCSPtr>(PropertyTypes::Font(), k_fontKey, CSCore::MakeDelegate(this, &TextComponent::GetFont), CSCore::MakeDelegate(this, &TextComponent::SetFont));
            RegisterProperty<Core::LocalisedTextCSPtr>(PropertyTypes::LocalisedText(), k_localisedTextKey, CSCore::MakeDelegate(this, &TextComponent::GetLocalisedText), CSCore::MakeDelegate(this, &TextComponent::SetLocalisedText));
            RegisterProperty<std::string>(Core::PropertyTypes::String(), k_localisedTextIdKey, CSCore::MakeDelegate(this, &TextComponent::GetLocalisedTextId), CSCore::MakeDelegate<TextComponent, TextComponent, void, const std::string&>(this, &TextComponent::SetLocalisedTextId));
            RegisterProperty<std::string>(Core::PropertyTypes::String(), k_textKey, CSCore::MakeDelegate(this, &TextComponent::GetText), CSCore::MakeDelegate<TextComponent, TextComponent, void, const std::string&>(this, &TextComponent::SetText));
            RegisterProperty<Core::Colour>(Core::PropertyTypes::Colour(), k_textColourKey, CSCore::MakeDelegate(this, &TextComponent::GetTextColour), CSCore::MakeDelegate(this, &TextComponent::SetTextColour));
            RegisterProperty<Rendering::HorizontalTextJustification>(PropertyTypes::HorizontalTextJustification(), k_horizontalJustificationKey, CSCore::MakeDelegate(this, &TextComponent::GetHorizontalJustification), CSCore::MakeDelegate(this, &TextComponent::SetHorizontalJustification));
            RegisterProperty<Rendering::VerticalTextJustification>(PropertyTypes::VerticalTextJustification(), k_verticalJustificationKey, CSCore::MakeDelegate(this, &TextComponent::GetVerticalJustification), CSCore::MakeDelegate(this, &TextComponent::SetVerticalJustification));
            RegisterProperty<f32>(Core::PropertyTypes::Float(), k_absCharSpacingOffsetKey, CSCore::MakeDelegate(this, &TextComponent::GetAbsoluteCharacterSpacingOffset), CSCore::MakeDelegate(this, &TextComponent::SetAbsoluteCharacterSpacingOffset));
            RegisterProperty<f32>(Core::PropertyTypes::Float(), k_absLineSpacingOffsetKey, CSCore::MakeDelegate(this, &TextComponent::GetAbsoluteLineSpacingOffset), CSCore::MakeDelegate(this, &TextComponent::SetAbsoluteLineSpacingOffset));
            RegisterProperty<f32>(Core::PropertyTypes::Float(), k_lineSpacingScaleKey, CSCore::MakeDelegate(this, &TextComponent::GetLineSpacingScale), CSCore::MakeDelegate(this, &TextComponent::SetLineSpacingScale));
            RegisterProperty<s32>(Core::PropertyTypes::Int(), k_maxNumberOfLinesKey, CSCore::MakeDelegate(this, &TextComponent::GetMaxNumberOfLines), CSCore::MakeDelegate(this, &TextComponent::SetMaxNumberOfLines));
            RegisterProperty<f32>(Core::PropertyTypes::Float(), k_textScaleKey, CSCore::MakeDelegate(this, &TextComponent::GetTextScale), CSCore::MakeDelegate(this, &TextComponent::SetTextScale));
            RegisterProperty<f32>(Core::PropertyTypes::Float(), k_minTextScaleKey, CSCore::MakeDelegate(this, &TextComponent::GetMinAutoTextScale), CSCore::MakeDelegate(this, &TextComponent::SetMinAutoTextScale));
            RegisterProperty<bool>(Core::PropertyTypes::Bool(), k_enableAutoScaledTextKey, CSCore::MakeDelegate(this, &TextComponent::IsTextAutoScaleEnabled), CSCore::MakeDelegate(this, &TextComponent::SetTextAutoScaleEnabled));
            
            ApplyRegisteredProperties(in_properties);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool TextComponent::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Component::InterfaceID == in_interfaceId || TextComponent::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const Rendering::FontCSPtr& TextComponent::GetFont() const
        {
            return m_font;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const Core::LocalisedTextCSPtr& TextComponent::GetLocalisedText() const
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
        const Core::Colour& TextComponent::GetTextColour() const
        {
            return m_textColour;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Rendering::HorizontalTextJustification TextComponent::GetHorizontalJustification() const
        {
            return m_textProperties.m_horizontalJustification;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Rendering::VerticalTextJustification TextComponent::GetVerticalJustification() const
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
        void TextComponent::SetFont(const Rendering::FontCSPtr& in_font)
        {
            CS_ASSERT(in_font != nullptr, "Cannot set a null font on a Text Drawable.");
            CS_ASSERT(in_font->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot set an incomplete font on a Text Drawable.");
            
            m_font = in_font;
            
            m_invalidateCache = true;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TextComponent::SetLocalisedText(const Core::LocalisedTextCSPtr& in_localisedText)
        {
#ifdef CS_ENABLE_DEBUG
            if (in_localisedText != nullptr)
            {
                CS_ASSERT(in_localisedText->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot set text using an incomplete localised text.");
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
        void TextComponent::SetLocalisedTextId(const std::string& in_localisedTextId, const Core::ParamDictionary& in_params, const TextIconDictionary& in_imageData)
        {
            CS_ASSERT(m_localisedText != nullptr, "Cannot set text using a null localised text.");
            
            ReplaceVariables(m_font, m_localisedText->GetText(in_localisedTextId), in_params, in_imageData, m_text, m_iconIndices);
            
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
            ReplaceVariables(m_font, in_text, {}, in_imageData, m_text, m_iconIndices);
            
            m_invalidateCache = true;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TextComponent::SetTextColour(const Core::Colour& in_textColour)
        {
            m_textColour = in_textColour;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TextComponent::SetHorizontalJustification(Rendering::HorizontalTextJustification in_horizontalJustification)
        {
            m_textProperties.m_horizontalJustification = in_horizontalJustification;
            
            m_invalidateCache = true;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TextComponent::SetVerticalJustification(Rendering::VerticalTextJustification in_verticalJustification)
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
        void TextComponent::ReplaceVariables(const Rendering::FontCSPtr& in_font, const std::string& in_text, const Core::ParamDictionary& in_params, const TextIconDictionary& in_iconDictionary,
                                             std::string& out_text, std::vector<TextIconIndex>& out_iconIndices)
        {
            out_text.clear();
            out_text.shrink_to_fit();
            
            out_iconIndices.clear();
            out_iconIndices.shrink_to_fit();
            
            u32 index = 0;
            
            auto it = in_text.begin();
            while(it < in_text.end())
            {
                auto character = Core::UTF8StringUtils::Next(it);
                
                if(character != '[')
                {
                    Core::UTF8StringUtils::Append(character, out_text);
                    if(character != ' ' && character != '\t' && character != '\n')
                    {
                        ++index;
                    }
                }
                else
                {
                    // Found a mark up, check it
                    ReplaceVariablesRecursive(in_font, in_params, in_iconDictionary, it, index, out_text, out_iconIndices);
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TextComponent::ReplaceVariablesRecursive(const Rendering::FontCSPtr& in_font, const Core::ParamDictionary& in_params, const TextIconDictionary& in_iconDictionary,
                                                      std::string::const_iterator& out_iterator, u32& out_index, std::string& out_text, std::vector<TextIconIndex>& out_iconIndices)
        {
            // Found some mark-up. What type is it?
            std::string type;
            Core::UTF8Char nextChar = '\0';
            
            // Marker for images
            Rendering::Font::CharacterInfo markerInfo;
            in_font->TryGetCharacterInfo(k_imageReplacementKey, markerInfo);
            
            // Spacing info
            Rendering::Font::CharacterInfo spaceInfo;
            in_font->TryGetCharacterInfo(' ', spaceInfo);
            
            while (nextChar != '=')
            {
                nextChar = Core::UTF8StringUtils::Next(out_iterator);
                
                if(nextChar != '=' && nextChar != ' ')
                {
                    type += nextChar;
                }
            }
            
            // Variable type has been located
            std::string varName;
            
            // There may be some whitespace that we need to ignore
            nextChar = Core::UTF8StringUtils::Next(out_iterator);
            if(nextChar != ' ')
            {
                varName += nextChar;
            }
            
            // Find the closing bracket
            while (nextChar != ']')
            {
                nextChar = Core::UTF8StringUtils::Next(out_iterator);
                
                if(nextChar != ']' && nextChar != '[' && nextChar != ' ')
                {
                    varName += nextChar;
                }
                
                // Nested variable
                if(nextChar == '[')
                {
                    std::vector<TextIconIndex> icons;
                    std::string variableName;
                    
                    ReplaceVariablesRecursive(in_font, in_params, in_iconDictionary, out_iterator, out_index, variableName, icons);
                    CS_ASSERT(icons.size() == 0, "Found text icons in a variable name in a TextComponent.");
                    
                    varName += variableName;
                }
            }
            
            bool variableReplaced = false;
            
            if (type == "var")
            {
                variableReplaced = AddVariable(in_params, varName, out_text);
            }
            else if (type == "img")
            {
                variableReplaced = AddIcon(in_font, in_iconDictionary, varName, spaceInfo, markerInfo, out_index, out_text, out_iconIndices);
            }
            
            // If not, readd the [xxx= ]
            if (variableReplaced == false)
            {
                out_text.append("[" + type + "= " + varName + "]");
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool TextComponent::AddVariable(const Core::ParamDictionary& in_params, const std::string& in_variableName, std::string& out_text)
        {
            std::string value;
            
            if (in_params.TryGetValue(in_variableName, value))
            {
                out_text.append(value);
                return true;
            }
            
            return false;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool TextComponent::AddIcon(const Rendering::FontCSPtr& in_font, const TextIconDictionary& in_iconDictionary, const std::string& in_iconName, const Rendering::Font::CharacterInfo& in_spaceInfo,
                                    const Rendering::Font::CharacterInfo& in_markerInfo, u32& out_index, std::string& out_text, std::vector<TextIconIndex>& out_iconIndices)
        {
            auto iconIt = in_iconDictionary.find(in_iconName);
            if (iconIt != in_iconDictionary.end())
            {
                //Create the Text Icon Index entry.
                TextIconIndex iconIndex;
                iconIndex.m_icon = iconIt->second;
                iconIndex.m_indexInText = out_index;
                out_iconIndices.push_back(iconIndex);
                
                //Make space for the Icon in the string. This doesn't include the text scale since that will be accounted for later.
                f32 aspectRatio = iconIndex.m_icon.GetOriginalSize().x / iconIndex.m_icon.GetOriginalSize().y;
                f32 width = aspectRatio * in_font->GetLineHeight() * iconIndex.m_icon.GetScale();
                
                u32 spacesNeeded = 0;
                if(in_spaceInfo.m_advance > 0.0f)
                {
                    spacesNeeded = std::ceil((width - in_spaceInfo.m_size.x) / in_spaceInfo.m_advance);
                    if(spacesNeeded % 2 == 1)
                    {
                        ++spacesNeeded;
                    }
                }
                std::string padding;
                padding.append(&k_imageReplacementKey, 1);
                for(u32 i = 0; i < spacesNeeded / 2; ++i)
                {
                    padding = " " + padding + " ";
                }
                out_text.append(padding);
                ++out_index;
                
                return true;
            }
            
            return false;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        std::vector<TextComponent::TextIconCachedData> TextComponent::BuildIcons(const Rendering::FontCSPtr& in_font, const Rendering::CanvasRenderer::BuiltText& in_builtText,
                                                                                 const std::vector<TextIconIndex>& in_iconIndices, f32 in_textScale)
        {
            std::vector<TextIconCachedData> output;
            
            for(const auto& iconIndex : in_iconIndices)
            {
                if (iconIndex.m_indexInText < in_builtText.m_characters.size())
                {
                    const Core::Vector2& charSize = in_builtText.m_characters[iconIndex.m_indexInText].m_packedImageSize;
                    f32 charPosX = in_builtText.m_characters[iconIndex.m_indexInText].m_position.x + charSize.x * 0.5f;
                    f32 charPosY = in_builtText.m_characters[iconIndex.m_indexInText].m_position.y - charSize.y * 0.5f;
                    
                    f32 croppedHeightRatio = iconIndex.m_icon.GetCroppedSize().y / iconIndex.m_icon.GetOriginalSize().y;
                    f32 height = in_font->GetLineHeight() * croppedHeightRatio * iconIndex.m_icon.GetScale() * in_textScale;
                    
                    f32 croppedIconAspectRatio = iconIndex.m_icon.GetCroppedSize().x / iconIndex.m_icon.GetCroppedSize().y;
                    f32 width = croppedIconAspectRatio * height;
                    
                    //calculate the offset from the centre of the original image quad to cropped
                    f32 croppedOffsetFromCentreX = iconIndex.m_icon.GetOffset().x + iconIndex.m_icon.GetCroppedSize().x * 0.5f - iconIndex.m_icon.GetOriginalSize().x * 0.5f;
                    f32 croppedOffsetFromCentreY = -(iconIndex.m_icon.GetOffset().y + iconIndex.m_icon.GetCroppedSize().y * 0.5f - iconIndex.m_icon.GetOriginalSize().y * 0.5f);
                    auto croppedOffset = Core::Vector2(croppedOffsetFromCentreX, croppedOffsetFromCentreY) * iconIndex.m_icon.GetScale() * in_textScale;
                    
                    TextIconCachedData iconData;
                    iconData.m_texture = iconIndex.m_icon.GetTexture();
                    iconData.m_size = Core::Vector2(width, height);
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
        void TextComponent::OnDraw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
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
				in_renderer->DrawBox(in_transform, iconData.m_size, iconData.m_offset, iconData.m_texture, iconData.m_uvs, GetWidget()->GetFinalColour(), Rendering::AlignmentAnchor::k_middleCentre);
            }
        }
    }
}