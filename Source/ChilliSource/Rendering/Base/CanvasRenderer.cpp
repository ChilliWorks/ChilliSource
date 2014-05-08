//
//  CanvasRenderer.cpp
//  Chilli Source
//  Created by Scott Downie on 12/01/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ColourUtils.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/GUI/Label/Label.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#ifdef CS_ENABLE_DEBUGSTATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

#include <algorithm>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            //----------------------------------------------------------------------------
            /// Get the width of the character
            ///
            /// @author S Downie
            ///
            /// @param Character
            /// @param Font
            //----------------------------------------------------------------------------
            f32 GetCharacterWidth(Core::UTF8String::Char in_character, const FontCSPtr& in_font)
            {
                Font::CharacterInfo charInfo;
                if(in_font->TryGetCharacterInfo(in_character, charInfo) == true)
                {
                    return charInfo.m_size.x;
                }
                
                return 0.0f;
            }
            //----------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Character
            ///
            /// @return Whether the character can safely be line broken on.
            //----------------------------------------------------------------------------
            bool IsBreakableCharacter(Core::UTF8String::Char in_character)
            {
                return in_character == ' ' || in_character == '\t' || in_character == '\n';
            }
            //----------------------------------------------------------------------------
            /// Calculate the distance in text space to the next 'breakable' character
            ///
            /// @author S Downie
            ///
            /// @param Text
            /// @param Iterator pointing to start
            /// @param Font
            //----------------------------------------------------------------------------
            f32 CalculateDistanceToNextBreak(const Core::UTF8String& in_text, Core::UTF8String::iterator in_itStart, const FontCSPtr& in_font)
            {
                f32 totalWidth = 0.0f;
                
                while(in_itStart < in_text.end())
                {
                    auto nextCharacter = in_text.next(in_itStart);
                    
                    if(IsBreakableCharacter(nextCharacter) == true)
                    {
                        break;
                    }
                    
                    totalWidth += GetCharacterWidth(nextCharacter, in_font);
                }
                
                return totalWidth;
            }
            //----------------------------------------------------------------------------
            /// Split the given text into lines based on any '\n' characters. The newline
            /// characters do not appear in the returned lines
            ///
            /// @author S Downie
            ///
            /// @param Text
            /// @param [Out] Array of lines split by '\n'
            //----------------------------------------------------------------------------
            void SplitByNewLine(const Core::UTF8String& in_text, std::vector<Core::UTF8String>& out_lines)
            {
                Core::UTF8String::iterator it = (Core::UTF8String::iterator)in_text.begin();
                Core::UTF8String line;
                while(it < in_text.end())
                {
                    Core::UTF8String::Char character = in_text.next(it);
                    
                    if(character != '\n')
                    {
                        line.appendChar(character);
                    }
                    else
                    {
                        out_lines.push_back(line);
                        line.clear();
                    }
                }
                
                if(line.size() > 0)
                {
                    out_lines.push_back(line);
                }
            }
            //----------------------------------------------------------------------------
            /// Split the given text into lines based on the constrained bounds.
            /// Splits will prefer to happen on breakable whitespace characters but
            /// will split mid-word if neccessary.
            ///
            /// @author S Downie
            ///
            /// @param Text
            /// @param Font
            /// @param Text scale
            /// @param Bounds
            /// @param [Out] Array of lines split to fit in bounds
            //----------------------------------------------------------------------------
            void SplitByBounds(const Core::UTF8String& in_text, const FontCSPtr& in_font, f32 in_textScale, const Core::Vector2& in_bounds, std::vector<Core::UTF8String>& out_lines)
            {
                f32 maxLineWidth = in_bounds.x;
 
                Core::UTF8String::iterator it = (Core::UTF8String::iterator)in_text.begin();
                Core::UTF8String line;
                f32 currentLineWidth = 0.0f;
                
                while(it < in_text.end())
                {
                    auto character = in_text.next(it);
                    currentLineWidth += (GetCharacterWidth(character, in_font) * in_textScale);
                    
                    //If we come across a character on which we can wrap we need
                    //to check ahead to see if the next space is within the bounds or
                    //whether we need to wrap now
                    if(IsBreakableCharacter(character) == true)
                    {
                        f32 nextBreakWidth = currentLineWidth + (CalculateDistanceToNextBreak(in_text, it, in_font) * in_textScale);
                        
                        if(nextBreakWidth >= maxLineWidth && line.size() > 0)
                        {
                            out_lines.push_back(line);
                            line.clear();
                            currentLineWidth = 0.0f;
                            continue;
                        }
                    }
                    
                    //If text has no characters to break on then we need break anyway should
                    //we exceed the bounds.
                    f32 nextCharacterWidth = 0.0f;
                    if(it < in_text.end())
                    {
                        auto itNext = it;
                        auto nextCharacter = in_text.next(itNext);
                        nextCharacterWidth = GetCharacterWidth(nextCharacter, in_font) * in_textScale;
                    }
                    
                    if((currentLineWidth + nextCharacterWidth) >= maxLineWidth)
                    {
                        out_lines.push_back(line);
                        line.clear();
                        currentLineWidth = 0.0f;
                    }
                    
                    line.appendChar(character);
                }
                
                if(line.size() > 0)
                {
                    out_lines.push_back(line);
                }
            }
            //----------------------------------------------------------------------------
            /// Create the data required to display a character. This includes
            /// the size, UV and position.
            ///
            /// @author S Downie
            ///
            /// @param Character
            /// @param Font
            /// @param Current cursor X pos
            /// @param Current cursor Y pos
            /// @param Text scale
            ///
            /// @return Display characer info
            //----------------------------------------------------------------------------
            CanvasRenderer::DisplayCharacterInfo BuildCharacter(Core::UTF8String::Char in_character, const FontCSPtr& in_font, f32 in_cursorX, f32 in_cursorY, f32 in_textScale)
            {
                CanvasRenderer::DisplayCharacterInfo result;
                
                Font::CharacterInfo info;
                if(in_font->TryGetCharacterInfo(in_character, info) == true)
                {
                    //TODO: The font maker seems to add a 2 unit padding to the font. We need to
                    //remove this or at least find out its purpose.
                    const f32 k_hackToolCorrection = 2.0f;
                    
                    result.m_UVs = info.m_UVs;
                    result.m_size = info.m_size * in_textScale;
                    result.m_position.x = in_cursorX;
                    result.m_position.y = in_cursorY - ((info.m_offset.y - k_hackToolCorrection) * in_textScale);
                }
                else
                {
                    CS_LOG_ERROR("Unknown character not provided by font: " + in_font->GetFilePath());
                }
                
                return result;
            }
            //----------------------------------------------------------------------------
            /// The text by default is left justfied. If another justification
            /// is required this function will update the character positions of a given line
            ///
            /// @author S Downie
            ///
            /// @param Horizontal justification
            /// @param Bounds width
            /// @param Index of the first character in a line
            /// @param Index of the last character in a line
            /// @param Line width in text space.
            /// @param [In/Out] List of display character infos that will be manipulated.
            ///         These are the charcters for all lines.
            //----------------------------------------------------------------------------
            void ApplyHorizontalTextJustifications(GUI::TextJustification in_horizontal, f32 in_boundsWidth, u32 in_lineStartIdx, u32 in_lineEndIdx, f32 in_lineWidth,
                                                   std::vector<CanvasRenderer::DisplayCharacterInfo>& inout_characters)
            {
                f32 horizontalOffset = 0.0f;
                
                switch(in_horizontal)
                {
                    default:
                    case GUI::TextJustification::k_left:
                        return;
                    case GUI::TextJustification::k_centre:
                        horizontalOffset = (in_boundsWidth * 0.5f) - (in_lineWidth * 0.5f);
                        break;
                    case GUI::TextJustification::k_right:
                        horizontalOffset = in_boundsWidth - in_lineWidth;
                        break;
                }
                
                for(u32 i=in_lineStartIdx; i<=in_lineEndIdx; ++i)
                {
                    inout_characters[i].m_position.x += horizontalOffset;
                }
            }
            //----------------------------------------------------------------------------
            /// The text by default is top justfied. If another justification
            /// is required this function will update all the characters positions
            ///
            /// @author S Downie
            ///
            /// @param Vertical justification
            /// @param Bounds height
            /// @param Height of all the built lines combined
            /// @param [In/Out] List of display character infos that will be manipulated
            //----------------------------------------------------------------------------
            void ApplyVerticalTextJustifications(GUI::TextJustification in_vertical, f32 in_boundsHeight, f32 in_totalLinesHeight, std::vector<CanvasRenderer::DisplayCharacterInfo>& inout_characters)
            {
                f32 verticalOffset = 0.0f;
                
                switch(in_vertical)
                {
                    default:
                    case GUI::TextJustification::k_top:
                        return;
                    case GUI::TextJustification::k_centre:
                        verticalOffset = (in_boundsHeight * 0.5f) - (in_totalLinesHeight * 0.5f);
                        break;
                    case GUI::TextJustification::k_bottom:
                        verticalOffset = in_boundsHeight - in_totalLinesHeight;
                        break;
                }
                
                for(auto& character : inout_characters)
                {
                    character.m_position.y -= verticalOffset;
                }
            }
            //-----------------------------------------------------
            /// Build the sprite from the given data
            ///
            /// @author S Downie
            ///
            /// @param Transform
            /// @param Size
            /// @param UVs
            /// @param Colour
            /// @param Alignment
            /// @param [Out] Sprite
            //-----------------------------------------------------
            void UpdateSpriteData(const Core::Matrix4x4& in_transform, const Core::Vector2& in_size, const Core::Rectangle& in_UVs, const Core::Colour& in_colour, AlignmentAnchor in_alignment,
                                  SpriteComponent::SpriteData& out_sprite)
            {
                const f32 k_nearClipDistance = 2.0f;
                
                Core::ByteColour Col = Core::ColourUtils::ColourToByteColour(in_colour);
                
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].Col = Col;
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].Col = Col;
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].Col = Col;
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].Col = Col;
                
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vTex = in_UVs.TopLeft();
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vTex = in_UVs.BottomLeft();
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vTex = in_UVs.TopRight();
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vTex = in_UVs.BottomRight();
                
                Core::Vector2 vHalfSize(in_size.x * 0.5f, in_size.y * 0.5f);
                Core::Vector2 vAlignedPos;
                Align(in_alignment, vHalfSize, vAlignedPos);
                
                Core::Vector4 vCentrePos(vAlignedPos.x, vAlignedPos.y, 0, 0);
                Core::Vector4 vTemp(-vHalfSize.x, vHalfSize.y, 0, 1.0f);
                
                const Core::Matrix4x4& matTransform(in_transform);
                vTemp += vCentrePos;
                Core::Matrix4x4::Multiply(&vTemp, &matTransform, &out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vPos);
                
                vTemp.x = vHalfSize.x;
                vTemp.y = vHalfSize.y;
                
                vTemp += vCentrePos;
                Core::Matrix4x4::Multiply(&vTemp, &matTransform, &out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vPos);
                
                vTemp.x = -vHalfSize.x;
                vTemp.y = -vHalfSize.y;
                
                vTemp += vCentrePos;
                Core::Matrix4x4::Multiply(&vTemp, &matTransform, &out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vPos);
                
                vTemp.x = vHalfSize.x;
                vTemp.y = -vHalfSize.y;
                
                vTemp += vCentrePos;
                Core::Matrix4x4::Multiply(&vTemp, &matTransform, &out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vPos);
                
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vPos.z = -k_nearClipDistance;
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vPos.w = 1.0f;
                
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vPos.z = -k_nearClipDistance;
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vPos.w = 1.0f;
                
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vPos.z = -k_nearClipDistance;
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vPos.w = 1.0f;
                
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vPos.z = -k_nearClipDistance;
                out_sprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vPos.w = 1.0f;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(CanvasRenderer);
        
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        CanvasRendererUPtr CanvasRenderer::Create()
        {
            return CanvasRendererUPtr(new CanvasRenderer());
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        bool CanvasRenderer::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == CanvasRenderer::InterfaceID;
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void CanvasRenderer::OnInit()
        {
            m_materialFactory = Core::Application::Get()->GetSystem<MaterialFactory>();
            CS_ASSERT(m_materialFactory != nullptr, "Must have a material factory");
            
            m_resourcePool = Core::Application::Get()->GetResourcePool();
            CS_ASSERT(m_resourcePool != nullptr, "Must have a resource pool");
            
            RenderSystem* renderSystem = Core::Application::Get()->GetRenderSystem();
            CS_ASSERT(renderSystem != nullptr, "Canvas renderer cannot find render system");
            
            m_overlayBatcher = DynamicSpriteBatchUPtr(new DynamicSpriteBatch(renderSystem));
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        MaterialCSPtr CanvasRenderer::GetGUIMaterialForTexture(const TextureCSPtr& in_texture)
        {
            auto itExistingEntry = m_materialGUICache.find(in_texture);
            if(itExistingEntry != m_materialGUICache.end())
            {
                return itExistingEntry->second;
            }
            else
            {
                std::string materialId("_GUI:" + in_texture->GetFilePath());
                MaterialCSPtr materialExisting = m_resourcePool->GetResource<Material>(materialId);
                
                if(materialExisting != nullptr)
                {
                    m_materialGUICache.insert(std::make_pair(in_texture, materialExisting));
                    return materialExisting;
                }
                
                MaterialSPtr materialNew = m_materialFactory->CreateGUI(materialId);
                materialNew->AddTexture(in_texture);
                m_materialGUICache.insert(std::make_pair(in_texture, materialNew));
                
                return materialNew;
            }
            
            CS_LOG_FATAL("CanvasRenderer: No GUI material created. Some logic has gone wrong");
            return nullptr;
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void CanvasRenderer::Render(GUI::GUIView* in_rootView)
		{
            CS_ASSERT(in_rootView != nullptr, "Canvas cannot render null view");
            
            in_rootView->Draw(this);
			
            m_overlayBatcher->DisableScissoring();
			m_overlayBatcher->ForceRender();
            
            m_materialGUICache.clear();
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void CanvasRenderer::PushClipBounds(const Core::Vector2& in_blPosition, const Core::Vector2& in_size)
        {
            if(m_scissorPositions.empty())
            {
                m_scissorPositions.push_back(in_blPosition);
                m_scissorSizes.push_back(in_size);
            } 
            else
            {
                Core::Vector2 vOldBottomLeft = m_scissorPositions.back();
                Core::Vector2 vOldTopRight = m_scissorSizes.back() + vOldBottomLeft;
                Core::Vector2 vNewBottomLeft = in_blPosition;
                Core::Vector2 vNewTopRight = in_blPosition + in_size;
                
                vNewBottomLeft.x = Core::MathUtils::Max(vNewBottomLeft.x, vOldBottomLeft.x);
                vNewBottomLeft.y = Core::MathUtils::Max(vNewBottomLeft.y, vOldBottomLeft.y);
                
                vNewTopRight.x = Core::MathUtils::Min(vNewTopRight.x, vOldTopRight.x);
                vNewTopRight.y = Core::MathUtils::Min(vNewTopRight.y, vOldTopRight.y);
                
                Core::Vector2 vNewSize = vNewTopRight - vNewBottomLeft;
                
                m_scissorPositions.push_back(vNewBottomLeft);
                m_scissorSizes.push_back(vNewSize);
            }
            
            m_overlayBatcher->EnableScissoring(m_scissorPositions.back(), m_scissorSizes.back());
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void CanvasRenderer::PopClipBounds()
        {
            if(!m_scissorPositions.empty())
            {
                m_scissorPositions.erase(m_scissorPositions.end()-1);
                m_scissorSizes.erase(m_scissorSizes.end()-1);
                
                if(!m_scissorPositions.empty())
                {
                    m_overlayBatcher->EnableScissoring(m_scissorPositions.back(), m_scissorSizes.back());
                }
            }
            
            if(m_scissorPositions.empty())
            {
                m_overlayBatcher->DisableScissoring();
            }
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void CanvasRenderer::DrawBox(const Core::Matrix3x3& in_transform, const Core::Vector2& in_size, const TextureCSPtr& in_texture, const Core::Rectangle& in_UVs,
                                     const Core::Colour& in_colour, AlignmentAnchor in_anchor)
        {
            m_canvasSprite.pMaterial = GetGUIMaterialForTexture(in_texture);
            
			UpdateSpriteData(in_transform, in_size, in_UVs, in_colour, in_anchor, m_canvasSprite);
            
            //Draw us!
			m_overlayBatcher->Render(m_canvasSprite);
            
#ifdef CS_ENABLE_DEBUGSTATS
            Core::Application::Get()->GetDebugStats()->AddToEvent("GUI", 1);
#endif
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        CanvasRenderer::BuiltText CanvasRenderer::BuildText(const Core::UTF8String& in_text, const FontCSPtr& in_font, f32 in_textScale, f32 in_lineSpacing,
                                                            const Core::Vector2& in_bounds, u32 in_numLines, GUI::TextJustification in_horizontal, GUI::TextJustification in_vertical) const
        {
            BuiltText result;
            result.m_width = 0.0f;
            result.m_height = 0.0f;
            result.m_characters.reserve(in_text.size());
            
            //NOTE: | denotes the bounds of the box
            //- |The quick brown fox| jumped over\nthe ferocious honey badger
            
            //Split the string into lines by the forced line breaks (i.e. the \n)
            //- |The quick brown fox| jumped over
            //- |the ferocious honey| badger
            std::vector<Core::UTF8String> linesOnNewLine;
            SplitByNewLine(in_text, linesOnNewLine);
            
            //Split the lines further based on the line width, breakable characters and the bounds
            //- |The quick brown fox|
            //- |jumped over        |
            //- |the ferocious honey|
            //- |badger             |
            std::vector<Core::UTF8String> linesOnBounds;
            for(const auto& line : linesOnNewLine)
            {
                SplitByBounds(line, in_font, in_textScale, in_bounds, linesOnBounds);
            }
            
            //Only build as many lines as we have been told to. If ZERO is specified
            //this means build all lines. We are also constrained by the size of the bounds
            u32 numLines = (in_numLines == 0) ? linesOnBounds.size() : std::min((u32)linesOnBounds.size(), in_numLines);
            
            f32 lineHeight = in_lineSpacing * (in_font->GetLineHeight() * in_textScale);
            f32 maxHeight = in_bounds.y;
            numLines = std::min(numLines, (u32)(maxHeight/lineHeight));
            
            //The middle of the text label is 0,0. We want to be starting at the top left.
            f32 cursorXReturnPos = -in_bounds.x * 0.5f;
            f32 cursorX = cursorXReturnPos;
            f32 cursorY = in_bounds.y * 0.5f;
            
            for(u32 lineIdx=0; lineIdx<numLines; ++lineIdx)
            {
                u32 lineStartIdx = result.m_characters.size();
                u32 numCharacters = linesOnBounds[lineIdx].size();
                for(u32 charIdx=0; charIdx<numCharacters; ++charIdx)
                {
                    auto character = linesOnBounds[lineIdx][charIdx];
                    auto builtCharacter(BuildCharacter(character, in_font, cursorX, cursorY, in_textScale));
                    
                    cursorX += builtCharacter.m_size.x;
                    
                    if(builtCharacter.m_size.y > 0.0f)
                    {
                        //No point rendering whitespaces
                        result.m_characters.push_back(builtCharacter);
                    }
                }
                
                f32 lineWidth = cursorX - cursorXReturnPos;
                ApplyHorizontalTextJustifications(in_horizontal, in_bounds.x, lineStartIdx, result.m_characters.size() - 1, lineWidth, result.m_characters);
                
                result.m_width = std::max(lineWidth, result.m_width);
                
                cursorX = cursorXReturnPos;
                cursorY -= lineHeight;
            }
            
            result.m_height = numLines * lineHeight;
            ApplyVerticalTextJustifications(in_vertical, in_bounds.y, result.m_height, result.m_characters);
            
            return result;
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void CanvasRenderer::DrawText(const std::vector<DisplayCharacterInfo>& in_characters, const Core::Matrix3x3& in_transform, const Core::Colour& in_colour, const TextureCSPtr& in_texture)
		{
            m_canvasSprite.pMaterial = GetGUIMaterialForTexture(in_texture);
            
            Core::Matrix4x4 matTransform(in_transform);
            Core::Matrix4x4 matTransformedLocal;
			
			for (const auto& character : in_characters)
            {
				Core::Matrix4x4 matLocal;
				matLocal.Translate(character.m_position.x, character.m_position.y, 0.0f);
                
                Core::Matrix4x4::Multiply(&matLocal, &matTransform, &matTransformedLocal);
                
                UpdateSpriteData(matTransformedLocal, character.m_size, character.m_UVs, in_colour, AlignmentAnchor::k_topLeft, m_canvasSprite);
				
                m_overlayBatcher->Render(m_canvasSprite);
			}
            
#ifdef CS_ENABLE_DEBUGSTATS
            Core::Application::Get()->GetDebugStats()->AddToEvent("GUI", 1);
#endif
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void CanvasRenderer::OnDestroy()
        {
            m_overlayBatcher = nullptr;
        }
	}
}
