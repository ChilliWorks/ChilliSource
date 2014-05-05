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
        //-----------------------------------------------------------
        /// Draw String
        //-----------------------------------------------------------
		void CanvasRenderer::DrawString(const Core::UTF8String & insString, const Core::Matrix3x3& inmatTransform, f32 infSize, const FontCSPtr& inpFont, CharacterList& outCharCache,
                                         const Core::Colour & insColour, const Core::Vector2 & invBounds, f32 infCharacterSpacing, f32 infLineSpacing, 
										 GUI::TextJustification ineHorizontalJustification, GUI::TextJustification ineVerticalJustification, bool inbFlipVertical, GUI::TextOverflowBehaviour ineBehaviour, u32 inudwNumLines, bool * outpClipped, bool *outpInvalidCharacterFound)
		{
            msCachedSprite.pMaterial = GetGUIMaterialForTexture(inpFont->GetTexture());
            
			//Get the data about how to draw each character
            //This will be in text space and will be in a single line
            if(outCharCache.empty())
            {
                if(outpInvalidCharacterFound)
                    (*outpInvalidCharacterFound)=false;
                BuildString(inpFont, insString, outCharCache, infSize, infCharacterSpacing, infLineSpacing, invBounds, inudwNumLines,
							ineHorizontalJustification, ineVerticalJustification, inbFlipVertical, ineBehaviour, outpClipped,outpInvalidCharacterFound);
            }
            
            Core::Matrix4x4 matTransform(inmatTransform);
            Core::Matrix4x4 matTransformedLocal;
			
            //Build each character sprite from the draw info
			for (u32 nChar = 0; nChar < outCharCache.size(); nChar++)
            {
				Core::Matrix4x4 matLocal;
                
                f32 fXPos = outCharCache[nChar].vPosition.x;
                f32 fYPos = outCharCache[nChar].vPosition.y - outCharCache[nChar].vSize.y * 0.5f;
				matLocal.Translate((fXPos), (fYPos), 0.0f);
                
                Core::Matrix4x4::Multiply(&matLocal, &matTransform, &matTransformedLocal);
                
                UpdateSpriteData(matTransformedLocal, outCharCache[nChar].vSize, outCharCache[nChar].sUVs, insColour, AlignmentAnchor::k_middleCentre);
				
                mOverlayBatcher.Render(msCachedSprite);
			}
            
#ifdef CS_ENABLE_DEBUGSTATS
            Core::Application::Get()->GetDebugStats()->AddToEvent("GUI", 1);
#endif
		}
        //-----------------------------------------------------------
        /// Calculate String Width
        ///
        /// Calculate the length of a string based on the font
        /// and attributes
        //------------------------------------------------------------
        f32 CanvasRenderer::CalculateStringWidth(const Core::UTF8String& insString, const FontCSPtr& inpFont, f32 infSize, f32 infCharSpacing, bool inbIgnoreLinesBreaks)
        {
            Core::Vector2 vSize;
            
            //Track the character width
            std::vector<f32> aLineWidths;
            aLineWidths.push_back(0.0f);
            
			f32 fLastCharacterWidth = 0.0f;
            
            //Make sure we scale the spacing if we have scaled the text
            infCharSpacing *= infSize;
			
            //Track all the characters in this line
            CharacterList CurrentLine;
            
            //Loop round each character and get it's size
            Core::UTF8String::iterator it = (Core::UTF8String::iterator)insString.begin();
			while(insString.end() != it)
			{
                //Get character for iterator codepoint and bump the iterator to the beginning of
                //the next character
                Core::UTF8String::Char Char = insString.next(it);
                
                // If we are breaking on new lines, create a new counter.
                if(!inbIgnoreLinesBreaks && (Char == k_returnCharacter))
                {
                    aLineWidths.push_back(0.0f);
                    continue;
                }
                
                // If kerning is supported, then we need the next character
                Core::UTF8String::Char NextChar = Char;
                if(inpFont->SupportsKerning() && insString.end() != it)
                {
                    Core::UTF8String::iterator it2 = it;
                    NextChar = insString.next(it2);
                }
                
                //Construct the characters position and size from the font sheet and add it to the line
                BuildCharacter(inpFont, Char, NextChar, vSize, infSize, infCharSpacing, fLastCharacterWidth, CurrentLine);
                
                aLineWidths.back() += fLastCharacterWidth;
            }
            
            // Return the largest line.
            return *std::max_element(aLineWidths.begin(), aLineWidths.end());
        }
        //-----------------------------------------------------------
        /// Calculate String Height
        ///
        /// Calculate the height of a string based on the font, width
        /// and attributes
        //------------------------------------------------------------
        f32 CanvasRenderer::CalculateStringHeight(const Core::UTF8String& insString, const FontCSPtr& inpFont, f32 infWidth, f32 infSize, f32 infCharSpacing, f32 infLineSpacing, u32 inudwNumLines)
        {
            Core::Vector2 vCursorPos;
            
            //Track the character height
			f32 fLastCharacterWidth = 0.0f;
            
            //Required to be passed into BuildCharacter
            CharacterList CurrentLine;
            
            //Make sure we scale the spacing if we have scaled the text
            infLineSpacing *= infSize;
            infCharSpacing *= infSize;
            const f32 fLineHeight = inpFont->GetLineHeight() * infLineSpacing;
        
            u32 udwCurrentNumLines = 1;

            //Loop round each character and get it's size
            Core::UTF8String::iterator it = (Core::UTF8String::iterator)insString.begin();
			while(it != insString.end())
			{
                //Get character for iterator codepoint and bump the iterator to the beginning of 
                //the next character
                Core::UTF8String::Char Char = insString.next(it);
                
                //Decide whether to wrap or clip. If max num lines is zero this means wrap text infinetly
                if(inudwNumLines == 0 || udwCurrentNumLines <= inudwNumLines)
                {
                    // If kerning is supported, then we need the next character
                    Core::UTF8String::Char NextChar = Char;
                    if(inpFont->SupportsKerning() && insString.end() != it)
                    {
                        Core::UTF8String::iterator it2 = it;
                        NextChar = insString.next(it2);
                    }
                    
                    //Construct the characters position and size from the font sheet to get the width
                    BuildCharacter(inpFont, Char, NextChar, Core::Vector2::ZERO, infSize, infCharSpacing, fLastCharacterWidth, CurrentLine);
                    vCursorPos.x += fLastCharacterWidth;

                    Core::UTF8String sTemp;
                    sTemp.appendChar(Char);
                    
                    //Added by Joe 9/1/14
                    //Prepare to relocate last character to the next line if it breaches the bounds
                    //by removing it from the current line and stepping back the iterator
                    bool bExceededBounds = false;
                    if(vCursorPos.x > infWidth)
                    {
                        bExceededBounds=true;
                        
                        if(CurrentLine.size()==1)
                        {
                            // this character won't ever fit in this label so stop here.
                            return udwCurrentNumLines * fLineHeight;
                        }
                        
                        if(insString.begin() != it && Char != k_spaceCharacter && Char != k_tabCharacter)
                        {
                            it--;
                            vCursorPos.x -= fLastCharacterWidth;
                            CurrentLine.pop_back();
                        }
                    }
                    
                    //If we are a return character or we exceed the bounds then we must wrap the text
                    if(Char == k_returnCharacter || bExceededBounds)
                    {
                        udwCurrentNumLines++;  
                        vCursorPos.x = 0.0f;
                        CurrentLine.clear();
                    }
                    //Check if we need to wrap before the next space so that words are not split
                    //across multiple lines
                    else if(Char == k_spaceCharacter || Char == k_tabCharacter)
                    {
                        //Find the length to the next space/tab from the cursor pos
                        //and if it exceed the bounds then wrap
                        f32 fLengthToNextSpace = vCursorPos.x;
                        Core::UTF8String::iterator jt = it;
                        Core::UTF8String::Char NextCharacter = 0;
                        
                        while(jt != insString.end() && NextCharacter != k_spaceCharacter && NextCharacter != k_tabCharacter && NextCharacter != k_returnCharacter)
                        {
                            NextCharacter = insString.next(jt);
                            
                            //Add it to the length
                            Font::CharacterInfo sInfo;
                            inpFont->TryGetCharacterInfo(NextCharacter, sInfo);
                            fLengthToNextSpace += (sInfo.m_size.x * infSize) + infCharSpacing;
                            
                            if(fLengthToNextSpace > infWidth)
                            {
                                //We can wrap to the next line
                                udwCurrentNumLines++;
                                vCursorPos.x = 0.0f;
                                CurrentLine.clear();
                                break;
                            }
                        }
                    }
                }
            }
            
            return udwCurrentNumLines * fLineHeight;
        }
		//-------------------------------------------
		/// Build String
		///
		/// Construct a list of character sprites
		/// from the given string
        //-------------------------------------------
		void CanvasRenderer::BuildString(const FontCSPtr& inpFont, const Core::UTF8String &inText, CharacterList &outCharacters, f32 infTextSize, f32 infCharacterSpacing, f32 infLineSpacing,
										  const Core::Vector2& invBounds, u32 inudwNumLines, GUI::TextJustification ineHorizontalJustification, GUI::TextJustification ineVerticalJustification,
                                          bool inbFlipVertical, GUI::TextOverflowBehaviour ineBehaviour, bool * outpClipped, bool * outpInvalidCharacterFound)
		{
            // don't attempt to draw zero or negative sized text
            if(infTextSize <= 0.0f)
                return;
            
            bool bClipped=false;
            
            outCharacters.reserve(inText.length());
			
			//This will be positioned in local space where the characters are relative to each either.
            Core::Vector2 vCursorPos;
			
			//Track the width of the last character so we can offset the next correctly
			f32 fLastCharacterWidth = 0.0f;
            
            //Make sure we scale the spacing if we have scaled the text
            infCharacterSpacing *= infTextSize;
			infLineSpacing *= infTextSize;
            const f32 fLineHeight = infLineSpacing * inpFont->GetLineHeight();
            
            CharacterList CurrentLine;
            u32 udwCurrentNumLines = 1;
            
            bool bNoMoreLines = false;
			
            Core::UTF8String::iterator it = (Core::UTF8String::iterator)inText.begin();
            while(inText.end() != it)
			{
                //Get character for iterator codepoint and bump the iterator to the beginning of
                //the next character
                Core::UTF8String::Char Char = inText.next(it);
                
                // If kerning is supported, then we need the next character
                Core::UTF8String::Char NextChar = Char;
                if(inpFont->SupportsKerning() && inText.end() != it)
                {
                    Core::UTF8String::iterator it2 = it;
                    NextChar = inText.next(it2);
                }
                
                //Decide whether to wrap or clip. If max num lines is zero this means wrap text infinetly
                if(!bNoMoreLines && (inudwNumLines == 0 || udwCurrentNumLines <= inudwNumLines))
                {
                    //Construct the characters position and size from the font sheet and add it to the line
                    BuildCharacter(inpFont, Char, NextChar, vCursorPos, infTextSize, infCharacterSpacing, fLastCharacterWidth, CurrentLine, outpInvalidCharacterFound);
                    
                    //Offset the cursor to the start of the next character
                    vCursorPos.x += fLastCharacterWidth;
                    
                    //Added by Joe 9/1/14
                    //Prepare to relocate last character to the next line if it breaches the bounds
                    //by removing it from the current line and stepping back the iterator
                    bool bExceededBounds = false;
                    if(vCursorPos.x > invBounds.x)
                    {
                        bExceededBounds=true;
                        
                        if(CurrentLine.size()==1)
                        {
                            // don't add any more lines because the width of the bounds is too
                            // small to allow one of the characters to appear at all.
                            bNoMoreLines=true;
                        }
                        
                        else if(inText.begin() != it && Char != k_spaceCharacter && Char != k_tabCharacter)
                        {
                            it--;
                            vCursorPos.x -= fLastCharacterWidth;
                            CurrentLine.pop_back();
                        }
                    }
                    
                    if(!bNoMoreLines)
                    {
                        //If we are a return character or we exceed the bounds then we must wrap the text
                        if(Char == k_returnCharacter || bExceededBounds)
                        {
                            Wrap(ineHorizontalJustification, fLineHeight, invBounds, CurrentLine, vCursorPos, outCharacters);
                            
                            //Make sure we don't exceed our vertical bounds
                            if(vCursorPos.y - fLineHeight <= -invBounds.y || udwCurrentNumLines==inudwNumLines)
                            {
                                bNoMoreLines = true;
                                vCursorPos.y += fLineHeight;
                            }
                            else
                            {
                                udwCurrentNumLines++;
                            }
                        }
                        //Check if we need to wrap before the next space so that words are not split
                        //across multiple lines
                        else if(Char == k_spaceCharacter || Char == k_tabCharacter)
                        {
                            //Find the length to the next space/tab from the cursor pos
                            //and if it exceed the bounds then wrap
                            f32 fLengthToNextSpace = vCursorPos.x;
                            Core::UTF8String::iterator jt = it;
                            Core::UTF8String::Char LookAheadChar;
                            Core::UTF8String::Char LookAheadNextChar;
                            CharacterList CurrentLineTemp;
                            
                            //This while loop exits through break statements only.
                            while(jt != inText.end())
                            {
                                LookAheadChar = inText.next(jt);
                                
                                //Break if the next spacing or return character is reached
                                if(LookAheadChar == k_spaceCharacter || LookAheadChar == k_tabCharacter || LookAheadChar == k_returnCharacter)
                                    break;
                                
                                LookAheadNextChar = LookAheadChar;
                                Core::UTF8String::iterator jt2 = jt;
                                if(jt!=inText.end())
                                    LookAheadNextChar=inText.next(jt2);
                                
                                //Construct the characters position and size from the font sheet to get the width
                                BuildCharacter(inpFont, LookAheadChar, LookAheadNextChar, Core::Vector2::ZERO, infTextSize, infCharacterSpacing, fLastCharacterWidth, CurrentLineTemp, outpInvalidCharacterFound);
                                fLengthToNextSpace += fLastCharacterWidth;
                                
                                if(fLengthToNextSpace > invBounds.x)
                                {
                                    //We can wrap to the next line
                                    Wrap(ineHorizontalJustification, fLineHeight, invBounds, CurrentLine, vCursorPos, outCharacters);
                                    
                                    //Make sure we don't exceed our vertical bounds
                                    if(vCursorPos.y - fLineHeight <= -invBounds.y || udwCurrentNumLines==inudwNumLines)
                                    {
                                        bNoMoreLines = true;
                                        vCursorPos.y += fLineHeight;
                                    }
                                    else
                                    {
                                        udwCurrentNumLines++;
                                    }
                                    
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    //We are out of room so we can either over-run the label or clip the text
                    switch(ineBehaviour)
                    {
                        case GUI::TextOverflowBehaviour::k_none:
                        case GUI::TextOverflowBehaviour::k_clip:
                            //Don't process any further characters
                            it = (Core::UTF8String::iterator)inText.end();
                            bClipped=true;
                            break;
                        case GUI::TextOverflowBehaviour::k_follow:
                            //Shunt the text backwards so it appears to scroll
                            ineHorizontalJustification = GUI::TextJustification::k_right;
                            break;
                    }
                }
			}
            
            //Add the last line to the string
            Wrap(ineHorizontalJustification, fLineHeight, invBounds, CurrentLine, vCursorPos, outCharacters);
            
            //flip the text vertically if required
            f32 fHeight = udwCurrentNumLines * fLineHeight;
            if (inbFlipVertical == true)
            {
                for(CharacterList::iterator itChar = outCharacters.begin(); itChar != outCharacters.end(); ++itChar)
                {
                    itChar->vPosition.y = -itChar->vPosition.y + itChar->vSize.y - fHeight;
                    itChar->sUVs.vOrigin.y += itChar->sUVs.vSize.y;
                    itChar->sUVs.vSize.y = -itChar->sUVs.vSize.y;
                }
            }
            
            //Now vertically justify the text
            f32 fOffsetY = 0.0f;
            
            switch(ineVerticalJustification)
            {
                case GUI::TextJustification::k_top:
                default:
                    fOffsetY = (invBounds.y * 0.5f);
                    break;
                case GUI::TextJustification::k_centre:
                    fOffsetY = (fHeight * 0.5f);
                    break;
                case GUI::TextJustification::k_bottom:
                    fOffsetY = -((invBounds.y * 0.5f) - fHeight);
                    break;
            };
            
			for(CharacterList::iterator itChar = outCharacters.begin(); itChar != outCharacters.end(); ++itChar)
			{
                itChar->vPosition.y += fOffsetY;
			}
            
            if(outpClipped)
                (*outpClipped)=bClipped;
		}
		//----------------------------------------------------
		/// Build Character
		//----------------------------------------------------
		void CanvasRenderer::BuildCharacter(const FontCSPtr& inpFont, Core::UTF8String::Char inCharacter, Core::UTF8String::Char inNextCharacter,
                                                         const Core::Vector2& invCursor, f32 infTextScale, f32 infCharSpacing,
                                                         f32 &outfCharacterWidth, CharacterList &outCharacters, bool * outpInvalidCharacterFound)
		{
			Font::CharacterInfo sInfo;
            if(inpFont->TryGetCharacterInfo(inCharacter, sInfo) == false)
            {
                outfCharacterWidth = 0.0f;
                if(outpInvalidCharacterFound)
                    (*outpInvalidCharacterFound)=true;
                CS_LOG_ERROR("Invalid character in text component");
                return;
            }

            sInfo.m_size *= infTextScale;
            sInfo.m_offset *= infTextScale;
            
            f32 fCharWidth = sInfo.m_size.x + infCharSpacing;
            
            if(sInfo.m_size.y > 0.0f)
            {
                PlacedCharacter sOutCharacter;
                sOutCharacter.sUVs = sInfo.m_UVs;
                sOutCharacter.vSize = sInfo.m_size;
                sOutCharacter.vPosition.x = invCursor.x + sInfo.m_offset.x;
                sOutCharacter.vPosition.y = invCursor.y - sInfo.m_offset.y;
                
                if(inpFont->SupportsKerning() && fCharWidth > 2)
                {
                    f32 fKernAmount = (inpFont->GetKerningBetweenCharacters(inCharacter, inNextCharacter) * infTextScale);
                    
                    if(fKernAmount > (fCharWidth * k_maxKernRatio))
                    {
                        fKernAmount = fCharWidth * k_maxKernRatio;
                    }
                    
                    fCharWidth -= fKernAmount;
                }
                
                outCharacters.push_back(sOutCharacter);
            }
            
            outfCharacterWidth = fCharWidth;
        }
		
        //----------------------------------------------------
        /// Wrap
        //----------------------------------------------------
        void CanvasRenderer::Wrap(GUI::TextJustification ineHorizontalJustification, f32 infLineSpacing, const Core::Vector2& invBounds,
								   CharacterList &inCurrentLine, Core::Vector2& outvCursor, CharacterList &outCharacters)
        {
            if(!inCurrentLine.empty())
            {
                //Move the characters based on the justification
                //We must centre align everything for rendering
                f32 fOffsetX = 0.0f;
                
                switch(ineHorizontalJustification)
                {
                    case GUI::TextJustification::k_left:
                    default:
                        fOffsetX = -(invBounds.x * 0.5f);
                        break;
                    case GUI::TextJustification::k_centre:
                        fOffsetX = -(outvCursor.x * 0.5f);
                        break;
                    case GUI::TextJustification::k_right:
                        fOffsetX = (invBounds.x * 0.5f) - outvCursor.x;
                        break;
                };
                
                for(CharacterList::iterator it = inCurrentLine.begin(); it != inCurrentLine.end(); ++it)
                {
                    it->vPosition.x += fOffsetX;
                    outCharacters.push_back(*it);
                }
                
                inCurrentLine.clear();
            }
            
            outvCursor.y -= infLineSpacing;
            outvCursor.x = 0.0f;
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
