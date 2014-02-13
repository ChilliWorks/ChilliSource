//
//  Font.h
//  MoFlow
//
//  Created by Scott Downie on 26/10/2010.
//  Copyright (c) 2010 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_RENDERING_FONT_H_
#define _MO_FLO_RENDERING_FONT_H_

#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/Rendering/SpriteSheet.h>

#include <ChilliSource/Core/Math/Shapes.h>
#include <ChilliSource/Core/Resource.h>

#include <ChilliSource/Core/Containers/UTF8String.h>

namespace moFlo
{
	namespace Rendering
	{
		//---Font Types
		typedef u32 FontAlignment;
		typedef f32 FontSpacing;
		
		typedef UTF8String CharacterSet;
		
		const UTF8String::Char kSimilarSpaceCharacter = '-';
		const UTF8String::Char kReturnCharacter = '\n';
		const UTF8String::Char kTabCharacter = '\t';
        const UTF8String::Char kSpaceCharacter = ' ';
        
        const u32 kudwSpacesPerTab = 5;
		
		//---Font Attributes
		struct FontAttributes
		{
			FontAttributes() : TabSpacing(10.0f), SpaceSpacing(6.0f){}
			
			FontSpacing TabSpacing;
            FontSpacing SpaceSpacing;
		};
		
		//---Font Values
		enum CharacterResults
		{
			CHAR_SPACE,
			CHAR_TAB,
			CHAR_RETURN,
			CHAR_INVALID,
			CHAR_OK,
		};
		
		//Details how a particular character must be drawn
		struct PlacedCharacter
		{
			Core::CRect sUVs;
			Core::CVector2 vSize;
			Core::CVector2 vPosition;
		};
		
		typedef DYNAMIC_ARRAY<PlacedCharacter> CharacterList;
        
		class CFont : public Core::IResource
		{
		public:

			//Internal cache of what a character should be built of
			struct CharacterInfo
			{
				Core::CRect sUVs;
                Core::CVector2 vSize;
                Core::CVector2 vOffset;
			};
            
            struct CKernLookup
			{
                //---------------------------------------------------------------------
                /// Constructor
                //---------------------------------------------------------------------
				CKernLookup(s16 inwCharacter, u16 inuwStart, u16 inuwLength)
				: wCharacter(inwCharacter)
				, uwStart(inuwStart)
				, uwLength(inuwLength)
				{
				}
				//---------------------------------------------------------------------
                /// Less then operator overload
                //---------------------------------------------------------------------
				bool operator < (const CKernLookup& inB) const
				{
                    return wCharacter < inB.wCharacter;
                }
                    
                s16 wCharacter;
                u16 uwStart;
                u16 uwLength;
            };
                    
            struct CKernPair
            {
                //---------------------------------------------------------------------
                /// Constructor
                //---------------------------------------------------------------------
                CKernPair(s16 inwCharacter, f32 infSpacing)
                : wCharacter(inwCharacter)
                , fSpacing(infSpacing)
                {
                }
                //---------------------------------------------------------------------
                /// Less then operator overload
                //---------------------------------------------------------------------
                bool operator < (const CKernPair& inB) const
                {
                    return wCharacter < inB.wCharacter;
                }
                
                f32 fSpacing;
                s16 wCharacter;				
            };
			
			virtual ~CFont(){}
			DECLARE_NAMED_INTERFACE(CFont);
			//---------------------------------------------------------------------
			/// Is A
			///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //-------------------------------------------
            /// Set Texture
            ///
            /// @param Texture containing the font
            //-------------------------------------------
            void SetTexture(const Rendering::TexturePtr& inpTex);
			//-------------------------------------------
			/// Get Texture
			///
			/// @return Font texture 
			//-------------------------------------------
			const Rendering::TexturePtr& GetTexture() const;
			//-------------------------------------------
			/// Set Character Data
			///
			/// Each character is a sprite on a tpage
			/// therefore we need the UV offsets
			///
			/// @param Sprite data containing UV's etc
			//-------------------------------------------
			void SetSpriteSheet(const Rendering::SpriteSheetPtr& inpData);
			//-------------------------------------------
			/// Get Mode Character Height
			///
			/// @return Mode height
			//-------------------------------------------
			f32 GetLineHeight() const;
            //-------------------------------------------
            /// Supports Kerning
            ///
            /// @return True is kerning supported, false otherwise
            //-------------------------------------------
            const bool SupportsKerning() const;
            //-------------------------------------------
			/// Get Info For Character
			///
			/// Get the info struct-a-majig for this character
			/// @param Character
			/// @param info struct to be filled with data for the character
			/// @return Success or invisible chars
			//-------------------------------------------
			CharacterResults GetInfoForCharacter(UTF8String::Char inChar, CharacterInfo& outInfo) const;
			//-------------------------------------------
			/// Get Attributes
			///
			/// @return Attributes - Spacing etc
			//-------------------------------------------
			const FontAttributes& GetAttributes() const;
			//-------------------------------------------
			/// Set Character Set
			///
			/// The list of characters that this font
			/// can display
			/// @param An array of the characters
			//-------------------------------------------
			void SetCharacterSet(const CharacterSet &inSet);
            //-------------------------------------------
            /// Get Kerning Between Characters
            ///
            /// Returns the amount that the glyph represented
            /// by inChar2 can be moved towards inChar1 without
            /// overlapping.
            /// This data is generated by the SpriteTool
            ///
            /// @param First character to test
            /// @param Second character to test
            /// @return Spacing between characters
            //-------------------------------------------
            f32 GetKerningBetweenCharacters(UTF8String::Char inChar1, UTF8String::Char inChar2) const;
            //-------------------------------------------
            /// Set Global Kerning Offset
            ///
            /// Sets a value that will be used to offset all kerning values
            ///
            /// @param Offset
            //-------------------------------------------
            static void SetGlobalKerningOffset(f32 infOffset);
		
        private:
            friend class IFontManager;
            friend class CFontLoader;
                    
			//Only the font manager can create this
            CFont() : mfLineHeight(0.0f)
            {
                maFirstLookup.clear();
                maPairs.clear();
            }
            //-------------------------------------------
            /// Set Kerning Info
            ///
            /// Sets kerning lookup and pair info
            ///
            /// @param Kerning lookup array
            /// @param Kerning pair array
            //-------------------------------------------
            void SetKerningInfo(const DYNAMIC_ARRAY<CKernLookup>& inaFirstReg, const DYNAMIC_ARRAY<CKernPair>& inaPairs);
            
			typedef HASH_MAP<UTF8String::Char, CharacterInfo> CharToCharInfoMap;
			
			CharToCharInfoMap mMapCharToCharInfo;
			
			//Holds the size, spacing, kerning etc
			FontAttributes mAttributes;
			
			//Holds all the characters that the font supports
			CharacterSet mCharacterSet;
			
            // Kerning lookup
            DYNAMIC_ARRAY<CKernLookup> maFirstLookup;
                    
            // Kerning pairs
            DYNAMIC_ARRAY<CKernPair> maPairs;
                    
			//The font bitmap
			Rendering::TexturePtr mpTexture;
			
			//The location and width etc of each character on the tpage
			Rendering::SpriteSheetPtr mpCharacterData;
            
            f32 mfLineHeight;
            
            static f32 mfGlobalKerningOffset;
		};
	}
}

#endif
