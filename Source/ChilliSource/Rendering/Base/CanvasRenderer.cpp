/*
 *  CanvasRenderer.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 12/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Base/ColourUtils.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>

#include <ChilliSource/GUI/Label/Label.h>

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
            const f32 k_maxKernRatio = 0.25;
            
            //----------------------------------------------------------------------------
            /// Get the width of the character pointed to by the iterator
            ///
            /// @author S Downie
            ///
            /// @param Text
            /// @param Iterator pointing to current character
            /// @param Font
            //----------------------------------------------------------------------------
            f32 GetCharacterWidth(const Core::UTF8String& in_text, Core::UTF8String::iterator in_currentItPos, const FontCSPtr& in_font)
            {
                Core::UTF8String::Char character = *in_currentItPos;
                
                Font::CharacterInfo charInfo;
                if(in_font->TryGetCharacterInfo(character, charInfo) == true)
                {
                    return charInfo.m_size.x;
                }
                
                return 0.0f;
            }
            //----------------------------------------------------------------------------
            /// Calculate the width of the text in text space. This does not take
            /// any breaks etc into account.
            ///
            /// @author S Downie
            ///
            /// @param Text
            /// @param Font
            //----------------------------------------------------------------------------
            f32 CalculateTextWidth(const Core::UTF8String& in_text, const FontCSPtr& in_font)
            {
                f32 totalWidth = 0.0f;
                
                Core::UTF8String::iterator it = (Core::UTF8String::iterator)in_text.begin();
                while(in_text.end() != it)
                {
                    totalWidth += GetCharacterWidth(in_text, it, in_font);
                    in_text.next(it);
                }
                
                return totalWidth;
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
            /// @param Iterator pointing to current character
            /// @param Font
            //----------------------------------------------------------------------------
            f32 CalculateDistanceToNextBreak(const Core::UTF8String& in_text, Core::UTF8String::iterator in_currentItPos, const FontCSPtr& in_font)
            {
                Core::UTF8String textToBreak;
                Core::UTF8String::iterator itToBreak = in_currentItPos;
                Core::UTF8String::Char nextCharacter = in_text.next(itToBreak);
                while(itToBreak != in_text.end() && IsBreakableCharacter(nextCharacter) == false)
                {
                    textToBreak.appendChar(nextCharacter);
                    nextCharacter = in_text.next(itToBreak);
                }
                
                return CalculateTextWidth(textToBreak, in_font);
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
                while(in_text.end() != it)
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
                
                while(it != in_text.end())
                {
                    currentLineWidth += (GetCharacterWidth(in_text, it, in_font) * in_textScale);
                    Core::UTF8String::Char character = in_text.next(it);
                    
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
                    
                    //If text has no characters to break on then we need break on the previous character mid "word".
                    if(currentLineWidth >= maxLineWidth)
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
                    result.m_UVs = info.m_UVs;
                    result.m_size = info.m_size * in_textScale;
                    result.m_position.x = in_cursorX;
                    result.m_position.y = in_cursorY - (info.m_offset.y * in_textScale);
                }
                else
                {
                    CS_LOG_ERROR("Unknown character not provided by font: " + in_font->GetFilePath());
                }
                
                return result;
            }
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
		CanvasRenderer::CanvasRenderer(RenderSystem* inpRenderSystem)
        : mOverlayBatcher(inpRenderSystem)
        , mfNearClippingDistance(0.0f)
		{

		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void CanvasRenderer::Init()
        {
            m_materialFactory = Core::Application::Get()->GetSystem<MaterialFactory>();
            CS_ASSERT(m_materialFactory != nullptr, "Must have a material factory");
            
            m_resourcePool = Core::Application::Get()->GetResourcePool();
            CS_ASSERT(m_resourcePool != nullptr, "Must have a resource pool");
        }
        //----------------------------------------------------------
		//----------------------------------------------------------
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
		//----------------------------------------------------------
		/// Render
		///
		/// Draw the UI
		//----------------------------------------------------------
		void CanvasRenderer::Render(GUI::GUIView* inpView, f32 infNearClipDistance)
		{
            //We use this to ensure our UI is never clipped
            mfNearClippingDistance = infNearClipDistance + 1.0f;
            
            inpView->Draw(this);
			
            mOverlayBatcher.DisableScissoring();
			mOverlayBatcher.ForceRender();
            
            m_materialGUICache.clear();
		}
        //----------------------------------------------------------
        /// Enable Clipping To Bounds
        ///
        /// Set the bounds beyond which any subviews will clip
        /// Pushes to a stack which tracks when to enable and
        /// disable scissoring
        //---------------------------------------------------------
        void CanvasRenderer::EnableClippingToBounds(const Core::Vector2& invPosition, const Core::Vector2& invSize)
        {
            if(mScissorPos.empty())
            {
                mScissorPos.push_back(invPosition);
                mScissorSize.push_back(invSize);
            } 
            else
            {
                Core::Vector2 vOldBottomLeft = mScissorPos.back();
                Core::Vector2 vOldTopRight = mScissorSize.back()+vOldBottomLeft;
                Core::Vector2 vNewBottomLeft = invPosition;
                Core::Vector2 vNewTopRight = invPosition+invSize;
                
                vNewBottomLeft.x = Core::MathUtils::Max(vNewBottomLeft.x, vOldBottomLeft.x);
                vNewBottomLeft.y = Core::MathUtils::Max(vNewBottomLeft.y, vOldBottomLeft.y);
                
                vNewTopRight.x = Core::MathUtils::Min(vNewTopRight.x, vOldTopRight.x);
                vNewTopRight.y = Core::MathUtils::Min(vNewTopRight.y, vOldTopRight.y);
                
                Core::Vector2 vNewSize = vNewTopRight - vNewBottomLeft;
                
                mScissorPos.push_back(vNewBottomLeft);
                mScissorSize.push_back(vNewSize);
            }
            
            mOverlayBatcher.EnableScissoring(mScissorPos.back(), mScissorSize.back());
        }
        //----------------------------------------------------------
        /// Disable Clipping To Bounds
        ///
        /// Pop the scissor tracking stack
        //----------------------------------------------------------                            
        void CanvasRenderer::DisableClippingToBounds()
        {
            if(!mScissorPos.empty())
            {
                mScissorPos.erase(mScissorPos.end()-1);
                mScissorSize.erase(mScissorSize.end()-1);
                
                if(!mScissorPos.empty())
                {
                    mOverlayBatcher.EnableScissoring(mScissorPos.back(), mScissorSize.back());
                }
            }
            
            if(mScissorPos.empty())
            {
                mOverlayBatcher.DisableScissoring();
            }
        }
        //-----------------------------------------------------------
        /// Draw Box
        ///
        /// Build a sprite box and batch it ready for rendering
        //-----------------------------------------------------------
        void CanvasRenderer::DrawBox(const Core::Matrix3x3& inmatTransform, const Core::Vector2 & invSize, const TextureCSPtr & inpTexture, 
                                      const Core::Rectangle& inUVs, const Core::Colour & insTintColour, AlignmentAnchor ineAlignment)
        {
            msCachedSprite.pMaterial = GetGUIMaterialForTexture(inpTexture);
            
			UpdateSpriteData(inmatTransform, invSize, inUVs, insTintColour, ineAlignment);
            
            //Draw us!
			mOverlayBatcher.Render(msCachedSprite);
            
#ifdef CS_ENABLE_DEBUGSTATS
            Core::Application::Get()->GetDebugStats()->AddToEvent("GUI", 1);
#endif
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        std::vector<CanvasRenderer::DisplayCharacterInfo> CanvasRenderer::BuildText(const Core::UTF8String& in_text, const FontCSPtr& in_font, f32 in_textScale, f32 in_lineSpacing, const Core::Vector2& in_bounds, u32 in_numLines) const
        {
            std::vector<CanvasRenderer::DisplayCharacterInfo> result;
            result.reserve(in_text.size());
            
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
                u32 numCharacters = linesOnBounds[lineIdx].size();
                for(u32 charIdx=0; charIdx<numCharacters; ++charIdx)
                {
                    auto character = linesOnBounds[lineIdx][charIdx];
                    auto builtCharacter(BuildCharacter(character, in_font, cursorX, cursorY, in_textScale));
                    
                    cursorX += builtCharacter.m_size.x;
                    
                    if(builtCharacter.m_size.y > 0.0f)
                    {
                        //No point rendering whitespaces
                        result.push_back(builtCharacter);
                    }
                }
                
                cursorX = cursorXReturnPos;
                cursorY -= lineHeight;
            }
            //TODO: Handle justification
            
            return result;
        }
        //-----------------------------------------------------------
        /// Draw String
        //-----------------------------------------------------------
		void CanvasRenderer::DrawText(const std::vector<DisplayCharacterInfo>& in_characters, const Core::Matrix3x3& in_transform, const Core::Colour& in_colour, const TextureCSPtr& in_texture)
		{
            msCachedSprite.pMaterial = GetGUIMaterialForTexture(in_texture);
            
            Core::Matrix4x4 matTransform(in_transform);
            Core::Matrix4x4 matTransformedLocal;
			
			for (const auto& character : in_characters)
            {
				Core::Matrix4x4 matLocal;
				matLocal.Translate(character.m_position.x, character.m_position.y, 0.0f);
                
                Core::Matrix4x4::Multiply(&matLocal, &matTransform, &matTransformedLocal);
                
                UpdateSpriteData(matTransformedLocal, character.m_size, character.m_UVs, in_colour, AlignmentAnchor::k_topLeft);
				
                mOverlayBatcher.Render(msCachedSprite);
			}
            
#ifdef CS_ENABLE_DEBUGSTATS
            Core::Application::Get()->GetDebugStats()->AddToEvent("GUI", 1);
#endif
		}
		//-----------------------------------------------------
		/// Update Sprite Data
		///
		/// Rebuild the sprite data
		//-----------------------------------------------------
		void CanvasRenderer::UpdateSpriteData(const Core::Matrix4x4 & inTransform, const Core::Vector2 & invSize, const Core::Rectangle& inUVs, const Core::Colour & insTintColour, AlignmentAnchor ineAlignment)
		{
			Core::ByteColour Col = Core::ColourUtils::ColourToByteColour(insTintColour);
			
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].Col = Col;
            msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].Col = Col;
            msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].Col = Col;
            msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].Col = Col;
			
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vTex = inUVs.TopLeft();
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vTex = inUVs.BottomLeft();
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vTex = inUVs.TopRight();
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vTex = inUVs.BottomRight();
			
			Core::Vector2 vHalfSize(invSize.x * 0.5f, invSize.y * 0.5f);
			Core::Vector2 vAlignedPos;
            Align(ineAlignment, vHalfSize, vAlignedPos);
            
            Core::Vector4 vCentrePos(vAlignedPos.x, vAlignedPos.y, 0, 0);
            Core::Vector4 vTemp(-vHalfSize.x, vHalfSize.y, 0, 1.0f);
			
            const Core::Matrix4x4 &matTransform(inTransform);
			vTemp += vCentrePos;
            Core::Matrix4x4::Multiply(&vTemp, &matTransform, &msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vPos);
            
            vTemp.x = vHalfSize.x;
            vTemp.y = vHalfSize.y;
			
			vTemp += vCentrePos;
            Core::Matrix4x4::Multiply(&vTemp, &matTransform, &msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vPos);
            
            vTemp.x = -vHalfSize.x;
            vTemp.y = -vHalfSize.y;
			
			vTemp += vCentrePos;
            Core::Matrix4x4::Multiply(&vTemp, &matTransform, &msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vPos);
            
            vTemp.x = vHalfSize.x;
            vTemp.y = -vHalfSize.y;
			
			vTemp += vCentrePos;
            Core::Matrix4x4::Multiply(&vTemp, &matTransform, &msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vPos);

			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vPos.z = -mfNearClippingDistance;
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vPos.w = 1.0f;
			
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vPos.z = -mfNearClippingDistance;
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vPos.w = 1.0f;
			
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vPos.z = -mfNearClippingDistance;
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vPos.w = 1.0f;
			
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vPos.z = -mfNearClippingDistance;
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vPos.w = 1.0f;
		}
	}
}
