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

#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Base/Application.h>

#ifdef DEBUG_STATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

#define MAX_KERN_RATIO 0.25

namespace ChilliSource
{
	namespace Rendering
	{
		//----------------------------------------------------------
		/// Constructor
		///
		//----------------------------------------------------------
		CanvasRenderer::CanvasRenderer(RenderSystem* inpRenderSystem) : mOverlayBatcher(inpRenderSystem), mpRenderSystem(inpRenderSystem), mfNearClippingDistance(0.0f)
		{
            MaterialFactory* pMaterialFactory = Core::CApplication::GetSystemImplementing<MaterialFactory>();
            if(pMaterialFactory != nullptr)
            {
                mpDistanceFont = pMaterialFactory->CreateGUIDistanceFont();
                mpDistanceFontOutlined = pMaterialFactory->CreateGUIDistanceFontOutlined();
                mpDefaultMaterial = pMaterialFactory->CreateGUI();
            }
            
            msCachedSprite.pMaterial = mpDefaultMaterial;
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
			
			mOverlayBatcher.ForceRender(mpRenderSystem);
            
            if(msCachedSprite.pMaterial)
            {
                msCachedSprite.pMaterial->SetScissoringEnabled(false);
                msCachedSprite.pMaterial->SetTexture(TextureSPtr());
            }
            
            mpRenderSystem->EnableScissorTesting(false);
            
            mpDefaultMaterial->SetTexture(TextureSPtr());
            mpDistanceFont->SetTexture(TextureSPtr());
		}
        //----------------------------------------------------------
        /// Enable Clipping To Bounds
        ///
        /// Set the bounds beyond which any subviews will clip
        /// Pushes to a stack which tracks when to enable and
        /// disable scissoring
        //---------------------------------------------------------
        void CanvasRenderer::EnableClippingToBounds(const Core::CVector2& invPosition, const Core::CVector2& invSize)
        {
            mOverlayBatcher.ForceCommandChange();
            
            if(mScissorPos.empty())
            {
                //We need to force render now as previous stuff will not want to
                //be affected by this
                mpDefaultMaterial->SetScissoringEnabled(true);
                mpDistanceFont->SetScissoringEnabled(true);
                
                mScissorPos.push_back(invPosition);
                mScissorSize.push_back(invSize);
            } 
            else
            {
                Core::CVector2 vOldBottomLeft = mScissorPos.back();
                Core::CVector2 vOldTopRight = mScissorSize.back()+vOldBottomLeft;
                Core::CVector2 vNewBottomLeft = invPosition;
                Core::CVector2 vNewTopRight = invPosition+invSize;
                
                vNewBottomLeft.x = Core::CMathUtils::Max(vNewBottomLeft.x, vOldBottomLeft.x);
                vNewBottomLeft.y = Core::CMathUtils::Max(vNewBottomLeft.y, vOldBottomLeft.y);
                
                vNewTopRight.x = Core::CMathUtils::Min(vNewTopRight.x, vOldTopRight.x);
                vNewTopRight.y = Core::CMathUtils::Min(vNewTopRight.y, vOldTopRight.y);
                
                Core::CVector2 vNewSize = vNewTopRight - vNewBottomLeft;
                
                mScissorPos.push_back(vNewBottomLeft);
                mScissorSize.push_back(vNewSize);
            }
            
            mpDefaultMaterial->SetScissoringRegion(mScissorPos.back(), mScissorSize.back());
            mpDistanceFont->SetScissoringRegion(mScissorPos.back(), mScissorSize.back());
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
                    mOverlayBatcher.ForceCommandChange();
                    mpDefaultMaterial->SetScissoringRegion(mScissorPos.back(), mScissorSize.back());
                    mpDistanceFont->SetScissoringRegion(mScissorPos.back(), mScissorSize.back());
                }
            }
            
            if(mScissorPos.empty())
            {
                //We need to force render now as previous stuff will not want to
                //be affected by this
                mOverlayBatcher.ForceCommandChange();
                mpDefaultMaterial->SetScissoringEnabled(false);
                mpDistanceFont->SetScissoringEnabled(false);
            }
        }
        //-----------------------------------------------------------
        /// Draw Box
        ///
        /// Build a sprite box and batch it ready for rendering
        //-----------------------------------------------------------
        void CanvasRenderer::DrawBox(const Core::CMatrix3x3& inmatTransform, const Core::CVector2 & invSize, const TextureSPtr & inpTexture, 
                                      const Core::CRect& inUVs, const Core::CColour & insTintColour, AlignmentAnchor ineAlignment)
        {
            //Flush buffer
            if(msCachedSprite.pMaterial != mpDefaultMaterial)
                msCachedSprite.pMaterial = mpDefaultMaterial;
            
			if(inpTexture != msCachedSprite.pMaterial->GetTexture())
            {
				mOverlayBatcher.ForceCommandChange();
                msCachedSprite.pMaterial->SetTexture(inpTexture);
			}
			
            //Transformers
			
			UpdateSpriteData(inmatTransform, invSize, inUVs, insTintColour, ineAlignment);
            
            //Draw us!
			mOverlayBatcher.Render(mpRenderSystem, msCachedSprite);
            
#ifdef DEBUG_STATS
            DebugStats::AddToEvent("GUI", 1);
#endif
        }
        //-----------------------------------------------------------
        /// Draw String
        //-----------------------------------------------------------
		void CanvasRenderer::DrawString(const Core::UTF8String & insString, const Core::CMatrix3x3& inmatTransform, f32 infSize, const FontSPtr& inpFont, CharacterList& outCharCache,
                                         const Core::CColour & insColour, const Core::CVector2 & invBounds, f32 infCharacterSpacing, f32 infLineSpacing, 
										 GUI::TextJustification ineHorizontalJustification, GUI::TextJustification ineVerticalJustification, bool inbFlipVertical, GUI::TextOverflowBehaviour ineBehaviour, u32 inudwNumLines, bool * outpClipped, bool *outpInvalidCharacterFound)
		{
            //Flush buffer
            if(msCachedSprite.pMaterial != mpDefaultMaterial)
                msCachedSprite.pMaterial = mpDefaultMaterial;
            
			if(inpFont->GetTexture() != msCachedSprite.pMaterial->GetTexture())
			{
				mOverlayBatcher.ForceCommandChange();
                msCachedSprite.pMaterial->SetTexture(inpFont->GetTexture());
			}
            
			//Get the data about how to draw each character
            //This will be in text space and will be in a single line
            if(outCharCache.empty())
            {
                if(outpInvalidCharacterFound)
                    (*outpInvalidCharacterFound)=false;
                BuildString(inpFont, insString, outCharCache, infSize, infCharacterSpacing, infLineSpacing, invBounds, inudwNumLines,
							ineHorizontalJustification, ineVerticalJustification, inbFlipVertical, ineBehaviour, outpClipped,outpInvalidCharacterFound);
            }
            
            Core::CMatrix4x4 matTransform(inmatTransform);
            Core::CMatrix4x4 matTransformedLocal;
			
            //Build each character sprite from the draw info
			for (u32 nChar = 0; nChar < outCharCache.size(); nChar++)
            {
				Core::CMatrix4x4 matLocal;
                
                f32 fXPos = outCharCache[nChar].vPosition.x;
                f32 fYPos = outCharCache[nChar].vPosition.y - outCharCache[nChar].vSize.y * 0.5f;
				matLocal.Translate((fXPos), (fYPos), 0.0f);
                
                Core::CMatrix4x4::Multiply(&matLocal, &matTransform, &matTransformedLocal);
                
                UpdateSpriteData(matTransformedLocal, outCharCache[nChar].vSize, outCharCache[nChar].sUVs, insColour, AlignmentAnchor::k_middleCentre);
				
                mOverlayBatcher.Render(mpRenderSystem, msCachedSprite);
			}
            
#ifdef DEBUG_STATS
            DebugStats::AddToEvent("GUI", 1);
#endif
		}
        //-----------------------------------------------------------
        /// Draw Distance Outlined String
        //-----------------------------------------------------------
        void CanvasRenderer::DrawDistanceOutlinedString(const Core::UTF8String & insString, const Core::CMatrix3x3& inmatTransform, f32 infSize, const FontSPtr& inpFont, CharacterList& outCharCache,
                                                 const Core::CColour & insColour, const Core::CColour& insOutlineColour, const Core::CVector2 & invBounds, f32 infCharacterSpacing, f32 infLineSpacing,
                                                 GUI::TextJustification ineHorizontalJustification, GUI::TextJustification ineVerticalJustification, bool inbFlipVertical, GUI::TextOverflowBehaviour ineBehaviour, u32 inudwNumLines)
		{
            //Flush buffer
            if(msCachedSprite.pMaterial != mpDistanceFontOutlined)
                msCachedSprite.pMaterial = mpDistanceFontOutlined;
            
			if(inpFont->GetTexture() != msCachedSprite.pMaterial->GetTexture())
			{
				mOverlayBatcher.ForceCommandChange();
                msCachedSprite.pMaterial->SetTexture(inpFont->GetTexture());
			}
            
            mpDistanceFontOutlined->SetShaderColourValue("uvOutlineColour", insOutlineColour);
            DrawDistanceStringInternal(insString, inmatTransform, infSize, inpFont, outCharCache, insColour, invBounds, infCharacterSpacing, infLineSpacing,
                                       ineHorizontalJustification, ineVerticalJustification, inbFlipVertical, ineBehaviour, inudwNumLines);
        }
        //-----------------------------------------------------------
        /// Draw Distance String
        //-----------------------------------------------------------
        void CanvasRenderer::DrawDistanceString(const Core::UTF8String & insString, const Core::CMatrix3x3& inmatTransform, f32 infSize, const FontSPtr& inpFont, CharacterList& outCharCache,
                                         const Core::CColour & insColour, const Core::CVector2 & invBounds, f32 infCharacterSpacing, f32 infLineSpacing,
										 GUI::TextJustification ineHorizontalJustification, GUI::TextJustification ineVerticalJustification, bool inbFlipVertical, GUI::TextOverflowBehaviour ineBehaviour, u32 inudwNumLines)
		{
            //Flush buffer
            if(msCachedSprite.pMaterial != mpDistanceFont)
                msCachedSprite.pMaterial = mpDistanceFont;
            
			if(inpFont->GetTexture() != msCachedSprite.pMaterial->GetTexture())
			{
				mOverlayBatcher.ForceCommandChange();
                msCachedSprite.pMaterial->SetTexture(inpFont->GetTexture());
			}
			
            DrawDistanceStringInternal(insString, inmatTransform, infSize, inpFont, outCharCache, insColour, invBounds, infCharacterSpacing, infLineSpacing,
                                       ineHorizontalJustification, ineVerticalJustification, inbFlipVertical, ineBehaviour, inudwNumLines);
            
		}
        //-----------------------------------------------------------
        /// Draw Distance String Internal
        //-----------------------------------------------------------
        void CanvasRenderer::DrawDistanceStringInternal(const Core::UTF8String & insString, const Core::CMatrix3x3& inmatTransform, f32 infSize, const FontSPtr& inpFont, CharacterList& outCharCache,
                                                 const Core::CColour & insColour, const Core::CVector2 & invBounds, f32 infCharacterSpacing, f32 infLineSpacing,
                                                 GUI::TextJustification ineHorizontalJustification, GUI::TextJustification ineVerticalJustification, bool inbFlipVertical, GUI::TextOverflowBehaviour ineBehaviour, u32 inudwNumLines)
        {
            //Get the data about how to draw each character
            //This will be in text space and will be in a single line
            if(outCharCache.empty())
            {
                BuildString(inpFont, insString, outCharCache, infSize, infCharacterSpacing, infLineSpacing, invBounds, inudwNumLines,
							ineHorizontalJustification, ineVerticalJustification, inbFlipVertical, ineBehaviour);
            }
            
            Core::CMatrix4x4 matTransform(inmatTransform);
            Core::CMatrix4x4 matTransformedLocal;
			
            //Build each character sprite from the draw info
			for (u32 nChar = 0; nChar < outCharCache.size(); nChar++)
            {
                Core::CMatrix4x4 matLocal;
                
                f32 fXPos = outCharCache[nChar].vPosition.x;
                f32 fYPos = outCharCache[nChar].vPosition.y - outCharCache[nChar].vSize.y * 0.5f;
				matLocal.Translate((fXPos), (fYPos), 0.0f);
				
                Core::CMatrix4x4::Multiply(&matLocal, &matTransform, &matTransformedLocal);
                
				UpdateSpriteData(matTransformedLocal, outCharCache[nChar].vSize, outCharCache[nChar].sUVs, insColour, AlignmentAnchor::k_middleCentre);
				
                for(u32 i = 0; i <kudwVertsPerSprite; i++)
                {
                    msCachedSprite.sVerts[i].vPos.w = infSize;
                    mOverlayBatcher.Render(mpRenderSystem, msCachedSprite);
                }
            }
            
#ifdef DEBUG_STATS
            DebugStats::AddToEvent("GUI", 1);
#endif
        }
        //-----------------------------------------------------------
        /// Calculate String Width
        ///
        /// Calculate the length of a string based on the font
        /// and attributes
        //------------------------------------------------------------
        f32 CanvasRenderer::CalculateStringWidth(const Core::UTF8String& insString, const FontSPtr& inpFont, f32 infSize, f32 infCharSpacing)
        {
            Core::CVector2 vSize;
            
            //Track the character width
            f32 fWidth = 0.0f;
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
                
                // If kerning is supported, then we need the next character
                Core::UTF8String::Char NextChar = Char;
                if(inpFont->SupportsKerning() && insString.end() != it)
                {
                    Core::UTF8String::iterator it2 = it;
                    NextChar = insString.next(it2);
                }
                
                //Construct the characters position and size from the font sheet and add it to the line
                BuildCharacter(inpFont, Char, NextChar, vSize, infSize, infCharSpacing, fLastCharacterWidth, CurrentLine);
                
                fWidth += fLastCharacterWidth;
            }
            
            return fWidth;
        }
        //-----------------------------------------------------------
        /// Calculate String Height
        ///
        /// Calculate the height of a string based on the font, width
        /// and attributes
        //------------------------------------------------------------
        f32 CanvasRenderer::CalculateStringHeight(const Core::UTF8String& insString, const FontSPtr& inpFont, f32 infWidth, f32 infSize, f32 infCharSpacing, f32 infLineSpacing, u32 inudwNumLines)
        {
            Core::CVector2 vCursorPos;
            
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
                    BuildCharacter(inpFont, Char, NextChar, Core::CVector2::ZERO, infSize, infCharSpacing, fLastCharacterWidth, CurrentLine);
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
                        
                        if(insString.begin() != it && Char != kSpaceCharacter && Char != kTabCharacter)
                        {
                            it--;
                            vCursorPos.x -= fLastCharacterWidth;
                            CurrentLine.pop_back();
                        }
                    }
                    
                    //If we are a return character or we exceed the bounds then we must wrap the text
                    if(Char == kReturnCharacter || bExceededBounds)
                    {
                        udwCurrentNumLines++;  
                        vCursorPos.x = 0.0f;
                        CurrentLine.clear();
                    }
                    //Check if we need to wrap before the next space so that words are not split
                    //across multiple lines
                    else if(Char == kSpaceCharacter || Char == kTabCharacter)
                    {
                        //Find the length to the next space/tab from the cursor pos
                        //and if it exceed the bounds then wrap
                        f32 fLengthToNextSpace = vCursorPos.x;
                        Core::UTF8String::iterator jt = it;
                        Core::UTF8String::Char NextChar = 0;
                        
                        while(jt != insString.end() && NextChar != kSpaceCharacter && NextChar != kTabCharacter && NextChar != kReturnCharacter)
                        {
                            NextChar = insString.next(jt);
                            
                            //Add it to the length
                            Font::CharacterInfo sInfo;
                            inpFont->GetInfoForCharacter(NextChar, sInfo);
                            fLengthToNextSpace += (sInfo.vSize.x * infSize) + infCharSpacing;
                            
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
		void CanvasRenderer::BuildString(const FontSPtr& inpFont, const Core::UTF8String &inText, CharacterList &outCharacters, f32 infTextSize, f32 infCharacterSpacing, f32 infLineSpacing,
										  const Core::CVector2& invBounds, u32 inudwNumLines, GUI::TextJustification ineHorizontalJustification, GUI::TextJustification ineVerticalJustification,
                                          bool inbFlipVertical, GUI::TextOverflowBehaviour ineBehaviour, bool * outpClipped, bool * outpInvalidCharacterFound)
		{
            bool bClipped=false;
            
            outCharacters.reserve(inText.length());
			
			//This will be positioned in local space where the characters are relative to each either.
            Core::CVector2 vCursorPos;
			
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
                        
                        if(inText.begin() != it && Char != kSpaceCharacter && Char != kTabCharacter)
                        {
                            it--;
                            vCursorPos.x -= fLastCharacterWidth;
                            CurrentLine.pop_back();
                        }
                    }
                    
                    //If we are a return character or we exceed the bounds then we must wrap the text
                    if(Char == kReturnCharacter || bExceededBounds)
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
                    else if(Char == kSpaceCharacter || Char == kTabCharacter)
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
                            if(LookAheadChar == kSpaceCharacter || LookAheadChar == kTabCharacter || LookAheadChar == kReturnCharacter)
                                break;
                            
                            LookAheadNextChar = LookAheadChar;
                            Core::UTF8String::iterator jt2 = jt;
                            if(jt!=inText.end())
                                LookAheadNextChar=inText.next(jt2);
                            
                            //Construct the characters position and size from the font sheet to get the width
                            BuildCharacter(inpFont, LookAheadChar, LookAheadNextChar, Core::CVector2::ZERO, infTextSize, infCharacterSpacing, fLastCharacterWidth, CurrentLineTemp, outpInvalidCharacterFound);
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
                for(CharacterList::iterator it = outCharacters.begin(); it != outCharacters.end(); ++it)
                {
                    it->vPosition.y = -it->vPosition.y + it->vSize.y - fHeight;
                    it->sUVs.vOrigin.y += it->sUVs.vSize.y;
                    it->sUVs.vSize.y = -it->sUVs.vSize.y;
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

			for(CharacterList::iterator it = outCharacters.begin(); it != outCharacters.end(); ++it)
			{
                it->vPosition.y += fOffsetY;
			}
            
            if(outpClipped)
                (*outpClipped)=bClipped;
		}
		//----------------------------------------------------
		/// Build Character
		//----------------------------------------------------
		CharacterResult CanvasRenderer::BuildCharacter(const FontSPtr& inpFont, Core::UTF8String::Char inCharacter, Core::UTF8String::Char inNextCharacter,
                                                         const Core::CVector2& invCursor, f32 infTextScale, f32 infCharSpacing,
                                                         f32 &outfCharacterWidth, CharacterList &outCharacters, bool * outpInvalidCharacterFound)
		{
			Font::CharacterInfo sInfo;
			CharacterResult Result = inpFont->GetInfoForCharacter(inCharacter, sInfo);
            
            sInfo.vSize *= infTextScale;
            sInfo.vOffset *= infTextScale;
			
			switch(Result)
			{
				case CharacterResult::k_ok:
				{
					PlacedCharacter sOutCharacter;
					sOutCharacter.sUVs = sInfo.sUVs;
					sOutCharacter.vSize = sInfo.vSize;
					sOutCharacter.vPosition.x = invCursor.x + sInfo.vOffset.x;
                    sOutCharacter.vPosition.y = invCursor.y - sInfo.vOffset.y;
					
                    f32 fCharWidth = sInfo.vSize.x + infCharSpacing;
                    
                    if(inpFont->SupportsKerning() && fCharWidth>2)
                    {
                        f32 fKernAmount = (inpFont->GetKerningBetweenCharacters(inCharacter, inNextCharacter) * infTextScale);
                        
                        if(fKernAmount > (fCharWidth * MAX_KERN_RATIO))
                        {
                            fKernAmount = fCharWidth * MAX_KERN_RATIO;
                        }
                        
                        fCharWidth -= fKernAmount;
                    }
                    
					outfCharacterWidth = fCharWidth;
					outCharacters.push_back(sOutCharacter);
					break;
				}
				case CharacterResult::k_space:
				{
					//No sprite just space. The cursor will increment anyway
					outfCharacterWidth = inpFont->GetAttributes().SpaceSpacing * infTextScale;
					break;
				}
				case CharacterResult::k_tab:
				{
					outfCharacterWidth = inpFont->GetAttributes().TabSpacing * infTextScale;
					break;
				}
				case CharacterResult::k_return:
				{
					outfCharacterWidth = 0.0f;
					break;
				}
				case CharacterResult::k_invalid:
				default:
				{
					outfCharacterWidth = 0.0f;
                    if(outpInvalidCharacterFound)
                        (*outpInvalidCharacterFound)=true;
					CS_ERROR_LOG("Invalid character in text component");
					break;
				}
			}
			
			return Result;
		}
		
        //----------------------------------------------------
        /// Wrap
        //----------------------------------------------------
        void CanvasRenderer::Wrap(GUI::TextJustification ineHorizontalJustification, f32 infLineSpacing, const Core::CVector2& invBounds,
								   CharacterList &inCurrentLine, Core::CVector2& outvCursor, CharacterList &outCharacters)
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
                outvCursor.y -= infLineSpacing;
            }
            
            outvCursor.x = 0.0f;
		}
		//-----------------------------------------------------
		/// Update Sprite Data
		///
		/// Rebuild the sprite data
		//-----------------------------------------------------
		void CanvasRenderer::UpdateSpriteData(const Core::CMatrix4x4 & inTransform, const Core::CVector2 & invSize, const Core::Rectangle& inUVs, const Core::CColour & insTintColour, AlignmentAnchor ineAlignment)
		{
			Core::CColour::ByteColour Col = Core::CColour::ColourToByteColour(insTintColour);
			
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].Col = Col;
            msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].Col = Col;
            msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].Col = Col;
            msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].Col = Col;
			
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vTex = inUVs.TopLeft();
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vTex = inUVs.BottomLeft();
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vTex = inUVs.TopRight();
			msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vTex = inUVs.BottomRight();
			
			Core::CVector2 vHalfSize(invSize.x * 0.5f, invSize.y * 0.5f);
			Core::CVector2 vAlignedPos;
            Align(ineAlignment, vHalfSize, vAlignedPos);
            
            Core::CVector4 vCentrePos(vAlignedPos.x, vAlignedPos.y, 0, 0);
            Core::CVector4 vTemp(-vHalfSize.x, vHalfSize.y, 0, 1.0f);
			
            const Core::CMatrix4x4 &matTransform(inTransform);
			vTemp += vCentrePos;
            Core::CMatrix4x4::Multiply(&vTemp, &matTransform, &msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vPos);
            
            vTemp.x = vHalfSize.x;
            vTemp.y = vHalfSize.y;
			
			vTemp += vCentrePos;
            Core::CMatrix4x4::Multiply(&vTemp, &matTransform, &msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_topRight].vPos);
            
            vTemp.x = -vHalfSize.x;
            vTemp.y = -vHalfSize.y;
			
			vTemp += vCentrePos;
            Core::CMatrix4x4::Multiply(&vTemp, &matTransform, &msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vPos);
            
            vTemp.x = vHalfSize.x;
            vTemp.y = -vHalfSize.y;
			
			vTemp += vCentrePos;
            Core::CMatrix4x4::Multiply(&vTemp, &matTransform, &msCachedSprite.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vPos);

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
