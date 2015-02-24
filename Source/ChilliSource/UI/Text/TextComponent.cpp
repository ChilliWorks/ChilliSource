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

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
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
                {Core::PropertyTypes::Float(), k_textScaleKey}
            };
        }
        
        CS_DEFINE_NAMEDTYPE(TextComponent);
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::vector<Core::PropertyMap::PropertyDesc>& TextComponent::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
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
            
            ApplyRegisteredProperties(in_properties);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        bool TextComponent::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Component::InterfaceID == in_interfaceId || TextComponent::InterfaceID == in_interfaceId);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Rendering::FontCSPtr& TextComponent::GetFont() const
        {
            return m_font;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Core::LocalisedTextCSPtr& TextComponent::GetLocalisedText() const
        {
            return m_localisedText;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::string& TextComponent::GetLocalisedTextId() const
        {
            return m_localisedTextId;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::string& TextComponent::GetText() const
        {
            return m_text;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Core::Colour& TextComponent::GetTextColour() const
        {
            return m_textColour;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        Rendering::HorizontalTextJustification TextComponent::GetHorizontalJustification() const
        {
            return m_textProperties.m_horizontalJustification;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        Rendering::VerticalTextJustification TextComponent::GetVerticalJustification() const
        {
            return m_textProperties.m_verticalJustification;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextComponent::GetAbsoluteCharacterSpacingOffset() const
        {
            return m_textProperties.m_absCharSpacingOffset;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextComponent::GetAbsoluteLineSpacingOffset() const
        {
            return m_textProperties.m_absLineSpacingOffset;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextComponent::GetLineSpacingScale() const
        {
            return m_textProperties.m_lineSpacingScale;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        s32 TextComponent::GetMaxNumberOfLines() const
        {
            return m_textProperties.m_maxNumLines;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 TextComponent::GetTextScale() const
        {
            return m_textProperties.m_textScale;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetFont(const Rendering::FontCSPtr& in_font)
        {
            CS_ASSERT(in_font != nullptr, "Cannot set a null font on a Text Drawable.");
            CS_ASSERT(in_font->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot set an incomplete font on a Text Drawable.");
            
            m_font = in_font;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetLocalisedText(const Core::LocalisedTextCSPtr& in_localisedText)
        {
#ifdef CS_ENABLE_DEBUG
            if (in_localisedText != nullptr)
            {
                CS_ASSERT(in_localisedText->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot set text using an incomplete localised text.");
            }
#endif
            
            m_cachedImages.clear();
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
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetLocalisedTextId(const std::string& in_localisedTextId)
        {
            m_cachedImages.clear();
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
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetLocalisedTextId(const std::string& in_localisedTextId, const Core::ParamDictionary& in_params, const TextIconDictionary& in_imageData)
        {
            CS_ASSERT(m_localisedText != nullptr, "Cannot set text using a null localised text.");
            
            m_cachedImages.clear();
            m_text = ReplaceVariables(m_localisedText->GetText(in_localisedTextId), in_params, in_imageData);
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetText(const std::string& in_text)
        {
            m_cachedImages.clear();
            m_text = in_text;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetText(const std::string& in_text, const TextIconDictionary& in_imageData)
        {
            m_cachedImages.clear();
            m_text = ReplaceVariables(in_text, {}, in_imageData);
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetTextColour(const Core::Colour& in_textColour)
        {
            m_textColour = in_textColour;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetHorizontalJustification(Rendering::HorizontalTextJustification in_horizontalJustification)
        {
            m_textProperties.m_horizontalJustification = in_horizontalJustification;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetVerticalJustification(Rendering::VerticalTextJustification in_verticalJustification)
        {
            m_textProperties.m_verticalJustification = in_verticalJustification;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetAbsoluteCharacterSpacingOffset(f32 in_offset)
        {
            m_textProperties.m_absCharSpacingOffset = in_offset;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetAbsoluteLineSpacingOffset(f32 in_offset)
        {
            m_textProperties.m_absLineSpacingOffset = in_offset;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetLineSpacingScale(f32 in_scale)
        {
            m_textProperties.m_lineSpacingScale = in_scale;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetMaxNumberOfLines(s32 in_numLines)
        {
            CS_ASSERT(in_numLines >= 0, "The maximum number of lines cannot be below 0.");
            
            m_textProperties.m_maxNumLines = in_numLines;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::SetTextScale(f32 in_scale)
        {
            m_textProperties.m_textScale = in_scale;
            
            m_invalidateCache = true;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        std::string TextComponent::ReplaceVariables(const std::string& in_text, const Core::ParamDictionary& in_params, const TextIconDictionary& in_imageData)
        {
            std::string output;
            u32 index = 0;
            
            auto it = in_text.begin();
            while(it < in_text.end())
            {
                auto character = Core::UTF8StringUtils::Next(it);
                
                if(character != '[')
                {
                    Core::UTF8StringUtils::Append(character, output);
                    if(character != ' ' && character != '\t' && character != '\n')
                    {
                        ++index;
                    }
                }
                else
                {
                    // Found a mark up, check it
                    ReplaceVariablesRecursive(in_params, in_imageData, it, index, output);
                }
            }
            
            return output;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void TextComponent::ReplaceVariablesRecursive(const Core::ParamDictionary& in_params, const TextIconDictionary& in_imageData, std::string::const_iterator& out_iterator, u32& out_index, std::string& out_text)
        {
            // Found some mark-up. What type is it?
            std::string type;
            Core::UTF8Char nextChar = '\0';
            
            // Marker for images
            const char k_marker = '-';
            Rendering::Font::CharacterInfo markerInfo;
            m_font->TryGetCharacterInfo(k_marker, markerInfo);
            
            // Spacing info
            Rendering::Font::CharacterInfo spaceInfo;
            m_font->TryGetCharacterInfo(' ', spaceInfo);
            
            while(nextChar != '=')
            {
                nextChar = Core::UTF8StringUtils::Next(out_iterator);
                
                if(nextChar != '=' && nextChar != ' ')
                {
                    type += nextChar;
                }
            }
            
            // Variable type has been located
            std::string varName;
            std::string varValue;
            
            // There may be some whitespace that we need to ignore
            nextChar = Core::UTF8StringUtils::Next(out_iterator);
            if(nextChar != ' ')
            {
                varName += nextChar;
            }
            
            // Find the closing bracket
            while(nextChar != ']')
            {
                nextChar = Core::UTF8StringUtils::Next(out_iterator);
                
                if(nextChar != ']' && nextChar != '[' && nextChar != ' ')
                {
                    varName += nextChar;
                }
                
                // Nested variable
                if(nextChar == '[')
                {
                    std::string variableName;
                    ReplaceVariablesRecursive(in_params, in_imageData, out_iterator, out_index, variableName);
                    varName += variableName;
                }
            }
            
            bool variableReplaced = false;
            
            if(type == "var")
            {
                if(in_params.TryGetValue(varName, varValue))
                {
                    //Let's replace the mark-up with the value
                    out_text.append(varValue);
                    variableReplaced = true;
                }
            }
            else if(type == "img")
            {
                // The image is known
                if(in_imageData.find(varName) != in_imageData.end())
                {
                    // Create a cached image
                    TextIconData iconData;
                    iconData.m_icon = in_imageData.at(varName);
                    iconData.m_indexInText = out_index;
                    
                    f32 height = m_font->GetLineHeight();
                    f32 aspectRatio = (f32)iconData.m_icon.GetTexture()->GetWidth() / (f32)iconData.m_icon.GetTexture()->GetHeight();
                    f32 width = aspectRatio * height;
                    iconData.m_size = Core::Vector2(width, height) * iconData.m_icon.GetScale();
                    
                    if (iconData.m_icon.GetTextureAtlas() != nullptr)
                    {
                        iconData.m_uvs = iconData.m_icon.GetTextureAtlas()->GetFrameUVs(iconData.m_icon.GetTextureAtlasID());
                    }
                    else
                    {
                        iconData.m_uvs = Rendering::UVs(0.0f, 0.0f, 1.0f, 1.0f);
                    }
                    
                    m_cachedImages.push_back(iconData);
                    
                    // Padding with spaces
                    u32 spacesNeeded = 0;
                    if(spaceInfo.m_advance > 0.0f)
                    {
                        spacesNeeded = std::ceil((iconData.m_size.x - markerInfo.m_size.x) / spaceInfo.m_advance);
                        if(spacesNeeded % 2 == 1)
                        {
                            ++spacesNeeded;
                        }
                    }
                    std::string padding;
                    padding.append(&k_marker, 1);
                    for(u32 i = 0; i < spacesNeeded / 2; ++i)
                    {
                        padding = " " + padding + " ";
                    }
                    out_text.append(padding);
                    ++out_index;
                    variableReplaced = true;
                }
            }
            
            // If not, readd the [xxx= ]
            if(!variableReplaced)
            {
                out_text.append("[" + type + "= " + varName + "]");
            }
        }
        //-------------------------------------------------------------------
        //--------------------------------------------------------------------
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
                m_cachedText = in_renderer->BuildText(m_text, m_font, in_absSize, m_textProperties);
                
                // Update images position
                for(auto& image : m_cachedImages)
                {
                    u32 index = image.m_indexInText;
                    const Core::Vector2& charSize = m_cachedText.m_characters[index].m_packedImageSize;
                    image.m_offset.x = m_cachedText.m_characters[index].m_position.x + charSize.x * 0.5f;
                    image.m_offset.y = m_cachedText.m_characters[index].m_position.y - charSize.y * 0.5f;
                }
            }
            
            // Draw text
            in_renderer->DrawText(m_cachedText.m_characters, in_transform, m_textColour, m_font->GetTexture());
            
            // Draw images
            for(const auto& iconData : m_cachedImages)
            {
                Core::Vector2 size = iconData.m_size * m_textProperties.m_textScale;
                in_renderer->DrawBox(in_transform, size, iconData.m_offset, iconData.m_icon.GetTexture(), iconData.m_uvs, Core::Colour::k_white, Rendering::AlignmentAnchor::k_middleCentre);
            }
        }
    }
}