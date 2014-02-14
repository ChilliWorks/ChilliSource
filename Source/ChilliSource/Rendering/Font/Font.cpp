//
//  Font.cpp
//  MoFlow
//
//  Created by Scott Downie on 26/10/2010.
//  Copyright (c) 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Font/Font.h>

#include <ChilliSource/Core/String/UTF8String.h>
#include <ChilliSource/Core/Math/MathUtils.h>

#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>

namespace moFlo
{
	namespace Rendering
	{
        
        f32 CFont::mfGlobalKerningOffset = 0.0f;
        
        DEFINE_NAMED_INTERFACE(CFont);
        
		//---------------------------------------------------------------------
		/// Is A
		///
		/// @return Whether this object is of given type
		//---------------------------------------------------------------------
		bool CFont::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CFont::InterfaceID;
		}
		//-------------------------------------------
		/// Set Character Set
		///
		/// The list of characters that this font
		/// can display
		/// @param An array of the characters
		//-------------------------------------------
		void CFont::SetCharacterSet(const CharacterSet &inSet)
		{
			mCharacterSet = inSet;
		}
		//-------------------------------------------
		/// Set Texture
		///
		/// @param Texture containing the font 
		//-------------------------------------------
		void CFont::SetTexture(const Rendering::TexturePtr& inpTex)
		{
			mpTexture = inpTex;
			mpTexture->SetFilter(Rendering::ITexture::TEX_FILTER_LINEAR, Rendering::ITexture::TEX_FILTER_LINEAR);
		}
		//-------------------------------------------
		/// Get Texture
		///
		/// @return Font texture 
		//-------------------------------------------
		const Rendering::TexturePtr& CFont::GetTexture() const
		{
			return mpTexture;
		}
		//-------------------------------------------
		/// Get Attributes
		///
		/// @return Attributes - Spacing etc
		//-------------------------------------------
		const FontAttributes& CFont::GetAttributes() const
		{
			return mAttributes;
		}
		//-------------------------------------------
		/// Get Mode Character Height
		///
		/// @return Mode height
		//-------------------------------------------
		f32 CFont::GetLineHeight() const
		{
			return mfLineHeight;
        }
        //-------------------------------------------
        /// Supports Kerning
        ///
        /// @return True is kerning supported, false otherwise
        //-------------------------------------------
        const bool CFont::SupportsKerning() const
        {
            return (!maFirstLookup.empty() && !maPairs.empty());
        }
		//-------------------------------------------
		/// Set Character Data
		///
		/// Each character is a sprite on a tpage
		/// therefore we need the UV offsets
		///
		/// @param Sprite data containing UV's etc
		//-------------------------------------------
		void CFont::SetSpriteSheet(const Rendering::SpriteSheetPtr& inpData)
		{
			mpCharacterData = inpData;
			mpTexture = inpData->GetTexture();
			
			mMapCharToCharInfo.clear();
			
            const f32 fSpriteSheetWidth = mpCharacterData->GetSpriteSheetWidth();
            const f32 fSpriteSheetHeight = mpCharacterData->GetSpriteSheetHeight();

			for (u32 nChar = 0; nChar < mCharacterSet.length(); nChar++) 
            {
				CharacterInfo sCI;
				
				Rendering::CSpriteSheet::Frame sFrame = mpCharacterData->GetSpriteFrameByID(nChar);
				
				//Get the UV co-ordinates of the character on the font tpage
				mpCharacterData->GetUVsForFrame(nChar, sCI.sUVs);
								
				sCI.sUVs.vOrigin.x = (f32)(sFrame.U - 0.5f) / fSpriteSheetWidth;
				sCI.sUVs.vOrigin.y = (f32)(sFrame.V - 0.5f) / fSpriteSheetHeight;
				sCI.sUVs.vSize.x = (f32)(sFrame.Width + 1.0f) / fSpriteSheetWidth;
				sCI.sUVs.vSize.y = (f32)(sFrame.Height + 1.0f) / fSpriteSheetHeight;
				
				sCI.vSize.x = sFrame.Width;
                sCI.vSize.y = sFrame.Height;
                
				sCI.vOffset.x = sCI.vSize.x * 0.5f;
				sCI.vOffset.y = (sFrame.OffsetY);
                
                mfLineHeight = std::max((f32)sFrame.OriginalHeight, mfLineHeight);

                UTF8String::Char Char = mCharacterSet[nChar];
				mMapCharToCharInfo.insert(std::make_pair(Char, sCI));
			}
		
			
			//Just assign the width of a space based on the similar space character in the 
			//font. This means it will scale relative to the font
			CharacterInfo sInfo;
			if(GetInfoForCharacter(kSimilarSpaceCharacter, sInfo) == CHAR_OK)
			{
				mAttributes.SpaceSpacing = sInfo.vSize.x;
				mAttributes.TabSpacing = sInfo.vSize.x * kudwSpacesPerTab;
			}
			else
			{
				ERROR_LOG("Invalid space character chosen in font please include a '-' in the font");
			}
            
        }
		//-------------------------------------------
		/// Get Frame By Character
		///
		/// Get the tpage data for the given char
		/// @param Wide char
		/// @param Frame to be filled with data 
		/// @return Success or invisible chars
		//-------------------------------------------
		CharacterResults CFont::GetInfoForCharacter(UTF8String::Char inChar, CharacterInfo& outInfo) const
		{
			//Check for space character. That won't be in the glyph file or the font bitmap.
			if(inChar == kSpaceCharacter)
			{
				return CHAR_SPACE;
			}
			else if(inChar == kTabCharacter)
			{
				return CHAR_TAB;
			}
			else if(inChar == kReturnCharacter)
			{
				return CHAR_RETURN;
			}
			
			CharToCharInfoMap::const_iterator pCharEntry = mMapCharToCharInfo.find(inChar);
			
			if (pCharEntry != mMapCharToCharInfo.end()) 
            {
				outInfo = pCharEntry->second;
				
				return CHAR_OK;
			}
			
			return CHAR_INVALID;
		}
        //-------------------------------------------
        /// Get Kerning Between Characters
        //-------------------------------------------
        f32 CFont::GetKerningBetweenCharacters(UTF8String::Char inChar1, UTF8String::Char inChar2) const
        {
            const CKernLookup* pLookup = &(*std::lower_bound(maFirstLookup.begin(), maFirstLookup.end(), CKernLookup(inChar1, 0, 0)));
			if(NULL == pLookup || pLookup->wCharacter != inChar1)
            {
				return mfGlobalKerningOffset;
            }
			
            u32 udwStart = pLookup->uwStart;
			u32 udwEnd = udwStart + pLookup->uwLength;
			const CKernPair* pPair = &(*std::lower_bound(maPairs.begin() + udwStart, maPairs.begin() + udwEnd, CKernPair(inChar2, 0.0f)));
			if(NULL == pPair || pPair->wCharacter != inChar2)
            {
				return mfGlobalKerningOffset;
			}

            return pPair->fSpacing + mfGlobalKerningOffset;
        }
        //-------------------------------------------
        /// Set Kerning Info
        //-------------------------------------------
        void CFont::SetKerningInfo(const DYNAMIC_ARRAY<CKernLookup>& inaFirstReg, const DYNAMIC_ARRAY<CKernPair>& inaPairs)
        {
            maFirstLookup = inaFirstReg;
            maPairs = inaPairs;
        }
        //-------------------------------------------
        /// Set Global Kerning Offset
        //-------------------------------------------
        void CFont::SetGlobalKerningOffset(f32 infOffset)
        {
            mfGlobalKerningOffset = infOffset;
        }
	}
}
